//SENZOR PULS
#include <PulseSensorPlayground.h>
PulseSensorPlayground senzorPuls;

//MOTOR
#include <Servo.h>
Servo motor;


//LCD
#include <LiquidCrystal.h>
LiquidCrystal LCD(12,11,10,9,8,7);


//PINI
const int pinBecV=2;
const int pinBecR=3;

const int pinMotor=4;

const int pinPuls=A0;


//VALORI
const int secundeMotor=2;
const int bataiPentruMedie=15;
const int pragMinciuna=15;


//PULS
int puls;

int numarPuls;
int sumaPuls;
int pulsMediu;

//MINCIUNA
int minciuna;


void citestePuls()
{
  puls=senzorPuls.getBeatsPerMinute();
}


void pornesteBec(const int pinBec)
{
  digitalWrite(pinBec,HIGH);
  delay(10);
}


void opresteBec(const int pinBec)
{
  digitalWrite(pinBec,LOW);
  delay(10);
}


void calculeazaPulsMediu()
{
  sumaPuls=sumaPuls+puls;
  numarPuls=numarPuls+1;

  if(numarPuls==bataiPentruMedie)
  {
    pulsMediu=sumaPuls/numarPuls;
  }
}


void verificaMinciuna()
{
  if(puls>=pulsMediu+pragMinciuna)
  {
    minciuna=1;
  }
  else
  {
    minciuna=0;
  }
}


void existaMinciuna()
{
  opresteBec(pinBecV);

  pornesteBec(pinBecR);

  scrieLCD(0,1,"MINCIUNA!",-1);
  scrieConsola("MINCIUNA!",-1);

  pornesteMotor();

  opresteBec(pinBecR);

  scrieLCD(0,1,"                ",-1);

  pornesteBec(pinBecV);
}


void pornesteMotor()
{
  motor.write(0);
  delay(secundeMotor*1000);
  motor.write(90);
  delay(300);
}


void scrieConsola(char* sir, int valoare)
{
  if(valoare!=-1)
  {
    Serial.print(sir);
    Serial.print(valoare);
    Serial.println();
  }
  else
  {
    Serial.print(sir);
    Serial.println();
  }
}


void scrieLCD(int coloana, int linie, char* sir, int valoare)
{
  LCD.setCursor(coloana,linie);

  int i;
  for(i=0;i<16;i++)
  {
    LCD.print(" ");
  }

  LCD.setCursor(coloana,linie);

  if(valoare!=-1)
  {
    LCD.print(sir);
    LCD.print(valoare);
  }
  else
  {
    LCD.print(sir);
  }

  delay(10);
}


void setup()
{
  pinMode(pinBecV,OUTPUT);
  pinMode(pinBecR,OUTPUT);
  pinMode(pinMotor,OUTPUT);

  pinMode(pinPuls,INPUT);
  senzorPuls.analogInput(pinPuls);
  senzorPuls.begin();

  motor.attach(pinMotor);

  LCD.begin(16,2);

  minciuna=0;

  numarPuls=0;
  sumaPuls=0;
  pulsMediu=0;

  pornesteBec(pinBecV);

  Serial.begin(9600);
}


void loop()
{
  citestePuls();

  scrieConsola("Puls: ",puls);

  scrieLCD(0,0,"Puls: ",puls);

  if(pulsMediu==0)
  {
    calculeazaPulsMediu();
  }
  else
  {
    scrieConsola("Puls mediu: ",pulsMediu);

    scrieLCD(0,0,"Puls: ",puls);
    scrieLCD(9,0,"Med: ",pulsMediu);

    verificaMinciuna();

    if(minciuna==1)
    {
     existaMinciuna();
    }
  }

  scrieConsola("",-1);

  delay(500);
}
