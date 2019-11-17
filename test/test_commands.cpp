#include "minitest.h"
#include "../commands.h"

static void test_exact_commands() {
  CHECK(parseCommand("forward") == CMD_FORWARD);
  CHECK(parseCommand("backward") == CMD_BACKWARD);
  CHECK(parseCommand("stop") == CMD_STOP);
}


static void test_left_and_right_only_need_to_be_contained() {
  CHECK(parseCommand("left") == CMD_LEFT);
  CHECK(parseCommand("turn left now") == CMD_LEFT);
  CHECK(parseCommand("right") == CMD_RIGHT);
  CHECK(parseCommand("go right") == CMD_RIGHT);
}

int main() {
  test_exact_commands();
  test_left_and_right_only_need_to_be_contained();
  return 0;
}
