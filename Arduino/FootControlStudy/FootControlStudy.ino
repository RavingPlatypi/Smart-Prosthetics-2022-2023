const int BUTTON1 = 2;
const int BUTTON2 = 3;
const int LED1 = 8;
const int LED2 = 9;
int BUTTONstate1 = 0;
int BUTTONstate2 = 0;
int TestBegin = 0;
int testcount=0;

void setup()
{
Serial.begin(9600);
  
  pinMode(BUTTON1, INPUT);
  pinMode(BUTTON2, INPUT);
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  Serial.println("System Ready:");
}

void loop()
{
  if (Serial.available() > 0){
  TestBegin = Serial.read();
  testcount = testcount +1;
  Serial.println();
Serial.println(testcount);}

  BUTTONstate1 = digitalRead(BUTTON1);
  if (BUTTONstate1 == HIGH)
  {
    digitalWrite(LED1, HIGH);
    Serial.print("BT");
  } 
  else{
    digitalWrite(LED1, LOW);
  }
  BUTTONstate2 = digitalRead(BUTTON2);
  if (BUTTONstate2 == HIGH)
  {
    digitalWrite(LED2, HIGH);
    Serial.print("4T");
  } 
  else{
    digitalWrite(LED2, LOW);
  }
  delay(200);
  TestBegin = -1;
}
