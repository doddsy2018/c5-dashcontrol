
#include <ArduinoJson.h>

int SpeedsinPin1 =  10;    
int SpeedsinPin2=  11;    
int SpeedcosPin1 =  12;    
int SpeedcosPin2 =  13;  
  
int Speedsin1Value=0;
int Speedsin2Value=0;
int Speedcos1Value=0;
int Speedcos2Value=255;

int RPMsinPin1 =  6;    
int RPMsinPin2=  7;    
int RPMcosPin1 =  8;    
int RPMcosPin2 =  9; 

int RPMsin1Value=0;
int RPMsin2Value=0;
int RPMcos1Value=0;
int RPMcos2Value=255;


int FuelsinPin1 =  2;    
int FuelsinPin2=  3;    
int FuelcosPin1 =  4;    
int FuelcosPin2 =  5; 

int Fuelsin1Value=0;
int Fuelsin2Value=0;
int Fuelcos1Value=0;
int Fuelcos2Value=255;


// Max Angle limita\Angles
int speedLimitAngle=230;
int rpmLimitAngle=170;
int batteryLimitAngle=180;
int fuelLimitAngle=180;
int pressureLimitAngle=180;
int tempLimitAngle=90;

int speedMax=200;
int rpmMax=7000;
int batteryMax=18;
int fuelMax=100;
int pressureMax=80;
int tempmax=260;

int limit=230;

void setPosition(int sinPin1, int sinPin2, int cosPin1, int cosPin2, int sin1Value, int sin2Value, int cos1Value, int cos2Value, int smoothness=5); //Declare default function value

void setup() {

  // wait for hardware serial to appear
  while (!Serial);
  Serial.begin(115200); // opens serial port, sets data rate to 115200 bps

  pinMode(SpeedsinPin1, OUTPUT);    
  pinMode(SpeedsinPin2, OUTPUT);    
  pinMode(SpeedcosPin1, OUTPUT);    
  pinMode(SpeedcosPin2, OUTPUT); 
   
  pinMode(RPMsinPin1, OUTPUT);    
  pinMode(RPMsinPin2, OUTPUT);    
  pinMode(RPMcosPin1, OUTPUT);    
  pinMode(RPMcosPin2, OUTPUT);

  pinMode(FuelsinPin1, OUTPUT);    
  pinMode(FuelsinPin2, OUTPUT);    
  pinMode(FuelcosPin1, OUTPUT);    
  pinMode(FuelcosPin2, OUTPUT);

  int setupSpeed;
  int setupRPM;
  int setupFuel;

  // Cycle Gauge
  for (int i=0; i<=limit;i++){

    setupSpeed=checkGageLimit (i,speedLimitAngle); 
    setupRPM=checkGageLimit (i,rpmLimitAngle);
    setupFuel=checkGageLimit (i,fuelLimitAngle);
    
    calcValues(setupSpeed, &Speedsin1Value, &Speedsin2Value, &Speedcos1Value, &Speedcos2Value);
    setPosition(SpeedsinPin1,SpeedsinPin2, SpeedcosPin1, SpeedcosPin2, Speedsin1Value, Speedsin2Value, Speedcos1Value, Speedcos2Value );

    calcValues(setupRPM, &RPMsin1Value, &RPMsin2Value, &RPMcos1Value, &RPMcos2Value);
    setPosition(RPMsinPin1,RPMsinPin2, RPMcosPin1, RPMcosPin2, RPMsin1Value, RPMsin2Value, RPMcos1Value, RPMcos2Value );

    calcValues(setupFuel, &Fuelsin1Value, &Fuelsin2Value, &Fuelcos1Value, &Fuelcos2Value);
    setPosition(FuelsinPin1,FuelsinPin2, FuelcosPin1, FuelcosPin2, Fuelsin1Value, Fuelsin2Value, Fuelcos1Value, Fuelcos2Value );
    
  }
  delay(1000);
  for (int i=(limit-1); i>=0;i--){

    setupSpeed=checkGageLimit (i,speedLimitAngle); 
    setupRPM=checkGageLimit (i,rpmLimitAngle); 
    setupFuel=checkGageLimit (i,fuelLimitAngle); 
    
    calcValues(setupSpeed, &Speedsin1Value, &Speedsin2Value, &Speedcos1Value, &Speedcos2Value);
    setPosition(SpeedsinPin1,SpeedsinPin2, SpeedcosPin1, SpeedcosPin2, Speedsin1Value, Speedsin2Value, Speedcos1Value, Speedcos2Value); 

    calcValues(setupRPM, &RPMsin1Value, &RPMsin2Value, &RPMcos1Value, &RPMcos2Value);
    setPosition(RPMsinPin1,RPMsinPin2, RPMcosPin1, RPMcosPin2, RPMsin1Value, RPMsin2Value, RPMcos1Value, RPMcos2Value );

    calcValues(setupFuel, &Fuelsin1Value, &Fuelsin2Value, &Fuelcos1Value, &Fuelcos2Value);
    setPosition(FuelsinPin1,FuelsinPin2, FuelcosPin1, FuelcosPin2, Fuelsin1Value, Fuelsin2Value, Fuelcos1Value, Fuelcos2Value );
  }
  delay(1000);

  
  Serial.println("Setup Complete");
}

