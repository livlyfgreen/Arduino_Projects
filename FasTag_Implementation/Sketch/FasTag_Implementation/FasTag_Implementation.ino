/*
  FasTag Implementation

  Language: Arduino

  The circuit:

  - RFID module on digital pins 9,10,11,12,13 of arduino
  - lcd on analog pins A0,A1,A2,A3,A4,A5
  - servo motor on D7
  - bluetooth module on digital pins 2 and 3
  - buzzer on D5

  created 01 Nov 2021

  by Aspire Technologies

  for more references, you can visit the public domain

  http://www.arduino.cc/

*/

#include <SPI.h> //libraries for rfid module
#include <MFRC522.h>
#include<LiquidCrystal.h> //library for lcd
#include<Servo.h> //library for servo motor
#include<SoftwareSerial.h> //library for bluetooth module

#define SS_PIN 10
#define RST_PIN 9
#define buz 5

MFRC522 mfrc522(SS_PIN, RST_PIN);   // Create MFRC522 instance. 
LiquidCrystal lcd(A0, A1, A2, A3, A4, A5); //create lcd instance
Servo servo; //create servo instance
SoftwareSerial bluetooth(2, 3); //create bluetooth instance

int mainbal = 100; //creating an integer variable to store the main balance and assigning a value for it
int recharge_amount = 0; //initially there is no recharge amount
int pos = 0; //servo motor's position
int fk = 0; //variable declared for maintainig the flow of the program
bool credit = true; //the main balance is greater than the fast tag credit amount(25) so credit is possible
bool recharge_request = false, upi_asking = false; //initially there is no recharge request and there for no upi asking
String clone = ""; //string variable for saving the upi id of the card
String data = ""; //tring variable for storing the inputs from the bluetooth terminal

