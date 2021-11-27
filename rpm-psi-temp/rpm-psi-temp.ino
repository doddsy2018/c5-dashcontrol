
#include <ArduinoJson.h>

int RPMSinPin1 =  10;    
int RPMSinPin2=  11;    
int RPMCosPin1 =  12;    
int RPMCosPin2 =  13;  
  
int RPMSin1Value=0;
int RPMSin2Value=0;
int RPMCos1Value=0;
int RPMCos2Value=255;

int PressureSinPin1 =  6;    
int PressureSinPin2=  7;    
int PressureCosPin1 =  8;    
int PressureCosPin2 =  9; 

int PressureSin1Value=0;
int PressureSin2Value=0;
int PressureCos1Value=0;
int PressureCos2Value=255;

int TempSinPin1 =  2;    
int TempSinPin2=  3;    
int TempCosPin1 =  4;    
int TempCosPin2 =  5; 

int TempSin1Value=0;
int TempSin2Value=0;
int TempCos1Value=0;
int TempCos2Value=255;

// Max Angle limita\Angles

int rpmLimitAngle=170;
int pressureLimitAngle=180;
int tempLimitAngle=180;

int rpmMax=7000;
int pressureMax=80;
int tempMax=260;

int limit=230;

void setPosition(int SinPin1, int SinPin2, int CosPin1, int CosPin2, int Sin1Value, int Sin2Value, int Cos1Value, int Cos2Value, int smoothness=5); //Declare default function value

void setup() {

  // wait for hardware serial to appear
  while (!Serial);
  Serial.begin(115200); // opens serial port, sets data rate to 115200 bps

  pinMode(RPMSinPin1, OUTPUT);    
  pinMode(RPMSinPin2, OUTPUT);    
  pinMode(RPMCosPin1, OUTPUT);    
  pinMode(RPMCosPin2, OUTPUT); 
   
  pinMode(PressureSinPin1, OUTPUT);    
  pinMode(PressureSinPin2, OUTPUT);    
  pinMode(PressureCosPin1, OUTPUT);    
  pinMode(PressureCosPin2, OUTPUT);

  pinMode(TempSinPin1, OUTPUT);    
  pinMode(TempSinPin2, OUTPUT);    
  pinMode(TempCosPin1, OUTPUT);    
  pinMode(TempCosPin2, OUTPUT);

  int setupRPM;
  int setupPressure;
  int setupTemp;

  // Cycle Gauge
  for (int i=0; i<=limit;i++){

    setupRPM=checkGageLimit (i,rpmLimitAngle); 
    setupPressure=checkGageLimit (i,pressureLimitAngle);
    setupTemp=checkGageLimit (i,tempLimitAngle);
    
    calcValues(setupRPM, &RPMSin1Value, &RPMSin2Value, &RPMCos1Value, &RPMCos2Value);
    setPosition(RPMSinPin1,RPMSinPin2, RPMCosPin1, RPMCosPin2, RPMSin1Value, RPMSin2Value, RPMCos1Value, RPMCos2Value );

    calcValues(setupPressure, &PressureSin1Value, &PressureSin2Value, &PressureCos1Value, &PressureCos2Value);
    setPosition(PressureSinPin1,PressureSinPin2, PressureCosPin1, PressureCosPin2, PressureSin1Value, PressureSin2Value, PressureCos1Value, PressureCos2Value );

    calcValues(setupTemp, &TempSin1Value, &TempSin2Value, &TempCos1Value, &TempCos2Value);
    setPosition(TempSinPin1,TempSinPin2, TempCosPin1, TempCosPin2, TempSin1Value, TempSin2Value, TempCos1Value, TempCos2Value );
    
  }
  delay(1000);
  for (int i=(limit-1); i>=0;i--){

    setupRPM=checkGageLimit (i,rpmLimitAngle); 
    setupPressure=checkGageLimit (i,pressureLimitAngle);
    setupTemp=checkGageLimit (i,tempLimitAngle);
    
    calcValues(setupRPM, &RPMSin1Value, &RPMSin2Value, &RPMCos1Value, &RPMCos2Value);
    setPosition(RPMSinPin1,RPMSinPin2, RPMCosPin1, RPMCosPin2, RPMSin1Value, RPMSin2Value, RPMCos1Value, RPMCos2Value); 

    calcValues(setupPressure, &PressureSin1Value, &PressureSin2Value, &PressureCos1Value, &PressureCos2Value);
    setPosition(PressureSinPin1,PressureSinPin2, PressureCosPin1, PressureCosPin2, PressureSin1Value, PressureSin2Value, PressureCos1Value, PressureCos2Value );

    calcValues(setupTemp, &TempSin1Value, &TempSin2Value, &TempCos1Value, &TempCos2Value);
    setPosition(TempSinPin1,TempSinPin2, TempCosPin1, TempCosPin2, TempSin1Value, TempSin2Value, TempCos1Value, TempCos2Value );
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
    float vspeed = doc["RPM"]; 
    float rpm = doc["rpm"]; 
    float temp = doc["Temp"]; 
    float battery = 13.5;
    float oiltemp = doc["oiltemp"]; 
    float oilpres = doc["oilpres"]; 

    float gageAngle=0;

    // Convert reading to angles before calling calcValues
    rpm=checkGageLimit (rpm,rpmMax);
    gageAngle = 0.1539103 + 0.02892786*rpm - 8.327303e-7*pow(rpm,2);
    calcValues(gageAngle, &RPMSin1Value, &RPMSin2Value, &RPMCos1Value, &RPMCos2Value);
    setPosition(RPMSinPin1,RPMSinPin2, RPMCosPin1, RPMCosPin2, RPMSin1Value, RPMSin2Value, RPMCos1Value, RPMCos2Value );
    
    oilpres=checkGageLimit (oilpres,pressureMax);
    gageAngle=2.804506 + 2.638895*oilpres - 0.005712883*pow(oilpres,2);
    calcValues(gageAngle, &PressureSin1Value, &PressureSin2Value, &PressureCos1Value, &PressureCos2Value);
    setPosition(PressureSinPin1,PressureSinPin2, PressureCosPin1, PressureCosPin2, PressureSin1Value, PressureSin2Value, PressureCos1Value, PressureCos2Value );

    oiltemp=(oiltemp * (9/5)) + 32;
    oiltemp=checkGageLimit (oiltemp,tempMax);
    if (oiltemp<100) {
      oiltemp=100;
    }
    gageAngle = -14.05772 - 0.1464019*oiltemp + 0.003325954*pow(oiltemp,2);
    calcValues(gageAngle, &TempSin1Value, &TempSin2Value, &TempCos1Value, &TempCos2Value);
    setPosition(TempSinPin1,TempSinPin2, TempCosPin1, TempCosPin2, TempSin1Value, TempSin2Value, TempCos1Value, TempCos2Value );
    
    
    /*
    Serial.println("***************");
    Serial.print("RPM=");
    Serial.println(vRPM);
    Serial.print("rpm=");
    Serial.println(rpm);
    Serial.print("Temp="); m 
    Serial.println(Temp);
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
