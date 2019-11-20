#ifndef motor_h
#define motor_h

// Define constants
#define MOTOR_L 'L'
#define MOTOR_R 'R'
#define MOTOR_BWD 0
#define MOTOR_FWD 1
#define FWD_MAX 0
#define FWD_STOP 255
#define BWD_MAX 255
#define BWD_STOP 0

// Represents a connected motor.
// Can set the direction and speed of the motor.
class Motor {
  int pwmPin;
  int dirPin;
  int dir; // dir can be 0 (backward) or 1 (forward)
  int motorSpeed;
  char id;

  public:
    Motor(int pwmPinArg, int dirPinArg, char idArg);

    // Sets the speed of the motor based on the given coordinates
    void setSpeedFromCoords(int xAxis, int yAxis);

    // Sets the direction of the motor
    void setDirection(int dirArg);

    // Sets the speed of the motor
    void setMotorSpeed(int motorSpeedArg);

  private:
    // Write the dir attribute to the DIR pin
    void writeDirection();

    // Write the motorSpeed attribute to the PWM pin
    void writeMotorSpeed();
};
#endif
