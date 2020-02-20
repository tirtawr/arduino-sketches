#include "HID-Project.h"

const int pinButtonPrevious = 11;
const int pinButtonPlayPause = 10;
const int pinButtonNext = 9;


void setup() {
  pinMode(pinButtonPrevious, INPUT_PULLUP);
  pinMode(pinButtonPlayPause, INPUT_PULLUP);
  pinMode(pinButtonNext, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(pinButtonPrevious), onButtonPressPrevious, FALLING);
  attachInterrupt(digitalPinToInterrupt(pinButtonPlayPause), onButtonPressPlayPause, FALLING);
  attachInterrupt(digitalPinToInterrupt(pinButtonNext), onButtonPressNext, FALLING);
  Consumer.begin();
}

void loop() {
  
}

void onButtonPressPrevious() {
  Consumer.write(MEDIA_PREVIOUS);
  delay(8);
}
void onButtonPressPlayPause() {
  Consumer.write(MEDIA_PLAY_PAUSE);
  delay(8);
}
void onButtonPressNext() {
  Consumer.write(MEDIA_NEXT);
  delay(8);
}
