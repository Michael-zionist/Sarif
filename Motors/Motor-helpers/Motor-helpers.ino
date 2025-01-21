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

/*
Function to drive forward/backward a given distance in cm at a given speed

Direction is evaluated from speed parameter:
positive speed = forward 
negative speed = backward
Input: int speed, int distance (in cm)
Output: movement in a straight line limited by distance
*/
void driveDistance(int speed, int distance) {
  bool direction = speed > 0; // Determine direction from speed
  digitalWrite(mRphasePin, direction); // Set direction for RIGHT motor
  digitalWrite(mLphasePin, direction); // Set direction for LEFT motor

  analogWrite(mRpwmPin, abs(speed)); // Set speed for RIGHT motor
  analogWrite(mLpwmPin, abs(speed) * slowingCoeff); // Set speed for LEFT motor

  delay(abs(round((distance * 9000 / speed)))); // Simulate distance covered (adjust factor as needed)

  analogWrite(mRpwmPin, 0); // Stop RIGHT motor
  analogWrite(mLpwmPin, 0); // Stop LEFT motor
}

/*
Function to drive forward/backward a given distance until stop == false, sampled by 50ms

Direction is evaluated from speed parameter:
positive speed = forward
negative speed = backward
Input: int speed, int distance
Output: movement in a straight line until interrupted by argument
*/
void drive(int speed, bool stop) {
  bool direction = speed > 0; // Determine direction from speed
  digitalWrite(mRphasePin, direction); // Set direction for RIGHT motor
  digitalWrite(mLphasePin, direction); // Set direction for LEFT motor

  analogWrite(mRpwmPin, abs(speed)); // Set speed for RIGHT motor
  analogWrite(mLpwmPin, abs(speed) * slowingCoeff); // Set speed for LEFT motor

  int coveredDistance = 0;
  if (!stop) {
    delay(50); // Simulate distance increments;
  } else {
    analogWrite(mRpwmPin, 0); // Stop RIGHT motor
    analogWrite(mLpwmPin, 0); // Stop LEFT motor
  }
}

/*
Function to negotiate a turn of certain angle in degrees on only the outer wheel

Direction of rotation is evaluated from degrees parameter:
positive degrees = turning clockwise
negative degrees = turning anticlockwise
Input: int speed, int deg
Output: movement forward on an arc limited by degrees
*/
void turn(int speed, int deg) {
  bool clockwise = deg > 0; // Determine direction of rotation from degrees
  if (clockwise) {
    analogWrite(mRpwmPin, 0);         // Stop RIGHT motor
    analogWrite(mLpwmPin, abs(speed) * slowingCoeff);    // Set speed for LEFT motor
  } else {
    analogWrite(mLpwmPin, 0);        // Stop LEFT motor
    analogWrite(mRpwmPin, abs(speed));    // Set speed for RIGHT motor
  }

  delay(abs(deg) * 10); // Simulate turn duration (adjust factor as needed)

  analogWrite(mRpwmPin, 0); // Stop RIGHT motor
  analogWrite(mLpwmPin, 0); // Stop LEFT motor
}

/*
Function to negotiate a turn of certain angle in degrees on both wheels

Direction of rotation is evaluated from degrees parameter:
positive degrees = turning clockwise
negative degrees = turning anticlockwise
Input: int speed, int deg
Output: rotating movement in place on an arc limited by degrees
*/
void rotate(int speed, int deg) {
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

  delay(abs(deg) * 10); // Simulate turn duration (adjust factor as needed)

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
  //Serial.println("Forward"); // Display motor direction
  blinkLED(1); // 3 sec countdown
  delay(1000); // 5 seconds
  blinkLED(1); // 3 sec countdown
  delay(1000); // 5 seconds
  blinkLED(1); // 3 sec countdown
  
  
}