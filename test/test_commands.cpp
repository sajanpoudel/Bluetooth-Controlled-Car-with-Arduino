#include "minitest.h"
#include "../commands.h"

static void test_exact_commands() {
  CHECK(parseCommand("forward") == CMD_FORWARD);
  CHECK(parseCommand("backward") == CMD_BACKWARD);
  CHECK(parseCommand("stop") == CMD_STOP);
}

int main() {
  test_exact_commands();
  return 0;
}
