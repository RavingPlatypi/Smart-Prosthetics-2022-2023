# Foot Control Code

## Description

Here contains the code for the wireless reciever and wireless transmitter. 
The parts used are nRFTransmitter, Arduino, Herkulex Servor Motor. The wireless transmitter is where the users will send inputs to the wireless reciever.
### Payload
The transmitter  and receiver  communicate  via  a protocol inspired by the MIDI protocol. Information  is sent  via a payload  of predetermined  size:  a 1x3 array  of integers.  It is up to the receiver  to read these packets  and respond  accordingly.
- (1,1) Type: Type of input: 0 for button, 1 for potentiometer and slider
- (1,2) Number: Number of the input (ie: if there are 5 buttons, number will  be one of  0 -4)
- (1,3) Value: The value of the input: HIGH or LOW for a button, 0-1023 for a potentiometer
## Requirements

The folders:
- RF24

are needed to run files "FootControlWirelessReciever" and "FootControlWirelessReciever".

# Important links and documentation
1.[nRF2401](https://nrf24.github.io/RF24/)


This is a test 