#include <MIDIUSB.h>
#include <pitchToNote.h>
const int buttonMainPin = 9;
const int buttonUpPin = 12;
const int buttonDownPin = 11;
const int buttonLeftPin = 13;
const int buttonRightPin = 14;
const int ledPin = 6;

//int majorChord[] = {0, 4, 7};
//int minorChord[] = {0, 3, 7};
//int diminishedChord[] = {0, 3, 6};
//int augmentedChord[] = {0, 4, 8};
//int majorSeventhChord[] = {0, 4, 8, 11};
int chordSize = 3;
int chord[] = {0, 4, 7};

// start with middle C:
//int tonic = pitchC4;
int tonic = 0;
// note to play:
int baseNote = tonic;

// previous state of the button:
int lastbuttonMainState = HIGH;
int lastbuttonUpState = HIGH;
int lastbuttonDownState = HIGH;
int lastbuttonLeftState = HIGH;
int lastbuttonRightState = HIGH;
unsigned int previousMillis;
unsigned int interval = 100;

boolean isTonePlaying = false;
int currentOffsetIndex = 0;

void setup() {
  Serial.begin(9600);
  pinMode(buttonMainPin, INPUT_PULLUP);
  pinMode(buttonUpPin, INPUT_PULLUP);
  pinMode(buttonDownPin, INPUT_PULLUP);
  pinMode(buttonLeftPin, INPUT_PULLUP);
  pinMode(buttonRightPin, INPUT_PULLUP);
  pinMode(ledPin, OUTPUT);
}

void loop() {
  checkButtonStates();

  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    if (digitalRead(buttonMainPin) == LOW) {
      playNote();
    }
  }
}

void playNote() {
  sendNoteOnEvent(baseNote + getNextOffset(chord));
}

void onButtonMainPressed() {
  digitalWrite(ledPin, HIGH);
}

void onButtonMainReleased() {
  digitalWrite(ledPin, LOW);
}

void onButtonUpPressed() {
  baseNote++;
  guardNoteTopThreshold();
}

void onButtonUpReleased() { }

void onButtonDownPressed() {
  baseNote--;
  guardNoteBottomThreshold();
}

void onButtonDownReleased() { }

void onButtonLeftPressed() {
  baseNote -= 12;
  guardNoteBottomThreshold();
}

void onButtonLeftReleased() { }

void onButtonRightPressed() {
  baseNote += 12;
  guardNoteTopThreshold();
}

void onButtonRightReleased() { }

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
  if (currentOffsetIndex >= chordSize) currentOffsetIndex = 0;
  return arr[currentOffsetIndex];
}

void sendMidiCommand(byte cmd, byte data1, byte  data2) {
  midiEventPacket_t midiMsg = {cmd >> 4, cmd, data1, data2};
  MidiUSB.sendMIDI(midiMsg);
}

void sendNoteOnEvent(byte note) {
  sendMidiCommand(0x90, note, 0x7F);
}

void sendNoteOffEvent(byte note) {
  sendMidiCommand(0x80, note, 0);
}

void guardNoteBottomThreshold() {
  if(baseNote < 0) baseNote = 0;
}

void guardNoteTopThreshold() {
  if(baseNote + 12 > 127) baseNote = 115;
}

void debounce() {
  delay(5);
}