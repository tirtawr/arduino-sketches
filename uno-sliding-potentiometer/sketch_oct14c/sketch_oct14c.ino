int potentiometerPin = 3;    // select the input pin for the potentiometer
int ledPin = 5;   // select the pin for the LED
int val = 0;       // variable to store the value coming from the sensor
int sensorValue = 0;
int outputValue = 0; 

void setup() {
  pinMode(ledPin, OUTPUT);  // declare the ledPin as an OUTPUT
  Serial.begin(9600);
}

void loop() {
  sensorValue = analogRead(potentiometerPin);
  outputValue = map(sensorValue, 0, 1023, 0, 100);
  Serial.print("sensor = ");
  Serial.print(outputValue);
  Serial.print("\n");
  delay(200);
}
