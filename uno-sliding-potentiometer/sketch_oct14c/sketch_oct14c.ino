int potentiometerPin = 0;    // select the input pin for the potentiometer
int ledPin = LED_BUILTIN;   // select the pin for the LED
int val = 0;       // variable to store the value coming from the sensor
int sensorValue = 0;
int outputValue = 0; 

void setup() {
  pinMode(ledPin, OUTPUT);  // declare the ledPin as an OUTPUT
  Serial.begin(9600);
}

void loop() {
  sensorValue = analogRead(potentiometerPin);
  outputValue = map(sensorValue, 0, 1023, 0, 25);
  outputValue = outputValue * 4;  // lower resolution for smoothness
  Serial.print("sensor = ");
  Serial.print(outputValue);
  Serial.print("\n");
  delay(200);
}
