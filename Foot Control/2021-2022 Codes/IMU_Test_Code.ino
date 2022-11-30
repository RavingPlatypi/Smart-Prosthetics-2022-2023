#include "Wire.h"
#include "I2Cdev.h"
#include "MPU6050.h"

MPU6050 accelgyro;
I2Cdev   I2C_M;

uint8_t buffer_m[6];

int16_t ax, ay, az;
int16_t gx, gy, gz;
int16_t   mx, my, mz;

float heading;
float tiltheading;

float Axyz[3];
float PrevAcc[3];
float Gxyz[3];
float PrevGyro[3];
float Mxyz[3];
float AyDiff = 0;
float GxDiff = 0; //Bending for the wrist
float GzDiff = 0; //Rotation of the wrist
long count;
bool systemActive = true;

#define sample_num_mdate  5000

volatile float mx_sample[3];
volatile float my_sample[3];
volatile float mz_sample[3];

static float mx_centre = 0;
static float my_centre = 0;
static float mz_centre = 0;

volatile int mx_max = 0;
volatile int my_max = 0;
volatile int mz_max = 0;

volatile int mx_min = 0;
volatile int my_min = 0;
volatile int mz_min = 0;


void setup() 
{
    Wire.begin();

    // initialize serial communication
    // (38400 chosen because it works as well at 8MHz as it does at 16MHz, but
    // it's really up to you depending on your project)
    Serial.begin(115200);
    pinMode(3, OUTPUT); //Green Light: System Active
    pinMode(9, OUTPUT); //Red Light: System Pause
    pinMode(8, OUTPUT); //Blue Light: Rotation Command
    pinMode(7, OUTPUT); //Yellow Light: Bending Command
    
    // initialize device
    while(!Serial);
    Serial.println("Initializing I2C devices...");
    accelgyro.initialize();

    // verify connection
    Serial.println("Testing device connections...");
    Serial.println(accelgyro.testConnection() ? "MPU6050 connection successful" : "MPU6050 connection failed");

    delay(1000);
    Serial.println("     ");

    for(int i = 0; i < 3; i++)
    {
      PrevAcc[i] = 0;
      PrevGyro[i] = 0;
    }
    
    Mxyz_init_calibrated();
}

void loop() 
{
    getAccel_Data();
    getGyro_Data();
    getCompassDate_calibrated(); // compass data has been calibrated here
    getHeading();        //before we use this function we should run 'getCompassDate_calibrated()' frist, so that we can get calibrated data ,then we can get correct angle .
    getTiltHeading();

    //digitalWrite(8, HIGH);
    
    Serial.println("Acceleration(g) of X,Y,Z:");
    Serial.print(Axyz[0]);
    Serial.print(",");
    Serial.print(Axyz[1]);
    Serial.print(",");
    //Serial.println(Axyz[2]);
    Serial.print(Axyz[2]);
    Serial.print("\t");
    if(abs(AyDiff) > 0.30) // Was 0.10 before
    {
      digitalWrite(9, HIGH); //Red Light
      digitalWrite(3, LOW); //Green Light
      systemActive = false;
    }
    else
    {
      Serial.println(" ");
      digitalWrite(3, HIGH); //Green Light
      digitalWrite(9, LOW); //Read Light
      systemActive = true;
    }
    Serial.println(" ");
    
    
    Serial.println("Gyro(degress/s) of X,Y,Z:");
    Serial.print(Gxyz[0]);
    Serial.print("\t");
    Serial.print(Gxyz[1]);
    Serial.print("\t");
    Serial.print(Gxyz[2]);
    Serial.println(" ");
    
    if((abs(Gxyz[0]) > 80) && systemActive) //abs(GyDiff) > 80)
    {
      digitalWrite(8, LOW); // Blue Light: Rotation
      digitalWrite(7, HIGH); // Yellow Light: Bending
    }
    else if(Gxyz[2] > 25 && systemActive)
    {
      digitalWrite(7, LOW); //Bending
      digitalWrite(8, HIGH); //Rotation
    }
    else
    {
      digitalWrite(7, LOW);
      digitalWrite(8, LOW);
    }
    
    count++;
    delay(300);
}

void getHeading(void) 
{
    heading = 180 * atan2(Mxyz[1], Mxyz[0]) / PI;
    if (heading < 0) 
    {
        heading += 360;
    }
}

void getTiltHeading(void) 
{
    float pitch = asin(-Axyz[0]);
    float roll = asin(Axyz[1] / cos(pitch));

    float xh = Mxyz[0] * cos(pitch) + Mxyz[2] * sin(pitch);
    float yh = Mxyz[0] * sin(roll) * sin(pitch) + Mxyz[1] * cos(roll) - Mxyz[2] * sin(roll) * cos(pitch);
    float zh = -Mxyz[0] * cos(roll) * sin(pitch) + Mxyz[1] * sin(roll) + Mxyz[2] * cos(roll) * cos(pitch);
    tiltheading = 180 * atan2(yh, xh) / PI;
    if (yh < 0) 
    {
        tiltheading += 360;
    }
}

