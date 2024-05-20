/*
  Smart Egg Incubator  
  
  Language: Arduino

  The circuit:

  - lcd on digital pins 13,12,11,10,9,8 of arduino

   - fan on D5 pin

   - temperature sensor on A2 pin

   - water level sensor on A3 pin

   - bulb on D7 pin

   - servo motor on D6 pin
  
  created 01 Jul 2021

  by Aspire Technologies

  for more references, you can visit the public domain

  http://www.arduino.cc/

*/

#include"LiquidCrystal.h" //library call for lcd
#include"Servo.h" //library call for servo motor
LiquidCrystal lcd(13,12,11,10,9,8); //object creation for lcd
Servo fill; //object creation for servo motor
int dcfan = 5; 
#define sensor A2
int temp; //variable for storing temperature
int light = 7;
int pot = A3;
int waterlvl; //variable for storing water level

void setup()
{
  pinMode(light,OUTPUT); //pin mode decleration
  pinMode(pot,INPUT);
  pinMode(sensor,INPUT);
  pinMode(dcfan,OUTPUT);
  fill.attach(6);
  lcd.begin(16,2); //initilizing lcd
  lcd.setCursor(0,0);
  lcd.print("Egg Incubator"); //printing on lcd
}
void loop()
{
  digitalWrite(dcfan,HIGH); //fan will always turned on to circulate air and temperature inside the incubator
  temp = analogRead(sensor); //reading temperature
  waterlvl = analogRead(pot); //reading water level
  if(temp > 250) //temperature high
  {
    digitalWrite(light,LOW); //bulb off
  }
  if(temp < 250) //temperature low
  {
    digitalWrite(light,HIGH); //bulb on
  }
  if(waterlvl < 200) //water level low
  {
    fill.write(90); //servo rotates to 90 degree, tap gets opened
  }
  if(waterlvl == 400) //water level is enough
  {
    fill.write(0); //servo rotates back to 0, tap will get closed
  }
  lcd.setCursor(0,1);
  lcd.print("Temperature: "); //printing temperature inside the incubator on lcd
  lcd.setCursor(13,1);
  lcd.print(temp);
  delay(100);
  
}
