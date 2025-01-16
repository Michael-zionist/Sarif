// ...pins taken from PINOUT.txt
const int mRpwmPin = 37; // RIGHT Motor PWM
const int mRphasePin = 38; // RIGHT Motor Phase
const int mLpwmPin = 39; // LEFT Motor PWM
const int mLphasePin = 20; // LEFT Motor Phase

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
Function to drive forward/backward a given distance at a given speed 

Uses no fs
Input: int speed, int distance, bool direction
Output: movement in a straight line limited by distance
*/
void driveDistance(int speed, int distance, bool direction) {
  digitalWrite(mRphasePin, direction); // Set direction for RIGHT motor
  digitalWrite(mLphasePin, direction); // Set direction for LEFT motor

  analogWrite(mRpwmPin, speed); // Set speed for RIGHT motor
  analogWrite(mLpwmPin, speed); // Set speed for LEFT motor

  delay(distance * 10); // Simulate distance covered (adjust factor as needed)

  analogWrite(mRpwmPin, 0); // Stop RIGHT motor
  analogWrite(mLpwmPin, 0); // Stop LEFT motor
}

/*
Function to drive forward/backward a given distance until stop == false

Uses no fs
Input: int speed, int distance, bool direction
Output: movement in a straight line until interrupted by argument
*/
void driveUntilStop(int speed, int distance, bool direction) {
  digitalWrite(mRphasePin, direction); // Set direction for RIGHT motor
  digitalWrite(mLphasePin, direction); // Set direction for LEFT motor

  analogWrite(mRpwmPin, speed); // Set speed for RIGHT motor
  analogWrite(mLpwmPin, speed); // Set speed for LEFT motor

  int coveredDistance = 0;
  while (coveredDistance < distance) {
    delay(10); // Simulate distance increments
    coveredDistance += 1;
  }

  analogWrite(mRpwmPin, 0); // Stop RIGHT motor
  analogWrite(mLpwmPin, 0); // Stop LEFT motor
}

/*
Function to negotiate a turn of certain angle in degrees left/right on only the outer wheel

Uses no fs
Input: int speed, int deg, bool right
Output: movement forward on an arc limited by degrees
*/
void turn(int speed, int deg, bool right) {
  if (right) {
    analogWrite(mRpwmPin, 0);         // Stop RIGHT motor
    analogWrite(mLpwmPin, speed);    // Set speed for LEFT motor
  } else {
    analogWrite(mLpwmPin, 0);        // Stop LEFT motor
    analogWrite(mRpwmPin, speed);    // Set speed for RIGHT motor
  }

  delay(deg * 10); // Simulate turn duration (adjust factor as needed)

  analogWrite(mRpwmPin, 0); // Stop RIGHT motor
  analogWrite(mLpwmPin, 0); // Stop LEFT motor
}

/*
Function to negotiate a turn of certain angle in degrees left/right on both wheels

Uses no fs
Input: int speed, int deg, bool right
Output: rotating movement in place on an arc limited by degrees
*/
void rotate(int speed, int deg, bool right) {
  if (right) {
    digitalWrite(mRphasePin, LOW); // RIGHT motor backward
    digitalWrite(mLphasePin, HIGH); // LEFT motor forward
  } else {
    digitalWrite(mRphasePin, HIGH); // RIGHT motor forward
    digitalWrite(mLphasePin, LOW);  // LEFT motor backward
  }

  analogWrite(mRpwmPin, speed); // Set speed for RIGHT motor
  analogWrite(mLpwmPin, speed); // Set speed for LEFT motor

  delay(deg * 10); // Simulate turn duration (adjust factor as needed)

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
  digitalWrite(mRphasePin, HIGH); // motor 1 forward
  digitalWrite(mLphasePin, HIGH); // motor 2 forward
  analogWrite(mRpwmPin, 55); // set speed of motor 1
  analogWrite(mLpwmPin, 50); // set speed of motor 2
  Serial.println("Forward"); // Display motor direction
  delay(2000); //2 seconds
  blinkLED(1); //Blinks once after setting to go forward
  
  digitalWrite(mRphasePin, LOW); // motor 1 forward
  digitalWrite(mLphasePin, LOW); // motor 2 forward
  analogWrite(mRpwmPin, 55); // set speed of motor 1
  analogWrite(mLpwmPin, 50); // set speed of motor 2
  Serial.println("Forward"); // Display motor direction
  delay(2000); // 2 seconds
  blinkLED(2); //Blinks 2 times after setting to go back
}