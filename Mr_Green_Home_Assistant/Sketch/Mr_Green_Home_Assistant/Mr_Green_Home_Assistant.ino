/*
  Mr Green Your Offline Home Assistant  

  Language: Arduino

  The circuit:

  - bluetooth module on digital pins 2,3 of Arduino
  - servo motor on D9
  - red led on D4
  - green led on D5 
  - blue led on D6

  created 21 Sep 2021

  by Aspire Technologies

  for more references, you can visit the public domain

  http://www.arduino.cc/

*/

#include<SoftwareSerial.h> //library for bluetooth module
#include<Servo.h> //library for servo motor
#include<String.h>
Servo myservo; //object created for servo motor
SoftwareSerial green(2, 3); //tx,rx //object created for bluetooth module
#define r 4
#define g 5
#define b 6
#define buz 8

int f1 = 0; //oject created for maintaining the program flow
String input, doorpassword = "aspire"; //strings created for storing the input and door password
bool opencommand = false, doorclose = true, dooropen = false; //boolean variables created for maintaining the program flow
bool light = false, changecommand = false;
void setup()
{
  green.begin(9600); //initializing the bluetooth module
  myservo.attach(9);
  myservo.write(0);
  pinMode(r, OUTPUT); //pin mode decleratons
  pinMode(g, OUTPUT);
  pinMode(b, OUTPUT);
  pinMode(buz,OUTPUT);
}
void loop()
{
  if (green.available() && f1 == 0) //portion that needs to be executed once that's why we declared the f1 variable
  {
    digitalWrite(buz,HIGH); //producing beep sound after every message reception
    delay(50);
    noTone(buz);
    green.print("h!, i'm mr green, your offline home assistant"); //printing on the mobile phone
    green.print(";"); //semi coloumn seperates the sentences
    delay(2000);
    green.print("how can i help you?");
    green.print(";");
    delay(1500);
    green.print("i'm able to process the following 6 commands from you");
    green.print(";");
    delay(2000);
    green.print("1. open the door");
    green.print(";");
    delay(500);
    green.print("2. close the door");
    green.print(";");
    delay(500);
    green.print("3. light on");
    green.print(";");
    delay(500);
    green.print("4. change colour");
    green.print(";");
    delay(500);
    green.print("5. light off");
    green.print(";");
    delay(500);
    green.print("6. tell me your story");
    green.print(";");
    delay(2000);
    green.print("remember, i only read small case letters, i actually don't care about other case letters!");
    green.print(";");
    f1 = 1;
  }
  if (green.available() && f1 == 1) //this part will be looped throughout the program
  {
    input = green.readString(); //getting inputs from mobile phone
  }
  if (input == "close the door" && doorclose == false && dooropen == true) //when input is to close the door and door is open
  {
    digitalWrite(buz,HIGH);
    delay(50);
    noTone(buz);
    
    for(int i = 60; i > 0; i--) //initially servo is at 0 degrees means the door is closed
  {
    myservo.write(i);
    delay(20);
  } //door closed
    input = ""; //cleared the input in the end to take the next
    doorclose = true; //status changed
    dooropen = false; //status changed
  }
  if (input == "close the door" && doorclose == true && dooropen == false) //when the same input is received after closing the door
  {
    digitalWrite(buz,HIGH);
    delay(50);
    noTone(buz);
    green.print("just look at the door dude, it's already been closed!!");
    green.print(";");
    delay(1500);
    green.print("why are you so afraid of closing the door every time?? what have you done??! ");
    green.print(";");
    input = "";
    doorclose = true;
    dooropen = false;
  }
  if (input == "open the door" && opencommand == false && doorclose == true && dooropen == false) //when the input is to open the door and the door is closed
  {
    digitalWrite(buz,HIGH);
    delay(50);
    noTone(buz);
    green.print("im a guy with a lot of trust issues! if you want to open the door,");
    green.print(";");
    delay(1500);
    green.print("please enter the door password that we both kept as a secret and turn on the door switch");
    green.print(";");
    input = "";
    opencommand = true; //a door open command will be saved and if the next input is the door password, the door will be open
  }
  if (input == doorpassword && opencommand == true && doorclose == true && dooropen == false) //if the input matches the door password
  {
    digitalWrite(buz,HIGH);
    delay(350);
    noTone(buz);
    for(int i = 0; i < 60; i++) //initially servo is at 0 degrees means the door is closed
  {
    myservo.write(i);
    delay(20);
  } //door opens
    opencommand = false;
    doorclose = false;
    dooropen = true;
    input = "";
  }
  if (input != doorpassword && input != "" && opencommand == true && doorclose == true && dooropen == false)
  {
    digitalWrite(buz,HIGH);
    delay(100);
    noTone(buz);
    delay(50);
    digitalWrite(buz,HIGH);
    delay(100);
    noTone(buz);
    delay(50);
    digitalWrite(buz,HIGH);
    delay(100);
    noTone(buz);
    delay(50);
    green.print("this is why i am having trust issues!!");
    green.print(";");
    delay(1000);
    green.print("the password you have entered is wrong!");
    green.print(";");
    delay(1000);
    green.print("please enter the right password..");
    green.print(";");
    opencommand = true;
    doorclose = true;
    dooropen = false;
    input = "";
  }
  if (input == "open the door" && opencommand == false && doorclose == false && dooropen == true) //if the same command receives after closing the input
  {
    digitalWrite(buz,HIGH);
    delay(50);
    noTone(buz);
    green.print("if you want to open your door, you have to close it first!");
    green.print(";");
    opencommand = false;
    doorclose = false;
    dooropen = true;
    input = "";
  }
  if (input == "light on" && light == false) //when input is light on command and light is off
  {
    digitalWrite(buz,HIGH);
    delay(50);
    noTone(buz);
    digitalWrite(b, HIGH); //light on!
    digitalWrite(r, LOW);
    digitalWrite(g, LOW);
    light = true; //state changed
    green.print("okay! light on!!");
    green.print(";");
    input = "";
  }
  if (input == "light on" && light == true && changecommand == false) //when same command repeats
  {
    digitalWrite(buz,HIGH);
    delay(50);
    noTone(buz);
    digitalWrite(b, HIGH);
    digitalWrite(r, LOW);
    digitalWrite(g, LOW);
    light = true;
    green.print("are you a blind?! it's already lit up!");
    green.print(";");
    input = "";
  }
  if (input == "change colour" && light == true && changecommand == false) //when input is to change the colour of the light and light is on
  {
    digitalWrite(buz,HIGH);
    delay(50);
    noTone(buz);
    green.print("in which colour you want to see your room?");
    green.print(";");
    delay(1000);
    green.print("i can switch your room into these colours..");
    green.print(";");
    delay(1000);
    green.print("1. red");
    green.print(";");
    delay(10);
    green.print("2. green");
    green.print(";");
    delay(10);
    green.print("3. blue");
    green.print(";");
    changecommand = true; //a colour change command will be saved and waits for the input to change the colour
    input = "";
  }
  if (input == "red" && light == true && changecommand == true) //when input is red
  {
    digitalWrite(buz,HIGH);
    delay(50);
    noTone(buz);
    digitalWrite(b, LOW);
    digitalWrite(r, HIGH);
    digitalWrite(g, LOW);
    green.print("yeah, red is the warmest colour!");
    green.print(";");
    changecommand = false;
    input = "";
  }
  if (input == "blue" && light == true && changecommand == true) //blue
  {
    digitalWrite(buz,HIGH);
    delay(50);
    noTone(buz);
    digitalWrite(b, HIGH);
    digitalWrite(r, LOW);
    digitalWrite(g, LOW);
    green.print("woaaah! looks like we are in the middle of an ocean");
    green.print(";");
    changecommand = false;
    input = "";
  }
  if (input == "green" && light == true && changecommand == true) //green
  {
    digitalWrite(buz,HIGH);
    delay(50);
    noTone(buz);
    digitalWrite(b, LOW);
    digitalWrite(r, LOW);
    digitalWrite(g, HIGH);
    green.print("i love green, green is the colour of mother earth! livlyfgreen!");
    green.print(";");
    changecommand = false;
    input = "";
  }
  if (input == "change colour" && light == false) //if the input is to change the colour before turning on the light
  {
    digitalWrite(buz,HIGH);
    delay(50);
    noTone(buz);
    green.print("please turn on the light to change its colour");
    green.print(";");
    input = "";
  }
  if (input == "light off" && light == true) //when the input is to turn off the light
  {
    digitalWrite(buz,HIGH);
    delay(50);
    noTone(buz);
    digitalWrite(b, LOW);
    digitalWrite(r, LOW);
    digitalWrite(g, LOW);
    green.print("peace... light off!");
    green.print(";");
    light = false;
    input = "";
  }
  if (input == "light off" && light == false) //when command repeats
  {
    digitalWrite(buz,HIGH);
    delay(50);
    noTone(buz);
    digitalWrite(b, LOW);
    digitalWrite(r, LOW);
    digitalWrite(g, LOW);
    green.print("why are you being so stupid?!");
    green.print(";");
    delay(1000);
    green.print("the light is already off !");
    green.print(";");
    light = false;
    input = "";
  }
  if (input == "tell me your story") //when the input is to tell the story of the bot
  {
    digitalWrite(buz,HIGH);
    delay(50);
    noTone(buz);
    green.print("my story is not that kinda odd one");
    green.print(";");
    delay(3000);
    green.print("im mr green.");
    green.print(";");
    delay(1500);
    green.print("im a robot that is invented by aspire technologies,");
    green.print(";");
    delay(3000);
    green.print("to perform automated, repetitive and pre-defined tasks.");
    green.print(";");
    delay(3000);
    green.print("i typically imitates or replaces human user behaviour.");
    green.print(";");
    delay(3000);
    green.print("to see more, call me twice");
    green.print(";");
    input = "";
  }
  if (input == "mr green") //if the input is mr green, the one time runnable portion will again run once
  {
    digitalWrite(buz,HIGH);
    delay(50);
    noTone(buz);
    delay(10);
    digitalWrite(buz,HIGH);
    delay(50);
    noTone(buz);
    delay(10);
    f1 = 0;
    input = "";
  }
}
