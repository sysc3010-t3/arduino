#include <Arduino.h>
#include "motorTest.h"
#include "motor.h"

MotorTest::MotorTest(Motor& motorLArg, Motor& motorRArg):
motorL(motorLArg), motorR(motorRArg) {
}

int MotorTest::runNextTest() {
  if (currIdx >= NUM_OF_TESTS) {
    Serial.println("All tests completed.");
    return 1;
  }

  Serial.print("Setting up ");
  Serial.println(testNames[currIdx]);
  Serial.flush();

  // Stop motors
  motorL.setDirection(MOTOR_FWD);
  motorR.setDirection(MOTOR_FWD);
  motorL.setMotorSpeed(FWD_STOP);
  motorR.setMotorSpeed(FWD_STOP);
  motorL.setDirection(MOTOR_BWD);
  motorR.setDirection(MOTOR_BWD);
  motorL.setMotorSpeed(BWD_STOP);
  motorR.setMotorSpeed(BWD_STOP);
  
  delay(1000);

  Serial.print("Running ");
  Serial.println(testNames[currIdx]);

  // Run test
  (*this.*(testPtrs[currIdx++]))();

  delay(2000);

  // Stop motors
  motorL.setDirection(MOTOR_FWD);
  motorR.setDirection(MOTOR_FWD);
  delay(500);
  motorL.setMotorSpeed(FWD_STOP);
  motorR.setMotorSpeed(FWD_STOP);

  if (currIdx >= NUM_OF_TESTS) {
    Serial.println("All tests completed.");
    return 1;
  }

  return 0;
}

void MotorTest::reset() {
  currIdx = 0;
}

void MotorTest::test_forward_from_stop() {
  motorL.setDirection(MOTOR_FWD);
  motorR.setDirection(MOTOR_FWD);
  delay(500);
  motorL.setMotorSpeed(FWD_STOP);
  motorR.setMotorSpeed(FWD_STOP);
  delay(500);
  motorL.setMotorSpeed(FWD_MAX);
  motorR.setMotorSpeed(FWD_MAX);
}

void MotorTest::test_backward_from_stop() {
  motorL.setDirection(MOTOR_BWD);
  motorR.setDirection(MOTOR_BWD);
  delay(500);
  motorL.setMotorSpeed(BWD_STOP);
  motorR.setMotorSpeed(BWD_STOP);
  delay(500);
  motorL.setMotorSpeed(BWD_MAX);
  motorR.setMotorSpeed(BWD_MAX);
}

void MotorTest::test_stop_motors() {
  motorL.setDirection(MOTOR_FWD);
  motorR.setDirection(MOTOR_FWD);
  delay(500);
  motorL.setMotorSpeed(FWD_STOP);
  motorR.setMotorSpeed(FWD_STOP);
}

void MotorTest::test_forward_to_backward() {
  // Move forward
  motorL.setDirection(MOTOR_FWD);
  motorR.setDirection(MOTOR_FWD);
  motorL.setMotorSpeed(FWD_STOP);
  motorR.setMotorSpeed(FWD_STOP);
  delay(500);
  motorL.setMotorSpeed(FWD_MAX);
  motorR.setMotorSpeed(FWD_MAX);
  delay(1000);
  // Change to backward direction
  motorL.setDirection(MOTOR_BWD);
  motorR.setDirection(MOTOR_BWD);
  motorL.setMotorSpeed(BWD_MAX);
  motorR.setMotorSpeed(BWD_MAX);
}

void MotorTest::test_backward_to_forward() {
  // Move backward
  motorL.setDirection(MOTOR_BWD);
  motorR.setDirection(MOTOR_BWD);
  delay(500);
  motorL.setMotorSpeed(BWD_MAX);
  motorR.setMotorSpeed(BWD_MAX);
  delay(1000);
  // Change to forward direction
  motorL.setDirection(MOTOR_FWD);
  motorR.setDirection(MOTOR_FWD);
  motorL.setMotorSpeed(FWD_MAX);
  motorR.setMotorSpeed(FWD_MAX);
}

void MotorTest::test_increase_forward_speed() {
  motorL.setDirection(MOTOR_FWD);
  motorR.setDirection(MOTOR_FWD);
  motorL.setMotorSpeed(FWD_STOP);
  motorR.setMotorSpeed(FWD_STOP);
  delay(500);
  motorL.setMotorSpeed(FWD_SLOW);
  motorR.setMotorSpeed(FWD_SLOW);
  delay(1000);
  motorL.setMotorSpeed(FWD_MAX);
  motorR.setMotorSpeed(FWD_MAX);
}

