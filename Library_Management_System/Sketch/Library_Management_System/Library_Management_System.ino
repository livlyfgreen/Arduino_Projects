/*
  Library Management System

  Language: Arduino

  The circuit:

  - rfid module attached to 13,12,11,10,9 pins of Arduino

  - bluetooth module on 6,7 

  - buzzer on D5 pin

  - red pin of the cc led on D4

  - green pin of the cc led on D3

  - blue pin of the cc led on D2

  created 17 Apr 2022

  by Aspire Technologies

  for more references, you can visit the public domain

  http://www.arduino.cc/

*/


#include<SPI.h> //library od spi bus for the rfid module
#include<MFRC522.h> //original library for the frid module
#include<SoftwareSerial.h> //original library for the bluetooth module
#define buzzer 5 //defining buzzer
#define r 4 //defining red led
#define g 3 //defining green led
#define b 2 //dfining blue led

MFRC522 module(10,9); //object creation for rfid module
SoftwareSerial blue(6,7); //object creation for bluetooth module

String clone = ""; //string variable for saving the uid from rfid module, initially kept as empty
String input = ""; //string variable for saving the text messages from bluetooth module, initially kept as empty
String book1num = "F3 67 5B 01"; //uid of the first book
String book2num = "34 E5 B5 30"; //uid of the second book
String book3num = "C1 A7 E8 FE"; //uid of the third book
String book1name = "THE SHINING"; //name of the first book
String book2name = "A LITTLE PRINCESS"; //name of the second book
String book3name = "REMEMBER ME?"; //name of the third book
String tempname = ""; //variable which stores the name of the book holder for a short period of time
String tempmob = ""; //variable which stores the mobile number of the book holder for a short period of time
String name1 = ""; //variable which stores the name of the book1 holder 
String name2 = "";//variable which stores the name of the book2 holder 
String name3 = "";//variable which stores the name of the book3 holder 
String mob1 = ""; //variable which stores the mobile number of the book1 holder 
String mob2 = ""; //variable which stores the mobile number of the book2 holder 
String mob3 = ""; //variable which stores the mobile number of the book3 holder 

double timer1 = 0; //timer for book 1 and its initial value is 0
double timer2 = 0; //timer for book 2 and its initial value is 0
double timer3 = 0; //timer for book 3 and its initial value is 0

//below are the boolean variables created for maintaining the flow of the program

bool start = false;
bool nameask = false;
bool mobask = false;
bool scanask = false;
bool clearchance = false;
bool rented1 = false;
bool rented2 = false;
bool rented3 = false;
bool clock1 = false;
bool clock2 = false;
bool clock3 = false;
bool member = false;

void setup() //setup function
{
  SPI.begin(); //initiates the spi bus
  module.PCD_Init(); //initiates the rfid module
  blue.begin(9600); //creating the channel for bluetooth module at 9600 baud rate
  
  //pin mode declarations
  
  pinMode(buzzer,OUTPUT);
  pinMode(r,OUTPUT);
  pinMode(g,OUTPUT);
  pinMode(b,OUTPUT);
}

