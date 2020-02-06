#include <RTCZero.h>
#include <Encoder.h>
#include <Wire.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_GFX.h>

// Pins
const int pinEncoderClk = 5;
const int pinEncoderDt = 6;
const int pinEncoderSw = 7; // Button

// Display related globals
#define OLED_ADDR   0x3C
Adafruit_SSD1306 display(-1);
String displayText = "HH:MM:SS";

// RTC Related globals
RTCZero rtc;
const byte seconds = 0;
const byte minutes = 10;
const byte hours = 14;
const byte day = 30;
const byte month = 1;
const byte year = 20;

// Encoder related globalshe
Encoder encoder(pinEncoderClk, pinEncoderDt);
long oldEncoderPosition  = -999;
String buttonState = "BUTTON_UP";

// Interval related globals
const long interval = 1000;
unsigned long previousMillis = 0;

void setup() {
  Serial.begin(9600);
  setupRTC();
  pinMode(pinEncoderSw, INPUT);
  setupDisplay();
}

void loop() {
  // put your main code here, to run repeatedly:
  checkInterval();
  checkEncoderRotation();
  checkEncoderButton();
  printDisplay();
  delay(4);
}

void checkInterval() {
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    // Interval logic here
    printTime(rtc);
  }
}

void checkEncoderRotation() {
  long newEncoderPosition = encoder.read();
  if (newEncoderPosition >= oldEncoderPosition + 4) {
    oldEncoderPosition = newEncoderPosition;
    onLeftTurn();
  } else if (newEncoderPosition <= oldEncoderPosition - 4) {
    oldEncoderPosition = newEncoderPosition;
    onRightTurn();
  }
}

void checkEncoderButton() {
  int sensorVal = digitalRead(pinEncoderSw);
  String newButtonState;
  if (sensorVal == LOW) {
    newButtonState = "BUTTON_UP";
  } else {
    newButtonState = "BUTTON_DOWN";
  }
  if (newButtonState == "BUTTON_UP" && buttonState == "BUTTON_DOWN") {
    onButtonPressed();
  }
  buttonState = newButtonState;
}

void onRightTurn() {
  Serial.println("RIGHT");
}

void onLeftTurn() {
  Serial.println("LEFT");
}

void onButtonPressed() {
  Serial.println("BUTTON_PRESSED");
}

void printTime(RTCZero rtc) {
  Serial.println(getFormattedTimeForSerialMonitor(rtc));
  displayText = getFormattedTimeForDisplay(rtc);
}

String getFormattedTimeForSerialMonitor(RTCZero rtc) {
  return twoDigit(rtc.getDay()) + "/" +
         twoDigit(rtc.getMonth()) + "/" +
         twoDigit(rtc.getYear()) + " " +
         twoDigit(rtc.getHours()) + ":" +
         twoDigit(rtc.getMinutes()) + ":" +
         twoDigit(rtc.getSeconds());
}

String getFormattedTimeForDisplay(RTCZero rtc) {
  return twoDigit(rtc.getHours()) + ":" +
         twoDigit(rtc.getMinutes()) + ":" +
         twoDigit(rtc.getSeconds());
}

String twoDigit(int number)
{
  if (number < 10) {
    return "0" + String(number);
  }
  return String(number);
}

void printDisplay() {
  display.fillScreen(BLACK);
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(16, 8);
  display.print(displayText);
  display.display();
}

void setupRTC() {
  rtc.begin();
  rtc.setHours(hours);
  rtc.setMinutes(minutes);
  rtc.setSeconds(seconds);
  rtc.setDay(day);
  rtc.setMonth(month);
  rtc.setYear(year);
}

void setupDisplay() {
  display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR);
  display.clearDisplay();
  display.display();
}
