#define BUTTON_PIN 2 //Digital

#define EVENT_BUTTON_PRESSED "BUTTON_PRESSED_GREEN"

//Button related globals
int buttonState = 0;
int lastButtonState = 0;


void setup() {
  pinMode(BUTTON_PIN, INPUT);

  Serial.begin(9600);
}

void loop() {
    if (wasButtonTriggered()) {
    Serial.println(EVENT_BUTTON_PRESSED);
  }
}

boolean wasButtonTriggered() {
  buttonState = digitalRead(BUTTON_PIN);
  boolean triggered = false;
  // compare the buttonState to its previous state
  if (buttonState != lastButtonState) {
    if (buttonState == HIGH) {
      // button went from not_pressed to pressed
    } else {
      // button went from pressed to not_pressed
      triggered = true;
    }
  }
  lastButtonState = buttonState;
  return triggered;
}
