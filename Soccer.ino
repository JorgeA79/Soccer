#include <Wire.h>

int IN1 = 12; //12
int IN2 = 13; //13
int IN3 = 10; //10
int IN4 = 11; //11
int ENA = 3;
int ENB = 2;



struct InfraredResult
{
  byte Direction;
  byte Strength;
};

class InfraredSeeker
{
  public:
    static void Initialize();
    static boolean Test();
    static void ReadACRaw(byte* buffer);
    static void ReadDCRaw(byte* buffer);
    static InfraredResult ReadAC();
    static InfraredResult ReadDC();
    static int DirectionAngle(byte Direction);
  private:
    static InfraredResult PopulateValues(byte* buffer);
    static void ReadValues(byte OffsetAddress, byte* buffer);
    static const int Address = 0x10 / 2; 
};

void InfraredSeeker::Initialize()
{
  Wire.begin();
  Wire.beginTransmission(InfraredSeeker::Address);
  Wire.write(0x00);
  Wire.endTransmission();
  while(Wire.available() > 0)
    Wire.read();
}

boolean InfraredSeeker::Test()
{
  Wire.beginTransmission(InfraredSeeker::Address);
  Wire.write(0x08);
  Wire.endTransmission();
  Wire.requestFrom(InfraredSeeker::Address, 16);
  char Manufacturer_Model[16];
  while(Wire.available() < 16);
  for(byte i=0; i < 16; i++)
  {
    Manufacturer_Model[i] = Wire.read();
  }
  while(Wire.available() > 0)
    Wire.read();
  return strncmp(Manufacturer_Model, "HiTechncNewIRDir", 16)==0;
}

void InfraredSeeker::ReadValues(byte OffsetAddress, byte* buffer)
{
  Wire.beginTransmission(InfraredSeeker::Address);
  Wire.write(OffsetAddress);
  Wire.endTransmission();
  Wire.requestFrom(InfraredSeeker::Address, 6);
  while(Wire.available() < 6);
  for(byte i = 0; i < 6; i++)
  {
    buffer[i] = Wire.read();
  }
  while(Wire.available() > 0)
    Wire.read();
}

void InfraredSeeker::ReadACRaw(byte* buffer)
{
  ReadValues(0x49, buffer);
}

void InfraredSeeker::ReadDCRaw(byte* buffer)
{
  ReadValues(0x42, buffer);
}

InfraredResult InfraredSeeker::PopulateValues(byte* buffer)
{
  InfraredResult Data;
  Data.Direction = buffer[0];
  if(buffer[0] != 0)
  {
    if(buffer[0] % 2 == 0)
    {
      Data.Strength = (buffer[buffer[0] / 2] + buffer[buffer[0] / 2 + 1]) / 2;
    }
    else
    {
      Data.Strength = buffer[buffer[0] / 2 + 1];
    }
  }
  else
  {
    Data.Strength = 0;
  }
  return Data;
}

InfraredResult InfraredSeeker::ReadAC()
{
  byte buffer[6];
  ReadACRaw(buffer);
  return PopulateValues(buffer);
}

InfraredResult InfraredSeeker::ReadDC()
{
  byte buffer[6];
  ReadDCRaw(buffer);
  return PopulateValues(buffer);
}

int DirectionAngle(byte Direction)
{
  return Direction * 30 - 150;
}

////////////////////////////////////////////////////////////////


void setup()
{  
  //InfraRojo
  Serial.begin(9600);
  Serial.println("HiTechnic IRSeeker V2");
  Serial.println();
  Serial.println();
  Serial.println("Dir\tAngle\tStrength");
  Serial.println();
  InfraredSeeker::Initialize();


  //Llantas
 pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

   pinMode(ENA, OUTPUT);
  pinMode(ENB, OUTPUT);
  
 
}

