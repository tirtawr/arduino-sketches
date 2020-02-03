#include <RTCZero.h>

// RTC Related globals
RTCZero rtc;
const byte seconds = 0;
const byte minutes = 10;
const byte hours = 14;
const byte day = 30;
const byte month = 1;
const byte year = 20;

// Interval related globals
const long interval = 1000;
unsigned long previousMillis = 0;

void setup() {
  Serial.begin(9600);
  setupRTC();
}

void loop() {
  // put your main code here, to run repeatedly:
  checkInterval();
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

void printTime(RTCZero rtc) {
  Serial.println(getFormattedTime(rtc));
}

String getFormattedTime(RTCZero rtc) {
  return twoDigit(rtc.getDay()) + "/" +
         twoDigit(rtc.getMonth()) + "/" +
         twoDigit(rtc.getYear()) + " " +
         twoDigit(rtc.getHours()) + ":" +
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

void setupRTC() {
  rtc.begin();
  rtc.setHours(hours);
  rtc.setMinutes(minutes);
  rtc.setSeconds(seconds);
  rtc.setDay(day);
  rtc.setMonth(month);
  rtc.setYear(year);
}
