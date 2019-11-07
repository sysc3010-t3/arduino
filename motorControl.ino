// wired connections
#define HG7881_A_IA 9 // D6 --> Motor A Input A --> MOTOR B +
#define HG7881_A_IB 8 // D7 --> Motor A Input B --> MOTOR B -
#define HG7881_B_IA 7 // D6 --> Motor B Input A --> MOTOR B +
#define HG7881_B_IB 6 // D7 --> Motor B Input B --> MOTOR B -
 
// functional connections
#define MOTOR_A_PWM HG7881_A_IA // Motor A PWM Speed
#define MOTOR_A_DIR HG7881_A_IB // Motor A Direction
#define MOTOR_B_PWM HG7881_B_IA // Motor B PWM Speed
#define MOTOR_B_DIR HG7881_B_IB // Motor B Direction

int motorSpeedA = 0;
int motorSpeedB = 0;

void setup() {
  pinMode(MOTOR_A_PWM, OUTPUT);
  pinMode(MOTOR_A_DIR, OUTPUT);
  pinMode(MOTOR_B_PWM, OUTPUT);
  pinMode(MOTOR_B_DIR, OUTPUT);
  
  Serial.begin(9600); // opens serial port, sets data rate to 9600 bps
}

void loop() {
  while (Serial.available() <= 0) {}
  // read the incoming byte:
  int xAxis = Serial.readStringUntil('\n').toInt();
  Serial.print("x-axis: ");
  Serial.println(xAxis);
  Serial.flush();
  while (Serial.available() <= 0) {}
  int yAxis = Serial.readStringUntil('\n').toInt();
  Serial.print("y-axis: ");
  Serial.println(yAxis);
  
  // Y-axis used for forward and backward control
  if (yAxis < 470) {
    // Set Motor A backward
    digitalWrite(MOTOR_A_PWM, HIGH);
    digitalWrite(MOTOR_A_DIR, LOW);
    // Set Motor B backward
    digitalWrite(MOTOR_B_PWM, HIGH);
    digitalWrite(MOTOR_B_DIR, LOW);
    // Convert the declining Y-axis readings for going backward from 470 to 0 into 0 to 255 value for the PWM signal for increasing the motor speed
    motorSpeedA = map(yAxis, 470, 0, 0, 255);
    motorSpeedB = map(yAxis, 470, 0, 0, 255);
  } else if (yAxis > 550) {
    // Set Motor A backward
    digitalWrite(MOTOR_A_PWM, LOW);
    digitalWrite(MOTOR_A_DIR, HIGH);
    // Set Motor B forward
    digitalWrite(MOTOR_B_PWM, LOW);
    digitalWrite(MOTOR_B_DIR, HIGH);
    // Convert the increasing Y-axis readings for going forward from 550 to 1023 into 0 to 255 value for the PWM signal for increasing the motor speed
    motorSpeedA = map(yAxis, 550, 1023, 0, 255);
    motorSpeedB = map(yAxis, 550, 1023, 0, 255);
  }
  // If joystick stays in middle the motors are not moving
  else {
    motorSpeedA = 0;
    motorSpeedB = 0;
  }
  // X-axis used for left and right control
  if (xAxis < 470) {
    // Convert the declining X-axis readings from 470 to 0 into increasing 0 to 255 value
    int xMapped = map(xAxis, 470, 0, 0, 255);
    // Move to left - decrease left motor speed, increase right motor speed
    motorSpeedA = motorSpeedA - xMapped;
    motorSpeedB = motorSpeedB + xMapped;
    // Confine the range from 0 to 255
    if (motorSpeedA < 0) {
      motorSpeedA = 0;
    }
    if (motorSpeedB > 255) {
      motorSpeedB = 255;
    }
  }
  if (xAxis > 550) {
    // Convert the increasing X-axis readings from 550 to 1023 into 0 to 255 value
    int xMapped = map(xAxis, 550, 1023, 0, 255);
    // Move right - decrease right motor speed, increase left motor speed
    motorSpeedA = motorSpeedA + xMapped;
    motorSpeedB = motorSpeedB - xMapped;
    // Confine the range from 0 to 255
    if (motorSpeedA > 255) {
      motorSpeedA = 255;
    }
    if (motorSpeedB < 0) {
      motorSpeedB = 0;
    }
  }
  // Prevent buzzing at low speeds (Adjust according to your motors. My motors couldn't start moving if PWM value was below value of 70)
  if (motorSpeedA < 70) {
    motorSpeedA = 0;
  }
  if (motorSpeedB < 70) {
    motorSpeedB = 0;
  }

  analogWrite(MOTOR_A_PWM, motorSpeedA); // Send PWM signal to motor A
  analogWrite(MOTOR_B_PWM, motorSpeedB); // Send PWM signal to motor B
}
