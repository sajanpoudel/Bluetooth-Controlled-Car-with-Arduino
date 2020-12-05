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

const unsigned long COMMAND_TIMEOUT_MS = 2000; // the car brakes when the phone goes silent for this long
unsigned long lastCommandAt = 0;
bool moving = false;

// Speeds change in small steps so the car does not jerk or lose the wheels' grip.
const int RAMP_STEP = 15;
const unsigned long RAMP_INTERVAL_MS = 20;
int targetLeft = 0;
int targetRight = 0;
int currentLeft = 0;
int currentRight = 0;
unsigned long lastRampAt = 0;

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
  updateRamp();
  String input = readCommand();
  Command command = parseCommand(input.c_str());

  if (command != CMD_NONE) {
    Serial.print("ok ");
    Serial.println(commandName(command));
    lastCommandAt = millis();
    moving = (command != CMD_STOP && command != CMD_SPEED && command != CMD_FASTER &&
              command != CMD_SLOWER && command != CMD_STATUS);
  } else if (moving && shouldFailsafeStop(millis(), lastCommandAt, COMMAND_TIMEOUT_MS)) {
    stp();
    moving = false;
  }

  switch (command) {
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
    case CMD_FASTER:
    case CMD_SLOWER:
      motorSpeed = adjustSpeed(motorSpeed, command);
      break;
    case CMD_STATUS:
      Serial.print("speed ");
      Serial.print(motorSpeed);
      Serial.println(moving ? " moving" : " stopped");
      break;
    case CMD_NONE:
      break;
  }
}

// Applies a set of drive signals to the motor driver.
void driveSignals(DriveSignals s){
  drive(s.leftSpeed, s.rightSpeed, s.l1, s.l2, s.r1, s.r2);
}

// Sets the direction pins of both motors at once and the speed the wheels should ramp up or down to.
// Braking is immediate, everything else goes through updateRamp().
void drive(int leftSpeed, int rightSpeed, int l1, int l2, int r1, int r2){
  targetLeft = constrain(leftSpeed, 0, 255);
  targetRight = constrain(rightSpeed, 0, 255);
  if (targetLeft == 0 && targetRight == 0) {
    currentLeft = 0;
    currentRight = 0;
    analogWrite(motorLpwm, 0);
    analogWrite(motorRpwm, 0);
  }
  digitalWrite(motorLpin1, l1);
  digitalWrite(motorLpin2, l2);
  digitalWrite(motorRpin1, r1);
  digitalWrite(motorRpin2, r2);
}

// Moves the PWM outputs a step closer to the target speeds every RAMP_INTERVAL_MS.
void updateRamp(){
  if (millis() - lastRampAt < RAMP_INTERVAL_MS) return;
  lastRampAt = millis();
  currentLeft = rampTowards(currentLeft, targetLeft, RAMP_STEP);
  currentRight = rampTowards(currentRight, targetRight, RAMP_STEP);
  analogWrite(motorLpwm, currentLeft);
  analogWrite(motorRpwm, currentRight);
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
