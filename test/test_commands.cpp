#include "minitest.h"
#include "../commands.h"

static bool same(DriveSignals a, int ls, int rs, int l1, int l2, int r1, int r2) {
  return a.leftSpeed == ls && a.rightSpeed == rs && a.l1 == l1 && a.l2 == l2 && a.r1 == r1 && a.r2 == r2;
}

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


static void test_forward_inside_text_is_not_forward() {
  CHECK(parseCommand("forward!") == CMD_SPEED);
  CHECK(parseCommand("stop now") == CMD_SPEED);
}


static void test_left_wins_over_right() {
  CHECK(parseCommand("left right") == CMD_LEFT);
}


static void test_clamp_keeps_values_in_range() {
  CHECK(clampPwm(0) == 0);
  CHECK(clampPwm(125) == 125);
  CHECK(clampPwm(255) == 255);
}


static void test_clamp_limits_extremes() {
  CHECK(clampPwm(-1) == 0);
  CHECK(clampPwm(-500) == 0);
  CHECK(clampPwm(256) == 255);
  CHECK(clampPwm(9999) == 255);
}


static void test_parse_speed_reads_numbers() {
  CHECK(parseSpeed("180") == 180);
  CHECK(parseSpeed("0") == 0);
}


static void test_parse_speed_clamps_big_values() {
  CHECK(parseSpeed("999") == 255);
  CHECK(parseSpeed("-20") == 0);
}


static void test_parse_speed_of_text_is_zero() {
  CHECK(parseSpeed("fast") == 0);
}


static void test_forward_drives_both_sides_forward() {
  CHECK(same(signalsFor(CMD_FORWARD, 125, 50), 125, 125, 1, 0, 1, 0));
}


static void test_backward_reverses_both_sides() {
  CHECK(same(signalsFor(CMD_BACKWARD, 125, 50), 125, 125, 0, 1, 0, 1));
}


static void test_left_slows_the_left_wheel() {
  CHECK(same(signalsFor(CMD_LEFT, 125, 50), 75, 175, 0, 1, 1, 0));
}


static void test_right_slows_the_right_wheel() {
  CHECK(same(signalsFor(CMD_RIGHT, 125, 50), 175, 75, 1, 0, 0, 1));
}


static void test_stop_brakes_both_motors() {
  CHECK(same(signalsFor(CMD_STOP, 125, 50), 0, 0, 1, 1, 1, 1));
  CHECK(same(signalsFor(CMD_NONE, 125, 50), 0, 0, 1, 1, 1, 1));
}


static void test_turns_never_leave_the_pwm_range() {
  CHECK(same(signalsFor(CMD_LEFT, 240, 50), 190, 255, 0, 1, 1, 0));
  CHECK(same(signalsFor(CMD_RIGHT, 20, 50), 70, 0, 1, 0, 0, 1));
}

static void test_failsafe_waits_for_the_timeout() {
  CHECK(!shouldFailsafeStop(1000, 500, 1000));
  CHECK(!shouldFailsafeStop(1500, 500, 1000));
  CHECK(shouldFailsafeStop(1501, 500, 1000));
}


static void test_failsafe_survives_the_millis_rollover() {
  uint32_t last = 0xFFFFFF00UL;
  CHECK(!shouldFailsafeStop(0x00000100UL, last, 1000));
  CHECK(shouldFailsafeStop(0x00000800UL, last, 1000));
}


static void test_every_command_has_a_name() {
  CHECK(strcmp(commandName(CMD_FORWARD), "forward") == 0);
  CHECK(strcmp(commandName(CMD_SPEED), "speed") == 0);
  CHECK(strcmp(commandName(CMD_NONE), "none") == 0);
}


int main() {
  test_exact_commands();
  test_left_and_right_only_need_to_be_contained();
  test_empty_text_is_no_command();
  test_numbers_are_speed_values();
  test_exact_words_are_case_sensitive();
  test_forward_inside_text_is_not_forward();
  test_left_wins_over_right();
  test_clamp_keeps_values_in_range();
  test_clamp_limits_extremes();
  test_parse_speed_reads_numbers();
  test_parse_speed_clamps_big_values();
  test_parse_speed_of_text_is_zero();
  test_forward_drives_both_sides_forward();
  test_backward_reverses_both_sides();
  test_left_slows_the_left_wheel();
  test_right_slows_the_right_wheel();
  test_stop_brakes_both_motors();
  test_turns_never_leave_the_pwm_range();
  test_every_command_has_a_name();
  test_failsafe_waits_for_the_timeout();
  test_failsafe_survives_the_millis_rollover();
  return 0;
}
