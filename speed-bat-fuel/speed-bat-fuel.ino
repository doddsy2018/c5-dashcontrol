
#include <ArduinoJson.h>

int SpeedSinPin1 =  10;    
int SpeedSinPin2=  11;    
int SpeedCosPin1 =  12;    
int SpeedCosPin2 =  13;  
  
int SpeedSin1Value=0;
int SpeedSin2Value=0;
int SpeedCos1Value=0;
int SpeedCos2Value=255;

int BatterySinPin1 =  6;    
int BatterySinPin2=  7;    
int BatteryCosPin1 =  8;    
int BatteryCosPin2 =  9; 

int BatterySin1Value=0;
int BatterySin2Value=0;
int BatteryCos1Value=0;
int BatteryCos2Value=255;

int FuelSinPin1 =  2;    
int FuelSinPin2=  3;    
int FuelCosPin1 =  4;    
int FuelCosPin2 =  5; 

int FuelSin1Value=0;
int FuelSin2Value=0;
int FuelCos1Value=0;
int FuelCos2Value=255;

// Max Angle limita\Angles
int speedLimitAngle=230;
int batteryLimitAngle=180;
int fuelLimitAngle=180;

int speedMax=200;
int batteryMax=18;
int fuelMax=100;

int limit=230;

void setPosition(int SinPin1, int SinPin2, int CosPin1, int CosPin2, int Sin1Value, int Sin2Value, int Cos1Value, int Cos2Value, int smoothness=5); //Declare default function value

