

void setup() {
  Serial.begin(9600);
}
 
void loop() {
 int reading = analogRead(A2);  
 float voltage = reading * 3.3 / 1024.0;
 Serial.print(voltage); Serial.println(" volts");
 float temperatureC = (voltage - 0.5) * 100 ;
 Serial.print(temperatureC); Serial.println(" degrees C");
 delay(1000);
}
