#include <Arduino.h>
#include "WiFi.h"
#include <FastLED.h>

const char *ssid = "MPL";
const char *password = "123456789";
String client_address = "C01";

#define LED_PIN 15
#define POWER 4
#define NUM_LEDS 144

#define LED_TYPE WS2812
#define COLOR_ORDER GRB

int Brightness = 0;

int status = WL_IDLE_STATUS;
unsigned int localPort = 3333;
unsigned long mill = millis();
int ledHight = 0;
int lastLedHight = 0;
WiFiUDP Udp;

CRGBArray<NUM_LEDS> ledsOld;
CRGBPalette16 currentPalette;
TBlendType currentBlending;

void setup()
{
  pinMode(POWER, OUTPUT);
  digitalWrite(POWER, LOW);
  Serial.begin(115200);
  currentPalette = RainbowColors_p;
  currentBlending = LINEARBLEND;
  while (status != WL_CONNECTED)
  {

    status = WiFi.begin(ssid, password);
    Serial.println("Verbindungsversuch");
    delay(1000);
  }

  Serial.println("Verbunden");

  FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(ledsOld, NUM_LEDS).setCorrection(TypicalLEDStrip);

  Brightness = 255;
  FastLED.setBrightness(Brightness);

  //fill_solid (leds, 144, CRGB(255, 255, 255));
  //FastLED.show();
  Udp.begin(localPort);
  digitalWrite(POWER, HIGH);
}

void FillLEDsFromPaletteColors(uint8_t colorIndex)
{
  uint8_t brightness = 255;

  for (int i = 0; i < ledHight; i++)
  {
    ledsOld[i] = ColorFromPalette(currentPalette, colorIndex, brightness, currentBlending);
    colorIndex += 3;
  }
}

void loop()
{

  int packetSize = Udp.parsePacket();
  if (packetSize)
  {
    char packetBuffer[255] = {0};

    int len = Udp.read(packetBuffer, 255);
    ledHight = atoi(packetBuffer);
    FastLED.clear();
    //Serial.println(ledHight);
    if (ledHight > 144)
    {
      ledHight = 144;
    }
  }
  if (mill + 9 < millis())
  {
    mill = millis();
    if (ledHight >= lastLedHight)
    {
      fill_rainbow(ledsOld, ledHight, CRGB(255, 255, 255));
      lastLedHight = ledHight;
    }
    else
    {
      lastLedHight--;
      fill_rainbow(ledsOld, lastLedHight, CRGB(255, 255, 255));
    }
    FastLED.show();
  }

  if(WiFi.status() != WL_CONNECTED)
  {
    status = WiFi.begin(ssid, password);
    Serial.println("erneuter Verbindungsversuch");
    delay(1000);
  }
}
