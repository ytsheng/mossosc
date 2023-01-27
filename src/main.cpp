#include <Arduino.h>
#include <Wire.h>
#include <OneButton.h>
#include "imu.h"

Imu imu;
OneButton button(9);

void setup()
{
  Serial.begin(115200);
  
  button.attachClick([]() {
    Wire.setPins(0, 1);
    if (!imu.begin()) {
      Serial.println("Error starting IMU");
    } else {
      Serial.println("Boom");
    }
  });
}

void loop()
{
  button.tick();
  imu.tick();

  static uint32_t last_print = 0;
  if (millis() - last_print > 500) {
    Serial.printf("Roll: %f  Pitch: %f  Yaw: %f\r\n", imu.getRoll(), imu.getPitch(), imu.getYaw());
    last_print = millis();
  }
}