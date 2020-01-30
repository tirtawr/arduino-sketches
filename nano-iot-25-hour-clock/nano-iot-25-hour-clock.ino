#include <RTCZero.h>
#include <Encoder.h>

// RTC Related globals
RTCZero rtc;
const byte seconds = 0;
const byte minutes = 10;
const byte hours = 14;
const byte day = 30;
const byte month = 1;
const byte year = 20;

// Encoder related globals
Encoder encoder(5, 6);
long oldEncoderPosition  = -999;

// Interval related globals
const long interval = 1000;
unsigned long previousMillis = 0;

// Button related globals
String buttonState = "BUTTON_DOWN";

// Mode related globals
String currentMode = "IDLE";

void setup()
{
  Serial.begin(9600);
  rtc.begin();
  rtc.setHours(hours);
  rtc.setMinutes(minutes);
  rtc.setSeconds(seconds);
  rtc.setDay(day);
  rtc.setMonth(month);
  rtc.setYear(year);

  pinMode(7, INPUT);
}

void loop()
{
  checkInterval();
  checkEncoder();
  checkButton();
  delay(2);
}

void checkButton() {
  int sensorVal = digitalRead(7);
  String newButtonState;
  if (sensorVal == HIGH) {
    newButtonState = "BUTTON_UP";
  } else {
    newButtonState = "BUTTON_DOWN";
  }
  if (buttonState == "BUTTON_DOWN" && newButtonState == "BUTTON_UP") {
    onButtonPressed();
  }
  buttonState = newButtonState;
}

void checkInterval() {
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    // Interval logic here
    printTime(rtc);
  }
}

void checkEncoder() {
  long newEncoderPosition = encoder.read();
  if (newEncoderPosition >= oldEncoderPosition + 4) {
    oldEncoderPosition = newEncoderPosition;
    onRightTurn();
  } else if (newEncoderPosition <= oldEncoderPosition - 4) {
    oldEncoderPosition = newEncoderPosition;
    onLeftTurn();
  }
}

void onButtonPressed() {
  goToNextMode();
  Serial.println("BUTTON_PRESSED");
}

void onRightTurn() {
  Serial.println("RIGHT");
}

void onLeftTurn() {
  Serial.println("LEFT");
}


void printTime(RTCZero rtc) {
  Serial.println(getFormattedTime(rtc));
}

String getFormattedTime(RTCZero rtc) {
  return twoDigit(rtc.getDay()) + "/" +
         twoDigit(rtc.getMonth()) + "/" +
         twoDigit(rtc.getYear()) + " " +
         twoDigit(rtc.getHours()) + ":" +
         twoDigit(rtc.getMinutes()) + ":" +
         twoDigit(rtc.getSeconds()) + " - " +
         currentMode;
}

String goToNextMode() {
  if (currentMode == "IDLE") {
    currentMode = "YEAR";
  } else if (currentMode == "YEAR") {
    currentMode = "MONTH";
  } else if (currentMode == "MONTH") {
    currentMode = "DAY";
  } else if (currentMode == "DAY") {
    currentMode = "HOUR";
  } else if (currentMode == "HOUR") {
    currentMode = "MINUTE";
  } else if (currentMode == "MINUTE") {
    currentMode = "SECOND";
  } else if (currentMode == "SECOND") {
    currentMode = "IDLE";
  }
}

String twoDigit(int number)
{
  if (number < 10) {
    return "0" + String(number);
  }
  return String(number);
}
