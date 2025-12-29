#pragma once

#include "esphome/core/component.h"
#include "esphome/core/log.h"

#include "esphome/components/uart/uart.h"
#include "esphome/components/sensor/sensor.h"
#include "esphome/components/binary_sensor/binary_sensor.h"


namespace esphome {
namespace s3km1110 {

static const char *const TAG = "s3km1110";

class S3KM1110Component : public Component {
 public:
  void set_uart(uart::UARTComponent *uart) { this->uart_ = uart; }

  void set_motion_energy_sensor(sensor::Sensor *s) { this->motion_energy_sensor_ = s; }
  void set_presence_sensor(sensor::Sensor *s) { this->presence_sensor_ = s; }
  void set_presence_binary(binary_sensor::BinarySensor *b) { this->presence_binary_ = b; }

  void setup() override {
    ESP_LOGI(TAG, "S3KM1110 component initialized");
    this->line_buffer_.reserve(64);
  }

  void loop() override {
    if (!this->uart_)
      return;

    int processed = 0;
    const int max_bytes = 32;

    while (this->uart_->available() && processed < max_bytes) {
      uint8_t byte;
      this->uart_->read_byte(&byte);
      processed++;

      char c = static_cast<char>(byte);

      if (c == '\r' || c == '\n') {
        if (!this->line_buffer_.empty()) {
          this->process_line_(this->line_buffer_);
          this->line_buffer_.clear();
        }
      } else {
        if (this->line_buffer_.size() < 63)
          this->line_buffer_.push_back(c);
      }
    }

    delay(1);
  }

 protected:
  uart::UARTComponent *uart_{nullptr};

  sensor::Sensor *motion_energy_sensor_{nullptr};
  sensor::Sensor *presence_sensor_{nullptr};
  binary_sensor::BinarySensor *presence_binary_{nullptr};

  std::string line_buffer_;

  void process_line_(const std::string &line) {
    auto trimmed = trim_(line);
    if (trimmed.empty())
      return;

    ESP_LOGD(TAG, "Received line: '%s'", trimmed.c_str());

    if (equals_ignore_case_(trimmed, "ON")) {
      if (this->presence_sensor_)
        this->presence_sensor_->publish_state(1.0f);
      if (this->presence_binary_)
        this->presence_binary_->publish_state(true);
      return;
    }

    if (equals_ignore_case_(trimmed, "OFF")) {
      if (this->presence_sensor_)
        this->presence_sensor_->publish_state(0.0f);
      if (this->presence_binary_)
        this->presence_binary_->publish_state(false);
      return;
    }

    const std::string prefix = "Range ";
    if (starts_with_(trimmed, prefix)) {
      float value = atof(trimmed.substr(prefix.size()).c_str());
      if (this->motion_energy_sensor_)
        this->motion_energy_sensor_->publish_state(value);
      return;
    }
  }

  static std::string trim_(const std::string &s) {
    size_t start = s.find_first_not_of(" \t");
    if (start == std::string::npos)
      return "";
    size_t end = s.find_last_not_of(" \t");
    return s.substr(start, end - start + 1);
  }

  static bool equals_ignore_case_(const std::string &a, const std::string &b) {
    if (a.size() != b.size())
      return false;
    for (size_t i = 0; i < a.size(); i++)
      if (tolower(a[i]) != tolower(b[i]))
        return false;
    return true;
  }

  static bool starts_with_(const std::string &s, const std::string &prefix) {
    return s.rfind(prefix, 0) == 0;
  }
};

}  // namespace s3km1110
}  // namespace esphome
