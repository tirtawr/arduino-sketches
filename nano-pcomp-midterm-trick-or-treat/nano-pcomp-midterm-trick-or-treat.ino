#define STEPPER_1_PIN_1 8 //Digital
#define STEPPER_1_PIN_2 7 //Digital
#define STEPPER_1_PIN_3 6 //Digital
#define STEPPER_1_PIN_4 5 //Digital
#define STEPPER_2_PIN_1 12 //Digital
#define STEPPER_2_PIN_2 11 //Digital
#define STEPPER_2_PIN_3 10 //Digital
#define STEPPER_2_PIN_4 9 //Digital
#define BUTTON_PIN 2 //Digital

#define EVENT_BUTTON_PRESSED "BUTTON_PRESSED"
#define EVENT_DISPENSE_FINISHED "DISPENSE_FINISHED"
#define EVENT_DISPENSE_TRICK "DISPENSE_TRICK"
#define EVENT_DISPENSE_TREAT "DISPENSE_TREAT"

#define STEPPER_DELAY_VALUE 2
#define STEPPER_ONE_ROTATION_STEPS 3000

//Button related globals
int buttonState = 0;
int lastButtonState = 0;

//Stepper motor related globals
int stepperMotor1StepNumber = 0;
int stepperMotor2StepNumber = 0;

void setup() {
  pinMode(STEPPER_1_PIN_1, OUTPUT);
  pinMode(STEPPER_1_PIN_2, OUTPUT);
  pinMode(STEPPER_1_PIN_3, OUTPUT);
  pinMode(STEPPER_1_PIN_4, OUTPUT);

  pinMode(STEPPER_2_PIN_1, OUTPUT);
  pinMode(STEPPER_2_PIN_2, OUTPUT);
  pinMode(STEPPER_2_PIN_3, OUTPUT);
  pinMode(STEPPER_2_PIN_4, OUTPUT);

  pinMode(BUTTON_PIN, INPUT);

  Serial.begin(9600);
}

void loop() {
  // TODO: Rotate motor when event is received from browser
  if (wasButtonTriggered()) {
    Serial.println(EVENT_BUTTON_PRESSED);
//    turnStepperMotor1();
//    turnStepperMotor2();
  }

  if (Serial.available() > 0) {   // see if there's incoming serial data
    String command = Serial.readStringUntil("\r\n"); // read it
    if (command == EVENT_DISPENSE_TRICK) {
      turnStepperMotor1();
    } else if (command == EVENT_DISPENSE_TREAT) {
      turnStepperMotor2();
    }
  }
  
  delay(2);
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

void turnStepperMotor1 () {
  for (int i = 0; i < STEPPER_ONE_ROTATION_STEPS; i++) {
    delay(STEPPER_DELAY_VALUE);
    oneStep(true, stepperMotor1StepNumber, STEPPER_1_PIN_1, STEPPER_1_PIN_2, STEPPER_1_PIN_3, STEPPER_1_PIN_4);
    stepperMotor1StepNumber++;
    if (stepperMotor1StepNumber > 3)
    {
      stepperMotor1StepNumber = 0;
    }
  }
}

void turnStepperMotor2() {
  for (int i = 0; i < STEPPER_ONE_ROTATION_STEPS; i++) {
    delay(STEPPER_DELAY_VALUE);
    oneStep(true, stepperMotor2StepNumber, STEPPER_2_PIN_1, STEPPER_2_PIN_2, STEPPER_2_PIN_3, STEPPER_2_PIN_4);
    stepperMotor2StepNumber++;
    if (stepperMotor2StepNumber > 3)
    {
      stepperMotor2StepNumber = 0;
    }
  }
}

void oneStep(bool isClockwise, int stepNumber, int pin1, int pin2, int pin3, int pin4) {
  if(isClockwise) {
    switch(stepNumber) {
      case 0:
        digitalWrite(pin1, HIGH);
        digitalWrite(pin2, LOW);
        digitalWrite(pin3, LOW);
        digitalWrite(pin4, LOW);
        break;
      case 1:
        digitalWrite(pin1, LOW);
        digitalWrite(pin2, HIGH);
        digitalWrite(pin3, LOW);
        digitalWrite(pin4, LOW);
        break;
      case 2:
        digitalWrite(pin1, LOW);
        digitalWrite(pin2, LOW);
        digitalWrite(pin3, HIGH);
        digitalWrite(pin4, LOW);
        break;
      case 3:
        digitalWrite(pin1, LOW);
        digitalWrite(pin2, LOW);
        digitalWrite(pin3, LOW);
        digitalWrite(pin4, HIGH);
        break;
    } 
  } else {
    switch(stepNumber) {
      case 0:
        digitalWrite(pin1, LOW);
        digitalWrite(pin2, LOW);
        digitalWrite(pin3, LOW);
        digitalWrite(pin4, HIGH);
        break;
      case 1:
        digitalWrite(pin1, LOW);
        digitalWrite(pin2, LOW);
        digitalWrite(pin3, HIGH);
        digitalWrite(pin4, LOW);
        break;
      case 2:
        digitalWrite(pin1, LOW);
        digitalWrite(pin2, HIGH);
        digitalWrite(pin3, LOW);
        digitalWrite(pin4, LOW);
        break;
      case 3:
        digitalWrite(pin1, HIGH);
        digitalWrite(pin2, LOW);
        digitalWrite(pin3, LOW);
        digitalWrite(pin4, LOW);
    } 
  }
}
