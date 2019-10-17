#define STEPPER_1_PIN_1 5
#define STEPPER_1_PIN_2 6
#define STEPPER_1_PIN_3 7
#define STEPPER_1_PIN_4 8
#define STEPPER_2_PIN_1 9
#define STEPPER_2_PIN_2 10
#define STEPPER_2_PIN_3 11
#define STEPPER_2_PIN_4 12
#define POTENTIOMETER_PIN 1

#define STEPPER_DELAY_VALUE 10
#define STEPPER_ONE_ROTATION_STEPS 1000

int sensorValue = 0;
int mappedSensorValue = 0;
int delayValue = 2;

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
}

void loop() {
  // TODO: Setup serial com with browser
  // TODO: Setup button press detection
  // TODO: Send button event to browser when button pressed
  // TODO: Rotate motor when event is received from browser
  delay(2);
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