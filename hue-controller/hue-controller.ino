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
    if (brightnessDisplayValue != 100) brightnessDisplayValue++;
  } else if (attribute == "Hue") {
    if (hueDisplayValue != 100) hueDisplayValue++;
  } else if (attribute == "Saturation") {
    if (saturationDisplayValue != 100) saturationDisplayValue++;
  }
}

void decreaseAttribute(String attribute) {
  currentMode = attribute;
  if (attribute == "Brightness") {
    if (brightnessDisplayValue != 0) brightnessDisplayValue--;
  } else if (attribute == "Hue") {
    if (hueDisplayValue != 0) hueDisplayValue--;
  } else if (attribute == "Saturation") {
    if (saturationDisplayValue != 0) saturationDisplayValue--;
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
