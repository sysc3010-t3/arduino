// wired connections
#define HG7881_A_IA 11 // D11 --> Motor A Input A --> MOTOR B +
#define HG7881_A_IB 12 // D12 --> Motor A Input B --> MOTOR B -
#define HG7881_B_IA 6 // D6 --> Motor B Input A --> MOTOR B +
#define HG7881_B_IB 7 // D7 --> Motor B Input B --> MOTOR B -
#define LEFT_LED 2
#define RIGHT_LED 4
#define LDR_1 0
#define LDR_2 1
#define LDR_3 2
 
// functional connections
#define MOTOR_A_PWM HG7881_A_IA // Motor A PWM Speed
#define MOTOR_A_DIR HG7881_A_IB // Motor A Direction
#define MOTOR_B_PWM HG7881_B_IA // Motor B PWM Speed
#define MOTOR_B_DIR HG7881_B_IB // Motor B Direction

#define INPUT_MAX 256

int motorSpeedA = 0;
int motorSpeedB = 0;
bool fwd = true;
bool ledON = false;

void setup() {
  pinMode(MOTOR_A_PWM, OUTPUT);
  pinMode(MOTOR_A_DIR, OUTPUT);
  pinMode(MOTOR_B_PWM, OUTPUT);
  pinMode(MOTOR_B_DIR, OUTPUT);

  
  // initialize timer1 

  noInterrupts();           // disable all interrupts

  TCCR1A = 0;
  TCCR1B = 0;
  TCNT1  = 0;

  OCR1A = 62500;            // compare match register 16MHz/256/1Hz
  TCCR1B |= (1 << WGM12);   // CTC mode
  TCCR1B |= (1 << CS12);    // 256 prescaler
  TIMSK1 |= (1 << OCIE1A);  // enable timer compare interrupt

  interrupts();             // enable all interrupts
  
  Serial.begin(9600); // opens serial port, sets data rate to 9600 bps
}

void loop() {
  if (Serial.available() > 0) {
    char input[INPUT_MAX];
    byte size = Serial.readBytesUntil('\n', input, INPUT_MAX);
    input[size] = 0;

    char tmp[size+1];
    strcpy(tmp, input);
    Serial.println(tmp);

    // Define variables to store inputs
    char msgType[8] = {0};
    int values[2] = {0};
    int i = 0; // current index of `values` array
    
    // Parse input
    char* strtokIdx = strtok(input, ":");
    while (strtokIdx != NULL) {
      char key[8] = {0};
      strcpy(key, strtokIdx);
      
      strtokIdx = strtok(NULL, ",");
      if (strtokIdx == NULL) {
        break;
      }

      char value[8] = {0};
      strcpy(value, strtokIdx);

      if (!strcmp(key, "type")) {
        strcpy(msgType, value);
      } else {
        values[i++] = atoi(value);
      }
      
      strtokIdx = strtok(NULL, ":");
    }

    if (!strcmp(msgType, "move")) {
      setMotorSpeed(values[0], values[1]);
    } else if (!strcmp(msgType, "lights")) {
      // Light states are 0 (off), 1 (on), 2 (auto)
      if (values[0] != 2) {
        TIMSK1 &= (0 << OCIE1A);  // disable timer compare interrupt
      } else {
        TIMSK1 |= (1 << OCIE1A);  // enable timer compare interrupt
      }
      
      setLights(values[0]);
    }
  }
}

void setMotorSpeed(int xAxis, int yAxis) {
  // Y-axis used for forward and backward control
  if (yAxis < 470) {
    fwd = false;
    // Convert the declining Y-axis readings for going backward from 470 to 0 into 0 to 255 value for the PWM signal for increasing the motor speed
    motorSpeedA = map(yAxis, 470, 0, 0, 255);
    motorSpeedB = map(yAxis, 470, 0, 0, 255);
  } else if (yAxis > 550) {
    fwd = true;
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
  if (motorSpeedA < 120) {
    motorSpeedA = 0;
  }
  if (motorSpeedB < 120) {
    motorSpeedB = 0;
  }

  Serial.print("motorSpeedA: ");
  Serial.println(motorSpeedA);
  Serial.print("motorSpeedB: ");
  Serial.println(motorSpeedB);

  if (fwd) {
    // Set Motor A forward
    digitalWrite(MOTOR_A_PWM, HIGH);
    digitalWrite(MOTOR_A_DIR, HIGH);
    // Set Motor B forward
    digitalWrite(MOTOR_B_PWM, HIGH);
    digitalWrite(MOTOR_B_DIR, HIGH);

    // Forward speed is inverted so speed increases from 255 to 0
    motorSpeedA = 255 - motorSpeedA;
    motorSpeedB = 255 - motorSpeedB;
  } else {
    // Set Motor A backward
    digitalWrite(MOTOR_A_PWM, LOW);
    digitalWrite(MOTOR_A_DIR, LOW);
    // Set Motor B backward
    digitalWrite(MOTOR_B_PWM, LOW);
    digitalWrite(MOTOR_B_DIR, LOW);
  }

  analogWrite(MOTOR_A_PWM, motorSpeedA); // Send PWM signal to motor A
  analogWrite(MOTOR_B_PWM, motorSpeedB); // Send PWM signal to motor B
}

void setLights(int state) {
  if (state) {
  Serial.println("setting lights to high");
    digitalWrite(LEFT_LED, HIGH);
    digitalWrite(RIGHT_LED, HIGH);
  } else {
  Serial.println("setting lights to low");
    digitalWrite(LEFT_LED, LOW);
    digitalWrite(RIGHT_LED, LOW);
  }
}


ISR(TIMER1_COMPA_vect) // timer compare interrupt service routine
{
  int ldrReading = analogRead(LDR_1);
  ldrReading += analogRead(LDR_2);
  ldrReading += analogRead(LDR_3);
  ldrReading = floor(ldrReading/3); // take average of the LDRs

  if (ldrReading < 800) {
    if (!ledON) {
      setLights(1);
      ledON = true;
    }
  } else {
    if (ledON) {
      setLights(0);
      ledON = false;
    }
  }
}
