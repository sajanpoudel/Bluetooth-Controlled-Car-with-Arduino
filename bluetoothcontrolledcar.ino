#include <SoftwareSerial.h>

#include "commands.h"

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

// Collects the characters that are waiting on the serial port into one command without
// surrounding whitespace.
String readCommand(){
  String input = "";
  while(Serial.available()){
    input += (char)Serial.read();
    delay(SERIAL_READ_DELAY_MS);
  }
  input.trim();  // phone apps and the Serial Monitor append a newline
  return input;
}

// Reads a command and runs the matching movement.
void loop() {
  String input = readCommand();

  switch (parseCommand(input.c_str())) {
    case CMD_FORWARD:
      fwd();
      break;
    case CMD_STOP:
      stp();
      break;
    case CMD_BACKWARD:
      rev();
      break;
    case CMD_LEFT:
      lft();
      break;
    case CMD_RIGHT:
      rght();
      break;
    case CMD_SPEED:
      motorSpeed = parseSpeed(input.c_str());
      break;
    case CMD_NONE:
      break;
  }
}

// Applies a set of drive signals to the motor driver.
void driveSignals(DriveSignals s){
  drive(s.leftSpeed, s.rightSpeed, s.l1, s.l2, s.r1, s.r2);
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
  driveSignals(signalsFor(CMD_FORWARD, motorSpeed, turn));
}

// Both sides backward.
void rev(){
  driveSignals(signalsFor(CMD_BACKWARD, motorSpeed, turn));
}

// Left side slower and reversed, so the car turns left.
void lft(){
  driveSignals(signalsFor(CMD_LEFT, motorSpeed, turn));
}

// Right side slower and reversed, so the car turns right.
void rght(){
  driveSignals(signalsFor(CMD_RIGHT, motorSpeed, turn));
}

// Brakes both motors.
void stp(){
  driveSignals(signalsFor(CMD_STOP, motorSpeed, turn));
}
