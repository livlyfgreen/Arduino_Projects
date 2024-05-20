/*
  Blind Man Apparatus

  Language: Arduino

  The circuit:

  - ultrasonic sensor attached on digital pins 3 and 2

  - ldr is connected on A1 pin

  - buzzer is on D13

  - Bluetooth module on digital pins 5 and 6

  created 01 Jul 2021

  by Aspire Technologies

  for more references, you can visit the public domain

  http://www.arduino.cc/

*/

#include<SoftwareSerial.h> //library for bluwtooth module
SoftwareSerial bluetooth(5,6); //bluetooth module object creation
int ldr = A1; 
int buzzer = 13;
int trig = 3;
int echo = 2;
int distance;
int duration;
int light; //variable that stores the input values from ldr
String input = ""; //string variable that stores the inputs from bluetooth mpodule, initially there is no data in it

void setup()
{
  Serial.begin(9600); //opens serial monitor at a baud rate of 9600
  bluetooth.begin(9600); //initiates the bluetooth module for serial communication at a baud rate of 9600
  pinMode(ldr,INPUT); //pin mode declarations
  pinMode(echo,INPUT);
  pinMode(buzzer,OUTPUT);
  pinMode(trig,OUTPUT);  
}

void loop()
{
  light = analogRead(ldr); //assigning the input value to the created variable
  if(bluetooth.available()) //when the bluetooth module is connected to the smartphone
  {
    input = bluetooth.readString(); //contineously reads the inputs from the connected device to the created string variable
  }
  digitalWrite(trig,LOW); //initially sets the trigger pin low
  delayMicroseconds(10); //providing a very minimal delay
  digitalWrite(trig,HIGH); //triggers the ultrasonic ray
  delayMicroseconds(20); // triggers for some minimal time
  digitalWrite(trig,LOW); //stops triggering and waits for the echo from objects
  duration = pulseIn(echo,HIGH); //whenevr the echo pin becomes high, that means the object is present in front of the sensor and calculates the duration to receives the echo signal
  distance = duration * 0.034/2; //equation for calculating the distance from the duration
  
  if(distance < 50) //when object is too close to the sensor
  {
    objectalert(); //calling the created function
  }
  if(light > 300 && distance > 50) //when there is no distance alert and it is day time
  {
    daytime(); //calling the corresponding created function
  }
  if(light < 300 && distance > 50) //when there is no distance alert and it is night time
  {
    nighttime(); //calling the corresponding created function
  } 
  if(input == "where is my stick") //when the blind man sends this voice command
  {
    digitalWrite(buzzer,HIGH); //buzzer beeps for 5 seconds and stops
    delay(5000);
    noTone(buzzer);
    input = ""; // clearing the string to avoid repeated execution of the loop
  }
}

void daytime() //buzzer beep for day time
{
  digitalWrite(buzzer,HIGH);
  delay(40);
  noTone(buzzer);
  delay(40);
  digitalWrite(buzzer,HIGH);
  delay(40);
  noTone(buzzer);
  delay(40);
  delay(1000);
}

void nighttime() //buzzer beep for night time
{
  digitalWrite(buzzer,HIGH);
  delay(100);
  noTone(buzzer);
  delay(100);
  delay(1000);
}

void objectalert() //buzzer beep for object alert
{
  digitalWrite(buzzer,HIGH);
  delay(30);
  noTone(buzzer);
  delay(30);
  digitalWrite(buzzer,HIGH);
  delay(30);
  noTone(buzzer);
  delay(30);
}
