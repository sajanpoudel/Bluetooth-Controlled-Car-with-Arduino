#ifndef COMMANDS_H
#define COMMANDS_H

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

#endif
