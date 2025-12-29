#include "s3km1110.h"
#include "esphome/core/log.h"

namespace esphome {
namespace s3km1110 {

static const char *const TAG = "s3km1110";

void S3KM1110Component::setup() {
  ESP_LOGI(TAG, "S3KM1110 UART component initialized");
}

void S3KM1110Component::loop() {
  // Read UART line-by-line
  while (this->available()) {
    std::string line = this->readline();

    if (line.empty()) {
      continue;
    }

    ESP_LOGD(TAG, "UART RX: %s", line.c_str());

    // Trim whitespace
    while (!line.empty() && (line.back() == '\r' || line.back() == '\n')) {
      line.pop_back();
    }

    // Presence ON
    if (line == "ON") {
      ESP_LOGI(TAG, "Presence detected");
      if (this->presence_binary_sensor_ != nullptr) {
        this->presence_binary_sensor_->publish_state(true);
      }
      continue;
    }

    // Presence OFF
    if (line == "OFF") {
      ESP_LOGI(TAG, "No presence");
      if (this->presence_binary_sensor_ != nullptr) {
        this->presence_binary_sensor_->publish_state(false);
      }
      continue;
    }

    // RANGE:<value>
    if (line.rfind("RANGE:", 0) == 0) {
      std::string value_str = line.substr(6);
      float distance = atof(value_str.c_str());

      ESP_LOGI(TAG, "Distance: %.2f", distance);

      if (this->distance_sensor_ != nullptr) {
        this->distance_sensor_->publish_state(distance);
      }
      continue;
    }

    // Unknown frame
    ESP_LOGW(TAG, "Unrecognized UART frame: %s", line.c_str());
  }
}

std::string S3KM1110Component::readline() {
  std::string result;
  while (this->available()) {
    char c = this->read();
    if (c == '\n') {
      break;
    }
    result.push_back(c);
  }
  return result;
}

}  // namespace s3km1110
}  // namespace esphome