void setup()
{
  Serial.begin(9600);   // Initiate a serial communication
  lcd.begin(16, 2); //initiate lcd
  bluetooth.begin(9600); //initiate bluetooth module at 9600
  servo.attach(7);
  servo.write(pos); //writing the servo's postition
  SPI.begin();      // Initiate  SPI bus
  mfrc522.PCD_Init();   // Initiate MFRC522
  
  pinMode(buz, OUTPUT); //pin mode decleration
  Serial.println("Approximate your card to the reader..."); //printing on serial monitor
  Serial.println();
  lcd.clear(); //clearing the lcd panel
  lcd.setCursor(0, 0);
  lcd.print("FasTag Service"); //printing on lcd


}
void loop()
{
  if (mainbal > 44) //when main balance is greater than the fats tag credit amount
  {
    credit = true; //credit is possible
  }
  if (mainbal < 44) //if the main balance is insufficient
  {
    credit = false; //credit is not possible
  }
  if (bluetooth.available()) //when the terminal device is connected to the bluetooth module
  {
    data = bluetooth.readString(); //contiouously reads the inputs from the terminal
  }
  if (data == "my balance") //if this is the input
  {
    bluetooth.print("Your FastTag balance is ");
    bluetooth.print(mainbal); //printing the current main balance on the terminal screen
    bluetooth.print(";");
    data = ""; //clearing the input to null in the end of the loop to avoid repeated printing of contents
  }
  if (data == "reset") //if this is the input
  {
    mainbal = 0; //resetting the main balance to zero 
    bluetooth.print("Your FastTag balance has been cleared");
    bluetooth.print(";");
    delay(1000);
    bluetooth.print("Your current FastTag balance is ");
    bluetooth.print(mainbal);
    bluetooth.print(";");
    data = "";
  }
  if (data == "recharge") //to recharge the main balance
  {
    bluetooth.print("Please enter the recharge amount"); //user have to select a recharge plan
    bluetooth.print(";");
    recharge_request = true; //recharge request enabled
    data = "";
  }
  if (data != "" && recharge_request == true) //if the recharge request is enabled and user choose a plan
  {
    recharge_amount = data.toInt(); //the recharge amount is set as the selected plan
    bluetooth.print("You have requested to recharge your FastTag account with ");
    bluetooth.print(recharge_amount);
    bluetooth.print(";");
    delay(500);
    bluetooth.print("Please enter your UPI pin to continue"); //asking user to enter the upi pin of his bank account
    bluetooth.print(";");
    upi_asking = true; //upi asking status changed
    recharge_request = false;
    data = "";
  }
  if (data == "123456" && upi_asking == true) //if the user enter the upi pin correctly
  {
    bluetooth.print("Recharge processing.."); //recharge will be processed
    bluetooth.print(";");
    delay(2000);
    mainbal = mainbal + recharge_amount; //updating the main balance after the recharge
    bluetooth.print("Recharge successful!");
    bluetooth.print(";");
    delay(500);
    bluetooth.print("Your new account balance is ");
    bluetooth.print(mainbal); //printing the new main balance
    bluetooth.print(";");
    data = "";
    recharge_amount = 0; //clearing the saved recharge amount for the next recharge
    upi_asking = false;
  }
  if (data != "" && data != "123456" && upi_asking == true) //if the entered upi pin is wrong
  {
    bluetooth.print("You have entered wrong UPI pin!!!");
    bluetooth.print(";");
    delay(500);
    bluetooth.print("Recharge cancelled!");
    bluetooth.print(";");
    upi_asking = false;
    recharge_request = false; //recharge cancelled
    recharge_amount = 0;
    data = "";
  }
  // Look for new cards
  if ( ! mfrc522.PICC_IsNewCardPresent())
  {
    return;
  }
  // Select one of the cards
  if ( ! mfrc522.PICC_ReadCardSerial())
  {
    return;
  }
  //Show UID on serial monitor
  Serial.print("UID tag :");
  String content = "";
  byte letter;
  for (byte i = 0; i < mfrc522.uid.size; i++)
  {
    Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
    Serial.print(mfrc522.uid.uidByte[i], HEX);
    content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
    content.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
  Serial.println();
  Serial.print("Message : ");
  content.toUpperCase();
  clone = content.substring(1);
  if (clone == "C6 2B 3A 2C" && credit == true) //if the uid matches and credit is possible
  {
    mainbal = mainbal - 44; //fast tag amount is debiting from the main account balance
    digitalWrite(buz,HIGH);
    delay(300);
    noTone(buz);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("FastTag Credit"); //printing status on the lcd
    lcd.setCursor(0, 1);
    lcd.print("done..");
    for (pos; pos < 90; pos ++) //gate opens
    {
      servo.write(pos);
      delay(20);
    }
    delay(2000);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Remaining Balance"); //printing the remaining balance
    lcd.setCursor(0, 1);
    lcd.print("is ");
    lcd.setCursor(4, 1);
    lcd.print(mainbal);
    
    delay(5000);
    lcd.clear();
    fk = 0;
    clone = ""; //resetting the uid to avoid repeation of the process
  }
  if (clone == "C6 2B 3A 2C" && credit == false) //when the card reaches the reader with insufficient balance
  {
    tone(buz, 2000); //buzzer alarm 
    delay(50);
    noTone(buz);
    delay(40);
    tone(buz, 2000);
    delay(50);
    noTone(buz);
    delay(20);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Insufficient");
    lcd.setCursor(0, 1);
    lcd.print("balance");
    delay(2000);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Please recharge!"); //asking to recharge the account
    delay(700);
    lcd.clear();
    fk = 0;
    clone = "";
  }
  if (clone == "") //when the card is gone away from the reader
  {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("FasTag Service");
    for (pos; pos > 0; pos --) //gate closes
    {
      servo.write(pos);
      delay(20);
    }
    if(fk == 0) //giving buzzer indication that the reader is ready to read the next card
    {
    tone(buz,1000);
    delay(75);
    noTone(buz);
    fk = 1;
    }
  }
}
