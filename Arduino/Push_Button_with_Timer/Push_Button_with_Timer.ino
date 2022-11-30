#include <Bounce2.h>
Bounce bigToe = Bounce();
Bounce smallToe = Bounce();

//#define bigToe 6
//#define smallToes 10 

//Button tap
int count = 0;
long currTime = millis();
long timerHold = 0;
boolean buttonPressed = false;

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

  pinMode(2,OUTPUT); //Green Light--> Relaxed
  pinMode(3,OUTPUT); //Yellow Light-> Power
  pinMode(4,OUTPUT); //White Light--> Tripod
  pinMode(7,OUTPUT); //Blue Light---> Pinch
  pinMode(8,OUTPUT); //Red Light----> Point 
}

void loop() 
{
  //------------------------BUTTON HOLD CODE--------------------------------//
 /* 
  if(digitalRead(3) == HIGH)
  {
    timerHold = 0;
    while(digitalRead(3) == HIGH)
    {
      //pressTime = timerHold;
      delay(1);
      timerHold++;
    }
    pressTime = timerHold;
    buttonHold = true;
    
  }  

  if((buttonHold == true) && (pressTime < holdTime))
  {
    digitalWrite(6, HIGH);
    delay(500);
    digitalWrite(6, LOW);
    buttonHold = false;
  }
  
  Serial.print(timerHold);
  Serial.print("\t");
  Serial.println(pressTime);
*/
  
//------------------------BUTTON TAP CODE--------------------------------//
  /*
  bigToe.update();
  if(bigToe.fell())
  {
    count++;
    if(count == 1)
    {
      currTime = millis();
    }
  }
  if((count == 3) && ((millis() - currTime) < timeLength))
  {
    Serial.println("command");
    digitalWrite(2, HIGH);
    delay(500);
    digitalWrite(2, LOW);
    count = 0;
    currTime = millis();
  }
  if((millis() - currTime) > 5000)
  {
    currTime = millis();
  } 
  Serial.print(count);
  Serial.print("\t");
  Serial.println(millis() - currTime);
  */

  //------------------------BUTTON TAP and HOLD CODE--------------------------------//
 
  bigToe.update();
  
  if (count == 0)
  {
    if(bigToe.fell())
    {
      count++;
      if(count == 1)
      {
        currTime = millis();
      }
    }
  }
 
  if((count == 1) && bigToe.fell())
  {
    timerHold = millis();
    while(bigToe.fell())
    {
    }
    if((lowerLimit < (millis() - timerHold)) && ((millis() - timerHold) < holdTime))
    {
      buttonHold = true;
    } 
    
  } 
  
  if((buttonHold == true) && (millis() - currTime) < timeLength)
  {
    digitalWrite(2, HIGH);
    delay(500);
    digitalWrite(2, LOW);
    count = 0;
    buttonHold = false;
  }
  
  if((millis() - currTime) > (timeLength + 1000))
  {
    count = 0;
  }
  Serial.print(count);
  Serial.print("\t");
  Serial.print(millis() - currTime);
  Serial.print("\t");
  Serial.println(millis() - timerHold);
  
}
