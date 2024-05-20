/*
  Highly Secured Bank Locker  

  Language: Arduino

  The circuit:

  - RFID module on digital pins 13,12,11,10,9 of Arduino
  - servo motors on digital pins 5 and 6
  - touch sensor on D4 
  - buzzer on D3

  created 21 Sep 2021

  by Aspire Technologies

  for more references, you can visit the public domain

  http://www.arduino.cc/

*/


#include <SPI.h> //serial library
#include <MFRC522.h> //library for rfid module
#include<Servo.h> //servo library
#define SS_PIN 10 //defining ss pin
#define RST_PIN 9 //defining rst pin
int buz = 3;
int touchsensor = 4;
int touch; //variable created for storing the input of the touch sensor
String clone = ""; //string created for storing the card uid when it is approached to the reader
bool owner = false, lockopen = false, threat = false; //initially setting the boolean variable statuses false
MFRC522 mfrc522(SS_PIN, RST_PIN); //object creation for rfid module
Servo toplid; //object creation for servo1 (shutter)
Servo bottomlid; //object creation for servo2 (locker)

void setup()
{
  SPI.begin(); // Initiate  SPI bus
  mfrc522.PCD_Init(); // Initiate MFRC522
  toplid.attach(5);
  bottomlid.attach(6);
  toplid.write(0); //initially setting the shutter to 0 degrees means the gate is closed
  bottomlid.write(0); //initially setting the locker to 0 degrees means the gate is closed
  pinMode(buz, OUTPUT); //pin mode declerations
  pinMode(touchsensor, INPUT);
}
void loop()
{
  touch = digitalRead(touchsensor); //assigning the input of touch sensor to the created variable
  if (touch == HIGH) //when a person touches the touch sensor
  { 
    toplid.write(180); //shutter opens
  }

  
  if ( ! mfrc522.PICC_IsNewCardPresent()) // Look for new cards
  {
    return;
  }
  if ( ! mfrc522.PICC_ReadCardSerial()) // Select one of the cards
  {
    return;
  }
  String content; //main string created to store the uid
  byte letter;
  for (byte i = 0; i < mfrc522.uid.size; i++)
  {
    content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
    content.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
  content.toUpperCase();
  clone = content.substring(1); //assigning the substring 1 value to the created string variable
  if (clone == "33 9F CB 16" && lockopen == false) //when the uid is genuine and locker is closed(change here the UID of the card/cards that you want to give access)
  {
    tone(buz, 800); //buzzer beeps while reading the card
    delay(250);
    noTone(buz);
    bottomlid.write(180); //locker opens
    lockopen = true; //lock open status changed
    threat = false; //no threat
    clone = ""; //reset the string in the end
    delay(3000); //giving 3 seconds delay before reading the next 
  }

  if (clone == "33 9F CB 16" && lockopen == true) //when approximating the same card after the locker is opened
  {
  tone(buz, 800);
  delay(150);
  noTone(buz);
  bottomlid.write(0); //locker closed
  toplid.write(0); //shutter closed
  lockopen = false;
  threat = false;
  clone = "";
  delay(3000);
}
if (clone != "" && clone != "33 9F CB 16") //when approximating the wrong card
{
  threat = true; //threat enables
  clone = "";
}
if (clone == "" && threat == true) //alarm on!!
{
  tone(buz, 800);
  delay(50);
  noTone(buz);
  bottomlid.write(0);
  lockopen = false;
  clone = "";
}
delay(100);
}
