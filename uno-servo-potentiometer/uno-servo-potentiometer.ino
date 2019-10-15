#include <Servo.h>      // include the servo library

int potentiometerPin = 0;    // select the input pin for the potentiometer
int ledPin = LED_BUILTIN;   // select the pin for the LED
int val = 0;       // variable to store the value coming from the sensor
int sensorValue = 0;
int outputValue = 0; 
Servo servoMotor;       // creates an instance of the servo object to control a servo
int servoPin = 11;       // Control pin for servo motor

void setup() {
  pinMode(ledPin, OUTPUT);  // declare the ledPin as an OUTPUT
  Serial.begin(9600);
  servoMotor.attach(servoPin);
}

void loop() {
  sensorValue = analogRead(potentiometerPin);
  outputValue = map(sensorValue, 0, 1023, 0, 179 / 5); // lower resolution for smoothness
  outputValue = outputValue * 5;  // lower resolution for smoothness

  int servoAngle = outputValue;

//  int servoAngle = 15;

  servoMotor.write(servoAngle);
  
  Serial.print("servoAngle = ");
  Serial.print(servoAngle);
  Serial.print("\n");
  delay(5);
}
