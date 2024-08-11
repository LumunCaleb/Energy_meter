//#include <LiquidCrystal_I2C.h>

#include <LiquidCrystal.h>

// EmonLibrary examples openenergymonitor.org, Licence GNU GPL V3


#include "EmonLib.h"                   // Include Emon Library
EnergyMonitor emon1;   
                // Create an instance
LiquidCrystal lcd(13,12,11,10,9,8); //LiquidCrystal lcd(rs,en, d4,d5,d6,d7)

//Measuring current using ACS712
const int sensor_pin=A1;
int sensitivity=185; //5V sensor
int offsetvoltage=2580;

void setup()
{  
  Serial.begin(9600);
  
  emon1.voltage(A0, 187, 1.7);  // Voltage: input pin, calibration, phase_shift
  //emon1.current(1, 111.1);       // Current: input pin, calibration.
lcd.begin(20,4);
lcd.setCursor(0,0);
lcd.print("   The HiTech Team");
lcd.setCursor(0,1);
lcd.print("   AC Energy meter ");

}

void loop()
{
  //Measure voltage

  emon1.calcVI(20,2000);         // Calculate all. No.of half wavelengths (crossings), time-out
  int Voltage=emon1.Vrms; //Extract Vrms into Variable

  lcd.setCursor(0,2);
  lcd.print("V= ");
  lcd.print(Voltage);
  lcd.print("v ");

  //Measure Curent
  unsigned int temp=0;
  float maxpoint=0;
  for(int i=0;i<500;i++)
  {
    if(temp=analogRead(sensor_pin), temp>maxpoint)
    {
      maxpoint=temp;
    }
  }  
  float ADCvalue = maxpoint; 
  double eVoltage =(ADCvalue/1024.0)*5000; //Value in mV
  double Current = ((eVoltage-offsetvoltage)/sensitivity);
  double AC_Current = (Current)/(sqrt(2));

  lcd.print(" I= ");
  lcd.print(AC_Current,2);
  lcd.print("A     ");

  //Measure power
  int Power = (Voltage*AC_Current);

  lcd.setCursor(0,3);
  lcd.print("P= ");
  lcd.print(Power);
  lcd.print("W ");

  //MEasure Energy
  long milisec = millis();  //calculate time in milliseconds
  long time=milisec/1000;  //convert milliseconds to seconds

  float Energy = (Power*time)/3600; //What-hour

  lcd.print("  E=");
  lcd.print(Energy,1);
   delay(200);

}