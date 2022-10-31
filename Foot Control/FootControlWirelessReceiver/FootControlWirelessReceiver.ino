/**
 * Foot controller wireless tester for reading and printing signals received from transmitter
 * 
 * https://nrf24.github.io/RF24/
 */


// #include <nRF24L01.h>
#include <SPI.h>
#include <RF24.h>
#include <printf.h>
#include <Servo.h>

RF24 radio(7, 8);  // CE, CSN reders to the pins of nRF transmitter
uint8_t address[][6] = { "1Node", "2Node" };
bool radioNumber = 0;
int payload[3] = {0, 0, 0}; // [Type, Number, Value]

int ledPins[] = {2, 3};
int numLedPins = (sizeof(ledPins) / sizeof(ledPins[0]));
int motorPins[] = {3, 5, 6, 9}; // Pins set for each finger
int activeMotorIndex = 1; // Finger currently moving 
const int numMotorPins = (sizeof(motorPins) / sizeof(motorPins[0]));
Servo servos[numMotorPins];

void setup() {
  Serial.begin(9600);

  for(int i = 0; i < numLedPins; i++) {
    pinMode(ledPins[i], OUTPUT);
  }
  for(int i = 0; i < numMotorPins; i++) {
    servos[i].attach(motorPins[i]);
  }

  // Initialize the transceiver on the SPI bus
  if (!radio.begin()) {
    Serial.println("Radio hardware is not responding!");
    while (1) {}  // Hold in infinite loop
  }
  radio.setPALevel(RF24_PA_LOW); // All radio settings must match the transmitter
  radio.setPayloadSize(sizeof(payload));
  radio.openReadingPipe(1, address[radioNumber]);
  radio.startListening();
  Serial.println("Receiver started!");
  delay(1000);
}

void loop() {
  uint8_t pipe;
  if (radio.available(&pipe)) {              // is there a payload? get the pipe number that recieved it
    uint8_t bytes = radio.getPayloadSize();  // get the size of the payload
    Serial.print("Received ");
    radio.read(&payload, sizeof(payload));             // fetch payload from FIFO
    Serial.print(payload[0]);
    Serial.print(payload[1]);
    Serial.print(payload[2]);
    Serial.println();  // print the payload's value

    if(payload[0] == 0) {
      // onButtonSignal(payload[1], payload[2]);
      if(payload[2] == 0) return; 
      activeMotorIndex++;
      if (activeMotorIndex > numMotorPins) {
        activeMotorIndex = 0;
      }
    } else {
      if(payload[1] == 1) return;
      onMotorSignal(activeMotorIndex, payload[2]);
    }
  }
}