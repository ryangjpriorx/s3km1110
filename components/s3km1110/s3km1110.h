#pragma once
#include "esphome.h"

namespace s3km1110 {

class S3KM1110Sensor : public esphome::Component, public esphome::uart::UARTDevice {
 public:
  void set_pm1_0(esphome::Sensor *s) { pm1_0 = s; }
  void set_pm2_5(esphome::Sensor *s) { pm2_5 = s; }
  void set_pm10(esphome::Sensor *s) { pm10 = s; }

  void loop() override {
    while (available() >= 20) {
      uint8_t frame[20];
      read_array(frame, 20);

      if (frame[0] != 0x42 || frame[1] != 0x4D)
        continue;

      uint16_t pm1  = (frame[10] << 8) | frame[11];
      uint16_t pm25 = (frame[12] << 8) | frame[13];
      uint16_t pm10v = (frame[14] << 8) | frame[15];

      if (pm1_0) pm1_0->publish_state(pm1);
      if (pm2_5) pm2_5->publish_state(pm25);
      if (pm10)  pm10->publish_state(pm10v);
    }
  }

 protected:
  esphome::Sensor *pm1_0{nullptr};
  esphome::Sensor *pm2_5{nullptr};
  esphome::Sensor *pm10{nullptr};
};

}  // namespace s3km1110
