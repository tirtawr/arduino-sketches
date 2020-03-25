#include "HID-Project.h"

uint8_t keyCode = 0;

void setup() {
  Keyboard.begin();
  Serial.begin(9600);
}

void loop() {
  keyCode++;
  Serial.println(keyCode);
  Keyboard.write(keyCode);
  delay(500);
}
