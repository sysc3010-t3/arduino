#include "motor.h"

#define NUM_OF_TESTS 17
#define NAME_LENGTH 256

class MotorTest {
  Motor motorL;
  Motor motorR;
  int currIdx;
  String testNames[NUM_OF_TESTS] = {
    "test_forward_from_stop",
    "test_backward_from_stop",
    "test_stop_motors",
    "test_forward_to_backward",
    "test_backward_to_forward",
    "test_increase_forward_speed",
    "test_decrease_forward_speed",
    "test_increase_backward_speed",
    "test_decrease_backward_speed",
    "test_increase_left_forward_speed",
    "test_decrease_left_forward_speed",
    "test_increase_right_forward_speed",
    "test_decrease_right_forward_speed",
    "test_increase_left_backward_speed",
    "test_decrease_left_backward_speed",
    "test_increase_right_backward_speed",
    "test_decrease_right_backward_speed"
  };
  
  void (MotorTest::*testPtrs[NUM_OF_TESTS])() = {
    &MotorTest::test_forward_from_stop,
    &MotorTest::test_backward_from_stop,
    &MotorTest::test_stop_motors,
    &MotorTest::test_forward_to_backward,
    &MotorTest::test_backward_to_forward,
    &MotorTest::test_increase_forward_speed,
    &MotorTest::test_decrease_forward_speed,
    &MotorTest::test_increase_backward_speed,
    &MotorTest::test_decrease_backward_speed,
    &MotorTest::test_increase_left_forward_speed,
    &MotorTest::test_decrease_left_forward_speed,
    &MotorTest::test_increase_right_forward_speed,
    &MotorTest::test_decrease_right_forward_speed,
    &MotorTest::test_increase_left_backward_speed,
    &MotorTest::test_decrease_left_backward_speed,
    &MotorTest::test_increase_right_backward_speed,
    &MotorTest::test_decrease_right_backward_speed
  };

  public:
    MotorTest(Motor& motorLArg, Motor& motorRArg);
    void runNextTest();
    void reset();

  private:
    void test_forward_from_stop();
    void test_backward_from_stop();
    void test_stop_motors();
    void test_forward_to_backward();
    void test_backward_to_forward();
    void test_increase_forward_speed();
    void test_decrease_forward_speed();
    void test_increase_backward_speed();
    void test_decrease_backward_speed();
    void test_increase_left_forward_speed();
    void test_decrease_left_forward_speed();
    void test_increase_right_forward_speed();
    void test_decrease_right_forward_speed();
    void test_increase_left_backward_speed();
    void test_decrease_left_backward_speed();
    void test_increase_right_backward_speed();
    void test_decrease_right_backward_speed();
};
