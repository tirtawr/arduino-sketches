
#include <RTCZero.h>
#include <SPI.h>
#include <WiFiNINA.h>
#include "arduino_secrets.h"

RTCZero rtc;
const int GMT = -5;

int status = WL_IDLE_STATUS;
char ssid[] = SECRET_SSID;
char pass[] = SECRET_PASS;

void setup() {
  Serial.begin(9600);
  connectToWifiAndSyncRtc();
}

void loop() {

  delay(100);
}

float getTemperatureReading() {
  // Note: Using a TMP36 temp sensor on a 3.3v working voltage
  int reading = analogRead(A2);
  float voltage = reading * 3.3 / 1024.0;
  float temperature = (voltage - 0.5) * 100;
  return temperature;
}

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
  while ( status != WL_CONNECTED) {
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(ssid);
    // Connect to WPA/WPA2 network. Change this line if using open or WEP network:
    status = WiFi.begin(ssid, pass);

    // wait 10 seconds for connection:
    delay(10000);
  }

  printWiFiStatus();
  unsigned long epoch = 0;
  while (epoch == 0) {
    epoch = WiFi.getTime();
    Serial.println("Fetching time from server");
  }
  Serial.print("Epoch received: ");
  Serial.println(epoch);
  rtc.setEpoch(epoch);
  rtc.begin();
}
