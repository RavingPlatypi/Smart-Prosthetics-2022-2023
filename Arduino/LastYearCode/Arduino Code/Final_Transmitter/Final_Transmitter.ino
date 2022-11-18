
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <RH_ASK.h>
#include <SPI.h> // Not actually used but needed to compile

// Initialize Transmitter
RH_ASK SPdriver(2000, 4, 3, 5); 

 Initialize BNO055 gyro sensor
Adafruit_BNO055 bno(55);
float degreeZ;

String str_deg;

 bool sensorVal1 = false;
 bool sensorVal2 = false;
 bool sensorVal3 = false;
    int but1 = 6;
    int but2 = 7;
    int but3 = 8;
   
void setup()
{
    Serial.begin(115200);	  // Debugging only
    if (!SPdriver.init())
         Serial.println("init failed");

    
    pinMode(but1,INPUT); //Button 1
    pinMode(but2,INPUT); //Button 2
    pinMode(but3,INPUT);  //Button 3

    Trap application into endless loop, if gyro sensor fails
  if (!bno.begin()) {
    Serial.println("Error initializing gyro sensor.\nPlease check connection!");
    while(1);
  }
  delay(200);
  bno.setExtCrystalUse(true);
}

void loop()
{
   sensorVal1 = LOW;
   sensorVal2 = LOW;
   sensorVal3 = LOW;

    
    //Buttons
   if (digitalRead(but3)==HIGH){
    sensorVal3 = !sensorVal3;
    delay(100);
  }

  if (digitalRead(but2)==HIGH){
    sensorVal2= !sensorVal2;
    delay(100);
  }

  if (digitalRead(but1)==HIGH){
    sensorVal1= !sensorVal1;
    delay(100);
  }
    
    if (sensorVal1 == HIGH && sensorVal2 == LOW && sensorVal3 == LOW) { //Send Gesture 1 
      digitalWrite(but1, HIGH);
      const char *msg = "0";
      SPdriver.send((uint8_t *)msg, strlen(msg));
      SPdriver.waitPacketSent();
      Serial.print("msg:");
      Serial.println(msg);
      delay(200);
    }
    
    if (sensorVal1 == LOW && sensorVal2 == HIGH && sensorVal3 == LOW) { //Send Gesture 2
      digitalWrite(but2, HIGH);
      const char *msg = "1";
      SPdriver.send((uint8_t *)msg, strlen(msg));
      SPdriver.waitPacketSent();
      Serial.print("msg:");
      Serial.println(msg);
      delay(200);
    }
    
    if (sensorVal1 == LOW && sensorVal2 == LOW && sensorVal3 == HIGH) { //Send Gesture 3
      digitalWrite(but3, HIGH);
      const char *msg = "2";
      SPdriver.send((uint8_t *)msg, strlen(msg));
      SPdriver.waitPacketSent();
      Serial.print("msg:");
      Serial.println(msg);
      delay(200);
    }
    
    if (sensorVal1 == HIGH && sensorVal2 == HIGH && sensorVal3 == LOW){ ////Send Gesture 4
      const char *msg ="3";
      SPdriver.send((uint8_t *)msg, strlen(msg));
      SPdriver.waitPacketSent();
      Serial.print("msg:");
      Serial.println(msg);
      delay(200);
    }
    if (sensorVal1 == LOW && sensorVal2 == HIGH && sensorVal3 == HIGH ){ ////Send Gesture 5
      const char *msg ="4";
      SPdriver.send((uint8_t *)msg, strlen(msg));
      SPdriver.waitPacketSent();
      Serial.print("msg:");
      Serial.println(msg);
      delay(200);
    }
     if (sensorVal1 == HIGH && sensorVal2 == HIGH && sensorVal3 == HIGH ){ ////Send Gesture 5
       // Read gyro sensor data
       sensors_event_t event;
       bno.getEvent(&event);

      // Get current degrees for Z axis
      degreeZ = event.orientation.z;
      str_deg=String(degreeZ);
      //String A=str_deg;
      const char *msg = str_deg.c_str();
      SPdriver.send((uint8_t *)msg, strlen(msg));
      SPdriver.waitPacketSent();
      Serial.print("msg:");
      Serial.println(msg);
      delay(200);
    
}
      Serial.print("SensorVal1:"); //Check button1 status
      Serial.println(sensorVal1);
      Serial.print("SensorVal2:"); //Check button2 status
      Serial.println(sensorVal2);
      Serial.print("SensorVal3:"); //Check button3 status
      Serial.println(sensorVal3);
      delay(20);
Serial.print("String:");
Serial.println(str_deg);
}
