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
char ssid[] = "Bears don't dig on dancing"; // your network SSID (name)
char pass[] = "dancedance44"; // your network password

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
    OSCMessage msg("/roll");

    msg.add(imu.getRoll());
    Udp.beginPacket("192.168.1.234", 9000);
    msg.send(Udp);
    Udp.endPacket();

    OSCMessage msg2("/pitch");
    msg2.add(imu.getPitch());
    Udp.beginPacket("192.168.1.234", 9000);
    msg2.send(Udp);
    Udp.endPacket();

    OSCMessage msg3("/yaw");
    msg3.add(imu.getYaw());
    Udp.beginPacket("192.168.1.234", 9000);
    msg3.send(Udp);
    Udp.endPacket();

    last_print = millis();
  }
}