   //////////////////////////////////////////////
  //        HALL EFFECT SENSOR DEMO           //
 //                                          //
//           http://www.educ8s.tv           //
/////////////////////////////////////////////

int hallSensorPin = 2;     
int ledPin =  13;    
int state = 0;          

void setup() {
  Serial.begin(9600);
  pinMode(ledPin, OUTPUT);      
  pinMode(hallSensorPin, INPUT);     
}

void loop(){
  
  state = digitalRead(hallSensorPin);
  

  if (state == LOW) {        
    digitalWrite(ledPin, HIGH); 
    Serial.println('1'); 
  } 
  else {
    digitalWrite(ledPin, LOW); 
    Serial.println('0'); 
  }
}
