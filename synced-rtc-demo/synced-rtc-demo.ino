#include <RTCZero.h>
#include <WiFiNINA.h>
#include "secrets.h"

// RTC related globals
RTCZero rtc;
const int GMT = -4;

// Wifi related globals
int wifiStatus = WL_IDLE_STATUS;
char ssid[] = SECRET_SSID;
char pass[] = SECRET_PASS;

// Interval related globals
const long interval = 1000;
unsigned long previousMillis = 0;


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
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(ssid);
    // Connect to WPA/WPA2 network. Change this line if using open or WEP network:
    wifiStatus = WiFi.begin(ssid, pass);

    // wait 4 seconds for connection:
    delay(4000);
  }

  printWiFiStatus();
  unsigned long epoch = 0;
  long epochOffset = GMT * 3600;
  while (epoch == 0) {
    epoch = WiFi.getTime();
    
    Serial.println("Fetching time from server");
    // wait 500ms for connection:
    delay(500);
  }  
  Serial.print("Epoch received: ");
  Serial.println(epoch);
  Serial.print("Offsetted epoch: ");
  Serial.println(epoch + epochOffset);
  rtc.begin();
  rtc.setEpoch(epoch + epochOffset);
}

void printTime() {
  char dateBuffer[12];
  sprintf(dateBuffer, "%d-%d-%d %d:%d:%d", rtc.getDay(), rtc.getMonth(), rtc.getYear(), rtc.getHours(), rtc.getMinutes(), rtc.getSeconds());
  Serial.println(dateBuffer);
}

void checkInterval() {
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    onInterval();
  }
}

void onInterval() {
  printTime();
}

void setup() {
  Serial.begin(9600);
  connectToWifiAndSyncRtc();
}

void loop() {
  checkInterval();
  delay(2); //debounce

}
