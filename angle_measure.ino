// Global encoder counts
volatile long encoderCount[6] = {0};  // encoderCount[0] to encoderCount[5]

// Pin definitions
const int ENCODER_PINS[6] = {2, 3, 18, 19, 20, 21};

// Counts per revolution (customize based on your encoder)
const float countsPerRev = 85000.0;

void setup() {
  Serial.begin(9600);

  for (int i = 0; i < 6; i++) {
    pinMode(ENCODER_PINS[i], INPUT_PULLUP);
  }

  // Attach interrupts (must be attached individually)
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

  // Print all angles
  for (int i = 0; i < 6; i++) {
    Serial.print("M");
    Serial.print(i + 1);
    Serial.print(": ");
    Serial.print(angle[i], 2);
    Serial.print("° | ");
  }
  Serial.println();

  delay(100); // Slower update for easier viewing
}

// Interrupt Service Routines
void countEncoder0() { encoderCount[0]++; }
void countEncoder1() { encoderCount[1]++; }
void countEncoder2() { encoderCount[2]++; }
void countEncoder3() { encoderCount[3]++; }
void countEncoder4() { encoderCount[4]++; }
void countEncoder5() { encoderCount[5]++; }
