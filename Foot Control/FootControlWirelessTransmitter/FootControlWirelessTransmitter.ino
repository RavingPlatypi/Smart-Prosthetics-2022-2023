/**
 * Foot controller wireless tester for reading and printing signals received from transmitter
 * 
 * https://nrf24.github.io/RF24/
 */


#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

RF24 radio(7, 8); // CE, CSN     
uint8_t address[][6] = { "1Node", "2Node" };
bool radioNumber = 0; // 
int payload[3] = {0,0,1}; // [type, number, value]

int btnPins[] = {2};
const int numBtnPins = (sizeof(btnPins) / sizeof(btnPins[0]));

byte potPins[] = {A0, A1};
const int numPotPins = (sizeof(potPins) / sizeof(potPins[0]));
int potValues[numPotPins];

int testMotorPos = 0;



void setup() {
  Serial.begin(9600);

  for(int i = 0; i < numBtnPins; i++) {
    pinMode(btnPins[i], INPUT_PULLUP);
  }

  // initialize the transceiver on the SPI bus
  if (!radio.begin()) {
    Serial.println("radio hardware is not responding!!");
    while (1) {}  // hold in infinite loop
  }
  radio.setPALevel(RF24_PA_LOW);
  radio.openWritingPipe(address[radioNumber]);
  radio.stopListening();
  radio.setPayloadSize(sizeof(payload));
  Serial.println("transmitter started.");
  delay(1000);
}

void loop() {
  // Read button values and transmit.
  int btnState = digitalRead(btnPins[0]);
  transmitPayload(0,0,!btnState);
  

  // Read potentiometer values and transmit.
  for (int i = 0; i < numPotPins; i++){
    potValues[i] = analogRead(potPins[i]);
    int potValue = map(potValues[i], 512, 1024, 0, 120);
    Serial.println(potValue);
    transmitPayload(1, i, potValue);
  }
}


/**
Transmit command to reciever.
Parameters:
  type: type of input(0 = button, 1 = potentiometer)
  num: specifies which input (incase of multiple inputs)
  value: high or low for button, 0-1024 for potentiometer
*/  
bool transmitPayload(int type, int num, int value){
  int payload[3] = {type,num,value};
  bool sent = radio.write(&payload, sizeof(payload));
  return sent;
  Serial.println(
    sent ? "transmission sent" : "failed to send transmission"
  );
}

/*
Below is the function for testing a Servomotor.
Put this in the loop to make a Servomotor rotate 180 deg and back.
*/
void testServoMessage() {
  int payloadBtn[3] = {1,0,testMotorPos};
  bool sent = radio.write(&payloadBtn, sizeof(payload));
  testMotorPos++;
  if (testMotorPos >= 180) testMotorPos = 0;
}
