/*
  Railway Gate Automation  

  Language: Arduino

  The circuit:

  - LCD module on analog pins A0,A1,A2,A3,A4,A5
  - servo motor on D9 pin
  - IR sensors are on D13 and D12 pins
  - buzzer on D8
  - red led on D11
  - green led on D10
  - yellow led on D7
  

  created 09 Apr 2022

  by Aspire Technologies

  for more references, you can visit the public domain

  http://www.arduino.cc/

*/

#include<LiquidCrystal.h> //library for lcd module
#include<Servo.h> //library for servo motor
#define sensor1 13 
#define sensor2 12
#define buz 8
#define red 11
#define green 10
#define yellow 7
LiquidCrystal lcd(A0, A1, A2, A3, A4, A5); //object creation for lcd module
Servo myservo; //object creation for servo motor
int read1, read2; //variables that stores the inputs from both ir sensors
bool train = false, threat = false, risk = false; //boolean variables created for maintatining the program flow
int pos = 90; //variable that stores the position of the servo motor, initially the cross is open, so it holds the value 90
int f1 = 0; //variable created for maintaining the program flow
int counter = 0; //variable to maintain the flo of the program
void setup()
{
  Serial.begin(9600); //opens serial monitor at a baud rate of 9600
  lcd.begin(16, 2); //initializing the lcd modules with 16 columns and 2 rows
  myservo.attach(9); //servo motor on D9 pin
  myservo.write(pos); //writing the variable to the servo motor
  pinMode(sensor1, INPUT); //pin mode declerations
  pinMode(sensor2, INPUT);
  pinMode(buz, OUTPUT);
  pinMode(red, OUTPUT);
  pinMode(green, OUTPUT);
  pinMode(yellow, OUTPUT);
}

void loop()
{
  sensorData(); //function created for reading the sensor values
  
  if (threat == false && risk == false) //if there is no threat and no risk
  {
    if(read2 == HIGH && f1 == 0) //when the second ir sensor is not detecting any object, the value of f1 variable will be 1
    {
      f1 = 1;
    }
    if (read1 == LOW && read2 == HIGH && train == false) //when the first ir sensor detects the train while there is no train present
    {
      train = true; //train is present
      crossClose(); //calling the level cross closing function
    }
    if (read1 == LOW && read2 == HIGH && train == true) //when the train is still present in front of the first ir sensor
    {
      train = true; //train is still there
      crossClose(); //cross will stay closed
    }
    if (read1 == HIGH && read2 == HIGH && train == true) //when the train passes the first ir sensor and goes to the second ir sensor
    {
      train = true; //trin is still present on the track
      crossClose(); //cross will stay closed
    }
    if (read1 == HIGH && read2 == HIGH && train == false) //when both ir sensors detects nothing and there is no train detected
    {
      crossOpen(); //cross will be opened
    }
    if (read1 == HIGH && read2 == LOW && train == true && f1 == 1) //after the train passes the first ir sensor and it reaches the second ir sensor
    {
      f1 = 0; //this step avoids the multiple readings of the same object
      train = false; //means the train gone a long way
      crossOpen(); //cross opens
    }

    sensorData(); 
    
    if (read1 == HIGH && read2 == LOW && train == false && f1 == 1) //when the second ir sensor triggers while the first on detects nothing
    {
      f1 = 0;
      threat = true; //means an object is there on the track, so a threat is there
    }
    if (read1 == LOW && read2 == LOW && train == true) //the second sensor detects an object while train is coming
    {
      risk = true; //risk found
    }
  }
  if (threat == true && risk == false) //if there is only threat
  {
    threatFound(); //function called for threat alert
  }
  if (threat == true && risk == true) //when both threat and risk are true
  {
    trainStopped(); //function called to stop the train
  }
  if(threat == true && read1 == LOW) //when the train comes while the threat is on
  {
    risk = true; //risk found
    trainStopped(); //function called to stop the train
  }
  delay(20); //delay to smoothen the loop execution
}

void sensorData() //function to read sensor data
{
  read1 = digitalRead(sensor1); //reading the data of the first ir sensor
  read2 = digitalRead(sensor2); //reading the data of the second ir sensor
}

void crossOpen() //function to open the level cross
{
  lcd.clear(); //clearing the lcd panel
  lcd.setCursor(0, 0); //setting cursor on the lcd
  lcd.print("Track is clear,"); //message displaying on the lcd
  lcd.setCursor(0, 1);
  lcd.print("Cross opened..");
  digitalWrite(yellow, HIGH); //yellow light will stay on
  digitalWrite(green, HIGH); //green light on
  digitalWrite(red, LOW); //red light off
  for (pos; pos > 90; pos --) //servo rotates from 180 to 90
  {
    myservo.write(pos);
    delay(50);
    tone(buz, 1000); //a beep sound happens while the servo rotates
  }
  noTone(buz);
}

void crossClose() //function for closing the level crosss
{
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Train passing,");
  lcd.setCursor(0, 1);
  lcd.print("Cross is closed..");
  digitalWrite(yellow, HIGH);
  digitalWrite(green, HIGH);
  digitalWrite(red, LOW);
  for (pos; pos < 180; pos ++) //servo rotates from 90 to 180 
  {
    myservo.write(pos);
    delay(50);
    tone(buz, 1000);
  }
  noTone(buz);
}

void threatFound() //function to be executed while the threat is found
{
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Threat found!!");
  lcd.setCursor(0, 1);
  lcd.print("Clear the track!");
  digitalWrite(yellow, HIGH);
  digitalWrite(green, LOW); //green light off
  digitalWrite(red, HIGH); //red light on
  for (pos; pos > 90; pos --) //servo at 90 
  {
    myservo.write(pos);
    delay(50);
    tone(buz, 1000);
  }
  digitalWrite(buz,HIGH); //multiple short bee sounds will happen contineously
  delay(50);
  noTone(buz);
  delay(50);
  digitalWrite(buz,HIGH);
  delay(50);
  noTone(buz);
  delay(30);
  delay(1000);
}

void trainStopped() //function to be executed while risk found
{
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Risk Found,");
  lcd.setCursor(0, 1);
  lcd.print("Train Stopped...!");
  digitalWrite(yellow, HIGH);
  digitalWrite(green, LOW); //green led off
  digitalWrite(red, HIGH); //red led on
  for (pos; pos < 180; pos ++) //servo at 180
  {
    myservo.write(pos);
    delay(50);
  }
  if(counter < 5) //we want this loop to be executed five times
  {
  digitalWrite(buz,HIGH); //a long beep will happen
  delay(1000);
  noTone(buz);
  delay(60);
  noTone(buz);
  counter = counter + 1; //counter variable increments..
  }
}
