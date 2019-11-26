#include "motor.h"
#include "motorTest.h"
#include "headlights.h"
#include "ldr.h"

// wired connections
#define HG7881_A_IA 11 // D11 --> Motor A Input A --> MOTOR B +
#define HG7881_A_IB 12 // D12 --> Motor A Input B --> MOTOR B -
#define HG7881_B_IA 6 // D6 --> Motor B Input A --> MOTOR B +
#define HG7881_B_IB 7 // D7 --> Motor B Input B --> MOTOR B -
#define LEFT_LED 2
#define RIGHT_LED 4
#define LDR_0 0
 
// functional connections
#define MOTOR_R_PWM HG7881_A_IA // Right Motor PWM Speed
#define MOTOR_R_DIR HG7881_A_IB // Right Motor A Direction
#define MOTOR_L_PWM HG7881_B_IA // Left Motor B PWM Speed
#define MOTOR_L_DIR HG7881_B_IB // Left Motor B Direction

// Serial input constants
#define INPUT_MIN 2
#define INPUT_MAX 5
#define VALUE_MAX 2

// Message types
#define MOVE 0
#define LED 1
#define MODE 2

// Mode types
#define STD_MODE 0
#define TEST_MODE 1
#define STUB_MODE 2

// Return code types
#define RC_NULL -1
#define RC_OK 0
#define RC_FAIL 1

// Define global variables
StubMotor stubMotorL = StubMotor(MOTOR_L_PWM, MOTOR_L_DIR, MOTOR_L);
StubMotor stubMotorR = StubMotor(MOTOR_R_PWM, MOTOR_R_DIR, MOTOR_R);
Motor baseMotorL = Motor(MOTOR_L_PWM, MOTOR_L_DIR, MOTOR_L);
Motor baseMotorR = Motor(MOTOR_R_PWM, MOTOR_R_DIR, MOTOR_R);
Motor *motorL = &baseMotorL;
Motor *motorR = &baseMotorR;
MotorTest motorTest = MotorTest(baseMotorL, baseMotorR);
LDR ldr = LDR(LDR_0);
Headlights headlights = Headlights(LEFT_LED, RIGHT_LED, ldr);

int mode = STD_MODE;
int testIdx = 0;

void setup() {
  pinMode(MOTOR_L_PWM, OUTPUT);
  pinMode(MOTOR_L_DIR, OUTPUT);
  pinMode(MOTOR_R_PWM, OUTPUT);
  pinMode(MOTOR_R_DIR, OUTPUT);

  pinMode(LEFT_LED, OUTPUT);
  pinMode(RIGHT_LED, OUTPUT);

  // Set up timer interrupt
  // Disable all interrupts
  noInterrupts();

  // Clear timer registers
  TCCR1A = 0;
  TCCR1B = 0;
  TCNT1  = 0;

  // Set the compare match register to 16MHz/256/1Hz = 62500
  OCR1A = 62500;
  // Set the timer to CTC mode so it clears when it hits the compare value
  TCCR1B |= (1 << WGM12);
  // Set a 256 prescaler
  TCCR1B |= (1 << CS12);
  // Enable the timer compare interrupt
  TIMSK1 |= (1 << OCIE1A);

  // Enable all interrupts
  interrupts();
  
  Serial.begin(9600); // opens serial port, sets data rate to 9600 bps
}

void loop() {
  switch(mode) {
    case STD_MODE:
    case STUB_MODE:
      checkCommand();
      break;
    case TEST_MODE:
      int rc = checkCommand();
      switch (rc) {
        case RC_FAIL:
          Serial.print(rc);
        case RC_NULL:
          if (motorTest.runNextTest()) {
            // Change mode back to standard if the tests are complete
            changeMode(STD_MODE);
          }
          break;
      }
  }
}

// If there are bytes in the Serial buffer, read and parse the input.
// The input is then handled based on the type of the message.
int checkCommand() {
  if (Serial.available() > 0) {
    // Define variables to store inputs
    int type = -1;
    int values[VALUE_MAX] = {-1};
    
    if (readAndParseInput(&type, values) == RC_FAIL) {
      // Error while trying to parse input
      return RC_FAIL;
    }

    switch (type) {
      case MOVE:
        if (values[0] == -1 || values[1] == -1) {
          // x and y values were not provided
          return RC_FAIL;
        }
  
        motorL->setSpeedFromCoords(values[0], values[1]);
        motorR->setSpeedFromCoords(values[0], values[1]);
        break;
      case LED:
        if (values[0] < LED_OFF || values[0] > LED_AUTO) {
          // Value was not set/invalid
          return RC_FAIL;
        }
        headlights.setState(values[0]);
        break;
      case MODE:
        if (values[0] < STD_MODE || values[0] > STUB_MODE) {
          // Value was not set/invalid
          return RC_FAIL;
        }
        changeMode(values[0]);
        break;
      default:
        // Invalid type was given
        return RC_FAIL;
    }

    return RC_OK;
  }

  return RC_NULL;
}

// Read the available Serial input and parse it according to the expected format:
// byte 1: type
// byte 2: value or the high byte of x value
// byte 3: low byte of x value
// byte 4: high byte of y value
// byte 5: low byte of y value
// Input can either be 2 bytes or 5 bytes long.
// Returns a 0 for a successful parse and a 1 to indicate a failure.
// msgType will be set to the value of the 'type' field in the input and keys and values args
// will be set to the keys and values of the succeeding entries.
int readAndParseInput(int *type, int values[]) {
  byte input[INPUT_MAX];
  byte inputSize = Serial.readBytesUntil('\n', input, INPUT_MAX);

  if (inputSize < INPUT_MIN) {
    // Not enough bytes were sent
    return RC_FAIL;
  }

  *type = (int) input[0];

  if (inputSize == INPUT_MIN) {
    values[0] = (int) input[1];
    return RC_OK;
  } else if (inputSize < INPUT_MAX) {
    // 16-bit words were not provided as inputs for the values
    return RC_FAIL;
  }
  
  values[0] = (int) (input[1] << 8 | input[2]);
  values[1] = (int) (input[3] << 8 | input[4]);

  return RC_OK;
}

// Change the mode of operation
void changeMode(int newMode) {
  if (newMode != mode) {
    noInterrupts();
    switch(newMode) {
      case TEST_MODE:
      case STD_MODE:
        motorL = &baseMotorL;
        motorR = &baseMotorR;
        break;
      case STUB_MODE:
        motorL = &stubMotorL;
        motorR = &stubMotorR;
        break;
      default:
        Serial.print("Invalid mode: ");
        Serial.println(mode);
        return;
    }

    mode = newMode;
    interrupts();
  }
}

// Timer compare interrupt service routine
ISR(TIMER1_COMPA_vect)
{
  headlights.autoSet();
}
