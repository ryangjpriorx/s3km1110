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
    ESP_LOGI("s3km1110", "S3KM1110 component initialized");
  }

  void loop() override {
    if (!this->uart_)
      return;

    // Read and log all incoming UART bytes
    while (this->uart_->available()) {
      uint8_t byte;
      this->uart_->read_byte(&byte);
      ESP_LOGD("s3km1110", "UART byte: 0x%02X", byte);

      // TODO: feed into real parser once we know the protocol
    }
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
