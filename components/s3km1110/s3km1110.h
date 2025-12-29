#pragma once

#include "esphome/core/component.h"
#include "esphome/components/uart/uart.h"
#include "esphome/components/sensor/sensor.h"

namespace esphome {
namespace s3km1110 {

class S3KM1110Component : public Component {
 public:
  void set_uart(uart::UARTComponent *uart) { this->uart_ = uart; }

  void set_micro_motion_sensor(sensor::Sensor *s) { this->micro_motion_sensor_ = s; }
  void set_presence_confidence_sensor(sensor::Sensor *s) { this->presence_confidence_sensor_ = s; }
  void set_motion_energy_sensor(sensor::Sensor *s) { this->motion_energy_sensor_ = s; }
  void set_presence_sensor(sensor::Sensor *s) { this->presence_sensor_ = s; }

  void setup() override {
    // Initialize hardware here if needed
  }

  void loop() override {
    if (this->uart_ == nullptr)
      return;

    // TODO: Replace with real UART parsing for S3KM1110
    while (this->uart_->available()) {
      uint8_t byte;
      this->uart_->read_byte(&byte);
      // Feed into your parser here
    }

    // Dummy values for testing
    float micro_motion = 1.0f;
    float presence_confidence = 80.0f;
    float motion_energy = 5.0f;
    float presence = 1.0f;  // 1 = present, 0 = not present

    if (this->micro_motion_sensor_ != nullptr)
      this->micro_motion_sensor_->publish_state(micro_motion);

    if (this->presence_confidence_sensor_ != nullptr)
      this->presence_confidence_sensor_->publish_state(presence_confidence);

    if (this->motion_energy_sensor_ != nullptr)
      this->motion_energy_sensor_->publish_state(motion_energy);

    if (this->presence_sensor_ != nullptr)
      this->presence_sensor_->publish_state(presence);
  }

 protected:
  uart::UARTComponent *uart_{nullptr};

  sensor::Sensor *micro_motion_sensor_{nullptr};
  sensor::Sensor *presence_confidence_sensor_{nullptr};
  sensor::Sensor *motion_energy_sensor_{nullptr};
  sensor::Sensor *presence_sensor_{nullptr};
};

}  // namespace s3km1110
}  // namespace esphome