void loop()
{   
   Serial.print("");
  InfraredResult InfraredBall = InfraredSeeker::ReadAC();
  Serial.print(InfraredBall.Direction);
  Serial.print("\t");
  Serial.print(DirectionAngle(InfraredBall.Direction));
  Serial.print("\t");
  Serial.print(InfraredBall.Strength);
  Serial.println();
 

if(InfraredBall.Strength<200){   
  Seguir();
}else
{
  if(InfraredBall.Strength>230){
    Patear();
    
  }
} 
 if(InfraredBall.Direction==0){
     if(InfraredBall.Direction>0)GirarIzquierda(80, 85);
   }
 else Adelante(120, 125);
}
 

// Movimientos

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Seguir(){                                                                                             //
  InfraredResult InfraredBall = InfraredSeeker::ReadAC();                                                  //   

  
  if(InfraredBall.Direction!=5){  
    
     if(InfraredBall.Direction<5 && InfraredBall.Strength>0)GirarIzquierda(80, 85);
     else if(InfraredBall.Direction>5 && InfraredBall.Strength>0)GirarDerecha(80, 85);
    }
    else {
      if(InfraredBall.Strength>0){
         Adelante(120, 125); 
         //Serial.println("Adelante");
      }
    }
    if(InfraredBall.Direction==0){
     if(InfraredBall.Direction>0)GirarIzquierda(80, 85);
   }
 else Adelante(120, 125);
}                                                                                                        // 
                                                                                                         //  
///////////////////////////////////////////////////////////////////////////////////////////////////////////
void Patear(){                                                                                           //
                                                                                                         // 
                                                                                                       
InfraredResult InfraredBall = InfraredSeeker::ReadAC(); 
Detras(120,125);
delay(300);

if(InfraredBall.Direction!=5){  
  delay(100);
     if(InfraredBall.Direction<5 && InfraredBall.Strength>0)GirarIzquierda(80, 150);
     else if(InfraredBall.Direction>5 && InfraredBall.Strength>0)GirarDerecha(150, 80);
    }
    else {
      if(InfraredBall.Strength>230){    
Adelante(150,255);
      }}
      
delay(1500); 

 }                                                                                                       //   
                                                                                                         //  
///////////////////////////////////////////////////////////////////////////////////////////////////////////

void Adelante(int vi, int vd){
    analogWrite(ENA, vi);
  analogWrite(ENB, vd);
  digitalWrite(IN1, HIGH);  //Derecha Delante Atras
  digitalWrite(IN2, LOW); //Derecha Delante Avanzar
  digitalWrite(IN3, HIGH);  //Izquierda Delante Atras
  digitalWrite(IN4, LOW); //Izquierda Delante Avanzar
  delay(30);
}

void GirarDerecha(int vi, int vd){
    analogWrite(ENA, vi);
  analogWrite(ENB, vd);
  digitalWrite(IN1, 0);  //Derecha Delante Atras
  digitalWrite(IN2, 1); //Derecha Delante Avanzar
  digitalWrite(IN3, 1);  //Izquierda Delante Atras
  digitalWrite(IN4, 0); //Izquierda Delante Avanzar
  delay(30);
}

void GirarIzquierda(int vi, int vd){
    analogWrite(ENA, vi);
  analogWrite(ENB, vd);
  digitalWrite(IN1, 1);  //Derecha Delante Atras
  digitalWrite(IN2, 0); //Derecha Delante Avanzar
  digitalWrite(IN3, 0);  //Izquierda Delante Atras
  digitalWrite(IN4, 1); //Izquierda Delante Avanzar
  delay(30);
}
void Detras(int vi, int vd){
    analogWrite(ENA, vi);
  analogWrite(ENB, vd);
  digitalWrite(IN1, 0);  //Derecha Delante Atras
  digitalWrite(IN2, 1); //Derecha Delante Avanzar
  digitalWrite(IN3, 0);  //Izquierda Delante Atras
  digitalWrite(IN4, 1); //Izquierda Delante Avanzar
  delay(30);
}
