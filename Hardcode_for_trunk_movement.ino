// Global encoder counts
volatile long encoderCount[6] = {0};

// PID error variables
float Perror[6] = {0}, Ierror[6] = {0};

// Pin definitions
const int PWM_PINS[6] = {13, 12, 11, 10, 9, 8}; // M1 to M6
const int DIR_PINS[6] = {22, 24, 26, 28, 30, 32};
const int ENCODER_PINS[6] = {2, 3, 18, 19, 20, 21};

// Encoder and angle settings
const float countsPerRev = 85000.0;
float targetAngle[6] = {0, 0, 0, 0, 0, 0}; // dynamic targets

// PID constants
float Kp = 5, kd = 2, ki = 100;

int motorStage = 0;

void setup() {
  Serial.begin(9600);

  for (int i = 0; i < 6; i++) {
    pinMode(PWM_PINS[i], OUTPUT);
    pinMode(DIR_PINS[i], OUTPUT);
    pinMode(ENCODER_PINS[i], INPUT_PULLUP);
  }

  // Attach encoder interrupts
  attachInterrupt(digitalPinToInterrupt(ENCODER_PINS[0]), countEncoder0, RISING);
  attachInterrupt(digitalPinToInterrupt(ENCODER_PINS[1]), countEncoder1, RISING);
  attachInterrupt(digitalPinToInterrupt(ENCODER_PINS[2]), countEncoder2, RISING);
  attachInterrupt(digitalPinToInterrupt(ENCODER_PINS[3]), countEncoder3, RISING);
  attachInterrupt(digitalPinToInterrupt(ENCODER_PINS[4]), countEncoder4, RISING);
  attachInterrupt(digitalPinToInterrupt(ENCODER_PINS[5]), countEncoder5, RISING);
}

void loop() {
  long snapshot[6];
  float angle[6];

  noInterrupts();
  for (int i = 0; i < 6; i++) snapshot[i] = encoderCount[i];
  interrupts();

  for (int i = 0; i < 6; i++)
    angle[i] = (snapshot[i] / countsPerRev) * 360.0;

  switch (motorStage) {
    case 0:
      Serial.println("M2 -> 68°");
      targetAngle[1] = 68;
      setMotorDirection(1, true);
      motorStage++;
      break;

    case 1:
      if (moveMotorToAngle(1, angle[1], targetAngle[1])) {
        Serial.println("M1 -> 45°");
        targetAngle[0] = 45;
        setMotorDirection(0, true);
        motorStage++;
      }
      break;

    case 2:
      if (moveMotorToAngle(0, angle[0], targetAngle[0])) {
        Serial.println("Delay 2s");
        delay(2000);
        Serial.println("M6 -> 67°");
        targetAngle[5] = 67;
        setMotorDirection(5, true);
        motorStage++;
      }
      break;

    case 3:
      if (moveMotorToAngle(5, angle[5], targetAngle[5])) {
        Serial.println("Delay 5s");
        delay(5000);
        Serial.println("M6 -> 134° (reverse)");
        targetAngle[5] = 134;
        setMotorDirection(5, false);
        motorStage++;
      }
      break;

    case 4:
      if (moveMotorToAngle(5, angle[5], targetAngle[5])) {
        Serial.println("M4 -> 84°");
        targetAngle[3] = 84;
        setMotorDirection(3, true);
        motorStage++;
      }
      break;

    case 5:
      if (moveMotorToAngle(3, angle[3], targetAngle[3])) {
        Serial.println("Delay 5s");
        delay(5000);
        Serial.println("M4 -> 168° (reverse)");
        targetAngle[3] = 168;
        setMotorDirection(3, false);
        motorStage++;
      }
      break;

    case 6:
      if (moveMotorToAngle(3, angle[3], targetAngle[3])) {
        Serial.println("M1 -> 90° (reverse)");
        targetAngle[0] = 90;
        setMotorDirection(0, false);
        motorStage++;
      }
      break;

    case 7:
      if (moveMotorToAngle(0, angle[0], targetAngle[0])) {
        Serial.println("M2 -> 136° (reverse)");
        targetAngle[1] = 136;
        setMotorDirection(1, false);
        motorStage++;
      }
      break;

    case 8:
      if (moveMotorToAngle(1, angle[1], targetAngle[1])) {
        Serial.println("Sequence complete.");
        for (int i = 0; i < 6; i++) analogWrite(PWM_PINS[i], 0);
        while (1); // stop
      }
      break;
  }

  // Print all motor angles
  for (int i = 0; i < 6; i++) {
    Serial.print("M");
    Serial.print(i + 1);
    Serial.print(": ");
    Serial.print(angle[i], 2);
    Serial.print(" | ");
  }
  Serial.println();

  delay(10);
}

bool moveMotorToAngle(int motorIndex, float currentAngle, float targetAngle) {
  float error = targetAngle - currentAngle;
  Ierror[motorIndex] += error;
  float control = Kp * error + kd * (error - Perror[motorIndex]) + ki * Ierror[motorIndex];
  Perror[motorIndex] = error;
  control = constrain(control, -255, 255);

  if (abs(error) > 1.0) {
    analogWrite(PWM_PINS[motorIndex], abs(control));
    return false;
  } else {
    analogWrite(PWM_PINS[motorIndex], 0);
    return true;
  }
}

void setMotorDirection(int i, bool forward) {
  // Change LOW/HIGH if your driver reverses
  digitalWrite(DIR_PINS[i], forward ? LOW : HIGH);
}

// Encoder ISRs
void countEncoder0() { encoderCount[0]++; }
void countEncoder1() { encoderCount[1]++; }
void countEncoder2() { encoderCount[2]++; }
void countEncoder3() { encoderCount[3]++; }
void countEncoder4() { encoderCount[4]++; }
void countEncoder5() { encoderCount[5]++; }
