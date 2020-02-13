/*
  A controller for a singular philips hue bulb
  created 13 Feb 2020
  by Tirta Rachman
*/
#include <Encoder.h>
#include <Wire.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_GFX.h>

// Pins
const int pinEncoderBriClk = 7;
const int pinEncoderBriDt = 6;
const int pinEncoderHueClk = 5;
const int pinEncoderHueDt = 4;
const int pinEncoderSatClk = 1;
const int pinEncoderSatDt = 0;

// Display related globals
#define OLED_ADDR   0x3C
Adafruit_SSD1306 display(-1);
String displayText = "HH:MM:SS";

// Encoder related globals
Encoder brightnessEncoder(pinEncoderBriClk, pinEncoderBriDt);
long oldBrightnessEncoderPosition  = -999;
Encoder hueEncoder(pinEncoderHueClk, pinEncoderHueDt);
long oldHueEncoderPosition  = -999;
Encoder saturationEncoder(pinEncoderSatClk, pinEncoderSatDt);
long oldSaturationEncoderPosition  = -999;

// Other globals
int brightnessDisplayValue
int hueDisplayValue
int saturationDisplayValue

void setup() {
  setupDisplay();
}

void loop() {
  checkBrightnessEncoderRotation();
  checkHueEncoderRotation();
  checkSaturationEncoderRotation();
  updateDisplay();
  delay(4);
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
  displayText = "B-Left";
}

void onBrightnessEncoderRightTurn() {
  displayText = "B-Right";
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
  displayText = "H-Left";
}

void onHueEncoderRightTurn() {
  displayText = "H-Right";
}

void checkSaturationEncoderRotation() {
  long newSaturationEncoderPosition = saturationEncoder.read();
  if (newSaturationEncoderPosition >= oldSaturationEncoderPosition + 4) {
    oldSaturationEncoderPosition = newSaturationEncoderPosition;
    onSaturationEncoderLeftTurn();
  } else if (newSaturationEncoderPosition <= oldSaturationEncoderPosition - 4) {
    oldSaturationEncoderPosition = newSaturationEncoderPosition;
    onHueEncoderRightTurn();
  }
}

void onSaturationEncoderLeftTurn() {
  displayText = "S-Left";
}

void onSaturationEncoderRightTurn() {
  displayText = "S-Right";
}


void updateDisplay() {
  display.fillScreen(BLACK);
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(16, 8);
  display.print(displayText);
  display.display();
}

void setupDisplay() {
  display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR);
  display.clearDisplay();
  display.display();
}