void loop() //opening the main loop
{
  while(blue.available()) //whenever there is a connection established between bluetooth module and smartphone
  {
    input = blue.readString(); //saving the inputs from the user to the created string variable
  }
  rentingBook(); //calling the seperatly written function to the main loop
  historyCheck(); //calling the seperatly written function to the main loop
  rfid(); //calling the seperatly written function to the main loop
  
  if(clone == "CA 91 85 78" && member == false) //if a person comes with a genuine card while the system was off
  {
    
    //card is genuine, so the system is turned on
    
    member = true;  
    digitalWrite(r,LOW);
    digitalWrite(b,LOW);
    digitalWrite(g,HIGH); 
    digitalWrite(buzzer,HIGH); 
    delay(300);
    noTone(buzzer);
    delay(2000);
    digitalWrite(g,LOW);
    digitalWrite(b,HIGH);
    clone = "";
  }
  if(clone != "" && clone != "CA 91 85 78" && clone != "F3 67 5B 01" && clone != "34 E5 B5 30" && clone != "C1 A7 E8 FE" && member == false) 
  //if a person comes with a fake card while the system was off, also we gives exception for the book uid's
  {

    //card isn't genuine so, the system will stay turned off, initiates the alarm and the red led indication
    
    member = false;
    digitalWrite(g,LOW);
    digitalWrite(b,LOW);
    digitalWrite(r,HIGH);
    digitalWrite(buzzer,HIGH);
    delay(50);
    noTone(buzzer);
    delay(50);
    digitalWrite(buzzer,HIGH);
    delay(50);
    noTone(buzzer);
    delay(30);
    digitalWrite(buzzer,HIGH);
    delay(50);
    noTone(buzzer);
    delay(50);
    delay(2000);
    digitalWrite(r,LOW);
    clone = "";
  }
}

void rfid() //seperate function for rfid module
{
  if(!module.PICC_IsNewCardPresent()) //while there is no card present in the proximity range of the rfid module
  {
    return;
  }
  if(!module.PICC_ReadCardSerial()) //while card data reading fails
  {
    return;
  }
  String data = ""; //string which stores the byte by byte data received from the rfid module
  for(byte i = 0; i < module.uid.size; i++) //receiving the card uid byte by byte
  {
    data.concat(String(module.uid.uidByte[i] < 0x10 ? " 0" : " ")); //combaining the received bytes to the main string
    data.concat(String(module.uid.uidByte[i], HEX)); //it's in the hex codes
  }
  data.toUpperCase(); //moving everything we have in the main string to upper case letter
  clone = data.substring(1); //assigning a substring from the main string to the created string variable
}

