#pragma once

#include "esphome/core/component.h"
#include "esphome/core/log.h"
#include "esphome/components/uart/uart.h"
#include "esphome/components/sensor/sensor.h"

namespace esphome {
namespace s3km1110 {

static const char *const TAG = "s3km1110";

class S3KM1110Component : public Component {
 public:
  void set_uart(uart::UARTComponent *uart) { this->uart_ = uart; }

  void set_micro_motion_sensor(sensor::Sensor *s) { this->micro_motion_sensor_ = s; }
  void set_presence_confidence_sensor(sensor::Sensor *s) { this->presence_confidence_sensor_ = s; }
  void set_motion_energy_sensor(sensor::Sensor *s) { this->motion_energy_sensor_ = s; }
  void set_presence_sensor(sensor::Sensor *s) { this->presence_sensor_ = s; }

  void setup() override {
    ESP_LOGI(TAG, "S3KM1110 component initialized");
    this->line_buffer_.reserve(64);
  }

void loop() override {
  if (!this->uart_)
    return;

  // Limit how many bytes we process per loop iteration
  int processed = 0;
  const int max_bytes = 32;  // prevents blocking

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
      if (this->line_buffer_.size() < 63) {
        this->line_buffer_.push_back(c);
      } else {
        this->line_buffer_.clear();
      }
    }
  }

  // Yield to allow WiFi/API tasks to run
  delay(1);
}

 protected:
  uart::UARTComponent *uart_{nullptr};

  sensor::Sensor *micro_motion_sensor_{nullptr};
  sensor::Sensor *presence_confidence_sensor_{nullptr};
  sensor::Sensor *motion_energy_sensor_{nullptr};
  sensor::Sensor *presence_sensor_{nullptr};

  std::string line_buffer_;

  void process_line_(const std::string &line) {
    ESP_LOGD(TAG, "Received line: '%s'", line.c_str());

    // Trim leading/trailing spaces
    auto trimmed = trim_(line);
    if (trimmed.empty())
      return;

    // Handle presence: "ON" / "OFF"
    if (equals_ignore_case_(trimmed, "ON")) {
      if (this->presence_sensor_ != nullptr) {
        this->presence_sensor_->publish_state(1.0f);
        ESP_LOGD(TAG, "Presence: ON (1)");
      }
      return;
    }

    if (equals_ignore_case_(trimmed, "OFF")) {
      if (this->presence_sensor_ != nullptr) {
        this->presence_sensor_->publish_state(0.0f);
        ESP_LOGD(TAG, "Presence: OFF (0)");
      }
      return;
    }

    // Handle "Range <number>"
    const std::string prefix = "Range ";
    if (starts_with_(trimmed, prefix)) {
      const std::string value_str = trimmed.substr(prefix.size());
      float value = 0.0f;
      if (parse_float_(value_str, &value)) {
        if (this->motion_energy_sensor_ != nullptr) {
          this->motion_energy_sensor_->publish_state(value);
          ESP_LOGD(TAG, "Range parsed as motion_energy: %.2f", value);
        }
      } else {
        ESP_LOGW(TAG, "Failed to parse range value from '%s'", trimmed.c_str());
      }
      return;
    }

    // Unknown line - just log it for now
    ESP_LOGW(TAG, "Unrecognized line format: '%s'", trimmed.c_str());
  }

  // Helpers

  static std::string trim_(const std::string &s) {
    size_t start = 0;
    while (start < s.size() && (s[start] == ' ' || s[start] == '\t'))
      start++;

    if (start == s.size())
      return "";

    size_t end = s.size() - 1;
    while (end > start && (s[end] == ' ' || s[end] == '\t'))
      end--;

    return s.substr(start, end - start + 1);
  }

  static bool equals_ignore_case_(const std::string &a, const std::string &b) {
    if (a.size() != b.size())
      return false;
    for (size_t i = 0; i < a.size(); i++) {
      char ca = a[i];
      char cb = b[i];
      if (ca >= 'A' && ca <= 'Z')
        ca = ca - 'A' + 'a';
      if (cb >= 'A' && cb <= 'Z')
        cb = cb - 'A' + 'a';
      if (ca != cb)
        return false;
    }
    return true;
  }

  static bool starts_with_(const std::string &s, const std::string &prefix) {
    if (s.size() < prefix.size())
      return false;
    return s.compare(0, prefix.size(), prefix) == 0;
  }

  static bool parse_float_(const std::string &s, float *out) {
    char *endptr = nullptr;
    const char *cstr = s.c_str();
    float val = strtof(cstr, &endptr);
    if (endptr == cstr)
      return false;
    *out = val;
    return true;
  }
};

}  // namespace s3km1110
}  // namespace esphome
