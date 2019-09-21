#include <Wire.h>

int IN1 = 12; //12
int IN2 = 13; //13
int IN3 = 10; //10
int IN4 = 11; //11

int IN5 = 6;
int IN6 = 7;
int IN7 = 4;
int IN8 = 5;




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

 pinMode(IN5, OUTPUT);
  pinMode(IN6, OUTPUT);
  pinMode(IN7, OUTPUT);
  pinMode(IN8, OUTPUT);
  
  
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
  delay(100);
  

  //Seguir
  
  if(InfraredBall.Direction!=5){  
    if(InfraredBall.Direction<5)GirarIzquierda();
    else if(InfraredBall.Direction>5)GirarDerecha();
  }
  else {
    if(InfraredBall.Strength>0){
        Adelante(); 
       
        Serial.println("Adelante");
    }
    
    else {
      
      Detenerse();
      Serial.println("Detenerse");
       delay(30);
    }
  }
  
}




// Movimientos

void Adelante(){
  
  digitalWrite(IN1, LOW);  //Derecha Delante Atras
  digitalWrite(IN2, HIGH); //Derecha Delante Avanzar
  digitalWrite(IN3, LOW);  //Izquierda Delante Atras
  digitalWrite(IN4, HIGH); //Izquierda Delante Avanzar

  digitalWrite(IN5, HIGH);  //Izquiera Atras Avanzar
  digitalWrite(IN6, LOW);  //Izquieda Atras Atras
  digitalWrite(IN7, HIGH); //Derecha Atras Avanzar
  digitalWrite(IN8, LOW);  //Derecha Atras Atras


}

void Detenerse(){


 digitalWrite(IN1, LOW);  //Derecha Delante Atras
  digitalWrite(IN2, LOW); //Derecha Delante Avanzar
  digitalWrite(IN3, LOW);  //Izquierda Delante Atras
  digitalWrite(IN4, LOW); //Izquierda Delante Avanzar

  digitalWrite(IN5, LOW);  //Izquiera Atras Avanzar
  digitalWrite(IN6, LOW);  //Izquieda Atras Atras
  digitalWrite(IN7, LOW); //Derecha Atras Avanzar
  digitalWrite(IN8, LOW);  //Derecha Atras Atras


  
}


void GirarDerecha(){
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  
    digitalWrite(IN5, HIGH);
  digitalWrite(IN6, LOW);
  digitalWrite(IN7, LOW);
  digitalWrite(IN8, HIGH);


  digitalWrite(IN5, LOW);
  digitalWrite(IN6, LOW);
  digitalWrite(IN7, LOW);
  digitalWrite(IN8, HIGH);


}

void GirarIzquierda(){
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  
    digitalWrite(IN5, HIGH);
  digitalWrite(IN6, LOW);
  digitalWrite(IN7, LOW);
  digitalWrite(IN8, HIGH);


  digitalWrite(IN5, LOW);
  digitalWrite(IN6, LOW);
  digitalWrite(IN7, LOW);
  digitalWrite(IN8, HIGH);

}
