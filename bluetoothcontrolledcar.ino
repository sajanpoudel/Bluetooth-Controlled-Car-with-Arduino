#include <SoftwareSerial.h>

SoftwareSerial BT(10, 11); // TX, RX respectively
String readdata;

// Motor driver pins: direction pins for each side, then the PWM speed pins.
const int motorLpin1 = 2;
const int motorLpin2 = 3;
const int motorRpin1 = 4;
const int motorRpin2 = 5;
const int motorLpwm = 10;
const int motorRpwm = 11;

const long SERIAL_BAUD = 9600;
const int SERIAL_READ_DELAY_MS = 5;

int motorSpeed = 125; // default speed, can be changed by sending a number
const int turn = 50;  // speed difference between the wheels while turning

void setup() {
  Serial.begin(SERIAL_BAUD);
  Serial.flush();
  pinMode(motorLpin1,OUTPUT);
  pinMode(motorLpin2,OUTPUT);
  pinMode(motorRpin1,OUTPUT);
  pinMode(motorRpin2,OUTPUT);
  pinMode(motorLpwm,OUTPUT);
  pinMode(motorRpwm,OUTPUT);
}

void loop() {
  String input="";
  while(Serial.available()){
    input+=(char)Serial.read();
    delay(SERIAL_READ_DELAY_MS);
  }
  
  if(input=="forward"){
    fwd();
  }
  else if(input=="stop"){
    stp();
  }
  else if(input=="backward"){
    rev();
  }
  else if(input.indexOf("left")>-1){
    lft();
  }
  else if(input.indexOf("right")>-1){
    rght();
  }
  else if(input!=""){
    motorSpeed=input.toInt();
  }
}

void fwd(){
  analogWrite(motorLpwm,motorSpeed);
  analogWrite(motorRpwm,motorSpeed);
  digitalWrite(motorLpin1,1);
  digitalWrite(motorLpin2,0);
  digitalWrite(motorRpin1,1);
  digitalWrite(motorRpin2,0);
}

void rev(){
  analogWrite(motorLpwm,motorSpeed);
  analogWrite(motorRpwm,motorSpeed);
  digitalWrite(motorLpin1,0);
  digitalWrite(motorLpin2,1);
  digitalWrite(motorRpin1,0);
  digitalWrite(motorRpin2,1);
}

void lft(){
  analogWrite(motorLpwm,motorSpeed-turn);
  analogWrite(motorRpwm,motorSpeed+turn);
  digitalWrite(motorLpin1,0);
  digitalWrite(motorLpin2,1);
  digitalWrite(motorRpin1,1);
  digitalWrite(motorRpin2,0);
}

void rght(){
  analogWrite(motorLpwm,motorSpeed+turn);
  analogWrite(motorRpwm,motorSpeed-turn);
  digitalWrite(motorLpin1,1);
  digitalWrite(motorLpin2,0);
  digitalWrite(motorRpin1,0);
  digitalWrite(motorRpin2,1);
}

void stp(){
  analogWrite(motorLpwm,0);
  analogWrite(motorRpwm,0);
  digitalWrite(motorLpin1,1);
  digitalWrite(motorLpin2,1);
  digitalWrite(motorRpin1,1);
  digitalWrite(motorRpin2,1);
}

