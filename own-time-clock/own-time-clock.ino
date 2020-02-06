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
byte seconds = 0;
byte minutes = 10;
byte hours = 14;
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

// Control related globals
String currentMode = "IDLE"; // IDLE -> HOUR -> MINUTE -> SECOND -> IDLE

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
  updateDisplay();
  delay(4);
}

void checkInterval() {
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    // Interval logic here
    if (currentMode == "IDLE") {
      setDisplayTime();
    }
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
  if (currentMode == "IDLE") return;
  if (currentMode == "HOUR") {
    if (hours == 23) {
      hours = 0;
    } else {
      hours++;
    }
  } else if (currentMode == "MINUTE") {
    if (minutes == 59) {
      minutes = 0;
    } else {
      minutes++;
    }
  } else if (currentMode == "SECOND") {
    if (seconds == 59) {
      seconds = 0;
    } else {
      seconds++;
    }
  }
  setDisplayTime();
}

void onLeftTurn() {
  Serial.println("LEFT");
  if (currentMode == "IDLE") return;
  if (currentMode == "HOUR") {
    if (hours == 0) {
      hours = 23;
    } else {
      hours--;
    }
  } else if (currentMode == "MINUTE") {
    if (minutes == 0) {
      minutes = 59;
    } else {
      minutes--;
    }
  } else if (currentMode == "SECOND") {
    if (seconds == 0) {
      seconds = 59;
    } else {
      seconds--;
    }
  }
  setDisplayTime();
}

void onButtonPressed() {
  Serial.println("BUTTON_PRESSED");
  currentMode = getNextMode();
  if (currentMode == "HOURS") {
    pauseClock();
  }
  if (currentMode == "IDLE") {
    continueClock();
  }
}

void pauseClock() {
  seconds = rtc.getSeconds();
  minutes = rtc.getMinutes();
  hours = rtc.getHours();
}

void continueClock() {
  rtc.setSeconds(seconds);
  rtc.setMinutes(minutes);
  rtc.setHours(hours);
}

void setDisplayTime() {
  //  Serial.println(getFormattedTimeForSerialMonitor());
  displayText = getFormattedTimeForDisplay();
}

String getFormattedTimeForSerialMonitor() {
  return twoDigit(rtc.getDay()) + "/" +
         twoDigit(rtc.getMonth()) + "/" +
         twoDigit(rtc.getYear()) + " " +
         twoDigit(rtc.getHours()) + ":" +
         twoDigit(rtc.getMinutes()) + ":" +
         twoDigit(rtc.getSeconds());
}

String getFormattedTimeForDisplay() {
  return twoDigit(hours) + ":" +
         twoDigit(minutes) + ":" +
         twoDigit(seconds);
}

String getNextMode() {
  if (currentMode == "IDLE") {
    return "HOUR";
  } else if (currentMode == "HOUR") {
    return "MINUTE";
  } else if (currentMode == "MINUTE") {
    return "SECOND";
  } else if (currentMode == "SECOND") {
    return "IDLE";
  }
  return "IDLE";
}

String twoDigit(int number)
{
  if (number < 10) {
    return "0" + String(number);
  }
  return String(number);
}

void updateDisplay() {
  updateDisplayedClockIfIdle();
  display.fillScreen(BLACK);
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(16, 8);
  display.print(displayText);
  drawUnderscore();
  display.display();
}

void updateDisplayedClockIfIdle() {
  if (currentMode == "IDLE") {
    // TODO: add offset logic
    seconds = rtc.getSeconds();
    minutes = rtc.getMinutes();
    hours = rtc.getHours();
  }
}

void drawUnderscore() {
  const int underscoreY = 26;
  const int underscoreWidth = 22;
  const int underscorePadding = 14;
  const int hourLeftX = 16;
  const int minuteLeftX = hourLeftX + underscoreWidth + underscorePadding;
  const int secondLeftX = minuteLeftX + underscoreWidth + underscorePadding;
  if (currentMode == "HOUR") {
    display.drawLine(hourLeftX, underscoreY, hourLeftX + underscoreWidth, underscoreY, WHITE);
  } else if (currentMode == "MINUTE") {
    display.drawLine(minuteLeftX, underscoreY, minuteLeftX + underscoreWidth, underscoreY, WHITE);
  } else if (currentMode == "SECOND") {
    display.drawLine(secondLeftX, underscoreY, secondLeftX + underscoreWidth, underscoreY, WHITE);
  }
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
