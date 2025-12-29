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

    // Presence ON
    if (line == "ON") {
      occupied_ = true;
      if (presence_binary_sensor_ != nullptr)
        presence_binary_sensor_->publish_state(true);
      continue;
    }

    // Presence OFF
    if (line == "OFF") {
      occupied_ = false;
      if (presence_binary_sensor_ != nullptr)
        presence_binary_sensor_->publish_state(false);
      continue;
    }

    // RANGE:<value>
    if (line.rfind("RANGE:", 0) == 0) {
      std::string value_str = line.substr(6);
      float distance = atof(value_str.c_str());
      presence_raw_ = distance;

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
