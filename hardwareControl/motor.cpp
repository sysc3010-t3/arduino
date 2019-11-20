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
  // Y-axis used for forward and backward control
  if (yAxis < 470) {
    dir = MOTOR_BWD;
    // Convert the declining Y-axis readings for going backward from 470 to 0 into 0 to 255 value for the PWM signal for increasing the motor speed
    motorSpeed = map(yAxis, 470, 0, 0, 255);
  } else if (yAxis > 550) {
    dir = MOTOR_FWD;
    // Convert the increasing Y-axis readings for going forward from 550 to 1023 into 0 to 255 value for the PWM signal for increasing the motor speed
    motorSpeed = map(yAxis, 550, 1023, 0, 255);
  } else {
    // If joystick stays in middle the motors are not moving
    motorSpeed = 0;
  }
  
  // X-axis used for left and right control
  if (xAxis < 470) {
    // Convert the declining X-axis readings from 470 to 0 into increasing 0 to 255 value
    int xMapped = map(xAxis, 470, 0, 0, 255);
    // Move to left - decrease left motor speed, increase right motor speed
    if (id == MOTOR_L) {
      motorSpeed = motorSpeed - xMapped;
      if (motorSpeed < 0) {
        motorSpeed = 0;
      }
    } else if (id == MOTOR_R) {
      motorSpeed = motorSpeed + xMapped;
      if (motorSpeed > 255) {
        motorSpeed = 255;
      }
    }
  } else if (xAxis > 550) {
    // Convert the increasing X-axis readings from 550 to 1023 into 0 to 255 value
    int xMapped = map(xAxis, 550, 1023, 0, 255);
    // Move right - decrease right motor speed, increase left motor speed
    if (id == MOTOR_R) {
      motorSpeed = motorSpeed - xMapped;
      if (motorSpeed < 0) {
        motorSpeed = 0;
      }
    } else if (id == MOTOR_L) {
      motorSpeed = motorSpeed + xMapped;
      if (motorSpeed > 255) {
        motorSpeed = 255;
      }
    }
  }
  
  // Prevent buzzing at low speeds
  if (motorSpeed < 120) {
    motorSpeed = 0;
  }

  if (dir) {
    // Forward speed is inverted so speed increases from 255 to 0
    motorSpeed = 255 - motorSpeed;
  }

  writeDirection();
  writeMotorSpeed();
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
