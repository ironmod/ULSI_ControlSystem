/*  NASA USLI Spring 2013 
    Team Rocket Power
    University of Central Florida 
    Reverse Flow Airbrake Electronics Control System
          Matthew Harrison-Program Manager
          Sean DelVecchio-Technical Operations Manager
          Jeff Leins-Flight Process Engineer
          Monica Bertram-Performance Engineer
          Ron Hofer-Mechanical Design Engineer
          Steven Darrow-Aerospace Design Engineer
          Dr. Marcel Ilie-Senior Design Team Advisor
          Mr. Joshua Steele-Electrical Team Advisor
          Mr. Brandon Frazer-Electrical Team Advisor
*/

#define cs   8    //Arduino pin 8 to CS on SparkFun SD Module 
#include "math.h"
#include <SD.h>

//Arduino pin 13 to SCL pin TFT module; pin 11 to SDA pin TFT module
const int chipSelect=8; //SD card select 
unsigned long altNew =0L;        //altitude value being read from stratologger
unsigned long altOld =0L;        //previous altitude used for average
unsigned long altGround=0L;      //ground state altitude 
float newVel=0L;         //Current Velocity
float preVel=0L;         //Previous Velocity used for average
float avgVel=0;         //average velocity
long SD_close =0L;      //Altitude the doors closed at written to the SD card
long SD_open =0L;       //Altitude the doors opened at written to the SD card
int relayOpen_pos=2;     //relay module trigger Digital Pin 2
int relayOpen_neg=4;     //relay module trigger Digital Pin 3
int relayClose_pos=3;    //relay module trigger Digital Pin 4
int relayClose_neg=5;    //relay module trigger Digital Pin 5
float VtSq=291568;          // Vt^2    Vt^2=((2*m*g))/(Cd*p*A))   where m=40 lbs, g=32.2 ft/sec^2, Cd=0.6, p=0.002329 slugs/ft^3, A=0.19635 ft^2
float VtMult=4527.45;          // (Vt^2)/2g
float lnConst=0;         //ln((avgVel + VtSq)/VtSq)
float yMax=0;            //yMax=VtMult*ln(lnConst)+altNew
double lnValue=0;        //initiliaze lnValue to zero
int feedbackValue = 0;   //Value read in from the actuator feedback used for comparison
int feedbackPin = A0;    //actuator feedback (yellow wire) connected to pin A0 (other 2 wires are +5V and Ground)
int trigger_check =0;    //counter to see if doors have trigger open
int brake_altitude=5280; //predicted height to open the doors
int actuator_limit=850;  //how far to let the actuator extend 

 /* PreConditions: alt_value is the value the alitmeter is currently sitting at. During the
      startup process, it will calculate the (ft) the altimeter is sitting at above Sea Level
      depending on the location (130-160ft Orlando, 500-600ft Alabama)
  Post Conditions: Once the ft above sea level has been calculated, 
      the alitmeter value will be >0, breaking out of thvae first while loop and exectue the code
*/
void setup() 
{
  Serial.begin(9600);
  Serial.println("Rocket Power!");
  pinMode(relayOpen_pos, OUTPUT);
  pinMode(relayOpen_neg, OUTPUT);
  pinMode(relayClose_pos, OUTPUT);
  pinMode(relayClose_neg, OUTPUT);
  digitalWrite(relayOpen_pos, HIGH);
  digitalWrite(relayOpen_neg, HIGH);
  
  //Make sure doors are shut before takeoff
  digitalWrite(relayClose_pos, LOW);
  digitalWrite(relayClose_neg, LOW);
  delay(3000);
  digitalWrite(relayClose_pos, HIGH);
  digitalWrite(relayClose_neg, HIGH);
  
  altGround=altitudeRead();
  Serial.print("Ground State in (ft): ");
  Serial.println(altGround);

    //SD Card Reader initiliaztion
  Serial.print("Initializing SD card...");
     pinMode(10, OUTPUT);
  // see if the card is present and can be initialized:
  if (!SD.begin(chipSelect)) {
    Serial.println("Check SD Card. (corrupted or not inserted)");
    return;
  }
  Serial.println("SD card initialized."); 
  
  feedbackValue=analogRead(feedbackPin);
  Serial.println("Initial Actuator Feedback Value: ");
  Serial.println(feedbackValue);
  
  Serial.flush();
  }//end setup










