#include <SPI.h>
#include <WiFiNINA.h>
#include "secrets.h"

// WIFI Related globals
char ssid[] = SECRET_SSID;        // your network SSID (name)
char pass[] = SECRET_PASS;    // your network password (use for WPA, or use as key for WEP)
int keyIndex = 0;            // your network key Index number (needed only for WEP)
int status = WL_IDLE_STATUS;
IPAddress server(10, 18, 146, 107);
WiFiClient client;

// Button related globals
String buttonStateUp = "UP";
String buttonStateDown = "UP";
String buttonStateLeft = "UP";
String buttonStateRight = "UP";

void setup() {
  //Initialize serial and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

  // attempt to connect to Wifi network:
  while (status != WL_CONNECTED) {
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(ssid);
    // Connect to WPA/WPA2 network. Change this line if using open or WEP network:
    status = WiFi.begin(ssid, pass);

    // wait 10 seconds for connection:
    delay(10000);
  }
  Serial.println("Connected to wifi");
  printWifiStatus();

  Serial.println("\nStarting connection to server...");
  // if you get a connection, report back via serial:
  if (client.connect(server, 8080)) {
    Serial.println("connected to server");
    // Make a TCP request:
    // Sets client name   
    client.println("n=TIRTA");
    client.println("i");
  }

  pinMode(2, INPUT_PULLUP);
  pinMode(3, INPUT_PULLUP);
  pinMode(4, INPUT_PULLUP);
  pinMode(5, INPUT_PULLUP);
}

void loop() {
  // put your main code here, to run repeatedly:
  checkButtonStates();
  checkConnection();
  delay(4);
}

void onButtonPressed(String button) {
  Serial.println(button);
  if (button == "BUTTON_UP") {
    client.println("u");
  } else if (button == "BUTTON_DOWN") {
    client.println("d");
  } else if (button == "BUTTON_LEFT") {
    client.println("l");
  } else if (button == "BUTTON_RIGHT") {
    client.println("r");
  }
}

void checkConnection() {
  // if the server's disconnected, stop the client:
  if (!client.connected()) {
    Serial.println();
    Serial.println("disconnecting from server.");
    client.stop();

    // do nothing forevermore:
    while (true);
  }
}

void checkButtonStates() {
  checkButtonStateUp();
  checkButtonStateDown();
  checkButtonStateLeft();
  checkButtonStateRight();
}

void checkButtonStateUp() {
  int sensorVal = digitalRead(5);
  String newButtonStateUp;
  if (sensorVal == HIGH) {
    newButtonStateUp = "UP";
  } else {
    newButtonStateUp = "DOWN";
  }
  if (buttonStateUp == "DOWN" && newButtonStateUp == "UP") {
    onButtonPressed("BUTTON_UP");
  }
  buttonStateUp = newButtonStateUp;
}

void checkButtonStateDown() {
  int sensorVal = digitalRead(2);
  String newButtonStateDown;
  if (sensorVal == HIGH) {
    newButtonStateDown = "UP";
  } else {
    newButtonStateDown = "DOWN";
  }
  if (buttonStateDown == "DOWN" && newButtonStateDown == "UP") {
    onButtonPressed("BUTTON_DOWN");
  }
  buttonStateDown = newButtonStateDown;
}

void checkButtonStateLeft() {
  int sensorVal = digitalRead(4);
  String newButtonStateLeft;
  if (sensorVal == HIGH) {
    newButtonStateLeft = "UP";
  } else {
    newButtonStateLeft = "DOWN";
  }
  if (buttonStateLeft == "DOWN" && newButtonStateLeft == "UP") {
    onButtonPressed("BUTTON_LEFT");
  }
  buttonStateLeft = newButtonStateLeft;
}

void checkButtonStateRight() {
  int sensorVal = digitalRead(3);
  String newButtonStateRight;
  if (sensorVal == HIGH) {
    newButtonStateRight = "UP";
  } else {
    newButtonStateRight = "DOWN";
  }
  if (buttonStateRight == "DOWN" && newButtonStateRight == "UP") {
    onButtonPressed("BUTTON_RIGHT");
  }
  buttonStateRight = newButtonStateRight;
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
