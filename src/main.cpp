#include <Arduino.h>
#include <Wire.h>
#include <OneButton.h>
#include <OSCMessage.h>
#include <WiFiUdp.h>
#include <WiFi.h>
#include "imu.h"


Imu imu;
OneButton button(9);
WiFiUDP Udp;
unsigned int localPort = 8888;
char ssid[] = "ButernutMansion"; // your network SSID (name)
char pass[] = "yellowsubmarine69"; // your network password

void setup()
{
  Serial.begin(460800);
  
  Wire.setPins(0, 1);
  if (!imu.begin()) {
    Serial.println("Error starting IMU");
  } else {
    Serial.println("Boom");
  }
  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Udp.begin(localPort);
}

void loop()
{
  button.tick();
  imu.tick();

  static uint32_t last_print = 0;
  if (millis() - last_print > 500) {
    Serial.printf("Roll: %f  Pitch: %f  Yaw: %f\r\n", imu.getRoll(), imu.getPitch(), imu.getYaw());
    // OSCMessage msg("/roll");
    // msg.add(map(imu.getRoll(), -180, 180, 0, 1));
    // Udp.beginPacket("192.168.1.218", 9000);
    // msg.send(Udp);
    // Udp.endPacket();

    // OSCMessage msg2("/pitch");
    OSCMessage msg2("/lx/mixer/channel/4/pattern/4/fuel");
    // Serial.println(map(imu.getPitch(), -90, 90, 0, 1));
    msg2.add((imu.getPitch() + 100)/200);
    Udp.beginPacket("192.168.1.244", 3030);
    msg2.send(Udp);
    Udp.endPacket();

    // OSCMessage msg3("/yaw");
    // msg3.add(map(imu.getYaw(), 0, 360, 0, 1));
    // Udp.beginPacket("192.168.1.218", 9000);
    // msg3.send(Udp);
    // Udp.endPacket();

    last_print = millis();
  }
}