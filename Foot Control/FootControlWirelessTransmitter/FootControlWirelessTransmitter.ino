/**
 * Foot controller wireless tester for reading and printing signals received from transmitter
 * 
 * https://nrf24.github.io/RF24/
 */


#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

RF24 radio(7, 8); // CE, CSN refers to the nRF transmitter    
uint8_t address[][6] = { "1Node", "2Node" };
bool radioNumber = 0; // 
int payload[3] = {0,0,1}; // [Type, Index, Value] refer to readme: https://github.com/RavingPlatypi/Smart-Prosthetics-2022-2023/blob/main/Foot%20Control/README.md

int btnPins[] = {2}; // Joystick button 
const int numBtnPins = (sizeof(btnPins) / sizeof(btnPins[0])); // Purpose to count the number of buttons, incase we want to add more

byte potPins[] = {A0, A1}; // For the joystick inputs in: Vrx / Vry
const int numPotPins = (sizeof(potPins) / sizeof(potPins[0])); //  Purpose to count the number of potentiometers
int potValues[numPotPins];

void setup() {
  Serial.begin(9600);

  for(int i = 0; i < numBtnPins; i++) {
    pinMode(btnPins[i], INPUT_PULLUP);
  }

  // Initialize the transceiver on the SPI bus
  if (!radio.begin()) {
    Serial.println("Radio hardware is not responding!");
    while (1) {}  // hold in infinite loop
  }
  // Need every radio.(method) in order for nRF to work eve
  radio.setPALevel(RF24_PA_LOW);
  radio.openWritingPipe(address[radioNumber]);
  radio.stopListening(); // This method is not used, but still must be included
  radio.setPayloadSize(sizeof(payload));
  Serial.println("Transmitter started!");
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