StaticJsonDocument<200> doc;
String message;
int count=0;

void loop() {
  
  count++;
  message = Serial.readStringUntil('\n');
  //Serial.println(String (count));
  //Serial.println(message);

  // Deserialize the JSON document
  DeserializationError error = deserializeJson(doc, message);
  
  // Test if parsing succeeds.
  if (error) {
    Serial.print(F("deserializeJson() failed: "));
    Serial.println(error.f_str());
  }
  else{
    Serial.println(message);
    float vspeed = doc["speed"]; // 200
    float rpm = doc["rpm"]; // 10000
    float fuel = doc["fuel"]; // 1000
    float oiltemp = doc["oiltemp"]; // 100
    float oilpres = doc["oilpres"]; // 100

    float gageAngle=0;

    // Convert reading to angles before calling calcValues
    vspeed=checkGageLimit (vspeed,speedMax); 
    gageAngle = 1.840542 + 1.795234*vspeed - 0.003303285*pow(vspeed, 2);
    calcValues(gageAngle, &Speedsin1Value, &Speedsin2Value, &Speedcos1Value, &Speedcos2Value);
    setPosition(SpeedsinPin1,SpeedsinPin2, SpeedcosPin1, SpeedcosPin2, Speedsin1Value, Speedsin2Value, Speedcos1Value, Speedcos2Value); 

    rpm=checkGageLimit (rpm,rpmMax);
    gageAngle = 0.1539103 + 0.02892786*rpm - 8.327303e-7*pow(rpm,2);
    calcValues(gageAngle, &RPMsin1Value, &RPMsin2Value, &RPMcos1Value, &RPMcos2Value);
    setPosition(RPMsinPin1,RPMsinPin2, RPMcosPin1, RPMcosPin2, RPMsin1Value, RPMsin2Value, RPMcos1Value, RPMcos2Value );

    fuel=checkGageLimit (fuel,fuelMax);
    gageAngle = 2.030739 + 2.194218*fuel - 0.004269457*pow(fuel,2);
    calcValues(gageAngle, &Fuelsin1Value, &Fuelsin2Value, &Fuelcos1Value, &Fuelcos2Value);
    setPosition(FuelsinPin1,FuelsinPin2, FuelcosPin1, FuelcosPin2, Fuelsin1Value, Fuelsin2Value, Fuelcos1Value, Fuelcos2Value );

    
    /*
    Serial.println("***************");
    Serial.print("speed=");
    Serial.println(vspeed);
    Serial.print("rpm=");
    Serial.println(rpm);
    Serial.print("fuel=");
    Serial.println(fuel);
    Serial.print("oiltemp=");
    Serial.println(oiltemp);
    Serial.print("oilpres=");
    Serial.println(oilpres);
    */

  }
}


int checkGageLimit (int askValue, int gageLimit){
    int returnValue=0;
    if (askValue>gageLimit) {
      returnValue=gageLimit;
    }
    else {
      returnValue=askValue;
    }
    return returnValue;
}


void setPosition(int sinPin1, int sinPin2, int cosPin1, int cosPin2, int sin1Value, int sin2Value, int cos1Value, int cos2Value, int smoothness){
  // Move air core gauge
  analogWrite(sinPin1, sin1Value);
  analogWrite(sinPin2, sin2Value);
  analogWrite(cosPin1, cos1Value);
  analogWrite(cosPin2, cos2Value);
  
  delay(smoothness);
}

int calcValues(int angle, int* sin1Value, int* sin2Value, int* cos1Value, int* cos2Value){
  float pi=3.14159;
  int scale=255;
  // Compute pin values based on position
  float positionRad=angle*pi/180;
  float sinCalc=scale*sin(positionRad);
  float cosCalc=scale*cos(positionRad-pi); // handle 180 phase shift
  if (sinCalc>0){
    *sin1Value=abs(sinCalc);
  }
  else{
    *sin1Value=0;
  }
  if (sinCalc<0){
    *sin2Value=abs(sinCalc);
  }
  else{
    *sin2Value=0;
  }
  
  if (cosCalc>0){
    *cos1Value=abs(cosCalc);
  }
  else{
    *cos1Value=0;
  }
  if (cosCalc<0){
    *cos2Value=abs(cosCalc);
  }
  else{
    *cos2Value=0;
  }
}
