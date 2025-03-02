// ...pins taken from PINOUT.txt
const int mRpwmPin = 37; // RIGHT Motor PWM
const int mRphasePin = 38; // RIGHT Motor Phase
const int mLpwmPin = 39; // LEFT Motor PWM
const int mLphasePin = 20; // LEFT Motor Phase

// ...motor calibration (native to script):
const float slowingCoeff = 0.92;  // Makes more efficient L motor slower to match R

/*
Fuction to flash the onboard LED a given number of times - only on board v 1.0

Uses no fs | Input: int times | Output: x LED flashes with a 0.3s period
*/
void blinkLED(int times) {
  for (int i = 0; i < times; i++) {
    digitalWrite(LED_BUILTIN, HIGH); // Turn the LED on
    delay(150);                      // Wait for 0.15 seconds
    digitalWrite(LED_BUILTIN, LOW);  // Turn the LED off
    delay(150);                      // Wait for 0.15 seconds
  }
}

// Function to drive the robot forward/backward a given distance in cm
void driveDistance(int speed, int distance, const float slowingCoeff = 0.92) {
  bool direction = speed < 0; // Determine direction from speed
  digitalWrite(mRphasePin, direction); // Set direction for RIGHT motor
  digitalWrite(mLphasePin, direction); // Set direction for LEFT motor

  analogWrite(mRpwmPin, abs(speed)); // Set speed for RIGHT motor
  analogWrite(mLpwmPin, abs(speed) * slowingCoeff); // Set speed for LEFT motor

  delay(abs(round((distance * 9000 / speed)))); // Simulate distance covered (adjust factor as needed)

  analogWrite(mRpwmPin, 0); // Stop RIGHT motor
  analogWrite(mLpwmPin, 0); // Stop LEFT motor
}

// Function to drive the robot forward/backward until stopped, sampled every 50ms
void drive(int speed, int step, bool stop, const float slowingCoeff = 0.92) {
  bool direction = speed < 0; // Determine direction from speed
  digitalWrite(mRphasePin, direction); // Set direction for RIGHT motor
  digitalWrite(mLphasePin, direction); // Set direction for LEFT motor

  analogWrite(mRpwmPin, abs(speed)); // Set speed for RIGHT motor
  analogWrite(mLpwmPin, abs(speed) * slowingCoeff); // Set speed for LEFT motor

  int coveredDistance = 0;
  if (!stop) {
    delay(step); // Simulate distance increments;
  } else {
    analogWrite(mRpwmPin, 0); // Stop RIGHT motor
    analogWrite(mLpwmPin, 0); // Stop LEFT motor
  }
}

// Function to turn the robot forward on the outer wheel
void turnForward(int speed, int deg, const float slowingCoeff = 0.92) {
  bool clockwise = deg > 0; // Determine direction of rotation from degrees
  if (clockwise) {
    analogWrite(mRpwmPin, 0);         // Stop RIGHT motor
    analogWrite(mLpwmPin, abs(speed) * slowingCoeff);    // Set speed for LEFT motor
  } else {
    analogWrite(mLpwmPin, 0);        // Stop LEFT motor
    analogWrite(mRpwmPin, abs(speed));    // Set speed for RIGHT motor
  }

  delay(round(abs(deg) * 2500 / speed)); // Simulate turn duration (adjust factor as needed)

  analogWrite(mRpwmPin, 0); // Stop RIGHT motor
  analogWrite(mLpwmPin, 0); // Stop LEFT motor
}

// Function to rotate the robot in place on both wheels
void rotate(int speed, int deg, const float slowingCoeff = 0.92) {
  bool clockwise = deg > 0; // Determine direction of rotation from degrees
  if (clockwise) {
    digitalWrite(mRphasePin, LOW); // RIGHT motor backward
    digitalWrite(mLphasePin, HIGH); // LEFT motor forward
  } else {
    digitalWrite(mRphasePin, HIGH); // RIGHT motor forward
    digitalWrite(mLphasePin, LOW);  // LEFT motor backward
  }

  analogWrite(mRpwmPin, abs(speed)); // Set speed for RIGHT motor
  analogWrite(mLpwmPin, abs(speed) * slowingCoeff); // Set speed for LEFT motor

  delay(round(abs(deg) * 1200 / speed)); // Simulate turn duration (adjust factor as needed)

  analogWrite(mRpwmPin, 0); // Stop RIGHT motor
  analogWrite(mLpwmPin, 0); // Stop LEFT motor
}

// the setup routine runs once when you press reset:
void setup() {
  Serial.begin(9600);

  // Configure pins as outputs
  pinMode(mRpwmPin, OUTPUT);
  pinMode(mRphasePin, OUTPUT);
  pinMode(mLpwmPin, OUTPUT);
  pinMode(mLphasePin, OUTPUT);
  blinkLED(3); // Blinks 3 times to confirm setup was done
}

// the loop routine runs over and over again continuously:
void loop() {
  blinkLED(1);
  delay(1000); // 1st sec countdown
  blinkLED(1); 
  delay(1000); // 2nd sec countdown
  
  driveDistance(-100, 100); // drive backward straight at top speed for dist. of 1m
  blinkLED(2);

  for(int i = 0; i <= 50; i++) {
    drive(80, 50, i < 40); // drive forward at speed 80 at step of 50ms until i >= 40
    if(i % 20 == 0) blinkLED(1); // blink every second (20*50 = 1000ms)
  }
  blinkLED(2);
  
  turnForward(100, -90); // rotate on outer-wheel 90 deg counter-clockwise at top speed
  blinkLED(4);

  rotate(100, 180); // rotate on both wheel 180 deg counter-clock at top speed
  blinkLED(5);
}