/*
  Bus Stop With A Brain

  Language: Arduino

  The circuit:

  - Servo motor on D7 pin of Arduino
  - RTC module on the SDA & SCL pins
  - lcd on the digital pins 13,12,11,10,9,8
  - rain sensor on A0 pin
  - ldr on A1 pin

  created 01 Nov 2021

  by Aspire Technologies

  for more references, you can visit the public domain

  http://www.arduino.cc/

*/

#include<Servo.h> //library for servo motor
#include <DS3231.h> //library for rtc module
#include<LiquidCrystal.h> //library for lcd
Servo myservo; //object created for servo motor
DS3231  rtc(SDA, SCL); //object created for servo motor
LiquidCrystal lcd(13,12,11,10,9,8); //object created for lcd
int rainsen = A0;
int ldr = A1;
int rain,light; //variable created for storing the reading of the rain sensor and ldr
int pos = 0; //servo motor's position, is set to 0 initially

void setup()
{
  Serial.begin(9600); //setting the serial post to 9600
  lcd.begin(16,2); //initiallizing the lcd
  rtc.begin(); //initiallizing the rtc module
  myservo.attach(7);
  myservo.write(pos); //setting the servo motor's position
  pinMode(rainsen,INPUT); pin mode declerations
  pinMode(ldr,INPUT);
  rtc.setDOW(THURSDAY);     // Set Day-of-Week to SUNDAY
  rtc.setTime(11, 42, 0);     // Set the time to 11:42:00 (24hr format)
  rtc.setDate(10, 7, 2021);   // Set the date to October 7th, 2021
}

void loop()
{
  rain = analogRead(rainsen); //asigning the rain sensor input to the created variable
  light = analogRead(ldr); //asigning the ldr input to the created variable
  Serial.print("rain = "); //printing the input values on the serial monitor for error checking
  Serial.println(rain);
  Serial.print("light = ");
  Serial.println(light);
  Serial.println("$$$$$$$$$");
   // Send Day-of-Week
  Serial.print(rtc.getDOWStr());  //time, date,and day will be printed on the serial monitor and the lcd display
  Serial.print(" ");
  lcd.clear();
  lcd.setCursor(4,0);
  lcd.print(rtc.getDOWStr());
  
  // Send date
  Serial.print(rtc.getDateStr());
  Serial.print(" -- ");
  lcd.setCursor(3,1);
  lcd.print(rtc.getDateStr());
  delay(1000);

  // Send time
  Serial.println(rtc.getTimeStr());
  lcd.clear();
  lcd.setCursor(4,0);
  lcd.print(rtc.getTimeStr());
  delay(1000);

  if(rain < 500 || light > 500) //if there is rain or there is heavy sun light
  {
    roofClose(); //calling the roof closing function
  }
  if(rain > 500 && light < 500) //if there is no rain and no heavy sun light
  {
    roofOpen(); //roof opens
  }  
}

void roofClose() //function created for closing the roof
{
  for(pos; pos < 180; pos++)
  {
    myservo.write(pos);
    delay(20);
  }
}
void roofOpen() //function created for opening the roof
{
  for(pos; pos > 0; pos--)
  {
    myservo.write(pos);
    delay(20);
  }
}

