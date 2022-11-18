int fsrAnalogPin = 0; // FSR is connected to analog 0
int LEDpin = 11;      // connect Red LED to pin 11 (PWM pin)
int fsrReading;      // the analog reading from the FSR resistor divider
int LEDbrightness;
const int buttonPin = 2;
int buttonState = 0;
int tap_count = 0;
 
void setup(void) {
  Serial.begin(9600);   // We'll send debugging information via the Serial monitor
  pinMode(LEDpin, OUTPUT);
  pinMode(buttonPin, INPUT);
}
 
void loop(void) {
 buttonState = digitalRead(buttonPin);
  
  fsrReading = analogRead(fsrAnalogPin);
  //Serial.print("Analog reading = ");
  //Serial.println(fsrReading);

  Serial.print("Tap Count = ");
  Serial.println(tap_count);

 if (fsrReading > 0){
  tap_count = tap_count + 1;
  fsrReading = 0;
  //digitalWrite(LEDpin,HIGH);
  delay (200);
   }

  if (tap_count >= 2) {
    digitalWrite (LEDpin,HIGH);
    tap_count = 0;
  }
  // we'll need to change the range from the analog reading (0-1023) down to the range
  // used by analogWrite (0-255) with map!
  //LEDbrightness = map(fsrReading, 0, 1023, 0, 255);
  // LED gets brighter the harder you press
   
   
//  if (buttonState == HIGH) {
//    // turn LED on:
//    digitalWrite(LEDpin, HIGH);
//  } else {
//    // turn LED off:
//    //digitalWrite(LEDpin, LOW);
//    analogWrite(LEDpin, LEDbrightness);
//  }
// 
// 
//  //delay(100);
}
