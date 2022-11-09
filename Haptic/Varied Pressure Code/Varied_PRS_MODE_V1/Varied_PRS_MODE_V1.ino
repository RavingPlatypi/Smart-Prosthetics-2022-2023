int pressureAnalogPin[] = { A0, A1, A2, A3 };  //pin where our pressure pad is located.
int pressureReading;                           //variable for storing our reading
int motorPins[4] = { 9, 10, 11, 6 };           //connected on pins 9, 10, 11, and 6

int btn = 2;       // 
int counter = 0;   // 
int btnState = 0;  // 

//Adjust these if required.
int noPressure = 100;         //max value for no pressure on the pad
int highestPressure = 900;    //max value for medium pressure on the pad
const int maxMotorIntensity = 900;  //max value for highest pressure on the pad

int motorIntensity;

void setup() {
  Serial.begin(115200);             // We'll send debugging information via the Serial monitor
  pinMode(btn, INPUT_PULLUP);       // 
  for (int i = 0; i < 4; i++) {     //loop for pins 9,10,11, and 6
    pinMode(motorPins[i], OUTPUT);  //connected to pin 9
  }
}

void loop() {
  for (int i = 0; i < 4; i++) {
    processMotor(i);
  }
  // Puga Start
  btnState = digitalRead(btn);
  if (btnState == LOW) {
    counter++;
    if (counter > 3) counter = 0;

    Serial.println(counter);
    delay(500);

    if (counter == 1) {
      Serial.println("Low Vibration");
      motorIntensity = 0.33 * maxMotorIntensity;
    }
    if (counter == 2) {
      Serial.println("Medium Vibration");
      motorIntensity = 0.66 * maxMotorIntensity;
    }
    if (counter == 3) {
      Serial.println("High Vibration");
      motorIntensity = 900;
    }
  }
}

void processMotor(int i) {
  pressureReading = analogRead(pressureAnalogPin[i]);
  Serial.println(pressureReading);
  if (pressureReading < noPressure) pressureReading = 0;

  int pressureValMapped = map(pressureReading, 0, highestPressure, 0, motorIntensity);
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