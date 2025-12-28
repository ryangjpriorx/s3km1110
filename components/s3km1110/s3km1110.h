#pragma once

#include "esphome/core/component.h"
#include "esphome/components/uart/uart.h"
#include "esphome/components/sensor/sensor.h"
#include "esphome/components/binary_sensor/binary_sensor.h"

namespace esphome {
namespace s3km1110 {

class S3KM1110Sensor : public Component, public uart::UARTDevice {
 public:
  void set_uart(uart::UARTComponent *uart) { this->set_uart_parent(uart); }

  void set_micro_motion(sensor::Sensor *s) { this->micro_motion_ = s; }
  void set_presence_confidence(sensor::Sensor *s) { this->presence_confidence_ = s; }
  void set_motion_energy(sensor::Sensor *s) { this->motion_energy_ = s; }
  void set_presence_binary(binary_sensor::BinarySensor *b) { this->presence_binary_ = b; }

  void loop() override;

 protected:
  sensor::Sensor *micro_motion_{nullptr};
  sensor::Sensor *presence_confidence_{nullptr};
  sensor::Sensor *motion_energy_{nullptr};
  binary_sensor::BinarySensor *presence_binary_{nullptr};
};

inline void S3KM1110Sensor::loop() {
  const size_t frame_len = 20;

  while (this->available() >= frame_len) {
    uint8_t frame[frame_len];
    auto read = this->read_array(frame, frame_len);
    if (read != frame_len) {
      return;
    }

    // NOTE: These header/offsets are placeholders based on your original code.
    // Adjust to the real S3KM1110 protocol if needed.
    if (frame[0] != 0x42 || frame[1] != 0x4D) {
      continue;
    }

    uint16_t micro = (static_cast<uint16_t>(frame[10]) << 8) | frame[11];
    uint16_t presence = (static_cast<uint16_t>(frame[12]) << 8) | frame[13];
    uint16_t energy = (static_cast<uint16_t>(frame[14]) << 8) | frame[15];

    if (this->micro_motion_ != nullptr) {
      this->micro_motion_->publish_state(micro);
    }
    if (this->presence_confidence_ != nullptr) {
      this->presence_confidence_->publish_state(presence);
    }
    if (this->motion_energy_ != nullptr) {
      this->motion_energy_->publish_state(energy);
    }

    if (this->presence_binary_ != nullptr) {
      bool present = (micro > 0 || presence > 0 || energy > 0);
      this->presence_binary_->publish_state(present);
    }
  }
}

}  // namespace s3km1110
}  // namespace esphome
