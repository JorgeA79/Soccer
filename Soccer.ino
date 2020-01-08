#include <Wire.h>
#include<AFMotor.h>

AF_DCMotor motor1(1);
AF_DCMotor motor2(2);
AF_DCMotor motor3(3);
AF_DCMotor motor4(4);

double x;
double y;
double Angle;

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


  pinMode(13, OUTPUT); //IZQUIERDA
  pinMode(8, OUTPUT); //ADELANTE
  pinMode(10, OUTPUT); //DERECHA
 
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
  Serial.print("\t");
  x = InfraredBall.Direction - 5;
  y = 221 - InfraredBall.Strength;
  Angle = atan2(y, x) * 180 / 3.141592 ;
  Serial.print(x);
  Serial.print("\t");
  Serial.print(y);
  Serial.print("\t");
  Serial.print(Angle);
  Serial.println();

/////////////////////////////////////////////////////////////////////////////////////////////////////////////

if(InfraredBall.Strength > 100){ 
if(InfraredBall.Strength > 65){ 
if(InfraredBall.Strength < 170){
  if(InfraredBall.Direction==5){
    Adelante();
  }
  if(InfraredBall.Direction<5 || InfraredBall.Direction==0){
      GirarIzquierda();
}
if(InfraredBall.Direction>5){
      GirarDerecha(); }
}else{
  Adelante();
       digitalWrite(13, 1);   
      digitalWrite(8, 1);
      digitalWrite(10,1);
}
}
}else if(InfraredBall.Direction==5){
 MoverIzquierda();
}else if(InfraredBall.Direction==7){
 MoverDerecha();
}else if(InfraredBall.Direction==6){
 Adelante();
 }else if(InfraredBall.Direction==0){
 GirarIzquierda();
}else if(InfraredBall.Strength > 65){
 Adelante();
}
}

                                                                                                           //
                                                                                                           //         
/////////////////////////////////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Detenerse(){
motor1.run(RELEASE);
motor2.run(RELEASE);
motor3.run(RELEASE);
motor4.run(RELEASE);
delay(30);
}
void Adelante(){
motor1.setSpeed(10);
motor2.setSpeed(255);
motor3.setSpeed(130);
motor4.setSpeed(115);
motor1.run(FORWARD);
motor2.run(FORWARD);
motor3.run(FORWARD);
motor4.run(FORWARD);
digitalWrite(13, 0);   
digitalWrite(8, 0);
digitalWrite(10, 1);

}
void GirarDerecha(){
motor1.setSpeed(120);
motor2.setSpeed(120);
motor3.setSpeed(120);
motor4.setSpeed(120);
motor1.run(FORWARD);
motor2.run(BACKWARD);
motor3.run(BACKWARD);
motor4.run(FORWARD);
digitalWrite(13, 0);   
digitalWrite(8, 1);
digitalWrite(10, 0);

}

void GirarIzquierda(){
motor1.setSpeed(120);
motor2.setSpeed(120);
motor3.setSpeed(120);
motor4.setSpeed(150);
motor1.run(BACKWARD);
motor2.run(FORWARD);
motor3.run(FORWARD);
motor4.run(BACKWARD);
digitalWrite(13, 1);   
digitalWrite(8, 0);
digitalWrite(10, 0);

}
void MoverIzquierda(){
motor1.setSpeed(120);
motor2.setSpeed(255);
motor3.setSpeed(220);
motor4.setSpeed(255);
motor1.run(FORWARD);
motor2.run(BACKWARD);
motor3.run(FORWARD);
motor4.run(BACKWARD);
digitalWrite(13, 1);   
digitalWrite(8, 0);
digitalWrite(10, 0);

}
void MoverDerecha(){
motor1.setSpeed(120);
motor2.setSpeed(230);
motor3.setSpeed(255);
motor4.setSpeed(255);
motor1.run(BACKWARD);
motor2.run(FORWARD);
motor3.run(BACKWARD);
motor4.run(FORWARD);
digitalWrite(13, 1);   
digitalWrite(8, 0);
digitalWrite(10, 0);

}
