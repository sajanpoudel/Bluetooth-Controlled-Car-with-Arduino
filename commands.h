#ifndef COMMANDS_H
#define COMMANDS_H

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

// Keeps a PWM value inside the 0 to 255 range that analogWrite accepts.
inline int clampPwm(int value) {
  if (value < 0) return 0;
  if (value > 255) return 255;
  return value;
}

#endif
