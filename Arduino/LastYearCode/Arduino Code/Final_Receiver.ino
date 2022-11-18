
#include <Herkulex.h>
#include <SoftwareSerial.h>
#include <RH_ASK.h>
#include <SPI.h> // Not actually used but needed to compile
 

// Initialize Reciever
RH_ASK SPdriver(2000, 3, 4, 5); //Receiver module

// Tactile Feedback Pins
int fsrAnalogPin1 = 4; // FSR is connected to analog 0
int fsrAnalogPin2 = 5;
int fsrAnalogPin3 = 6;
int fsrAnalogPin4 = 7;
int LEDpin1 = 9; // connect Red LED to pin 11 (PWM pin)
int LEDpin2 = 8;
int LEDpin3 = 7;
int LEDpin4 = 6;
int fsrReading1;      // the analog reading from the FSR resistor divider
int fsrReading2;
int fsrReading3;
int fsrReading4;
int LEDbrightness1;
int LEDbrightness2;
int LEDbrightness3;
int LEDbrightness4;

//Motor names
int n1=0x1; //motor ID1
int n2=0x2; //motor ID2
int n3=0x3; //motor ID3
int n4=0x4; //motor ID4
int n5=0x5; //motor ID5

bool powerOn=false;

String str_deg;
double degZ;

void setup()
{
    
    Serial.begin(115200);    // Debugging only
    //uncomment later
    pinMode(LEDpin1, OUTPUT);
    pinMode(LEDpin2, OUTPUT);
    pinMode(LEDpin3, OUTPUT);
    pinMode(LEDpin4, OUTPUT);
    if (!SPdriver.init())
         Serial.println("init failed");
    Herkulex.begin(115200,10,11); //Open serial with rx=0 and tx=1
    //Purple:11 & Black:10
    Herkulex.reboot(n1); //Reboot first motor
    Herkulex.reboot(n2); //Reboot second motor
    Herkulex.reboot(n3); //Reboot third motor
    Herkulex.reboot(n4); //Reboot fourth motor
    Herkulex.reboot(n5); //Reboot fifth motor
    delay(500);
    Herkulex.initialize(); //initialize motor
    delay(200);
}

void loop() {
//uncomment later
  fsrReading1 = analogRead(fsrAnalogPin1);
  fsrReading2 = analogRead(fsrAnalogPin2);
  fsrReading3 = analogRead(fsrAnalogPin3);
  fsrReading4 = analogRead(fsrAnalogPin4);
  Serial.print("Analog reading = ");
  Serial.println(fsrReading1);
  Serial.println(fsrReading2);
  Serial.println(fsrReading3);
  Serial.println(fsrReading4);
 
  // we'll need to change the range from the analog reading (0-1023) down to the range
  // used by analogWrit e (0-255) with map!
  LEDbrightness1 = map(fsrReading1, 0, 1023, 0, 255);
  LEDbrightness2 = map(fsrReading2, 0, 1023, 0, 255);
  LEDbrightness3 = map(fsrReading3, 0, 1023, 0, 255);
  LEDbrightness4 = map(fsrReading4, 0, 1023, 0, 255);
  // LED gets brighter the harder you press
  analogWrite(LEDpin1, LEDbrightness1);
  analogWrite(LEDpin2, LEDbrightness2);
  analogWrite(LEDpin3, LEDbrightness3);
  analogWrite(LEDpin4, LEDbrightness4);
 
  delay(100);
  
  if (digitalRead(3)==HIGH){
    powerOn= !powerOn;
    delay(100);
  }
    
    Serial.print("PowerON:");
    Serial.println(powerOn);
    
    uint8_t buf[RH_ASK_MAX_MESSAGE_LEN];
    uint8_t buflen = sizeof(buf);


    if (SPdriver.recv(buf, &buflen)) // Non-blocking
    {
    
//    str_deg = String((char*)buf);
//    degZ = str_deg.toInt();
    
  int i;
      if(buf[0]=='0'){  //First Gesture Point
        Herkulex.moveOneAngle(n1, -20, 1120, LED_CYAN);
        Herkulex.moveOneAngle(n2, -150, 1120, LED_CYAN);
        Herkulex.moveOneAngle(n3, 60, 1120, LED_CYAN);
        Herkulex.moveOneAngle(n4, -30, 920, LED_CYAN);
        Serial.print("First Gesture");
        SPdriver.printBuffer("Got:", buf, buflen);
        delay(1000);
    
      }
      
      if(buf[0]=='1'){  //Second Gesture Close
        Herkulex.moveOneAngle(n1, -20, 1120, LED_GREEN);
        Herkulex.moveOneAngle(n2, 20, 1120, LED_GREEN);
        Herkulex.moveOneAngle(n3, 60, 1120, LED_GREEN);
        Herkulex.moveOneAngle(n4, -40, 920, LED_GREEN);
        Serial.print("Second Gesture");
        SPdriver.printBuffer("Got:", buf, buflen);
        delay(1000);
       
      }
      
      if(buf[0]=='2'){  //Third Gesture Tripod
        Herkulex.moveOneAngle(n1, -20, 1120, LED_BLUE);
        Herkulex.moveOneAngle(n2, 0, 1120, LED_BLUE);
        Herkulex.moveOneAngle(n3, -150, 1120, LED_BLUE);
        Herkulex.moveOneAngle(n4, -70, 920, LED_BLUE);
        Serial.print("Third Gesture");
        SPdriver.printBuffer("Got:", buf, buflen);
        delay(1000);
      }
      
      if(buf[0]=='3'){  //Fourth Gesture Pinch
        Herkulex.moveOneAngle(n1, -150, 1120, LED_BLUE);
        Herkulex.moveOneAngle(n2, 0, 1120, LED_CYAN);
        Herkulex.moveOneAngle(n3, -150, 1120, LED_BLUE);
        Herkulex.moveOneAngle(n4, -70, 920, LED_CYAN);
        Serial.print("Fourth Gesture");
        SPdriver.printBuffer("Got:", buf, buflen);
        delay(1000);
      }
      if(buf[0]=='4'){  //Fifth Gesture Palmer Neutral
Serial.print("Fifth Gesture");
        Herkulex.moveOneAngle(n1, -80, 1120, LED_GREEN);
        Herkulex.moveOneAngle(n2, -60, 1120, LED_BLUE);
        Herkulex.moveOneAngle(n3, -40, 1120, LED_GREEN);
        Herkulex.moveOneAngle(n4, -80, 1120, LED_BLUE);
        SPdriver.printBuffer("Got:", buf, buflen);
        delay(1000);
      }
      
      if(buf[5]="str_deg"){  //IMU
        String str_deg = ((char*)buf);
        str_deg = str_deg.substring(0,5);
        degZ = str_deg.toDouble();
        Serial.print("int: ");
        Serial.println(degZ);
        Serial.print("Str_deg: ");
        Serial.println(str_deg);
        Serial.print("IMU Activation");       
        
        if(powerOn == true){
          // Move Grip Strength Herkulex  motor with IMU
          //Serial.println("Move Angle to IMU angle");
          Herkulex.moveOneAngle(n5,degZ, 1120, LED_BLUE);
          delay(1000);
          //Serial.print("Get Herkulex Angle");
          //Serial.println(Herkulex.getAngle(n5));
          delay(1000);

        }else {
          //Serial.println("Move Angle to IMU angle");
          Herkulex.moveOneAngle(n5, -80, 1120, LED_GREEN);
          delay(1000);
          Serial.print("Get Herkulex Angle");
          Serial.println(Herkulex.getAngle(n5));
          delay(1000);
        }
     }
    }
 }
