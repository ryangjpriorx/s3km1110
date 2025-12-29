#include "s3km1110.h"

namespace esphome {
namespace s3km1110 {

static const char *const TAG = "s3km1110";

void S3KM1110Component::setup() {
  ESP_LOGI(TAG, "S3KM1110 UART component initialized");
}

void S3KM1110Component::loop() {
  while (this->available()) {
    std::string line = this->readline();

    if (line.empty())
      continue;

    ESP_LOGD(TAG, "UART RX: %s", line.c_str());

    // Trim CR/LF
    while (!line.empty() && (line.back() == '\r' || line.back() == '\n'))
      line.pop_back();

    // Normalize to uppercase for easier matching
    std::string upper = line;
    for (auto &c : upper) c = toupper(c);

    // Presence ON
    if (upper == "ON") {
      occupied_ = true;
      if (presence_binary_sensor_ != nullptr)
        presence_binary_sensor_->publish_state(true);
      continue;
    }

    // Presence OFF
    if (upper == "OFF") {
      occupied_ = false;
      if (presence_binary_sensor_ != nullptr)
        presence_binary_sensor_->publish_state(false);
      continue;
    }

    // RANGE:<value> or Range <value>
    if (upper.rfind("RANGE", 0) == 0) {
      // Remove "RANGE" or "Range"
      std::string value_str = line.substr(5);

      // Remove separators like ":" or " "
      while (!value_str.empty() && (value_str[0] == ':' || value_str[0] == ' '))
        value_str.erase(0, 1);

      float distance = atof(value_str.c_str());
      presence_raw_ = distance;

      ESP_LOGI(TAG, "Distance parsed: %.2f", distance);

      if (presence_raw_sensor_ != nullptr)
        presence_raw_sensor_->publish_state(distance);

      continue;
    }

    ESP_LOGW(TAG, "Unrecognized UART frame: %s", line.c_str());
  }
}

std::string S3KM1110Component::readline() {
  std::string result;
  while (this->available()) {
    char c = this->read();
    if (c == '\n')
      break;
    result.push_back(c);
  }
  return result;
}

}  // namespace s3km1110
}  // namespace esphome
