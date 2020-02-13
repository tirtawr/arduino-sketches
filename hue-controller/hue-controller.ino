/*
  A controller for a singular philips hue bulb
  created 13 Feb 2020
  by Tirta Rachman
*/
#include <Encoder.h>
#include <Wire.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_GFX.h>
#include <WiFiNINA.h>
#include "secrets.h"

// Pins
const int pinEncoderBriClk = 7;
const int pinEncoderBriDt = 6;
const int pinEncoderHueClk = 5;
const int pinEncoderHueDt = 4;
const int pinEncoderSatClk = 1;
const int pinEncoderSatDt = 0;

// Wifi
char ssid[] = SECRET_SSID;
char pass[] = SECRET_PASS;
int status = WL_IDLE_STATUS;
char serverAddress[] = "172.22.151.183";
String username = "spy1lJe396BQfvulW7HxBbwW3LdNMYcFUvcgBQr8";
String lightIndex = "6";
WiFiClient client;

// Display related globals
#define OLED_ADDR   0x3C
Adafruit_SSD1306 display(-1);

// Encoder related globals
Encoder brightnessEncoder(pinEncoderBriClk, pinEncoderBriDt);
long oldBrightnessEncoderPosition  = -999;
Encoder hueEncoder(pinEncoderHueClk, pinEncoderHueDt);
long oldHueEncoderPosition  = -999;
Encoder saturationEncoder(pinEncoderSatClk, pinEncoderSatDt);
long oldSaturationEncoderPosition  = -999;

// Other globals
int brightnessDisplayValue = 50;
int hueDisplayValue = 50;
int saturationDisplayValue = 50;
String currentMode = "Brightness";

void setup() {
  setupDisplay();
  setupWifi();
}

void loop() {
  checkBrightnessEncoderRotation();
  checkHueEncoderRotation();
  checkSaturationEncoderRotation();
  updateDisplay();
  delay(4);
}

void increaseAttribute(String attribute) {
  currentMode = attribute;
  if (attribute == "Brightness") {
    if (brightnessDisplayValue != 100) brightnessDisplayValue += 10;
  } else if (attribute == "Hue") {
    if (hueDisplayValue != 100) hueDisplayValue += 10;
  } else if (attribute == "Saturation") {
    if (saturationDisplayValue != 100) saturationDisplayValue += 10;
  }
  putData(generateRequestBody());
}

void decreaseAttribute(String attribute) {
  currentMode = attribute;
  if (attribute == "Brightness") {
    if (brightnessDisplayValue != 0) brightnessDisplayValue -= 10;
  } else if (attribute == "Hue") {
    if (hueDisplayValue != 0) hueDisplayValue -= 10;
  } else if (attribute == "Saturation") {
    if (saturationDisplayValue != 0) saturationDisplayValue -= 10;
  }
  putData(generateRequestBody());
}

String generateRequestBody() {
  int brightnessRequestValue = map(brightnessDisplayValue, 0, 100, 0, 254);
  int hueRequestValue = map(hueDisplayValue, 0, 100, 0, 65535);
  int saturationRequestValue = map(saturationDisplayValue, 0, 100, 0, 254);
  return String("{\"on\":true,\"bri\":") + brightnessRequestValue
         + String(",\"hue\":") + hueRequestValue + String(",\"sat\":")
         + saturationRequestValue + String(",\"effect\":\"none\",\"alert\":\"none\"}");
}

void putData(String body) {
  Serial.println(body);
  if (WiFi.status() == WL_CONNECTED) {
    if (client.connect(serverAddress, 80)) {
      client.println("PUT /api/" + username + "/lights/6/state HTTP/1.1");
      client.print("Host: ");
      client.println(serverAddress);
      client.println("Content-type: application/json");
      client.println("Accept: */*");
      client.println("Cache-Control: no-cache");
      client.print("Host: ");
      client.println(serverAddress);
      client.println("Accept-Encoding: gzip, deflate");
      client.print("Content-Length: ");
      client.println(body.length());
      client.println("Connection: close");
      client.println();
      client.println(body);
      client.println();
    } else {
      Serial.println("Error connecting to server");
    }
  }
}

void checkBrightnessEncoderRotation() {
  long newBrightnessEncoderPosition = brightnessEncoder.read();
  if (newBrightnessEncoderPosition >= oldBrightnessEncoderPosition + 4) {
    oldBrightnessEncoderPosition = newBrightnessEncoderPosition;
    onBrightnessEncoderLeftTurn();
  } else if (newBrightnessEncoderPosition <= oldBrightnessEncoderPosition - 4) {
    oldBrightnessEncoderPosition = newBrightnessEncoderPosition;
    onBrightnessEncoderRightTurn();
  }
}

void onBrightnessEncoderLeftTurn() {
  decreaseAttribute("Brightness");
}

void onBrightnessEncoderRightTurn() {
  increaseAttribute("Brightness");
}

void checkHueEncoderRotation() {
  long newHueEncoderPosition = hueEncoder.read();
  if (newHueEncoderPosition >= oldHueEncoderPosition + 4) {
    oldHueEncoderPosition = newHueEncoderPosition;
    onHueEncoderLeftTurn();
  } else if (newHueEncoderPosition <= oldHueEncoderPosition - 4) {
    oldHueEncoderPosition = newHueEncoderPosition;
    onHueEncoderRightTurn();
  }
}

void onHueEncoderLeftTurn() {
  decreaseAttribute("Hue");
}

void onHueEncoderRightTurn() {
  increaseAttribute("Hue");
}

void checkSaturationEncoderRotation() {
  long newSaturationEncoderPosition = saturationEncoder.read();
  if (newSaturationEncoderPosition >= oldSaturationEncoderPosition + 4) {
    oldSaturationEncoderPosition = newSaturationEncoderPosition;
    onSaturationEncoderLeftTurn();
  } else if (newSaturationEncoderPosition <= oldSaturationEncoderPosition - 4) {
    oldSaturationEncoderPosition = newSaturationEncoderPosition;
    onSaturationEncoderRightTurn();
  }
}

void onSaturationEncoderLeftTurn() {
  decreaseAttribute("Saturation");
}

void onSaturationEncoderRightTurn() {
  increaseAttribute("Saturation");
}


void updateDisplay() {
  display.fillScreen(BLACK);

  //label
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(28, 23);
  display.print(currentMode);

  //current value
  int currentValue;
  if (currentMode == "Brightness") {
    currentValue = brightnessDisplayValue;
  } else if (currentMode == "Hue") {
    currentValue = hueDisplayValue;
  } else if (currentMode == "Saturation") {
    currentValue = saturationDisplayValue;
  }
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(28, 6);
  display.print(currentValue);

  display.display();
}

void setupDisplay() {
  display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR);
  display.clearDisplay();
  display.display();
}

void setupWifi() {
  Serial.println("Starting Wifi Connection");
  while (status != WL_CONNECTED) {
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(ssid);
    status = WiFi.begin(ssid, pass);
    delay(3000);
    Serial.println("Connected!");
    printWifiStatus();
  }
}

void printWifiStatus() {
  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your board's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  // print the received signal strength:
  long rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
}