void Mxyz_init_calibrated() 
{

    Serial.println(F("Before using 9DOF,we need to calibrate the compass frist,It will takes about 2 minutes."));
    Serial.print("  ");
    Serial.println(F("During  calibratting ,you should rotate and turn the 9DOF all the time within 2 minutes."));
    Serial.print("  ");
    Serial.println(F("If you are ready ,please sent a command data 'ready' to start sample and calibrate."));
    while (!Serial.find("ready"));
    Serial.println("  ");
    Serial.println("ready");
    Serial.println("Sample starting......");
    Serial.println("waiting ......");

    get_calibration_Data();

    Serial.println("     ");
    Serial.println("compass calibration parameter ");
    Serial.print(mx_centre);
    Serial.print("     ");
    Serial.print(my_centre);
    Serial.print("     ");
    Serial.println(mz_centre);
    Serial.println("    ");
}


void get_calibration_Data() 
{
    for (int i = 0; i < sample_num_mdate; i++) 
    {
        get_one_sample_date_mxyz();
        
        if (mx_sample[2] >= mx_sample[1]) 
        {
            mx_sample[1] = mx_sample[2];
        }
        if (my_sample[2] >= my_sample[1]) 
        {
            my_sample[1] = my_sample[2];    //find max value
        }
        if (mz_sample[2] >= mz_sample[1]) 
        {
            mz_sample[1] = mz_sample[2];
        }

        if (mx_sample[2] <= mx_sample[0]) 
        {
            mx_sample[0] = mx_sample[2];
        }
        if (my_sample[2] <= my_sample[0]) 
        {
            my_sample[0] = my_sample[2];    //find min value
        }
        if (mz_sample[2] <= mz_sample[0]) 
        {
            mz_sample[0] = mz_sample[2];
        }

    }

    mx_max = mx_sample[1];
    my_max = my_sample[1];
    mz_max = mz_sample[1];

    mx_min = mx_sample[0];
    my_min = my_sample[0];
    mz_min = mz_sample[0];

    mx_centre = (mx_max + mx_min) / 2;
    my_centre = (my_max + my_min) / 2;
    mz_centre = (mz_max + mz_min) / 2;

}

void get_one_sample_date_mxyz() 
{
    getCompass_Data();
    mx_sample[2] = Mxyz[0];
    my_sample[2] = Mxyz[1];
    mz_sample[2] = Mxyz[2];
}

void getAccel_Data(void) 
{
    if(count > 0)
    {
      PrevAcc[0] = Axyz[0];
      PrevAcc[1] = Axyz[1];
      PrevAcc[2] = Axyz[2];
    }
    accelgyro.getMotion9(&ax, &ay, &az, &gx, &gy, &gz, &mx, &my, &mz);
    Axyz[0] = (double) ax / 16384;
    Axyz[1] = (double) ay / 16384;
    Axyz[2] = (double) az / 16384;
    
    AyDiff = abs(Axyz[1]) - abs(PrevAcc[1]);
}

void getGyro_Data(void) 
{
    if(count > 0)
    {
      PrevGyro[0] = Gxyz[0];
      PrevGyro[1] = Gxyz[1];
      PrevGyro[2] = Gxyz[2];
    }
    accelgyro.getMotion9(&ax, &ay, &az, &gx, &gy, &gz, &mx, &my, &mz);
    Gxyz[0] = (double) gx * 250 / 32768;
    Gxyz[1] = (double) gy * 250 / 32768;
    Gxyz[2] = (double) gz * 250 / 32768;

    GzDiff = abs(Gxyz[2]) - abs(PrevGyro[2]);
    GxDiff = abs(Gxyz[0]) - abs(PrevGyro[0]);
}

void getCompass_Data(void) 
{
    I2C_M.writeByte(MPU9150_RA_MAG_ADDRESS, 0x0A, 0x01); //enable the magnetometer
    delay(10);
    I2C_M.readBytes(MPU9150_RA_MAG_ADDRESS, MPU9150_RA_MAG_XOUT_L, 6, buffer_m);

    mx = ((int16_t)(buffer_m[1]) << 8) | buffer_m[0] ;
    my = ((int16_t)(buffer_m[3]) << 8) | buffer_m[2] ;
    mz = ((int16_t)(buffer_m[5]) << 8) | buffer_m[4] ;

    Mxyz[0] = (double) mx * 1200 / 4096;
    Mxyz[1] = (double) my * 1200 / 4096;
    Mxyz[2] = (double) mz * 1200 / 4096;
}

void getCompassDate_calibrated() 
{
    getCompass_Data();
    Mxyz[0] = Mxyz[0] - mx_centre;
    Mxyz[1] = Mxyz[1] - my_centre;
    Mxyz[2] = Mxyz[2] - mz_centre;
}
