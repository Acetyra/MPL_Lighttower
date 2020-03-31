#include <Arduino.h>
#include "WiFi.h"

const char * ssid           = "MPL";
const char * password       = "123456789";
String       client_address = "C01";
int          udpPort        = 3333; // Make sure this port matches the 'Server' port!
int          Control_pin    = 22;   // The device pin on the client to control e.g. an LED or Relay On/Off
char befehl[10];


WiFiUDP udp;

void setup()
{
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  if (WiFi.waitForConnectResult() != WL_CONNECTED) {
    Serial.println("WiFi Failed");
    while (1) {
      delay(1000);
    }
  }
  udp.begin(udpPort);
  Serial.println("Listening on Port: " + String(udpPort));
 
}



void loop()
{
int packetSize, len;
  // UDP
  packetSize = udp.parsePacket();
  // Da ist was da
  if (packetSize) {
    Serial.print("Empfangen "); Serial.print(packetSize);
    Serial.print(" von IP "); Serial.print(udp.remoteIP());
    Serial.print(" Port "); Serial.println(udp.remotePort());
    len = udp.read(befehl, 10);
    Serial.println(befehl); // erstes Byte 0 oder 1
  }
}

