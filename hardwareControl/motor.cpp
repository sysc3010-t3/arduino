#include <Arduino.h>
#include "motor.h"

// Represents a connected motor.
// Can set the direction and speed of the motor.
Motor::Motor(int pwmPinArg, int dirPinArg, char idArg) {
  pwmPin = pwmPinArg;
  dirPin = dirPinArg;
  id = idArg;
  dir = MOTOR_FWD; // direction is set to forward by default
  motorSpeed = FWD_STOP;
}

// Calculates the speed and direction from the given joystick coordinates
// and outputs them to the PWM and DIR pins.
void Motor::setSpeedFromCoords(int xAxis, int yAxis) {
  dir = MOTOR_FWD;
  // Y-axis used for forward and backward control
  if (yAxis < COORD_LOW) {
    dir = MOTOR_BWD;
    // Convert the declining Y-axis readings for going backward from 470 to 0 into 0 to 255 value for the PWM signal for increasing the motor speed
    motorSpeed = map(yAxis, COORD_LOW, COORD_MIN, BWD_SLOW, BWD_MAX);
  } else if (yAxis > COORD_HIGH) {
    // Convert the increasing Y-axis readings for going forward from 550 to 1023 into 0 to 255 value for the PWM signal for increasing the motor speed
    motorSpeed = map(yAxis, COORD_HIGH, COORD_MAX, BWD_SLOW, BWD_MAX);
  } else {
    // If joystick stays in middle the motors are not moving
    motorSpeed = BWD_STOP;
  }
  
  // X-axis used for left and right control
  if (xAxis < COORD_LOW) {
    // Convert the declining X-axis readings from 470 to 0 into increasing 0 to 255 value
    int xMapped = map(xAxis, COORD_LOW, COORD_MIN, 0, X_OFFSET_MAX);
    // Move to left - decrease left motor speed, increase right motor speed
    if (id == MOTOR_L) {
      motorSpeed = motorSpeed - xMapped;
      if (motorSpeed < BWD_STOP) {
        motorSpeed = BWD_STOP;
      }
    } else if (id == MOTOR_R) {
      motorSpeed = motorSpeed + xMapped;
      if (motorSpeed > BWD_MAX) {
        motorSpeed = BWD_MAX;
      }
    }
  } else if (xAxis > COORD_HIGH) {
    // Convert the increasing X-axis readings from 550 to 1023 into 0 to 255 value
    int xMapped = map(xAxis, COORD_HIGH, COORD_MAX, 0, X_OFFSET_MAX);
    // Move right - decrease right motor speed, increase left motor speed
    if (id == MOTOR_R) {
      motorSpeed = motorSpeed - xMapped;
      if (motorSpeed < BWD_STOP) {
        motorSpeed = BWD_STOP;
      }
    } else if (id == MOTOR_L) {
      motorSpeed = motorSpeed + xMapped;
      if (motorSpeed > BWD_MAX) {
        motorSpeed = BWD_MAX;
      }
    }
  }

  if (dir) {
    // Forward speed is inverted so speed increases from 255 to 0
    motorSpeed = BWD_MAX - motorSpeed;
  }
}

// Sets the direction of the motor
void Motor::setDirection(int dirArg) {
  if (dirArg != dir) {
    dir = dirArg;
    writeDirection();
  }
}

// Sets the motor speed
void Motor::setMotorSpeed(int motorSpeedArg) {
  if (motorSpeedArg != motorSpeed) {
    motorSpeed = motorSpeedArg;
    writeMotorSpeed();
  }
}

// Sets the direction of the motor
void Motor::writeDirection() {
  digitalWrite(pwmPin, dir);
  digitalWrite(dirPin, dir);
}

// Sets the motor speed
void Motor::writeMotorSpeed() {
  analogWrite(pwmPin, motorSpeed);
}

StubMotor::StubMotor(int pwmPinArg, int dirPinArg, char idArg)
  :Motor(pwmPinArg, dirPinArg, idArg) {
}

// Sets the direction of the motor
void StubMotor::writeDirection() {
  Serial.print("dir_");
  Serial.print(id);
  Serial.print("=");
  Serial.println(dir);
}

// Sets the motor speed
void StubMotor::writeMotorSpeed() {
  Serial.print("speed_");
  Serial.print(id);
  Serial.print("=");
  Serial.println(motorSpeed);
}
