#include <MIDIUSB.h>
#include <pitchToNote.h>
#include "Interval.h"

boolean turnOffNotesAfterPlaying = false;

int potentiometerPin = 3; // Analog
const int buttonMainPin = 5; // Digital
const int buttonUpPin = 12; // Digital
const int buttonDownPin = 11; // Digital
const int buttonLeftPin = 13; // Digital
const int buttonRightPin = 14; // Digital
const int ledPin = 6; // Digital

//int majorChord[] = {0, 4, 7};
//int minorChord[] = {0, 3, 7};
//int diminishedChord[] = {0, 3, 6};
//int augmentedChord[] = {0, 4, 8};
//int majorSeventhChord[] = {0, 4, 8, 11};
//int majorEleventhChord[] = {0, 4, 7, 11, 14};

int chordSize = 5;
int chord[] = {0, 4, 7, 11, 14};

//int tonic = pitchC4;
int tonic = 0; // Starting tonic
int baseNote = tonic; // Base note for chord progression
int currentNote = baseNote; // Current note that is playing

// previous state of the button:
int lastbuttonMainState = HIGH;
int lastbuttonUpState = HIGH;
int lastbuttonDownState = HIGH;
int lastbuttonLeftState = HIGH;
int lastbuttonRightState = HIGH;
unsigned int previousMillis;
unsigned int noteIntervalValue = 0;

boolean isTonePlaying = false;
int currentOffsetIndex = 0;
Interval ledTimeout;
Interval noteOffTimeout;

void setup() {
  Serial.begin(9600);
  randomSeed(analogRead(0));
  pinMode(buttonMainPin, INPUT_PULLUP);
  pinMode(buttonUpPin, INPUT_PULLUP);
  pinMode(buttonDownPin, INPUT_PULLUP);
  pinMode(buttonLeftPin, INPUT_PULLUP);
  pinMode(buttonRightPin, INPUT_PULLUP);
  pinMode(ledPin, OUTPUT);
}

void loop() {
  checkButtonStates();
  guardNoteTopThreshold();
  guardNoteBottomThreshold();
  updateNoteIntervalValue();
  ledTimeout.check();
  noteOffTimeout.check();
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= noteIntervalValue) {
    previousMillis = currentMillis;
    if (isPressed(buttonMainPin)) {
      playNote();
    }
  }
}

void playNote() {
  currentNote = baseNote + getNextOffset(chord);
  sendNoteOnEvent();
}

void turnOffLed() {
  digitalWrite(ledPin, LOW);
}

void onButtonMainPressed() { }

void onButtonMainReleased() { }

void onButtonUpPressed() { }

void onButtonUpReleased() { }

void onButtonDownPressed() { }

void onButtonDownReleased() { }

void onButtonLeftPressed() {
  baseNote -= 12;
}

void onButtonLeftReleased() { }

void onButtonRightPressed() {
  baseNote += 12;
}

void onButtonRightReleased() { }

void updateNoteIntervalValue() {
  int sensorValue = analogRead(potentiometerPin);
  noteIntervalValue = linearMap(sensorValue, 0, 1023, 800, 50);
  Serial.println(noteIntervalValue);
}

void checkButtonStates() {
  debounce();
  int buttonMainState = digitalRead(buttonMainPin);
  int buttonUpState = digitalRead(buttonUpPin);
  int buttonDownState = digitalRead(buttonDownPin);
  int buttonLeftState = digitalRead(buttonLeftPin);
  int buttonRightState = digitalRead(buttonRightPin);
  if (buttonMainState != lastbuttonMainState) {
    if (buttonMainState == LOW) {
      onButtonMainPressed();
    } else  {
      onButtonMainReleased();
    }
    lastbuttonMainState = buttonMainState;
  }
  if (buttonMainState != lastbuttonMainState) {
    if (buttonMainState == LOW) {
      onButtonMainPressed();
    } else  {
      onButtonMainReleased();
    }
    lastbuttonMainState = buttonMainState;
  }
  if (buttonUpState != lastbuttonUpState) {
    if (buttonUpState == LOW) {
      onButtonUpPressed();
    } else  {
      onButtonUpReleased();
    }
    lastbuttonUpState = buttonUpState;
  }
  if (buttonDownState != lastbuttonDownState) {
    if (buttonDownState == LOW) {
      onButtonDownPressed();
    } else  {
      onButtonDownReleased();
    }
    lastbuttonDownState = buttonDownState;
  }
  if (buttonLeftState != lastbuttonLeftState) {
    if (buttonLeftState == LOW) {
      onButtonLeftPressed();
    } else  {
      onButtonLeftReleased();
    }
    lastbuttonLeftState = buttonLeftState;
  }
  if (buttonRightState != lastbuttonRightState) {
    if (buttonRightState == LOW) {
      onButtonRightPressed();
    } else  {
      onButtonRightReleased();
    }
    lastbuttonRightState = buttonRightState;
  }
}

int getNextOffset(int arr[]) {
  currentOffsetIndex++;
  if (currentOffsetIndex >= chordSize) {
    currentOffsetIndex = 0;
    int risingCutoff = 66;
    int loweringCutoff = 33;
    // Give higher chance to go up if up button is held down
    // and give higher chance to go down if down button is held down
    if (isPressed(buttonUpPin)) {
      risingCutoff = 20;
      loweringCutoff = 0;
    } else if (isPressed(buttonDownPin)) {
      risingCutoff = 100;
      loweringCutoff = 80;
    }
    int randomNumber = random(100);
    if (randomNumber > risingCutoff) {
      baseNote += random(8);
    } else if (randomNumber < loweringCutoff) {
      baseNote -= random(8);
    }
  }
  return arr[currentOffsetIndex];
}

void sendMidiCommand(byte cmd, byte data1, byte  data2) {
  midiEventPacket_t midiMsg = {cmd >> 4, cmd, data1, data2};
  MidiUSB.sendMIDI(midiMsg);
}

void sendNoteOnEvent() {
  sendMidiCommand(0x90, currentNote, 0x7F);
  digitalWrite(ledPin, HIGH);
  ledTimeout.setTimeout(turnOffLed, noteIntervalValue / 2);
  if (turnOffNotesAfterPlaying) {
    noteOffTimeout.setTimeout(sendNoteOffEvent, noteIntervalValue * 8 / 10);
  }

}

void sendNoteOffEvent() {
  sendMidiCommand(0x80, currentNote, 0);
}

void guardNoteBottomThreshold() {
  if (baseNote < 0) baseNote = 0;
}

void guardNoteTopThreshold() {
  if (baseNote + 12 > 127) baseNote = 115;
}

boolean isPressed(int pin) {
  return digitalRead(pin) == LOW;
}

//int logarithmicMap(value, valueFloor, valueCeiling, targetFloor, targetCeiling) {
//  map(sensorValue, 0, 1023, 800, 50);
//}

int linearMap(int value, int valueFloor, int valueCeiling, int targetFloor, int targetCeiling) {
  int linearTarget = (value - valueFloor) * (targetCeiling - targetFloor) / (valueCeiling - valueFloor) + targetFloor;
  return linearTarget;
}

void debounce() {
  delay(5);
}
