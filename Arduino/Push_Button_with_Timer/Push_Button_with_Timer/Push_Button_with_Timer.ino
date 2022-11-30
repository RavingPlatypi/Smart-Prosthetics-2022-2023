#include <Bounce2.h>
Bounce bigToe = Bounce();
Bounce smallToe = Bounce();

//#define bigToe 6
//#define smallToes 10 

//Button tap
int bigCount = 0;
int smallCount = 0;
long currTime = millis();
long timerHold = 0;
long runTime = millis();
boolean buttonPressed = false;
boolean isPoint = true;

//Button hold
int countHold = 0;
long pressTime = 0;
boolean buttonHold = false;

const int timeLength = 5000;
const int holdTime = 2000;
const int lowerLimit = 800;

void setup() 
{
  Serial.begin(115200);

  smallToe.attach(10, INPUT); //Right Button: X/Y
  bigToe.attach(6, INPUT); //Left Button: A/B
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
  //------------------------BUTTON HOLD CODE--------------------------------//
 /* 
  if(digitalRead(3) == HIGH)
  {
    timerHold = millis();
    while(digitalRead(3) == HIGH)
    {
    }
    pressTime = millis() - timerHold;
    buttonHold = true;
  }  

  if((buttonHold == true) && (timerHold < holdTime))
  {
    digitalWrite(6, HIGH);
    delay(500);
    digitalWrite(6, LOW);
    buttonHold = false;
  }

*/
  
//------------------------BUTTON TAP CODE--------------------------------//
  
  bigToe.update();
  smallToe.update();

  if(bigToe.fell())
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
    buttonHold = true;
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
  
  if((millis() - runTime) > 1500)
  {
    if((bigCount == 2) && ((millis() - currTime) < 2000))
    {
      Serial.println("command");
      digitalWrite(4, HIGH);
      delay(500);
      digitalWrite(4, LOW);
      bigCount = 0;
      currTime = millis();
      isPoint = true;
    }
    else if((bigCount == 1) && ((millis() - currTime) < 2000))
    {
      digitalWrite(2, HIGH);
      delay(500);
      digitalWrite(2, LOW);
      bigCount = 0;
      currTime = millis();
      isPoint = true;
    }
    else if((smallCount == 1) && ((millis() - currTime) < 2000) 
              && (isPoint == true))
    {
      digitalWrite(8, HIGH);
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
    else if((buttonHold == true) && (timerHold < holdTime))
    {
      digitalWrite(3, HIGH);
      delay(500);
      digitalWrite(3, LOW);
      buttonHold = false;
    }
    else if((millis() - currTime) > 2500)
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
  Serial.println(millis() - currTime);
}
