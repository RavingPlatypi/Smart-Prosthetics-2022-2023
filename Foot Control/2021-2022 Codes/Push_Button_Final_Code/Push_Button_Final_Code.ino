#include <Bounce2.h>
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

const long holdLowLimit = 800;
const long holdUpLimit = 1200;
const long timeDelay = 1500;
const long timeLimit = 2000;

void setup() 
{
  Serial.begin(115200);

  smallToe.attach(10, INPUT); //Right Button: X/Y 3
  bigToe.attach(6, INPUT); //Left Button: A/B 2
  smallToe.interval(25);
  bigToe.interval(25);

  pinMode(2,OUTPUT); //Green Light--> Relaxed: A
  pinMode(3,OUTPUT); //Yellow Light-> Power: B
  pinMode(4,OUTPUT); //White Light--> Tripod: AA
  pinMode(7,OUTPUT); //Blue Light---> Pinch: AX
  pinMode(8,OUTPUT); //Red Light----> Point: X
}

void loop() 
{
  bigToe.update();
  smallToe.update();

  if((bigToe.fell()) || (digitalRead(6) == HIGH))
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
    while(digitalRead(6) == HIGH)
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
  else if(smallToe.fell())
  {
    runTime = millis();
    smallCount++;
    if((smallCount == 1) && (isPoint == true))
    {
      currTime = millis();
    }
  }
  
  if((millis() - runTime) > timeDelay)
  {
    if((bigCount == 2) && ((millis() - currTime) < timeLimit))
    {
      Serial.println("command");
      digitalWrite(4, HIGH); //white light: Tripod
      delay(500);
      digitalWrite(4, LOW);
      bigCount = 0;
      currTime = millis();
      isPoint = true;
    }
    else if((bigCount == 1) && ((millis() - currTime) < timeLimit) && (buttonHold == false)
        && (pressTime < holdLowLimit))
    {
      digitalWrite(2, HIGH); //green light: relaxed
      delay(500);
      digitalWrite(2, LOW);
      bigCount = 0;
      currTime = millis();
      isPoint = true;
    }
    else if((smallCount == 1) && ((millis() - currTime) < timeLimit) 
              && (isPoint == true))
    {
      digitalWrite(8, HIGH); //Red light: Point
      delay(500);
      digitalWrite(8, LOW);
      smallCount = 0;
      currTime = millis();
    }
    else if((bigCount == 1) && (smallCount == 1))
    {
      digitalWrite(7, HIGH);
      delay(500);
      digitalWrite(7, LOW);
      bigCount = 0;
      smallCount = 0;
      currTime = millis();
      isPoint = true;
    }
    //hold code
    else if((buttonHold == true) && (bigCount == 1))
    {
      digitalWrite(3, HIGH);
      delay(500);
      digitalWrite(3, LOW);
      buttonHold = false;
    }
    else if((millis() - currTime) > (timeLimit + 500))
    {
      bigCount = 0;
      smallCount = 0;
      isPoint = true;
    } 
  }
  Serial.print(bigCount);
  Serial.print("\t");
  Serial.print(smallCount);
  Serial.print("\t");
  Serial.print(millis() - currTime);
  Serial.print("\t");
  Serial.println(pressTime);
}
