#define sensor1 7
#define sensor2 6
#define red 8
#define green 10
#define buz1 5
#define buz2 4
int read1, read2;
bool people = false;

void setup()
{
  pinMode(sensor1,INPUT);
  pinMode(sensor2,INPUT);
  pinMode(red,OUTPUT);
  pinMode(green,OUTPUT);
  pinMode(buz1,OUTPUT);
  pinMode(buz2,OUTPUT);
}

void loop()
{
 read1 = digitalRead(sensor1);           
 read2 = digitalRead(sensor2);
 if(people == false)
 {
  digitalWrite(red,LOW);
  digitalWrite(green,HIGH);
  noTone(buz2);
  digitalWrite(buz1,HIGH);
  delay(50);
  noTone(buz1);
  delay(30);
  digitalWrite(buz1,HIGH);
  delay(50);
  noTone(buz1); 
 }
 if(people == true)
 {
  digitalWrite(green,LOW);
  digitalWrite(red,HIGH);
  noTone(buz1);
  digitalWrite(buz2,HIGH);
  delay(50);
  noTone(buz2);
  delay(30);
  digitalWrite(buz2,HIGH);
  delay(50);
  noTone(buz2);
 }

 if(read1 == HIGH && read2 == HIGH && people == true)
 {
  people = true;
 }

 

 if(read1 == LOW && read2 == HIGH && people == false)
 {
  people = true;
 }
 if(read1 == LOW && read2 == HIGH && people == true)
 {
  people = true;
 }



 if(read1 == LOW && read2 == LOW && people == false)
 {
  people = true;
 }
 if(read1 == LOW && read2 == LOW && people == true)
 {
  people = true;
 }


 if(read1 == HIGH && read2 == LOW && people == true)
 {
  people = false;
 }
 delay(30);
}



