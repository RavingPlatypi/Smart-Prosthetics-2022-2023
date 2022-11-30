#include <Bounce2.h>
Bounce button = Bounce();

//Button tap
int count = 0;
int timer = 0;
boolean buttonPressed = false;

//Button hold
int countHold = 0;
int timerHold = 0;
int pressTime = 0;
boolean buttonHold = false;

const int timeLength = 5000;
const int holdTime = 2000;
const int lowerLimit = 800;

void setup() 
{
  Serial.begin(9600);

  button.attach(3, INPUT);
  button.interval(25);
  pinMode(6, OUTPUT);
  
  TCCR0A |= (1 << WGM01); //Set the CTC mode
  OCR0A = 0xF9;          //Set the value for 1ms
  TIMSK0 |= (1 << OCIE0A); //Set the interrupt request
  sei();                 //Enable interrupt
  TCCR0B |= (1 << CS01); //Set the prescale 1/64 clock
  TCCR0B |= (1 << CS00);
}

void loop() 
{
  //------------------------BUTTON HOLD CODE--------------------------------//
  
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
  // */
  
//------------------------BUTTON TAP CODE--------------------------------//
  /*
  button.update();
  if(button.fell())
  {
    count++;
    if(count == 1)
    {
      timer = 0;
    }
  }
  if((count == 3) && (timer < timeLength))
  {
    Serial.println("command");
    digitalWrite(6, HIGH);
    delay(500);
    digitalWrite(6, LOW);
    count = 0;
    timer = 0;
  }
  if(timer > 5000)
  {
    count = 0;
  } 
  Serial.print(count);
  Serial.print("\t");
  Serial.println(timer);
  */

  //------------------------BUTTON TAP and HOLD CODE--------------------------------//
 /*
  button.update();
  
  if (count == 0)
  {
    if(button.fell())
    {
      count++;
      if(count == 1)
      {
        timer = 0;
      }
    }
  }
  
  if(count == 1)
  {
    if(digitalRead(3) == HIGH)
    {
      timerHold = 0;
      while(digitalRead(3) == HIGH)
      {
        delay(1);
        timerHold++;
        timer++;
      }
      pressTime = timerHold;
      if((lowerLimit < pressTime) && (pressTime < holdTime))
      {
        buttonHold = true;
      } 
    }
    if((buttonHold == true) && timer < timeLength)
    {
      digitalWrite(6, HIGH);
      delay(500);
      digitalWrite(6, LOW);
      count = 0;
      buttonHold = false;
    }
  }
  if(timer > (timeLength + 1000))
  {
    count = 0;
  }
  Serial.print(count);
  Serial.print("\t");
  Serial.print(timer);
  Serial.print("\t");
  Serial.println(pressTime);
  // */
  
}

ISR(TIMER0_COMPA_vect)
{
  timer++;
  timerHold++;
}
