# Haptic Feedback Code



## Description

Here includes the code for testing the motor vibration and force pressure sensor reading. The parts used are Arduino Uno card, 4 vibration motors, 1 A101 force sensor, and as an option, a potentiometer.

## Usage

To read force sensor code, you must comment out the lines that have testMotors, analogWrite, potVal, and Analog reading. To have the motor vibration in a cycle, you must uncomment the lines previously commented EXCEPT Analog reading. To run the code with a potentiometer, you must comment out the testMotors so it won't run through a loop.

## Wiring Diagram
![Wiring Diagram](https://imgur.com/a/bCV7BrQ) 


##### Notes
This code is only prototype testing. 

