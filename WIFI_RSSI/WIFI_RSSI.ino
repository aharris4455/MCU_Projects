#include <SPI.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#ifndef STASSID
#define STASSID "-----"
#define STAPSK "-----"
#endif

const char* ssid = STASSID;
const char* psk = STAPSK;

long RSSI;

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

void setup() {
  Serial.begin(115200);
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3D for 128x64
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }
  display.display();
  delay(200);
  display.clearDisplay();
  display.drawPixel(10, 10, WHITE);
  display.display(); //Must be called after drawing to display
  delay(1000);
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0,10);

  Serial.println();
  Serial.println();
  Serial.println("Connecting to ");
  display.println("Connecting...");
  display.display();
  delay(500);
  Serial.println(ssid);
  WiFi.mode(WIFI_STA); //Set to client mode
  WiFi.begin(ssid, psk);
  while(WiFi.status() != WL_CONNECTED){
    delay(500);
    Serial.println(".");
  }
  Serial.println("");
  Serial.println("WIFI CONNECTED");
  Serial.println("IP ADDR: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  RSSI = WiFi.RSSI();
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(45, 35);
  display.println(RSSI);
  display.display();
  Serial.println(RSSI);
  delay(500);
}
