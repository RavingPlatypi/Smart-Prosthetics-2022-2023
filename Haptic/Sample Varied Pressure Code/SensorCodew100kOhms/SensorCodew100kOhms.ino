int pressureAnalogPin = 0;  //pin where our pressure pad is located.
int pressureReading;        //variable for storing our reading
int motorPin = 9 //connected to PMW pin 9

//Adjust these if required.
int noPressure = 5;        //max value for no pressure on the pad
int lightPressure = 100;   //max value for light pressure on the pad
int mediumPressure = 200;  //max value for medium pressure on the pad

void setup(void) {
  Serial.begin(9600);
}

void loop(void) {
  pressureReading = analogRead(pressureAnalogPin);

  Serial.print("Pressure Pad Reading = ");
  Serial.println(pressureReading);

  if (pressureReading < noPressure) {
    Serial.println(" - No pressure");
  } else if (pressureReading < lightPressure) {
    Serial.println(" - Light Pressure");
  } else if (pressureReading < mediumPressure) {
    Serial.println(" - Medium Pressure");
  } else {
    Serial.println(" - High Pressure");
  }
  delay(100);
}