void rentingBook() //seperate function created for the book renting process
{
  if(input == "start" && start == false && member == true) //when the start message is sent after identifying the user
  {
    start = true;
    blue.print("You are now connected to the library manageent system prepared by Aspire Technologies"); //these messages will be sent as reeplies
    blue.print(";");
    delay(500);
    blue.print("If you are giving book from the library, please send <BOOK RENTING>");
    blue.print(";");
    delay(500);
    blue.print("If you want to check your book renting history, send <HISTORY CHECK>");
    blue.print(";");
    input = ""; //clearing the input value to avoid the repeated execution of the loop
  }
  if(input != "" && input == "BOOK RENTING" && start == true) //after start variable becomes true, if the input is book renting
  {
    start = false; //start variable changes its state to its initial state
    blue.print("Please enter the name of the book holder"); //asks to enter the name of the book holder
    blue.print(";");
    nameask = true; //nameask variable becomes true because name asked
    input = "";
  }
  if(input != "" && nameask == true) //if an input sent after name asked
  {
    tempname = input; //saving the input vallue to the temp variable
    nameask = false; //status changes
    blue.print("Name of the holder has been updated!");
    blue.print(";");
    delay(500);
    blue.print("Now enter the mobile number of the book holder"); //asking for the mobile number of the book holedr
    blue.print(";");
    mobask = true; //mobask variable becomes true
    input = "";  
  }
  if(input != "" && mobask == true) //if there is an input comes after asking for mobile number
  {
    tempmob = input; //saving the input data to the temporary variable
    mobask = false; //status changed
    blue.print("Now before giving the book, please scan the tag of the book");
    blue.print(";");
    delay(300);
    blue.print("The long beep indicates the successful registration of the holder"); 
    blue.print(";");
    blue.print("Please hand over the book to the person after the long beep"); //asking to scan the book tag
    blue.print(";");
    scanask = true; //scanask variable becomes true
    input = "";
  }
  if(clone == book1num && scanask == true && rented1 == false) //if the scanned book is book1
  {
    digitalWrite(buzzer,HIGH); //long beep
    delay(1500); 
    noTone(buzzer);
    name1 = tempname; //saving the data from the temporary variable to the name1 varianle 
    mob1 = tempmob; //saving the data from the temporary variable to to the mob1 varianle 
    clock1 = true; //the renting is started, so the timer variable becomes true
    scanask = false; //status changed
    clearchance = true; //now the user have a chance to return the book, so this variable kept true
    rented1 = true; //book 1 is rented, so the variable is kept true
    clone = "";
    tempname = ""; //clearing the temporary variables
    tempmob = "";
    member = false; //forgetting the user and turning the system off
    delay(500);
    digitalWrite(b,LOW);
  }
  if(clone == book2num && scanask == true && rented2 == false) //if the scanned book is book2, the same as above will be happened to variables related to book 2
  {
    digitalWrite(buzzer,HIGH);
    delay(1500);
    noTone(buzzer);
    name2 = tempname;
    mob2 = tempmob;
    clock2 = true;
    scanask = false;
    clearchance = true;
    rented2 = true;
    clone = "";
    tempname = "";
    tempmob = "";
    member = false;
    delay(500);
    digitalWrite(b,LOW);
  }
  if(clone == book3num && scanask == true && rented3 == false) //if the scanned book is book3, the same as above will be happened to variables related to book 3
  {
    digitalWrite(buzzer,HIGH);
    delay(1500);
    noTone(buzzer);
    name3 = tempname;
    mob3 = tempmob;
    clock3 = true;
    scanask = false;
    clearchance = true;
    rented3 = true;
    clone = "";
    tempname = "";
    tempmob = "";
    member = false;
    delay(500);
    digitalWrite(b,LOW);  
    }
  if(clone == book1num && clock1 == true && rented1 == true && clearchance == true && member == true) //when the user returns the rented book
  {
    digitalWrite(buzzer,HIGH); //two short beeps will be produced by the buzzer
    delay(50);
    noTone(buzzer);
    delay(50);
    digitalWrite(buzzer,HIGH);
    delay(50);
    noTone(buzzer);
    name1 = ""; //clearing the name1 variable
    mob1 = ""; //clearing the mob1 variable
    clock1 = false; //timer has to stop, so changing the state
    timer1 = 0; //timer is reset to 0
    rented1 = false; //no more renting of the book 1
    clone = "";
    member = false; //forgetting the user and turning the system off
    delay(500);
    digitalWrite(b,LOW); 
  }
  if(clone == book2num && clock2 == true && rented2 == true && clearchance == true && member == true) 
  //if the returned book is book2, the same as above will be happened to variables related to book 2
  {
    digitalWrite(buzzer,HIGH);
    delay(50);
    noTone(buzzer);
    delay(50);
    digitalWrite(buzzer,HIGH);
    delay(50);
    noTone(buzzer);
    name2 = "";
    mob2 = "";
    clock2 = false;
    timer2 = 0;
    rented2 = false;
    clone = "";
    member = false;
    delay(500);
    digitalWrite(b,LOW); 
  }
  if(clone == book3num && clock3 == true && rented3 == true && clearchance == true && member == true)
  //if the returned book is book3, the same as above will be happened to variables related to book 3
  {
    digitalWrite(buzzer,HIGH);
    delay(50);
    noTone(buzzer);
    delay(50);
    digitalWrite(buzzer,HIGH);
    delay(50);
    noTone(buzzer);
    name3 = "";
    mob3 = "";
    clock3 = false;
    timer3 = 0;
    rented3 = false;
    clone = "";
    member = false;
    delay(500);
    digitalWrite(b,LOW); 
  }
  
  if(clock1 == true && rented1 == true) //timer running when these conditions becomes true, which means when the first book is on rent
  {
    timer1 = timer1 + 0.001;
  }
  if(clock2 == true && rented2 == true) //timer running when these conditions becomes true, which means when the second book is on rent
  {
    timer2 = timer2 + 0.001;
  }
  if(clock3 == true && rented3 == true) //timer running when these conditions becomes true, which means when the third book is on rent
  {
    timer3 = timer3 + 0.001;
  }
}

