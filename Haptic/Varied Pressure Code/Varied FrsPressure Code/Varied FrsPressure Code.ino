int pressureAnalogPin[] = {A0, A1, A2, A3};  //pin where our pressure pad is located.
int pressureReading;        //variable for storing our reading
int motorPins[4] = { 9, 10, 11, 6 };  //connected on pins 9, 10, 11, and 6

//Adjust these if required.
int noPressure = 100;        //max value for no pressure on the pad
int highestPressure = 600;  //max value for medium pressure on the pad
int maxMotorIntensity = 900; //max value for highest pressure on the pad

void setup() {
  Serial.begin(115200);               // We'll send debugging information via the Serial monitor
  for (int i = 0; i < 4; i++) {     //loop for pins 9,10,11, and 6
    pinMode(motorPins[i], OUTPUT);  //connected to pin 9
  }
}

void loop() {
  for (int i = 0; i < 4; i++) {
    processMotor(i);
  }
}

void processMotor(int i) {
  pressureReading = analogRead(pressureAnalogPin[i]);
  Serial.println(pressureReading);
  if (pressureReading < noPressure) pressureReading = 0;

  int pressureValMapped = map(pressureReading, 0, highestPressure, 0, maxMotorIntensity);
  analogWrite(motorPins[i], pressureValMapped);
}

void testMotors() {
  for (float i = 0.0; i <= 1; i += 0.1) {
    float motorPWMVal = 255 * i;
    Serial.print("Motor V:");
    Serial.println(motorPWMVal);

    for (int i = 0; i < 4; i++) {
      analogWrite(motorPins[i], motorPWMVal);
    }
    delay(100);
  }
}
