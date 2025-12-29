#pragma once

#include "esphome/core/component.h"
#include "esphome/components/sensor/sensor.h"

namespace esphome {
namespace s3km1110 {

class S3KM1110Sensor : public sensor::Sensor, public Component {
 public:
  void setup() override {
    // Initialize hardware here
  }

  void loop() override {
    // Read your radar value
    float value = 0.0f;  // replace with real reading
    publish_state(value);
  }
};

}  // namespace s3km1110
}  // namespace esphome
