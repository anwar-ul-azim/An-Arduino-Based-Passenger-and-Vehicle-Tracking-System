// this program count the current passenger and location of a bus.
#include <LiquidCrystal.h>    //lcd library
#include <SoftwareSerial.h>   //serial library
#include <TinyGPS.h>          // gps library
LiquidCrystal lcd(12, 11, 5, 4, 3, 2 );  //set up lcd pin(Rs, En, D4, D5, D6, D7)
SoftwareSerial gpsSerial(6,13);   //set up gps pin(Rx, Tx)
TinyGPS gps;
const int buzzer = A3;  //analog pin A3 to buzzer
const int led = A4;     //analog pin A4 to led
const int btn = A5;     //analog pin A5 to button
const int trigPin1 = 7;  //digital pin 7 to sonar 1 trig
const int echoPin1 = 8;  //digital pin 8 to sonar 1 echo
const int trigPin2 = 9;  //digital pin 9 to sonar 2 trig
const int echoPin2 = 10; //digital pin 10 to sonar 2 echo
String place="";   //create place variable
double duration1, duration2;  //create distance variable
int distanceCm1, distanceCm2, f=0, t=0, c=0, h=0, s=0;//s-station/b-btnState/c-min/h-hour/t-totalP/val-btnInput/
int P[5]={0,0,0,0,0}, T[5]={0,0,0,0,0}, S[5]={0,0,0,0,0};//create 3 array
float lat = 23.8121543,lon = 90.4210534; // create lat,lon variable
void setup() {
Serial.begin(9600); 
gpsSerial.begin(9600);
lcd.begin(16,2);           //lcd set up
pinMode(btn, INPUT);       //set up button
digitalWrite(btn, HIGH);   //default set up button
pinMode(led, OUTPUT);     //set up led
pinMode(buzzer, OUTPUT);  //set up buzzer 
//set up sonar 
pinMode(trigPin1, OUTPUT);
pinMode(echoPin1, INPUT);
pinMode(trigPin2, OUTPUT);
pinMode(echoPin2, INPUT);
}
void loop() {
//set time
c=millis()/60000;
if(c>60){
  c=0;
  h=h+1;
}
//sonars start to work 
digitalWrite(trigPin1, LOW);
digitalWrite(trigPin2, LOW);
delayMicroseconds(2);
digitalWrite(trigPin1, HIGH);
digitalWrite(trigPin2, HIGH);
delayMicroseconds(10);
digitalWrite(trigPin1, LOW);
duration1 = pulseIn(echoPin1, HIGH);
distanceCm1= duration1*0.034/2;           //get sonar 1 data
digitalWrite(trigPin2, LOW);
duration2 = pulseIn(echoPin2, HIGH);
distanceCm2= duration2*0.034/2;           // get sonar 2 data
if(distanceCm1-distanceCm2 < -2 || distanceCm1-distanceCm2 > 2){   // device working indicator
  digitalWrite(led, LOW);
}else{
  digitalWrite(led, HIGH);  
}
if(distanceCm1<100 || distanceCm2 < 100){    //set working threshold
  if(distanceCm1-distanceCm2 > 10){
    t=t+1;              //increase passenger
    f=f+1;  
    tone(buzzer, 650);  //tone the buzzer single tone for increase
    delay(70);
    noTone(buzzer);
    delay(850);        //change the delay to calibrate the sensors 
  }if(distanceCm1-distanceCm2 < -10){
    if(t>0){
      t=t-1;            //decrease  passenger
    }
    tone(buzzer, 850); //tone the buzzer 
    delay(60);
    noTone(buzzer);
    delay(50);
    tone(buzzer, 850); //double tone for decrease 
    delay(60);
    noTone(buzzer);
    delay(750);          //change the delay to calibrate the sensors 
}}
//lcd print current data
lcd.setCursor(0,0);
lcd.print("P=");
lcd.print(t);
lcd.print("T:");
lcd.print(h);
lcd.print(":");
lcd.print(c);
lcd.print("S:");
lcd.print(s);
delay(10);   
while(gpsSerial.available()){  
  if(gps.encode(gpsSerial.read())){  
    gps.f_get_position(&lat,&lon); 
  }}
//process gps data
//get station from gps data
if((lat - 23.8200)<=0.00009 && (lon - 90.420)<=0.0009 && (lat - 23.8200)>=0.0000009 && (lon - 90.420)>=0.000009){
  s=101;
  place="Kuril Bishwa Road";
  P[0]=t;
  T[0]=h*60+c;
  S[0]=s;
}if((lat - 23.8161)<=0.00009 && (lon - 90.421)<=0.0009 && (lat - 23.8161)>=0.0000009 && (lon - 90.421)>=0.000009){
  s=110;
  place="Kuril Chowrasta";
  P[1]=t;
  T[1]=h*60+c;
  S[1]=s;
}if((lat - 23.8121)<=0.00009 && (lon - 90.421)<=0.0009 && (lat - 23.8121)>=0.0000009 && (lon - 90.421)>=0.000009){
  s=210;
  place="Bashundhara";
  P[2]=t;
  T[2]=h*60+c;
  S[2]=s;
}if((lat - 23.8094)<=0.00009 && (lon - 90.421)<=0.0009 && (lat - 23.8094)>=0.0000009 && (lon - 90.421)>=0.000009){
  s=201;
  place="Nadda";
  P[3]=t;
  T[3]=h*60+c;
  S[3]=s;
}if((lat - 23.7979)<=0.00009 && (lon - 90.423)<=0.0009 && (lat - 23.7979)>=0.0000009 && (lon - 90.423)>=0.000009){
  s=310;
  place="Natun Bazaar";
  P[4]=t;
  T[4]=h*60+c;
  S[4]=s;
}
//check if the button is pressed
if(digitalRead(btn)==LOW){
    //show previous station data
    for(int i = 0; i < 5; i = i + 1){
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print(i);
      lcd.print(": P=");
      lcd.print(P[i]);
      lcd.print(";(");
      lcd.print(f);
      lcd.print(")");
      lcd.setCursor(0,1);
      lcd.print("T: ");
      lcd.print(T[i]);
      lcd.print(", S:");
      lcd.print(S[i]);
      delay(1500);
    }
    lcd.clear();
}else{
  lcd.setCursor(0,1);
  lcd.print(place);        //print place name in lcd
}
delay(100); 
}
