int LED1 = 8;
int LED2 = 9;

int button1 = 2;
int button2 = 3;

long buttonTimer = 0;
long buttonTime = 250;

boolean buttonActive = false;
boolean longPressActive = false;

boolean button1Active = false;
boolean button2Active = false;

boolean LED1Active = true;
boolean LED2Active = true;


int TestBegin = 0;
int testcount=0;


void setup() {
  
  Serial.begin(9600);
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);

  pinMode(button1, INPUT);
  pinMode(button2, INPUT);

  Serial.println("System Ready:");
  
}


void loop() {

  if (Serial.available() > 0){
  
  TestBegin = Serial.read();
  testcount = testcount +1;
  Serial.print("\t");
  Serial.println();
  Serial.println(testcount);}

  if (digitalRead(button1) == HIGH) {
      digitalWrite(LED1, HIGH);
    if (buttonActive == false) {

      buttonActive = true;
      buttonTimer = millis();

    }

        button1Active = true;

  }

  if (digitalRead(button2) == HIGH) {
      digitalWrite(LED2, HIGH);
    if (buttonActive == false) {

      buttonActive = true;
      buttonTimer = millis();

    }

    button2Active = true;

  }

  if ((buttonActive == true) && (millis() - buttonTimer > buttonTime) && (longPressActive == false)) {

    longPressActive = true;

    if ((button1Active == true) && (button2Active == true)) {
 
      digitalWrite(LED1, LED1Active);
      Serial.print("AllH");
      delay(50);

    } else if((button1Active == true) && (button2Active == false)) {

      digitalWrite(LED1, LED1Active);
      Serial.print("BTH");
      delay(50);

    } else {

      digitalWrite(LED2, LED2Active);
      Serial.print("4TH");
      delay(50);

    }

  }


  if ((buttonActive == true) && (digitalRead(button1) == LOW) && (digitalRead(button2) == LOW)) {

    if (longPressActive == true) {

      longPressActive = false;

    } else {

      if ((button1Active == true) && (button2Active == true)) {

        LED1Active = !LED1Active;
        digitalWrite(LED1, LED1Active);
        Serial.print("ALL");
        delay(50);

      } else if ((button1Active == true) && (button2Active == false)) {

        LED1Active = !LED1Active;
        digitalWrite(LED1, LED1Active);
        Serial.print("BT");
        delay(50);
        

      } else {

        LED2Active = !LED2Active;
        digitalWrite(LED2, LED2Active);
        Serial.print("4T");
        delay(50);

      }

    }

    buttonActive = false;
    button1Active = false;
    button2Active = false;
    digitalWrite(LED2, LOW);
    digitalWrite(LED1, LOW);


  }

}