void setup() {

  // wait for hardware serial to appear
  while (!Serial);
  Serial.begin(115200); // opens serial port, sets data rate to 115200 bps

  pinMode(SpeedSinPin1, OUTPUT);    
  pinMode(SpeedSinPin2, OUTPUT);    
  pinMode(SpeedCosPin1, OUTPUT);    
  pinMode(SpeedCosPin2, OUTPUT); 
   
  pinMode(BatterySinPin1, OUTPUT);    
  pinMode(BatterySinPin2, OUTPUT);    
  pinMode(BatteryCosPin1, OUTPUT);    
  pinMode(BatteryCosPin2, OUTPUT);

  pinMode(FuelSinPin1, OUTPUT);    
  pinMode(FuelSinPin2, OUTPUT);    
  pinMode(FuelCosPin1, OUTPUT);    
  pinMode(FuelCosPin2, OUTPUT);

  int setupSpeed;
  int setupBattery;
  int setupFuel;

  // Cycle Gauge
  for (int i=0; i<=limit;i++){

    setupSpeed=checkGageLimit (i,speedLimitAngle); 
    setupBattery=checkGageLimit (i,batteryLimitAngle);
    setupFuel=checkGageLimit (i,fuelLimitAngle);
    
    calcValues(setupSpeed, &SpeedSin1Value, &SpeedSin2Value, &SpeedCos1Value, &SpeedCos2Value);
    setPosition(SpeedSinPin1,SpeedSinPin2, SpeedCosPin1, SpeedCosPin2, SpeedSin1Value, SpeedSin2Value, SpeedCos1Value, SpeedCos2Value );

    calcValues(setupBattery, &BatterySin1Value, &BatterySin2Value, &BatteryCos1Value, &BatteryCos2Value);
    setPosition(BatterySinPin1,BatterySinPin2, BatteryCosPin1, BatteryCosPin2, BatterySin1Value, BatterySin2Value, BatteryCos1Value, BatteryCos2Value );

    calcValues(setupFuel, &FuelSin1Value, &FuelSin2Value, &FuelCos1Value, &FuelCos2Value);
    setPosition(FuelSinPin1,FuelSinPin2, FuelCosPin1, FuelCosPin2, FuelSin1Value, FuelSin2Value, FuelCos1Value, FuelCos2Value );
    
  }
  delay(1000);
  for (int i=(limit-1); i>=0;i--){

    setupSpeed=checkGageLimit (i,speedLimitAngle); 
    setupBattery=checkGageLimit (i,batteryLimitAngle); 
    setupFuel=checkGageLimit (i,fuelLimitAngle); 
    
    calcValues(setupSpeed, &SpeedSin1Value, &SpeedSin2Value, &SpeedCos1Value, &SpeedCos2Value);
    setPosition(SpeedSinPin1,SpeedSinPin2, SpeedCosPin1, SpeedCosPin2, SpeedSin1Value, SpeedSin2Value, SpeedCos1Value, SpeedCos2Value); 

    calcValues(setupBattery, &BatterySin1Value, &BatterySin2Value, &BatteryCos1Value, &BatteryCos2Value);
    setPosition(BatterySinPin1,BatterySinPin2, BatteryCosPin1, BatteryCosPin2, BatterySin1Value, BatterySin2Value, BatteryCos1Value, BatteryCos2Value );

    calcValues(setupFuel, &FuelSin1Value, &FuelSin2Value, &FuelCos1Value, &FuelCos2Value);
    setPosition(FuelSinPin1,FuelSinPin2, FuelCosPin1, FuelCosPin2, FuelSin1Value, FuelSin2Value, FuelCos1Value, FuelCos2Value );
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
  
  // Test if parSing succeeds.
  if (error) {
    Serial.print(F("deserializeJson() failed: "));
    Serial.println(error.f_str());
  }
  else{
    Serial.println(message);
    float vspeed = doc["speed"]; 
    float rpm = doc["rpm"]; 
    float fuel = doc["fuel"]; 
    float battery = 13.5;
    float oiltemp = doc["oiltemp"]; 
    float oilpres = doc["oilpres"]; 

    float gageAngle=0;

    // Convert reading to angles before calling calcValues
    vspeed=checkGageLimit (vspeed,speedMax); 
    gageAngle = 1.840542 + 1.795234*vspeed - 0.003303285*pow(vspeed, 2);
    calcValues(gageAngle, &SpeedSin1Value, &SpeedSin2Value, &SpeedCos1Value, &SpeedCos2Value);
    setPosition(SpeedSinPin1,SpeedSinPin2, SpeedCosPin1, SpeedCosPin2, SpeedSin1Value, SpeedSin2Value, SpeedCos1Value, SpeedCos2Value); 

    battery=checkGageLimit (battery,batteryMax);
    gageAngle=-181.263 + 25.29143*battery - 0.2965933*pow(battery,2);
    calcValues(gageAngle, &BatterySin1Value, &BatterySin2Value, &BatteryCos1Value, &BatteryCos2Value);
    setPosition(BatterySinPin1,BatterySinPin2, BatteryCosPin1, BatteryCosPin2, BatterySin1Value, BatterySin2Value, BatteryCos1Value, BatteryCos2Value );

    fuel=checkGageLimit (fuel,fuelMax);
    gageAngle = 2.030739 + 2.194218*fuel - 0.004269457*pow(fuel,2);
    calcValues(gageAngle, &FuelSin1Value, &FuelSin2Value, &FuelCos1Value, &FuelCos2Value);
    setPosition(FuelSinPin1,FuelSinPin2, FuelCosPin1, FuelCosPin2, FuelSin1Value, FuelSin2Value, FuelCos1Value, FuelCos2Value );

    
    /*
    Serial.println("***************");
    Serial.print("speed=");
    Serial.println(vspeed);
    Serial.print("rpm=");
    Serial.println(rpm);
    Serial.print("fuel="); m 
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


void setPosition(int SinPin1, int SinPin2, int CosPin1, int CosPin2, int Sin1Value, int Sin2Value, int Cos1Value, int Cos2Value, int smoothness){
  // Move air core gauge
  analogWrite(SinPin1, Sin1Value);
  analogWrite(SinPin2, Sin2Value);
  analogWrite(CosPin1, Cos1Value);
  analogWrite(CosPin2, Cos2Value);
  
  delay(smoothness);
}

int calcValues(int angle, int* Sin1Value, int* Sin2Value, int* Cos1Value, int* Cos2Value){
  float pi=3.14159;
  int scale=255;
  // Compute pin values based on position
  float positionRad=angle*pi/180;
  float SinCalc=scale*sin(positionRad);
  float CosCalc=scale*cos(positionRad-pi); // handle 180 phase shift
  if (SinCalc>0){
    *Sin1Value=abs(SinCalc);
  }
  else{
    *Sin1Value=0;
  }
  if (SinCalc<0){
    *Sin2Value=abs(SinCalc);
  }
  else{
    *Sin2Value=0;
  }
  
  if (CosCalc>0){
    *Cos1Value=abs(CosCalc);
  }
  else{
    *Cos1Value=0;
  }
  if (CosCalc<0){
    *Cos2Value=abs(CosCalc);
  }
  else{
    *Cos2Value=0;
  }
}
