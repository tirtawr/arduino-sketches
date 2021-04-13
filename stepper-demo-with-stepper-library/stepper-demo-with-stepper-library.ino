#include <Stepper.h>

#define STEPPER_PIN_1 1
#define STEPPER_PIN_2 2
#define STEPPER_PIN_3 3
#define STEPPER_PIN_4 4
#define STEPPER_STEPS_PER_REVOLUTION 720
//#define STEPPER_STEPS_PER_REVOLUTION 2038

#define BUTTON_PIN 0

#define DEBOUNCE_VALUE 2

int prevButtonState = 0;
int currButtonState = 0;

int currStep = 0;

Stepper sunStepper = Stepper(STEPPER_STEPS_PER_REVOLUTION, STEPPER_PIN_1, STEPPER_PIN_3, STEPPER_PIN_2, STEPPER_PIN_4);

void setup() {
  Serial.begin(9600);
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  sunStepper.setSpeed(15);
}

void loop() {
  
  currButtonState = digitalRead(BUTTON_PIN);
  if (currButtonState == LOW && prevButtonState == HIGH) {
    Serial.println("PRESSED");
    currStep = 0;
    rotateTo(270);
  }
  prevButtonState = currButtonState;
  sunStepper.step(720);
  delay(2000);
}

// Rotate needle to a specific point in the clock face
// 0 degree is left side of the horizon
void rotateTo(int degrees) {
  
  int desiredStep = ((STEPPER_STEPS_PER_REVOLUTION / 360) * degrees);
  int stepsNeeded = desiredStep - currStep;
  sunStepper.step(stepsNeeded);
  currStep = desiredStep;
}
