#include <SPI.h>
#include <WiFiNINA.h>
#include "secrets.h"

// WIFI Related globals
char ssid[] = SECRET_SSID;        // your network SSID (name)
char pass[] = SECRET_PASS;    // your network password (use for WPA, or use as key for WEP)
int keyIndex = 0;            // your network key Index number (needed only for WEP)
int status = WL_IDLE_STATUS;
IPAddress server(10, 18, 146, 107);
const int port = 8080;
WiFiClient client;

// PINS
const int pinUp = 2;
const int pinDown = 3;
const int pinLeft = 5;
const int pinRight = 4;
const int pinConnect = 11;
const int pinLED = 12;

// Control buttons related globals
String buttonStateUp = "UP";
String buttonStateDown = "UP";
String buttonStateLeft = "UP";
String buttonStateRight = "UP";

// Connection button related globals
const int ledPin =  LED_BUILTIN;
int ledState = LOW;
unsigned long previousMillis = 0;
const long blinkIntervalSlow = 400;
const long blinkIntervalFast = 200;
String buttonStateConnect = "UP";
String connectionStatus = "disconnected";

void setup() {
  //Initialize serial and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

  // attempt to connect to Wifi network:
  while (status != WL_CONNECTED) {
    connectionStatus = "CONNECTING";
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(ssid);
    // Connect to WPA/WPA2 network. Change this line if using open or WEP network:
    status = WiFi.begin(ssid, pass);

    // wait 10 seconds for connection:
    delay(10000);
  }
  Serial.println("Connected to wifi");
  printWifiStatus();

  connectToServer();

  // control buttons
  pinMode(pinUp, INPUT_PULLUP);
  pinMode(pinDown, INPUT_PULLUP);
  pinMode(pinLeft, INPUT_PULLUP);
  pinMode(pinRight, INPUT_PULLUP);

  // connection button
  pinMode(pinLED, OUTPUT);
  pinMode(pinConnect, INPUT_PULLUP);
}

void loop() {
  // put your main code here, to run repeatedly:
  checkButtonStates();
  checkConnection();
  checkLED();
  delay(4);
}

void connectToServer() {
  Serial.println("\nStarting connection to server...");
  // if you get a connection, report back via serial:
  if (client.connect(server, port)) {
    Serial.println("connected to server");
    connectionStatus = "CONNECTED";
    // Make a TCP request:
    // Sets client name
    client.println("n=TIRTA");
    client.println("i");
  }
}

void disconnectFromServer() {
  Serial.println("Disconnecting from server");
  client.println("x");
  connectionStatus = "DISCONNECTED";
  Serial.println("Disconnected from server");
}

void onButtonConnectPressed() {
  if (connectionStatus == "DISCONNECTED") {
    connectToServer();
  } else if (connectionStatus == "CONNECTING") {
    // do nothing
  } else if (connectionStatus == "CONNECTED") {
    disconnectFromServer();
  }
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
  } else if (button == "BUTTON_CONNECT") {
    onButtonConnectPressed();
  }
}

void checkLED() {
  if (connectionStatus == "DISCONNECTED") {
    blink(blinkIntervalSlow);
  } else if (connectionStatus == "CONNECTING") {
    // Blink fast
    blink(blinkIntervalFast);
  } else if (connectionStatus == "CONNECTED") {
    digitalWrite(pinLED, HIGH);
  }

}

void blink(long interval) {
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    if (ledState == LOW) {
      ledState = HIGH;
    } else {
      ledState = LOW;
    }
    digitalWrite(pinLED, ledState);
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
  checkButtonStateConnect();
}

void checkButtonStateConnect() {
  int sensorVal = digitalRead(pinConnect);
  String newButtonStateConnect;
  if (sensorVal == HIGH) {
    newButtonStateConnect = "UP";
  } else {
    newButtonStateConnect = "DOWN";
  }
  if (buttonStateConnect == "DOWN" && newButtonStateConnect == "UP") {
    onButtonPressed("BUTTON_CONNECT");
  }
  buttonStateConnect = newButtonStateConnect;
}

void checkButtonStateUp() {
  int sensorVal = digitalRead(pinUp);
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
  int sensorVal = digitalRead(pinDown);
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
  int sensorVal = digitalRead(pinLeft);
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
  int sensorVal = digitalRead(pinRight);
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
