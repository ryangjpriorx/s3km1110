#pragma once

#include "esphome/core/component.h"
#include "esphome/core/log.h"
#include "esphome/components/uart/uart.h"
#include "esphome/components/sensor/sensor.h"
#include "esphome/components/binary_sensor/binary_sensor.h"

namespace esphome {
namespace s3km1110 {

class S3KM1110Component : public Component, public uart::UARTDevice {
 public:
  void setup() override;
  void loop() override;

  // Sensor objects
  sensor::Sensor *motion_energy_sensor_{nullptr};
  sensor::Sensor *presence_raw_sensor_{nullptr};
  binary_sensor::BinarySensor *presence_binary_sensor_{nullptr};

  // Internal state
  float motion_energy_{0.0f};
  float presence_raw_{0.0f};
  bool occupied_{false};

  // Getters for template sensors
  float get_motion_energy() const { return motion_energy_; }
  float get_presence_raw() const { return presence_raw_; }
  bool is_occupied() const { return occupied_; }

 protected:
  std::string readline();
};

}  // namespace s3km1110
}  // namespace esphome
