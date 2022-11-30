#include "LSM6DS3.h"
#include "Wire.h"

//Create a instance of class LSM6DS3
LSM6DS3 myIMU(I2C_MODE, 0x6A);    //I2C device address 0x6A

#define green 3
#define red 5
#define blue 8
#define yellow 7

bool systemActive = true;
bool commandActive = false;

void setup() {
    // put your setup code here, to run once:
    Serial.begin(115200);
    while (!Serial);
    //Call .begin() to configure the IMUs
    if (myIMU.begin() != 0) {
        Serial.println("Device error");
    } else {
        Serial.println("Device OK!");
    }

    pinMode(green, OUTPUT); //Green Light: System Active
    pinMode(red, OUTPUT); //Red Light: System Pause
    pinMode(blue, OUTPUT); //Blue Light: Rotation Command
    pinMode(yellow, OUTPUT); //Yellow Light: Bending Command
}

void loop() 
{
  
    //Accelerometer
    Serial.print("\nAccelerometer:\n");
    Serial.print(" X1 = ");
    Serial.println(myIMU.readFloatAccelX(), 4);
    Serial.print(" Y1 = ");
    Serial.println(myIMU.readFloatAccelY(), 4);
    Serial.print(" Z1 = ");
    Serial.println(myIMU.readFloatAccelZ(), 4);

    if((myIMU.readFloatAccelY() < -0.01) && commandActive == false)
    //((myIMU.readFloatAccelX() > 0.7) && commandActive == false)
    {
      digitalWrite(red, HIGH); //Red Light
      digitalWrite(green, LOW); //Green Light
      systemActive = false;
    }
    else
    {
      digitalWrite(green, HIGH); //Green Light
      digitalWrite(red, LOW); //Red Light
      systemActive = true;
    }
   
    //Gyroscope
    Serial.print("\nGyroscope:\n");
    Serial.print(" X1 = ");
    Serial.println(myIMU.readFloatGyroX(), 4);
    Serial.print(" Y1 = ");
    Serial.println(myIMU.readFloatGyroY(), 4);
    Serial.print(" Z1 = ");
    Serial.println(myIMU.readFloatGyroZ(), 4);

    if((myIMU.readFloatGyroX() > 30) && systemActive) 
    {
      digitalWrite(blue, LOW); // Blue Light: Rotation
      digitalWrite(yellow, HIGH); // Yellow Light: Bending
      commandActive = true;
    }
    else if((myIMU.readFloatGyroZ() < -80) && systemActive)
    {
      digitalWrite(yellow, LOW); //Bending
      digitalWrite(blue, HIGH); //Rotation
      commandActive = true;
    }
    else
    {
      digitalWrite(yellow, LOW);
      digitalWrite(blue, LOW);
      commandActive = false;
    }

    /*
    //Thermometer
    Serial.print("\nThermometer:\n");
    Serial.print(" Degrees C1 = ");
    Serial.println(myIMU.readTempC(), 4);
    Serial.print(" Degrees F1 = ");
    Serial.println(myIMU.readTempF(), 4);
  */
    delay(300);
}
