/*
  A USB Human Input Device that mimics the functionality
  of the macbook pro touch bar 
  created 20 Mar 2020
  by Tirta Rachman
*/

#include "HID-Project.h"

const int pinButtonEsc = 0;
const int pinButtonF1 = 1;
const int pinButtonF2 = 2;
const int pinButtonF3 = 3;
const int pinButtonF4 = 4;
const int pinButtonF5 = 5;
const int pinButtonF6 = 6;
const int pinButtonF7 = 7;
const int pinButtonF8 = 8;
const int pinButtonF9 = 9;
const int pinButtonF10 = 10;
const int pinButtonF11 = 11;

int buttonStates[] = {HIGH, HIGH, HIGH, HIGH, HIGH, HIGH, HIGH, HIGH, HIGH, HIGH, HIGH, HIGH};

void setup() {
  pinMode(pinButtonEsc, INPUT_PULLUP);
  pinMode(pinButtonF1, INPUT_PULLUP);
  pinMode(pinButtonF2, INPUT_PULLUP);
  pinMode(pinButtonF3, INPUT_PULLUP);
  pinMode(pinButtonF4, INPUT_PULLUP);
  pinMode(pinButtonF5, INPUT_PULLUP);
  pinMode(pinButtonF6, INPUT_PULLUP);
  pinMode(pinButtonF7, INPUT_PULLUP);
  pinMode(pinButtonF8, INPUT_PULLUP);
  pinMode(pinButtonF9, INPUT_PULLUP);
  pinMode(pinButtonF10, INPUT_PULLUP);
  pinMode(pinButtonF11, INPUT_PULLUP);
  Keyboard.begin();
}

void loop() {
  if(isPressed(pinButtonEsc)) {
    Keyboard.write(KEY_ESC);
  }
  if(isPressed(pinButtonF1)) {
    Keyboard.write(KEY_F14);
  }
  if(isPressed(pinButtonF2)) {
    Keyboard.write(KEY_F15);
  }
  if(isPressed(pinButtonF3)) {
    Keyboard.write(KEY_F22);
  }
  if(isPressed(pinButtonF4)) {
    Keyboard.write(KEY_F23);
  }
  if(isPressed(pinButtonF5)) {
    Keyboard.write(MEDIA_PREVIOUS);
  }
  if(isPressed(pinButtonF6)) {
    Keyboard.write(MEDIA_PLAY_PAUSE);
  }
  if(isPressed(pinButtonF7)) {
    Keyboard.write(MEDIA_NEXT);
  }
  if(isPressed(pinButtonF8)) {
    Keyboard.write(KEY_VOLUME_MUTE);
  }
  if(isPressed(pinButtonF9)) {
    Keyboard.write(KEY_VOLUME_DOWN);
  }
  if(isPressed(pinButtonF10)) {
    Keyboard.write(KEY_VOLUME_UP);
  }
  if(isPressed(pinButtonF11)) {
    Keyboard.write(KEY_0);
  }
  delay(5);
}

boolean isPressed(int pinNumber) {
  int currentReading = digitalRead(pinNumber);
  boolean isButtonPressed = currentReading == LOW && buttonStates[pinNumber] == HIGH;
  buttonStates[pinNumber] = currentReading;
  return isButtonPressed;
}
