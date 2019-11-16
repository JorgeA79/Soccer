#include <Wire.h>

int MOTORIDA = 11, MOTORIDR = 12, MOTORIAA = 10, MOTORIAR = 9; 
int MOTORDDA = 6, MOTORDDR = 5, MOTORDAA = 3, MOTORDAR = 4; 
int ena=8 , enb=13;
int ena1=2, enb1=7;


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

  pinMode(MOTORIDA, OUTPUT);
  pinMode(MOTORIDR, OUTPUT);
  pinMode(MOTORIAA, OUTPUT);
  pinMode(MOTORIAR, OUTPUT);

  pinMode(ena, OUTPUT);
  pinMode(enb, OUTPUT);
  
  pinMode(MOTORDDA, OUTPUT);
  pinMode(MOTORDDR, OUTPUT);
  pinMode(MOTORDAA, OUTPUT);
  pinMode(MOTORDAR, OUTPUT);

  pinMode(ena1, OUTPUT);
  pinMode(enb1, OUTPUT);
  
 
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

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
   analogWrite(ena, 80);
  analogWrite(enb, 85);
  analogWrite(ena1, 80);
  analogWrite(enb1, 85);
  digitalWrite(MOTORIDA, 1);
  digitalWrite(MOTORIDR, 0);
  digitalWrite(MOTORIAA, 0);
  digitalWrite(MOTORIAR, 1);
  digitalWrite(MOTORDDA, 1);
  digitalWrite(MOTORDDR, 0);
  digitalWrite(MOTORDAA, 0); 
  digitalWrite(MOTORDAR, 1);                                                                                
                                                                                                                                                                                                                   

  if(InfraredBall.Direction!=5)
    {  
    
    if(InfraredBall.Direction<5 && InfraredBall.Strength>0)
    {
      Serial.print("Izquierda");
      GirarIzquierda(80, 85, 80, 85);
      }
    
    else if(InfraredBall.Direction>5 && InfraredBall.Strength>0)
    {
      Serial.print("Derecha");
      GirarDerecha(80, 85, 80, 85);
    }
    }
   
}    
                                                                                                           //
                                                                                                           //         
/////////////////////////////////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Adelante(){
  analogWrite(ena, 0);
  analogWrite(enb, 0);
  analogWrite(ena1, 0);
  analogWrite(enb1, 0);
  digitalWrite(MOTORIDA, 0);
  digitalWrite(MOTORIDR, 0);
  digitalWrite(MOTORIAA, 0);
  digitalWrite(MOTORIAR, 0);
  digitalWrite(MOTORDDA, 0);
  digitalWrite(MOTORDDR, 0);
  digitalWrite(MOTORDAA, 0); 
  digitalWrite(MOTORDAR, 0); 
  delay(30);
}

void GirarDerecha(int vi, int vd,int vi1,int vd1){
  analogWrite(ena, vi);
  analogWrite(enb, vd);
  analogWrite(ena1, vi1);
  analogWrite(enb1, vd1);
  digitalWrite(MOTORIDA, 1);
  digitalWrite(MOTORIDR, 0);
  digitalWrite(MOTORIAA, 0);
  digitalWrite(MOTORIAR, 1);
  digitalWrite(MOTORDDA, 1);
  digitalWrite(MOTORDDR, 0);
  digitalWrite(MOTORDAA, 0); 
  digitalWrite(MOTORDAR, 1); 
}

void GirarIzquierda(int vi, int vd,int vi1,int vd1){
  
  analogWrite(ena, vi);
  analogWrite(enb, vd);
  analogWrite(ena1, vi1);
  analogWrite(enb1, vd1);
  digitalWrite(MOTORIDA, 0);
  digitalWrite(MOTORIDR, 1);
  digitalWrite(MOTORIAA, 1);
  digitalWrite(MOTORIAR, 0);
  digitalWrite(MOTORDDA, 0);
  digitalWrite(MOTORDDR, 1);
  digitalWrite(MOTORDAA, 1); 
  digitalWrite(MOTORDAR, 0); 
}
