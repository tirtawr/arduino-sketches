
// Button related globals
String buttonStateUp = "UP";
String buttonStateDown = "UP";
String buttonStateLeft = "UP";
String buttonStateRight = "UP";

void setup() {
  Serial.begin(9600);
  pinMode(2,INPUT_PULLUP);
  pinMode(3,INPUT_PULLUP);
  pinMode(4,INPUT_PULLUP);
  pinMode(5,INPUT_PULLUP);
}

void loop() {
  // put your main code here, to run repeatedly:
  checkButtonStates();
  delay(4);
}

void onButtonPressed(String button) {
  Serial.println(button);
}

void checkButtonStates() {
  checkButtonStateUp();
  checkButtonStateDown();
  checkButtonStateLeft();
  checkButtonStateRight();
}

void checkButtonStateUp() {
  int sensorVal = digitalRead(5);
  String newButtonStateUp;
  if (sensorVal == HIGH) {
    newButtonStateUp = "UP";
  } else {
    newButtonStateUp = "DOWN";
  }
  if (buttonStateUp == "DOWN" && newButtonStateUp == "UP") {
    onButtonPressed("BUTTON_UP");
  }
  buttonStateUp = newButtonStateUp;
}

void checkButtonStateDown() {
  int sensorVal = digitalRead(2);
  String newButtonStateDown;
  if (sensorVal == HIGH) {
    newButtonStateDown = "UP";
  } else {
    newButtonStateDown = "DOWN";
  }
  if (buttonStateDown == "DOWN" && newButtonStateDown == "UP") {
    onButtonPressed("BUTTON_DOWN");
  }
  buttonStateDown = newButtonStateDown;
}

void checkButtonStateLeft() {
  int sensorVal = digitalRead(4);
  String newButtonStateLeft;
  if (sensorVal == HIGH) {
    newButtonStateLeft = "UP";
  } else {
    newButtonStateLeft = "DOWN";
  }
  if (buttonStateLeft == "DOWN" && newButtonStateLeft == "UP") {
    onButtonPressed("BUTTON_LEFT");
  }
  buttonStateLeft = newButtonStateLeft;
}

void checkButtonStateRight() {
  int sensorVal = digitalRead(3);
  String newButtonStateRight;
  if (sensorVal == HIGH) {
    newButtonStateRight = "UP";
  } else {
    newButtonStateRight = "DOWN";
  }
  if (buttonStateRight == "DOWN" && newButtonStateRight == "UP") {
    onButtonPressed("BUTTON_RIGHT");
  }
  buttonStateRight = newButtonStateRight;
}
