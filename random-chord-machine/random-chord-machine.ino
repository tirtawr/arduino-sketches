#include <MIDIUSB.h>
#include <pitchToNote.h>
const int buttonMainPin = 9;
const int buttonUpPin = 12;
const int buttonDownPin = 11;
const int buttonLeftPin = 13;
const int buttonRightPin = 14;
const int ledPin = 6;

// the intervals in a major and natural minor scale:
int majorScale[] = {2, 2, 1, 2, 2, 2, 1};
int naturalMinorScale[] = {2, 1, 2, 2, 1, 2, 2};
// an array to hold the final notes of the scale:
int scale[8];

// start with middle C:
int tonic = pitchC4;
// note to play:
int noteValue = tonic;

// previous state of the button:
int lastbuttonMainState = HIGH;
int lastbuttonUpState = HIGH;
int lastbuttonDownState = HIGH;
int lastbuttonLeftState = HIGH;
int lastbuttonRightState = HIGH;

boolean isTonePlaying = false;

void setup() {
  Serial.begin(9600);
  pinMode(buttonMainPin, INPUT_PULLUP);
  pinMode(buttonUpPin, INPUT_PULLUP);
  pinMode(buttonDownPin, INPUT_PULLUP);
  pinMode(buttonLeftPin, INPUT_PULLUP);
  pinMode(buttonRightPin, INPUT_PULLUP);
  pinMode(ledPin, OUTPUT);
  // fill the scale array with the scale you want:
  // start with the initial note:
  scale[0] = tonic;
  int note = scale[0];
  // iterate over the intervals, adding each to the next note
  // in the scale. You can change major to naturalMinor
  // if you want that kind of scale instead:
  for (int n = 0; n < 7; n++) {
    note = note + majorScale[n];
    scale[n + 1] = note;
  }
}

void loop() {
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

void onButtonMainPressed() {
  digitalWrite(ledPin, HIGH);
  // pick a random note in the scale:
  noteValue = scale[random(8)];
  sendNoteOnEvent(noteValue);
  isTonePlaying = true;
  onButtonUpPressed();
}

void onButtonMainReleased() {
  digitalWrite(ledPin, LOW);
  sendNoteOffEvent(noteValue);
  isTonePlaying = false;
  onButtonUpReleased();
}

void onButtonUpPressed() {
  if (isTonePlaying) {
    sendNoteOnEvent(noteValue);
    sendNoteOnEvent(noteValue + 4);
    sendNoteOnEvent(noteValue + 7);
  }
}

void onButtonUpReleased() {
  sendNoteOffEvent(noteValue);
  sendNoteOffEvent(noteValue + 4);
  sendNoteOffEvent(noteValue + 7);
}

void onButtonDownPressed() {
  if (isTonePlaying) {
    sendNoteOnEvent(noteValue);
    sendNoteOnEvent(noteValue + 3);
    sendNoteOnEvent(noteValue + 7);
  }
}

void onButtonDownReleased() {
  sendNoteOffEvent(noteValue);
  sendNoteOffEvent(noteValue + 3);
  sendNoteOffEvent(noteValue + 7);
}

void onButtonLeftPressed() {
  if (isTonePlaying) {
    sendNoteOnEvent(noteValue);
    sendNoteOnEvent(noteValue + 3);
    sendNoteOnEvent(noteValue + 6);
  }
}

void onButtonLeftReleased() {
  sendNoteOffEvent(noteValue);
  sendNoteOffEvent(noteValue + 3);
  sendNoteOffEvent(noteValue + 6);
}

void onButtonRightPressed() {
  if (isTonePlaying) {
    sendNoteOnEvent(noteValue);
    sendNoteOnEvent(noteValue + 4);
    sendNoteOnEvent(noteValue + 8);
  }
}

void onButtonRightReleased() {
  sendNoteOffEvent(noteValue);
  sendNoteOffEvent(noteValue + 4);
  sendNoteOffEvent(noteValue + 8);
}

// send a 3-byte midi message
void sendMidiCommand(byte cmd, byte data1, byte  data2) {
  // First parameter is the event type (top 4 bits of the command byte).
  // Second parameter is command byte combined with the channel.
  // Third parameter is the first data byte
  // Fourth parameter second data byte

  midiEventPacket_t midiMsg = {cmd >> 4, cmd, data1, data2};
  MidiUSB.sendMIDI(midiMsg);
}

void sendNoteOnEvent(byte note) {
  sendMidiCommand(0x90, note, 0x7F);
}

void sendNoteOffEvent(byte note) {
  sendMidiCommand(0x80, note, 0);
}

void debounce() {
  delay(5);
}
