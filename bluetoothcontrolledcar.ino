#include <SoftwareSerial.h>

// Bluetooth module link. NOTE: pins 10 and 11 are also used for motor PWM below.
SoftwareSerial BT(10, 11); // TX, RX respectively
String readdata; // reserved for data read from the Bluetooth module

// Motor driver pins: direction pins for each side, then the PWM speed pins.
const int motorLpin1 = 2;
const int motorLpin2 = 3;
const int motorRpin1 = 4;
const int motorRpin2 = 5;
const int motorLpwm = 10;
const int motorRpwm = 11;

const long SERIAL_BAUD = 9600; // matches the default HC-05 baud rate
const int SERIAL_READ_DELAY_MS = 5; // lets the next byte of a command arrive

int motorSpeed = 125; // default speed, can be changed by sending a number
const int turn = 50;  // speed difference between the wheels while turning

// Opens the serial port and prepares every motor pin as an output.
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

// Collects the characters that are waiting on the serial port into one command.
String readCommand(){
  String input = "";
  while(Serial.available()){
    input += (char)Serial.read();
    delay(SERIAL_READ_DELAY_MS);
  }
  return input;
}

// Reads a command and runs the matching movement.
void loop() {
  String input = readCommand();

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

// Sets the PWM speed of each side and the direction pins of both motors.
void drive(int leftSpeed, int rightSpeed, int l1, int l2, int r1, int r2){
  analogWrite(motorLpwm, constrain(leftSpeed, 0, 255));
  analogWrite(motorRpwm, constrain(rightSpeed, 0, 255));
  digitalWrite(motorLpin1, l1);
  digitalWrite(motorLpin2, l2);
  digitalWrite(motorRpin1, r1);
  digitalWrite(motorRpin2, r2);
}

// Both sides forward.
void fwd(){
  drive(motorSpeed, motorSpeed, 1, 0, 1, 0);
}

// Both sides backward.
void rev(){
  drive(motorSpeed, motorSpeed, 0, 1, 0, 1);
}

void lft(){
  drive(motorSpeed - turn, motorSpeed + turn, 0, 1, 1, 0);
}

void rght(){
  drive(motorSpeed + turn, motorSpeed - turn, 1, 0, 0, 1);
}

void stp(){
  drive(0, 0, 1, 1, 1, 1);
}
