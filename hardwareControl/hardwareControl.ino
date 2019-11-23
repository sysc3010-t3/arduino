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
#define INPUT_MAX 256
#define TYPE_MAX 8
#define VALUE_MAX 2
#define BASE_10 10
#define RC_OK 0
#define RC_FAIL 1

#define STD_MODE 0
#define TEST_MODE 1
#define STUB_MODE 2

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
      if (!checkCommand()) {
        if (motorTest.runNextTest()) {
          changeMode(STD_MODE);
        }
      }
      break;
  }
}

int checkCommand() {
  if (Serial.available() > 0) {
    // Define variables to store inputs
    char msgType[TYPE_MAX] = {0};
    char keys[VALUE_MAX][TYPE_MAX] = {0};
    int values[VALUE_MAX] = {-1};
    
    if (readAndParseInput(msgType, keys, values)) {
      // Error while trying to parse input
      Serial.println("Invalid input");
      return 0;
    }

    if (!strcmp(msgType, "move")) {
      if (keys[1][0] == 0 || strcmp(keys[0], "xaxis") || strcmp(keys[1], "yaxis") ||
          values[0] == -1 || values[1] == -1) {
        // Second key not set, keys are not "xaxis" and "yaxis" in that order, or values are not both set
        Serial.println("'move' command is invalid");
        return 0;
      }

      motorL->setSpeedFromCoords(values[0], values[1]);
      motorR->setSpeedFromCoords(values[0], values[1]);
    } else if (!strcmp(msgType, "lights")) {
      if (strcmp(keys[0], "state") || values[0] < LED_OFF || values[0] > LED_AUTO) {
        // Key is not "state" or the value was not set/invalid
        Serial.println("'lights' command is invalid");
        return 0;
      }
      
      headlights.setState(values[0]);
    } else if (!strcmp(msgType, "mode")) {
      if (strcmp(keys[0], "mode") || values[0] < STD_MODE || values[0] > STUB_MODE) {
        Serial.println("'mode' command is invalid");
        return 0;
      }

      changeMode(values[0]);
    }
    
    return 1;
  }

  return 0;
}

// Read the available Serial input and parse it according to the expected format:
// key:value,key:value,...
// The max number of entries in an input is 3.
// Returns a 0 for a successful parse and a 1 to indicate a failure.
// msgType will be set to the value of the 'type' field in the input and keys and values args
// will be set to the keys and values of the succeeding entries.
int readAndParseInput(char msgType[], char keys[][TYPE_MAX], int values[]) {
  char input[INPUT_MAX];
  byte size = Serial.readBytesUntil('\n', input, INPUT_MAX);
  input[size] = 0;

  // Initialize temporary values
  char tmpType[TYPE_MAX] = {0};
  char tmpKeys[VALUE_MAX][TYPE_MAX] = {0};
  int tmpValues[VALUE_MAX] = {0};
  
  int valIdx = 0; // current index of `values` array
  
  // Get the section of characters before the first ':' character (i.e. the key)
  char* strtokIdx = strtok(input, ":");
  while (strtokIdx != NULL) { // While characters are available
    if (valIdx >= VALUE_MAX) {
      // Exceeded max number of values
      return RC_FAIL;
    }

    // Store the key in a variable
    char key[8] = {0};
    strcpy(key, strtokIdx);

    // Get the characters from, but excluding, the ':' character until the next ',' character
    strtokIdx = strtok(NULL, ",");
    if (strtokIdx == NULL) {
      // No characters were available
      return RC_FAIL;
    }

    // Store the value in a variable
    char value[8] = {0};
    strcpy(value, strtokIdx);

    if (!strcmp(key, "type")) {
      // This entry was the message type, so copy it to the msgType argument
      strcpy(tmpType, value);
    } else {
      // This entry was a value
      
      // Confirm the value is a valid integer
      char* endPtr;
      long int longIntVal = strtol(value, &endPtr, BASE_10);
      if (*endPtr || (longIntVal < 0 || longIntVal > 1023)) {
        // Value is not a valid integer or is out of bounds
        return RC_FAIL;
      }

      strcpy(tmpKeys[valIdx], key);
      
      // Casting longIntVal to int will be okay since the range of expected values is [0,1023]
      // so the value won't be cut off
      tmpValues[valIdx++] = (int) longIntVal;
    }

    // Get the characters from, but excluding, the ',' character until the next ':' character
    // i.e. get the next key
    strtokIdx = strtok(NULL, ":");
  }

  if (tmpType[0] == 0 || tmpKeys[0][0] == 0) {
    // Type or first key was not set, so there was an error in the input
    return RC_FAIL;
  }

  // Set arguments to temporary values
  strcpy(msgType, tmpType);
  for (int i = 0; i < valIdx; i++) {
    strcpy(keys[i], tmpKeys[i]);
    values[i] = tmpValues[i];
  }
  
  return RC_OK;
}

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
