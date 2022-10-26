int fsrAnalogPin1 = A0;  // FSR is connected to analog A0
int potPin = A5;
int fsrReading1;     // the analog reading from the FSR resistor divider
 int motorPins[4] = { 9, 10, 11, 6 }; //connected on pins 9, 10, 11, and 6

void setup() {
  Serial.begin(9600);          // We'll send debugging information via the Serial monitor
  for (int i = 0; i < 4; i++) { //loop for pins 9,10,11, and 6
    pinMode(motorPins[i], OUTPUT);  //connected to pin 9
    digitalWrite(motorPins[i], HIGH);
  }
}

void loop() {
 // testMotors();
  fsrReading1 = analogRead(fsrAnalogPin1);
  //int potVal = analogRead(potPin);
 // int potValMapped = map(potVal, 0, 1023, 0, 255);
  //analogWrite(motorPins, potValMapped);
  // analogWrite(motorPins);
 // Serial.print("Motor V:");
 // Serial.println(potValMapped);
  //Serial.print("Analog reading = ");
  Serial.println(fsrReading1);  // prints out the reading for analogread
}


void testMotors() {
  for (float i = 0.0; i <= 1; i += 0.1) {
    float motorPWMVal = 255 * i;
    Serial.print("Motor V:");
    Serial.println(motorPWMVal);

    for (int i = 0; i < 4; i++) {
      analogWrite(motorPins[i], motorPWMVal);
    }
    delay(1000);
  }
}