#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <Bounce2.h>
#include <LSM6DS3.h>
#include <Wire.h>

LSM6DS3 myIMU(I2C_MODE, 0x6A);    //I2C device address 0x6A
float aX, aY, aZ, gX, gY, gZ;
const float accelerationThreshold = 0.5; // threshold of significant in G's

float prevAcc = 1.48;
float aSum = 0;
int activeCount = 0;

RF24 radio(1, 0); // CE, CSN         
const byte address[6] = "00001";     //Byte of array representing the address. This is the address where we will send the data. This should be same on the receiving side.

Bounce bigToe = Bounce();
Bounce smallToe = Bounce();

int bigCount = 0;
int smallCount = 0;

long currTime = millis();
long timerHold = 0;
long runTime = millis();
long pressTime = 0;

boolean isPoint = true;
boolean buttonHold = false;
boolean systemActive = true;

const long holdLowLimit = 800;
const long holdUpLimit = 3000;
const long timeDelay = 1500;
const long timeLimit = 2000;

void setup() 
{
  Serial.begin(115200);
  //pinMode(button_pin, INPUT);

  smallToe.attach(3, INPUT); //Right Button: X/Y 3
  bigToe.attach(2, INPUT); //Left Button: A/B 2
  smallToe.interval(25);
  bigToe.interval(25);

  if (myIMU.begin() != 0) 
  {
    Serial.println("Device error");
  } 
  else 
  {
    Serial.println("aX,aY,aZ,gX,gY,gZ");
  }
  
  radio.begin();                  //Starting the Wireless communication
  radio.openWritingPipe(address); //Setting the address where we will send the data
  radio.setPALevel(RF24_PA_MIN);  //You can set it as minimum or maximum depending on the distance between the transmitter and receiver.
  radio.stopListening();          //This sets the module as transmitter
}
/*
A: Power
B: Pinch
C: Point
D: Tripod
E: Relaxed
*/
void loop()
{
  int bending = myIMU.readFloatGyroZ();
  int rotating = myIMU.readFloatGyroY();
  bigToe.update();
  smallToe.update();

  aX = myIMU.readFloatAccelX();
  aY = myIMU.readFloatAccelY();
  aZ = myIMU.readFloatAccelZ();
  
  // sum up the absolutes
  aSum = fabs(aX) + fabs(aY) + fabs(aZ);

  if ((abs(aSum-prevAcc) >= accelerationThreshold) && systemActive) 
  {
    systemActive = false;
    activeCount = 0;
  }
  else
  {
    activeCount++;
    if(activeCount > 3)
    {
      systemActive = true;
    }
  }

  if(systemActive)
  {
    //IMU Code 
    if (bending > 100)
    {
      const char text[] = "F";
      radio.write(&text, sizeof(text));  
      delay(1000); 
      const char text2[] = "K";
      radio.write(&text2, sizeof(text2));
    }
    else if (bending < -100)
    {
      const char text[] = "R";
      radio.write(&text, sizeof(text)); 
      delay(1000);
      const char text2[] = "K";
      radio.write(&text2, sizeof(text2));
    }
    
    if (rotating > 100)
    {
      const char text[] = "I";
      radio.write(&text, sizeof(text));  
      delay(1000);
      const char text2[] = "K";
      radio.write(&text2, sizeof(text2));  
    }
    else if (rotating < -100)
    {
      const char text[] = "J";
      radio.write(&text, sizeof(text)); 
      delay(1000);
      const char text2[] = "K";
      radio.write(&text2, sizeof(text2)); 
    }
  
  //Push Button
    if((bigToe.fell()) || (digitalRead(2) == HIGH))
    {
      runTime = millis();
      isPoint = false;
      bigCount++;
      if(bigCount == 1)
      {
        currTime = millis();
      }
      //hold code
      timerHold = millis();
      while(digitalRead(2) == HIGH)
      {
      }
      pressTime = millis() - timerHold;
      if((pressTime > holdLowLimit) && (pressTime < holdUpLimit))
      {
        buttonHold = true; 
      }
      else 
      {
        buttonHold = false;
      }
    }
    else if(smallToe.fell() || digitalRead(3) == HIGH)
    {
      runTime = millis();
      smallCount++;
      if((smallCount == 1) && (isPoint == true))
      {
        currTime = millis();
      }
      timerHold = millis();
      while(digitalRead(3) == HIGH)
      {
      }
      pressTime = millis() - timerHold;
      if((pressTime > holdLowLimit) && (pressTime < holdUpLimit))
      {
        buttonHold = true; 
      }
      else 
      {
        buttonHold = false;
      }
    }
    
    if((millis() - runTime) > timeDelay)
    {
      if((bigCount == 2) && ((millis() - currTime) < timeLimit))
      {
        const char text[] = "D";
        radio.write(&text, sizeof(text));
        bigCount = 0;
        currTime = millis();
        isPoint = true;
      }
      else if((bigCount == 1) && (smallCount == 1))
      {
        const char text[] = "B";
        radio.write(&text, sizeof(text));
        bigCount = 0;
        smallCount = 0;
        currTime = millis();
        isPoint = true;
      }
      else if((bigCount == 1) && ((millis() - currTime) < timeLimit) && (buttonHold == false)
          && (pressTime < holdLowLimit))
      {
        const char text[] = "E";
        radio.write(&text, sizeof(text));
        bigCount = 0;
        currTime = millis();
        isPoint = true;
      }
      else if((smallCount == 1) && ((millis() - currTime) < timeLimit) && (isPoint == true)
          && (buttonHold == false) && (pressTime < holdLowLimit))
      {
        const char text[] = "C";
        radio.write(&text, sizeof(text));
        smallCount = 0;
        currTime = millis();
      }
      
      //hold code
      else if((buttonHold == true) && (bigCount == 1))
      {
        const char text[] = "A";
        radio.write(&text, sizeof(text));
        buttonHold = false;
      }
      else if((buttonHold == true) && (smallCount == 1))
      {
        const char text[] = "S"; //Scissors
        radio.write(&text, sizeof(text));
        buttonHold = false;
      }
      else if((millis() - currTime) > (timeLimit + 500))
      {
        bigCount = 0;
        smallCount = 0;
        isPoint = true;
      } 
    }
  }
}
