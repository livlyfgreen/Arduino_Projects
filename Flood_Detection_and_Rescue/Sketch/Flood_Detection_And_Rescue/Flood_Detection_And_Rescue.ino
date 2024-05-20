/*
  Flood Detection And Rescue    
  
  Language: Arduino

  The circuit:

  - water level sensor is on A2 pin of arduino

  - servo motor is on D11 pin of arduino

  - red led is on D9 pin

  - green led is on D8 pin

  - buzzer is on D10 pin

  - keypad is on digital pins 0,1,2,3,4,5,6,7
  
  created 01 Jul 2021

  by Aspire Technologies

  for more references, you can visit the public domain

  http://www.arduino.cc/

*/

#include<Servo.h> //calling library for servo motor
#include <Keypad.h> //calling library for keypad
 
const byte ROWS = 4; //four rows
const byte COLS = 4; //four columns
char keys[ROWS][COLS] = { //creating keypad architecture in matrix form
{'1','2','3','A'},
{'4','5','6','B'},
{'7','8','9','C'},
{'*','0','#','D'}
};
byte rowPins[ROWS] = {0,1,2,3}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {4,5,6,7}; //connect to the column pinouts of the keypad
 
Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS ); //object creation for keypd
 
String password = "123"; //this is the shutter password
String mypassword; //variable to store user inputs

Servo myservo; //object creation for servo motor
int water = A2;
int green = 8;
int red = 9;
int buz = 10;
int reading; //variable to store water level

void setup()
{
  pinMode(green,OUTPUT); //pin mode declerations
  pinMode(red,OUTPUT);
  pinMode(buz,OUTPUT);
  pinMode(water,INPUT);
  myservo.attach(11);
  myservo.write(0); //initially the drainage shutter will be closed
  Serial.begin(9600); //sets serial port to 9600
}
void loop()
{
  reading = analogRead(water); //assigning sensor reading to the created variable
  Serial.println(reading); //water level will be printed on the seral monitor
  if(reading<500) //if water level is normal
  {
    myservo.write(0); //shutter is closed
    digitalWrite(green,HIGH); //green led is on
    digitalWrite(red,LOW); //red led is off
    noTone(buz); //no alarm
    delay(100);
  }
  if(reading>500) //if water level increases
  {
   digitalWrite(red,HIGH); //red led on
   digitalWrite(green,LOW); //green led off
   tone(buz,900); //buzzer activates alarm
   delay(100);
   noTone(buz);
   delay(100);
   }
   /* to open the drainage system, an authentic person needs to 
      enter the shutter password on the keypad. if the entered 
      password is correct, shutter will open. o the excess amount of 
      water will be gone through the drainage system 
    */
   keypadfunction();  //calling the created function for taking inputs from keypad
}

void keypadfunction() //function created for keypad reading
{
 char key = keypad.getKey(); //checks for any keypress
  
  if (key){ //if a keypress occour
    Serial.print(key); //pressed key will be displayed on serial monitor
  }
  if (key == '1') //if the pressed key is 1
  {
 
    mypassword = mypassword + 1;  /*that 1 will be added to the created string. all the 
                                   other inputs will also added to the string like this*/ 
  }
  
    if (key == '2')
  {
 
    mypassword = mypassword + 2;  
  }
  
  if (key == '3')
  {
 
    mypassword = mypassword + 3; 
  }
  
   if (key == '4')
  {
  
    mypassword = mypassword + 4;  
  }
  
  if (key == '5')
  {
  
    mypassword = mypassword + 5;
  }
  
   if (key == '6')
  {
   
    mypassword = mypassword + 6; 
  }
  
   if (key == '7')
  {
 
    mypassword = mypassword + 7; 
  }
 
   if (key == '8')
  {
 
    mypassword = mypassword + 8; 
  }
  
  if (key == '9')
  {
 
    mypassword = mypassword + 9;
  }
             
  if (key == '0')
  {
 
    mypassword = mypassword + 0; 
  }

  if (key == 'A' || key == 'B' || key == 'C' || key == 'D')
  {
 
    mypassword = ""; //to clear the entered password, you can press keys A or B or C or D
  }
  
  if (key == '*' || key == '#')
  {
    Serial.println(mypassword); /*if the password is entered, user has to press the * or # buttons to 
                                  open the shutter */
    
if ( password == mypassword ) //if the password is true
{
mypassword = ""; //string will be cleared
myservo.write(90); //shutter opens
}
else //if the entered password is not true
{
Serial.println("wrong");
mypassword = "";
myservo.write(0);
}
  }
}
