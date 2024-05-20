/*
  Front Gate Automation  

  Language: Arduino

  The circuit:

  - RFID module on digital pins 13,12,11,10,9 of Arduino
  - servo motor on D5
  - ir sensor on D3 
  - buzzer on D4
  - green led on D7
  - red led on D6

  created 21 Sep 2021

  by Aspire Technologies

  for more references, you can visit the public domain

  http://www.arduino.cc/

*/

#include <SPI.h> //library for serial communication
#include <MFRC522.h> //rfid library
#include<Servo.h> //servo library
#define SS_PIN 10 //defining ss pin
#define RST_PIN 9 //defining rst pin
int irsensor = 3;
int reading; //variable created for storing the input of the ir sensor
int buz = 4;
int green = 7;
int red = 6;
MFRC522 mfrc522(SS_PIN, RST_PIN); //object creation for rfid module
Servo myservo; //object creation for servo
bool gateopen = false; //initially setting the gate open status false
String clone = ""; //string created for storing the card uid when it is approached to the reader

void setup()
{
  SPI.begin(); // Initiate  SPI bus
  mfrc522.PCD_Init(); // Initiate MFRC522
  myservo.attach(5);
  myservo.write(0); //initially setting the servo to 0 degrees means the gate is closed
  digitalWrite(red, HIGH); //red led will be turned on when the gate is closed
  digitalWrite(green, LOW); //green led off
  pinMode(irsensor, INPUT); //pin mode declerations
  pinMode(buz, OUTPUT);
  pinMode(red, OUTPUT);
  pinMode(green, OUTPUT);
}
void loop()
{
  reading = digitalRead(irsensor); //assigning the input of ir sensor to the created variable
  if (reading == LOW && gateopen == true) //when ir sensor detects object and gate's status is open
  {
    myservo.write(180); //servo is at 180 degrees means the gate is open
  }
  if (reading == LOW && gateopen == false) //when ir sensor detects objects but the gate open status is false
  {
    myservo.write(0); //gate stays closed
  }
  if (reading == HIGH && gateopen == true) //when ir sensor not detects object ang gate is open
  {
    delay(2000); //waits for 2 seconds before closing the gate
    myservo.write(0); /closes the gate
    digitalWrite(red, HIGH); //red led on
    digitalWrite(green, LOW); //green off
    gateopen = false; //gate open status changed
  }
  if (reading == HIGH && gateopen == false) //when ir sensor is not detecting any objects and gate is closed
  {
    myservo.write(0); //gate stays closed
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
  if (clone == "33 9F CB 16") //when the uid is genuine (change here the UID of the card/cards that you want to give access)
  {
    gateopen = true; //gate open status will be true
    digitalWrite(green, HIGH); //green led is on
    digitalWrite(red, LOW); //red led off
    tone(buz, 900); //buzzer beeps while reading the card
    delay(100);
    noTone(buz);
    delay(2000);
    clone = ""; //clearing the string in the end
  }
  if (clone != "33 9F CB 16" && clone != "") //if the string holds a different uid 
  {
    gateopen = false; //gte open ststus changed 
    digitalWrite(red, HIGH); //red led on
    digitalWrite(green, LOW); //green led off
    tone(buz, 900); //buzzer makes noise when wrong card gets approximated
    delay(35);
    noTone(buz);
    clone = "";
  }

}