void historyCheck() //seperate function created for the history checking process
{
  if(input != "" && input == "HISTORY CHECK" && start == true && rented1 == false && rented2 == false && rented3 == false) 
  //when the user checks history when all the books are in the library
  {
    start = false; //state changed
    clearchance = false; //when there is no renting, which means there is no possiblity of returning a book, so this variable is kept false
    blue.print("Checking history..");
    blue.print(";");
    delay(2000);
    blue.print("This may take some time..");
    blue.print(";");
    delay(3000);
    blue.print("No renting records found!"); 
    blue.print(";");
    input = "";
    member = false; //forgetting the user and turning off the system
    delay(500);
    digitalWrite(b,LOW);
  }
  if(input != "" && input == "HISTORY CHECK" && start == true && rented1 == false && rented2 == false && rented3 == true)
  //when the user checks history when the third book is on rent, the renting informations related to the book 3 will be displayed 
  {
    start = false;
    blue.print("Checking history..");
    blue.print(";");
    delay(2000);
    blue.print("This may take some time..");
    blue.print(";");
    delay(3000);
    blue.print("Book name: ");
    blue.println(book3name);
    blue.print("Holder: ");
    blue.println(name3);
    blue.print("Mob.No: ");
    blue.println(mob3);
    blue.print("Renting time: ");
    blue.print(timer3);
    blue.print(" Minutes");
    blue.print(";");
    member = false;
    delay(500);
    digitalWrite(b,LOW);
  }
  if(input != "" && input == "HISTORY CHECK" && start == true && rented1 == false && rented2 == true && rented3 == false)
  //when the user checks history when the second book is on rent, the renting informations related to the book 2 will be displayed 
  {
    start = false;
    blue.print("Checking history..");
    blue.print(";");
    delay(2000);
    blue.print("This may take some time..");
    blue.print(";");
    delay(3000);
    blue.print("Book name: ");
    blue.println(book2name);
    blue.print("Holder: ");
    blue.println(name2);
    blue.print("Mob.No: ");
    blue.println(mob2);
    blue.print("Renting time: ");
    blue.print(timer2);
    blue.print(" Minutes");
    blue.print(";");
    member = false;
    delay(500);
    digitalWrite(b,LOW);
  }
  if(input != "" && input == "HISTORY CHECK" && start == true && rented1 == false && rented2 == true && rented3 == true)
  //when the user checks history when the second and third book are on rent, the renting informations related to the book 2 and book 3 will be displayed 
  {
    start = false;
    blue.print("Checking history..");
    blue.print(";");
    delay(2000);
    blue.print("This may take some time..");
    blue.print(";");
    delay(3000);
    blue.print("Book name: ");
    blue.println(book2name);
    blue.print("Holder: ");
    blue.println(name2);
    blue.print("Mob.No: ");
    blue.println(mob2);
    blue.print("Renting time: ");
    blue.print(timer2);
    blue.print(" Minutes");
    blue.print(";");
    delay(1000);
    blue.print("Book name: ");
    blue.println(book3name);
    blue.print("Holder: ");
    blue.println(name3);
    blue.print("Mob.No: ");
    blue.println(mob3);
    blue.print("Renting time: ");
    blue.print(timer3);
    blue.print(" Minutes");
    blue.print(";");
    member = false;
    delay(500);
    digitalWrite(b,LOW);
  }
  if(input != "" && input == "HISTORY CHECK" && start == true && rented1 == true && rented2 == false && rented3 == false)
  //when the user checks history when the first book is on rent, the renting informations related to the book 1 will be displayed 
  {
    start = false;
    blue.print("Checking history..");
    blue.print(";");
    delay(2000);
    blue.print("This may take some time..");
    blue.print(";");
    delay(3000);
    blue.print("Book name: ");
    blue.println(book1name);
    blue.print("Holder: ");
    blue.println(name1);
    blue.print("Mob.No: ");
    blue.println(mob1);
    blue.print("Renting time: ");
    blue.print(timer1);
    blue.print(" Minutes");
    blue.print(";");
    member = false;
    delay(500);
    digitalWrite(b,LOW);
  }
  if(input != "" && input == "HISTORY CHECK" && start == true && rented1 == true && rented2 == false && rented3 == true)
  //when the user checks history when the first and third book are on rent, the renting informations related to the book 1 and book 3 will be displayed 
  {
    start = false;
    blue.print("Checking history..");
    blue.print(";");
    delay(2000);
    blue.print("This may take some time..");
    blue.print(";");
    delay(3000);
    blue.print("Book name: ");
    blue.println(book1name);
    blue.print("Holder: ");
    blue.println(name1);
    blue.print("Mob.No: ");
    blue.println(mob1);
    blue.print("Renting time: ");
    blue.print(timer1);
    blue.print(" Minutes");
    blue.print(";");
    delay(1000);
    blue.print("Book name: ");
    blue.println(book3name);
    blue.print("Holder: ");
    blue.println(name3);
    blue.print("Mob.No: ");
    blue.println(mob3);
    blue.print("Renting time: ");
    blue.print(timer3);
    blue.print(" Minutes");
    blue.print(";");
    member = false;
    delay(500);
    digitalWrite(b,LOW);
  }
  if(input != "" && input == "HISTORY CHECK" && start == true && rented1 == true && rented2 == true && rented3 == false)
  //when the user checks history when the first and second book are on rent, the renting informations related to the book 1 and book 2 will be displayed 
  {
    start = false;
    blue.print("Checking history..");
    blue.print(";");
    delay(2000);
    blue.print("This may take some time..");
    blue.print(";");
    delay(3000);
    blue.print("Book name: ");
    blue.println(book1name);
    blue.print("Holder: ");
    blue.println(name1);
    blue.print("Mob.No: ");
    blue.println(mob1);
    blue.print("Renting time: ");
    blue.print(timer1);
    blue.print(" Minutes");
    blue.print(";");
    delay(1000);
    blue.print("Book name: ");
    blue.println(book2name);
    blue.print("Holder: ");
    blue.println(name2);
    blue.print("Mob.No: ");
    blue.println(mob2);
    blue.print("Renting time: ");
    blue.print(timer2);
    blue.print(" Minutes");
    blue.print(";");
    member = false;
    delay(500);
    digitalWrite(b,LOW);
  }
  if(input != "" && input == "HISTORY CHECK" && start == true && rented1 == true && rented2 == true && rented3 == true)
  //when the user checks history when all the books in the library are on rent, the renting informations related to the books will be displayed 
  {
    start = false;
    blue.print("Checking history..");
    blue.print(";");
    delay(2000);
    blue.print("This may take some time..");
    blue.print(";");
    delay(3000);
    blue.print("Book name: ");
    blue.println(book1name);
    blue.print("Holder: ");
    blue.println(name1);
    blue.print("Mob.No: ");
    blue.println(mob1);
    blue.print("Renting time: ");
    blue.print(timer1);
    blue.print(" Minutes");
    blue.print(";");
    delay(1000);
    blue.print("Book name: ");
    blue.println(book2name);
    blue.print("Holder: ");
    blue.println(name2);
    blue.print("Mob.No: ");
    blue.println(mob2);
    blue.print("Renting time: ");
    blue.print(timer2);
    blue.print(" Minutes");
    blue.print(";");
    delay(1000);
    blue.print("Book name: ");
    blue.println(book3name);
    blue.print("Holder: ");
    blue.println(name3);
    blue.print("Mob.No: ");
    blue.println(mob3);
    blue.print("Renting time: ");
    blue.print(timer3);
    blue.print(" Minutes");
    blue.print(";");
    member = false;
    delay(500);
    digitalWrite(b,LOW);
  }
}
