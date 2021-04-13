#include <TimeLib.h>
#include <RTCZero.h>
#include <WiFiNINA.h>
#include <Stepper.h>
#include "Dusk2Dawn.h"
#include "secrets.h"

//Debug mode toggle
const bool DEBUG_MODE = false;

// RTC related globals
RTCZero rtc;
const int GMT = -4;

// Wifi related globals
int wifiStatus = WL_IDLE_STATUS;
char ssid[] = SECRET_SSID;
char pass[] = SECRET_PASS;

// Interval related globals
const long interval = 60 * 1000;
unsigned long previousMillis = 0;

// Sun angle related globals
const float LAT = 40.69253;
const float LNG = -73.97974;
const bool IS_DAYLIGHT_SAVING = false;
Dusk2Dawn sunTimes(LAT, LNG, GMT);

// Stepper related globals
#define STEPPER_PIN_1 9
#define STEPPER_PIN_2 8
#define STEPPER_PIN_3 7
#define STEPPER_PIN_4 6
#define STEPPER_STEPS_PER_REVOLUTION 2038
int currStep = 0;
Stepper sunStepper = Stepper(STEPPER_STEPS_PER_REVOLUTION, STEPPER_PIN_1, STEPPER_PIN_3, STEPPER_PIN_2, STEPPER_PIN_4);

void printWiFiStatus() {
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);
  long rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
}

void connectToWifiAndSyncRtc() {
  // attempt to connect to WiFi network:
  while ( wifiStatus != WL_CONNECTED) {
    if (DEBUG_MODE) {
      Serial.print("Attempting to connect to SSID: ");
      Serial.println(ssid);
    }
    
    // Connect to WPA/WPA2 network. Change this line if using open or WEP network:
    wifiStatus = WiFi.begin(ssid, pass);

    // wait 4 seconds for connection:
    delay(4000);
  }

  if (DEBUG_MODE) {
    printWiFiStatus();
  }
  
  unsigned long epoch = 0;
  long epochOffset = GMT * 3600;
  while (epoch == 0) {
    epoch = WiFi.getTime();
    if (DEBUG_MODE) {
      Serial.println("Fetching time from server");  
    }
    // wait 500ms for connection:
    delay(500);
  }
  if (DEBUG_MODE) {
    Serial.print("Epoch received: ");
    Serial.println(epoch);
    Serial.print("Offsetted epoch: ");
    Serial.println(epoch + epochOffset);
  }
  rtc.begin();
  rtc.setEpoch(epoch + epochOffset);
  WiFi.disconnect();
}

void printTime() {
  char str[80];
  sprintf(str, "Today %d-%d-%d %d:%d:%d - %d\n", rtc.getDay(), rtc.getMonth(), rtc.getYear(), rtc.getHours(), rtc.getMinutes(), rtc.getSeconds(), rtc.getEpoch());
  Serial.print(str);
}

// Returns sun angle in degrees
// 0 -> sunrise
// 90 -> solar noon
// 180 -> sunset
// 270 -> solar midnight
int getSunAngle() {
  // Epoch are in seconds
  unsigned long todayEpoch = rtc.getEpoch();
  unsigned long yesterdayEpoch = todayEpoch - 86400;
  unsigned long tormorrowEpoch = todayEpoch + 86400;

  long secondsFromMidnight = (hour(todayEpoch) * 60 * 60) + (minute(todayEpoch) * 60) + second(todayEpoch);

  int yesterdayDay = day(yesterdayEpoch);
  int yesterdayMonth = month(yesterdayEpoch);
  int yesterdayYear = year(yesterdayEpoch);

  int todayDay = day(todayEpoch);
  int todayMonth = month(todayEpoch);
  int todayYear = year(todayEpoch);

  int tomorrowDay = day(tormorrowEpoch);
  int tomorrowMonth = month(tormorrowEpoch);
  int tomorrowYear = year(tormorrowEpoch);

  // All in minutes from midnight
  unsigned long yesterdaySunset = sunTimes.sunset(yesterdayYear, yesterdayMonth, yesterdayDay, IS_DAYLIGHT_SAVING);
  unsigned long todaySunrise = sunTimes.sunrise(todayYear, todayMonth, todayDay, IS_DAYLIGHT_SAVING);
  unsigned long todaySunset = sunTimes.sunset(todayYear, todayMonth, todayDay, IS_DAYLIGHT_SAVING);
  unsigned long tomorrowSunrise = sunTimes.sunrise(tomorrowYear, tomorrowMonth, tomorrowDay, IS_DAYLIGHT_SAVING);

  unsigned long yesterdaySunsetEpoch = yesterdayEpoch - secondsFromMidnight + (yesterdaySunset * 60);
  unsigned long todaySunriseEpoch = todayEpoch - secondsFromMidnight + (todaySunrise * 60);
  unsigned long todaySunsetEpoch = todayEpoch - secondsFromMidnight + (todaySunset * 60);
  unsigned long tomorrowSunriseEpoch = tormorrowEpoch - secondsFromMidnight + (tomorrowSunrise * 60);
  unsigned long nowEpoch = todayEpoch;

  unsigned long baseAngle, angleOffset;
  if (nowEpoch < todaySunriseEpoch) {
    // before sunrise today
    baseAngle = 180;
    angleOffset = 180 * (nowEpoch - yesterdaySunsetEpoch) / (todaySunriseEpoch - yesterdaySunsetEpoch);
  } else if (todaySunriseEpoch <= nowEpoch && nowEpoch <= todaySunsetEpoch) {
    // between sunrise and sunset today  
    baseAngle = 0;
    angleOffset = 180 * (nowEpoch - todaySunriseEpoch) / (todaySunsetEpoch - todaySunriseEpoch);
  } else {
    // after sunset today
    baseAngle = 180;
    angleOffset = 180 * (nowEpoch - todaySunsetEpoch) / (tomorrowSunriseEpoch - todaySunsetEpoch);
  }
  
  return (int) (baseAngle + angleOffset);
}

void printSunAngle() {
  char str[80];
  sprintf(str, "Sun Angle = %d degree\n", getSunAngle());
  Serial.print(str);
}

// Rotate needle to a specific point in the clock face
// 0 degree is left side of the horizon
void rotateTo(int degrees) {
  int desiredStep = ((STEPPER_STEPS_PER_REVOLUTION / 360) * degrees);
  int stepsNeeded = desiredStep - currStep;
  sunStepper.step(stepsNeeded);
  currStep = desiredStep;
}

void refreshClock() {
  int sunAngle = getSunAngle();
  rotateTo(sunAngle);  
}

void checkInterval() {
  // TODO: handle overflow
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    onInterval();
  }
}

void onInterval() {
  refreshClock();
  if (DEBUG_MODE) {
    printSunAngle();
  }
}

void setup() {
  if (DEBUG_MODE) {
    Serial.begin(9600);
  }
  sunStepper.setSpeed(2);
  connectToWifiAndSyncRtc();
}

void loop() {
  checkInterval();
  delay(2); //debounce
}
