#include <Ultrasonic.h>

Ultrasonic ultrasonic(8, 7); //trig, echo
float distance;
bool distanceRead = false;

int timer = 0;
int count = 0;

const int distanceDesired = 5;
void setup()
{
  Serial.begin(9600);
  TCCR0A |= (1 << WGM01); //Set the CTC mode
  OCR0A = 0xF9;          //Set the value for 1ms
  TIMSK0 |= (1 << OCIE0A); //Set the interrupt request
  sei();                 //Enable interrupt
  TCCR0B |= (1 << CS01); //Set the prescale 1/64 clock
  TCCR0B |= (1 << CS00);
}

void loop()
{
  distance = ultrasonic.read(CM);

  if (distance > distanceDesired)
  {
    
      while(distance > 5)
      {
      distance = ultrasonic.read(CM);
      }
      count++; //count = count + 1;
      if (count == 1)
      {
        timer = 0;
      }
      /*
    if (distanceRead == false)
    {
      distanceRead = true;
    }
    if (distanceRead == true)
    {
      while(distance > distanceDesired)
      {
        distance = ultrasonic.read(CM);
      }
      distanceRead = false;
      count++;
    }*/
  }

  if ((count == 3) && (timer < 4000))
  {
    Serial.println("command");

    count = 0;
    timer = 0;
  }
  if (timer > 10000)
  {
    count = 0;
    timer = 0;
  }
  Serial.print(count);
  Serial.print("\t");
  Serial.print(distance);
  Serial.print("\t");
  Serial.println(timer);
}

ISR(TIMER0_COMPA_vect)
{
  timer++;
}
