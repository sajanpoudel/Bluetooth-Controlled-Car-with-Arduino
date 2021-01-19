#ifndef COMMANDS_H
#define COMMANDS_H

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

// Movement commands that can arrive over the serial port.
enum Command {
  CMD_NONE,
  CMD_FORWARD,
  CMD_BACKWARD,
  CMD_LEFT,
  CMD_RIGHT,
  CMD_STOP,
  CMD_SPEED
};


// Turns the text received over serial into a command. Matching is the same as in the sketch:
// forward, backward and stop must match exactly, left and right only have to be contained,
// and any other non empty text is a speed value.
inline Command parseCommand(const char* text) {
  if (text == 0 || text[0] == '\0') return CMD_NONE;
  if (strcmp(text, "forward") == 0) return CMD_FORWARD;
  if (strcmp(text, "stop") == 0) return CMD_STOP;
  if (strcmp(text, "backward") == 0) return CMD_BACKWARD;
  if (strstr(text, "left") != 0) return CMD_LEFT;
  if (strstr(text, "right") != 0) return CMD_RIGHT;
  return CMD_SPEED;
}

// The short name of a command, sent back to the phone as an acknowledgement.
inline const char* commandName(Command command) {
  switch (command) {
    case CMD_FORWARD: return "forward";
    case CMD_BACKWARD: return "backward";
    case CMD_LEFT: return "left";
    case CMD_RIGHT: return "right";
    case CMD_STOP: return "stop";
    case CMD_SPEED: return "speed";
    default: return "none";
  }
}

// Keeps a PWM value inside the 0 to 255 range that analogWrite accepts.
inline int clampPwm(int value) {
  if (value < 0) return 0;
  if (value > 255) return 255;
  return value;
}

// Reads a speed from text such as "180". Text that is not a number gives 0.
inline int parseSpeed(const char* text) {
  return clampPwm(atoi(text));
}

// What the motor driver pins and PWM outputs should be for one command.
struct DriveSignals {
  int leftSpeed;
  int rightSpeed;
  int l1;
  int l2;
  int r1;
  int r2;
};

// Outputs for a movement command at the given base speed. turn is the speed difference
// between the wheels while turning. Speeds are already clamped to 0 to 255.
inline DriveSignals signalsFor(Command command, int speed, int turn) {
  DriveSignals s = {0, 0, 1, 1, 1, 1};  // brake
  switch (command) {
    case CMD_FORWARD:
      s = {clampPwm(speed), clampPwm(speed), 1, 0, 1, 0};
      break;
    case CMD_BACKWARD:
      s = {clampPwm(speed), clampPwm(speed), 0, 1, 0, 1};
      break;
    case CMD_LEFT:
      s = {clampPwm(speed - turn), clampPwm(speed + turn), 0, 1, 1, 0};
      break;
    case CMD_RIGHT:
      s = {clampPwm(speed + turn), clampPwm(speed - turn), 1, 0, 0, 1};
      break;
    default:
      break;
  }
  return s;
}

// True when the car has gone longer than timeoutMs without a command and should brake.
// The counters are 32 bit like millis() on an Uno and may wrap around.
inline bool shouldFailsafeStop(uint32_t now, uint32_t lastCommandAt, uint32_t timeoutMs) {
  return (uint32_t)(now - lastCommandAt) > timeoutMs;
}

#endif