void MotorTest::test_decrease_forward_speed() {
  motorL.setDirection(MOTOR_FWD);
  motorR.setDirection(MOTOR_FWD);
  motorL.setMotorSpeed(FWD_STOP);
  motorR.setMotorSpeed(FWD_STOP);
  delay(500);
  motorL.setMotorSpeed(FWD_MAX);
  motorR.setMotorSpeed(FWD_MAX);
  delay(1000);
  motorL.setMotorSpeed(FWD_SLOW);
  motorR.setMotorSpeed(FWD_SLOW);
}

void MotorTest::test_increase_backward_speed() {
  motorL.setDirection(MOTOR_BWD);
  motorR.setDirection(MOTOR_BWD);
  delay(500);
  motorL.setMotorSpeed(BWD_SLOW);
  motorR.setMotorSpeed(BWD_SLOW);
  delay(1000);
  motorL.setMotorSpeed(BWD_MAX);
  motorR.setMotorSpeed(BWD_MAX);
}

void MotorTest::test_decrease_backward_speed() {
  motorL.setDirection(MOTOR_BWD);
  motorR.setDirection(MOTOR_BWD);
  delay(500);
  motorL.setMotorSpeed(BWD_MAX);
  motorR.setMotorSpeed(BWD_MAX);
  delay(1000);
  motorL.setMotorSpeed(BWD_SLOW);
  motorR.setMotorSpeed(BWD_SLOW);
}

void MotorTest::test_increase_left_forward_speed() {
  motorL.setDirection(MOTOR_FWD);
  motorR.setDirection(MOTOR_FWD);
  motorL.setMotorSpeed(FWD_STOP);
  motorR.setMotorSpeed(FWD_STOP);
  delay(500);
  motorL.setMotorSpeed(FWD_SLOW);
  motorR.setMotorSpeed(FWD_SLOW);
  delay(1000);
  motorL.setMotorSpeed(FWD_MAX);
}

void MotorTest::test_decrease_left_forward_speed() {
  motorL.setDirection(MOTOR_FWD);
  motorR.setDirection(MOTOR_FWD);
  motorL.setMotorSpeed(FWD_STOP);
  motorR.setMotorSpeed(FWD_STOP);
  delay(500);
  motorL.setMotorSpeed(FWD_MAX);
  motorR.setMotorSpeed(FWD_MAX);
  delay(1000);
  motorL.setMotorSpeed(FWD_SLOW);
}

void MotorTest::test_increase_right_forward_speed() {
  motorL.setDirection(MOTOR_FWD);
  motorR.setDirection(MOTOR_FWD);
  motorL.setMotorSpeed(FWD_STOP);
  motorR.setMotorSpeed(FWD_STOP);
  delay(500);
  motorL.setMotorSpeed(FWD_SLOW);
  motorR.setMotorSpeed(FWD_SLOW);
  delay(1000);
  motorR.setMotorSpeed(FWD_MAX);
}

void MotorTest::test_decrease_right_forward_speed() {
  motorL.setDirection(MOTOR_FWD);
  motorR.setDirection(MOTOR_FWD);
  motorL.setMotorSpeed(FWD_STOP);
  motorR.setMotorSpeed(FWD_STOP);
  delay(500);
  motorL.setMotorSpeed(FWD_MAX);
  motorR.setMotorSpeed(FWD_MAX);
  delay(1000);
  motorR.setMotorSpeed(FWD_SLOW);
}

void MotorTest::test_increase_left_backward_speed() {
  motorL.setDirection(MOTOR_BWD);
  motorR.setDirection(MOTOR_BWD);
  delay(500);
  motorL.setMotorSpeed(BWD_SLOW);
  motorR.setMotorSpeed(BWD_SLOW);
  delay(1000);
  motorL.setMotorSpeed(BWD_MAX);
}

void MotorTest::test_decrease_left_backward_speed() {
  motorL.setDirection(MOTOR_BWD);
  motorR.setDirection(MOTOR_BWD);
  delay(500);
  motorL.setMotorSpeed(BWD_MAX);
  motorR.setMotorSpeed(BWD_MAX);
  delay(1000);
  motorL.setMotorSpeed(BWD_SLOW);
}

void MotorTest::test_increase_right_backward_speed() {
  motorL.setDirection(MOTOR_BWD);
  motorR.setDirection(MOTOR_BWD);
  delay(500);
  motorL.setMotorSpeed(BWD_SLOW);
  motorR.setMotorSpeed(BWD_SLOW);
  delay(1000);
  motorR.setMotorSpeed(BWD_MAX);
}

void MotorTest::test_decrease_right_backward_speed() {
  motorL.setDirection(MOTOR_BWD);
  motorR.setDirection(MOTOR_BWD);
  delay(500);
  motorL.setMotorSpeed(BWD_MAX);
  motorR.setMotorSpeed(BWD_MAX);
  delay(1000);
  motorR.setMotorSpeed(BWD_SLOW);
}
