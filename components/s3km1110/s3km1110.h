#pragma once

#include "esphome/core/component.h"
#include "esphome/components/uart/uart.h"
#include "esphome/components/sensor/sensor.h"
#include "esphome/components/binary_sensor/binary_sensor.h"

namespace esphome {
namespace s3km1110 {

class S3KM1110Component : public Component {
 public:
  void set_uart(uart::UARTComponent *uart) { this->uart_ = uart; }

  void set_micro_motion_sensor(sensor::Sensor *s) { this->micro_motion_sensor_ = s; }
  void set_presence_confidence_sensor(sensor::Sensor *s) { this->presence_confidence_sensor_ = s; }
  void set_motion_energy_sensor(sensor::Sensor *s) { this->motion_energy_sensor_ = s; }
  void set_presence_binary_sensor(binary_sensor::BinarySensor *bs) { this->presence_binary_sensor_ = bs; }

  void setup() override {
    // TODO: Initialize the S3KM1110 here if required
  }

  void loop() override {
    if (this->uart_ == nullptr) {
      return;
    }

    // TODO: Replace this stub with real UART protocol parsing
    // This is just placeholder logic to show the sensors can update.

    while (this->uart_->available()) {
      uint8_t byte;
      this->uart_->read_byte(&byte);
      // TODO: feed bytes into a state machine / frame parser
    }

    // Example dummy values (remove once real values are parsed)
    const float dummy_micro_motion = 1.0f;
    const float dummy_presence_confidence = 80.0f;
    const float dummy_motion_energy = 5.0f;
    const bool dummy_presence = true;

    if (this->micro_motion_sensor_ != nullptr) {
      this->micro_motion_sensor_->publish_state(dummy_micro_motion);
    }

    if (this->presence_confidence_sensor_ != nullptr) {
      this->presence_confidence_sensor_->publish_state(dummy_presence_confidence);
    }

    if (this->motion_energy_sensor_ != nullptr) {
      this->motion_energy_sensor_->publish_state(dummy_motion_energy);
    }

    if (this->presence_binary_sensor_ != nullptr) {
      this->presence_binary_sensor_->publish_state(dummy_presence);
    }

    // Slow this down a bit so we don’t spam HA
    delay(500);
  }

 protected:
  uart::UARTComponent *uart_{nullptr};

  sensor::Sensor *micro_motion_sensor_{nullptr};
  sensor::Sensor *presence_confidence_sensor_{nullptr};
  sensor::Sensor *motion_energy_sensor_{nullptr};
  binary_sensor::BinarySensor *presence_binary_sensor_{nullptr};
};

}  // namespace s3km1110
}  // namespace esphome
