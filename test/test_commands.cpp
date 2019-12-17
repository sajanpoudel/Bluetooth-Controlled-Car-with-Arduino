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


static void test_empty_text_is_no_command() {
  CHECK(parseCommand("") == CMD_NONE);
  CHECK(parseCommand(0) == CMD_NONE);
}


static void test_numbers_are_speed_values() {
  CHECK(parseCommand("200") == CMD_SPEED);
  CHECK(parseCommand("0") == CMD_SPEED);
}


static void test_exact_words_are_case_sensitive() {
  CHECK(parseCommand("Forward") == CMD_SPEED);
  CHECK(parseCommand("STOP") == CMD_SPEED);
}

int main() {
  test_exact_commands();
  test_left_and_right_only_need_to_be_contained();
  test_empty_text_is_no_command();
  test_numbers_are_speed_values();
  test_exact_words_are_case_sensitive();
  return 0;
}
