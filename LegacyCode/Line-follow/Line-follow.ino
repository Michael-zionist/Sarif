// Define pins for motor control
const int mRpwmPin = 37;    // RIGHT Motor PWM
const int mRphasePin = 38;  // RIGHT Motor Phase
const int mLpwmPin = 39;    // LEFT Motor PWM
const int mLphasePin = 20;  // LEFT Motor Phase
const int topSpeed = 180;

// Define pins for line-following sensors
const int A1pin = 4;        // Analog pin 1 (line-follow B/W 5-sensor)
const int A2pin = 5;        // Analog pin 2 (line-follow B/W 5-sensor)
const int A3pin = 6;        // Analog pin 3 (line-follow B/W 5-sensor)
const int A4pin = 7;        // Analog pin 4 (line-follow B/W 5-sensor)
const int A5pin = 15;       // Analog pin 5 (line-follow B/W 5-sensor)
int AnalogPin[5] = {A1pin, A2pin, A3pin, A4pin, A5pin};

// Motor calibration
const float slowingCoeff = 0.92; // Slower left motor to match right motor

// Function to blink the onboard LED a given number of times
void blinkLED(int times) {
  for (int i = 0; i < times; i++) {
    digitalWrite(LED_BUILTIN, HIGH); // Turn LED on
    delay(150);                      // Wait for 0.15 seconds
    digitalWrite(LED_BUILTIN, LOW);  // Turn LED off
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

// Function to convert the input of the 5 analog black/white sensors into a spectrum value
int readSensors(int whiteThreshold, int* AnalogPin) {
  static int BinaryArray[5]; // Static array to ensure it persists after the function returns
  int AnalogValue[5];

  // Read sensor values and convert to binary (black or white)
  for (int i = 0; i < 5; i++) {
    AnalogValue[i] = analogRead(AnalogPin[i]);

    // Add 1 or 0 to BinaryArray depending on threshold
    if (AnalogValue[i] < whiteThreshold) {
      BinaryArray[i] = 1;
    } else {
      BinaryArray[i] = 0;
    }
  }

  // Print the sensor values
  //Serial.print("Values: ");
  for (int i = 0; i < 5; i++) {
    //Serial.print(AnalogValue[i]); // Print sensor values
    //Serial.print(" ");
  }
  //Serial.println("");

  // Convert binary array to spectrum value (decimal)
  int spectrum = 0;
  for (int i = 0; i < 5; i++) {
    spectrum = (spectrum << 1) | BinaryArray[i];
  }
  return spectrum;
}

// Function to convert the spectrum value to the corresponding degrees
int directionController(int spectrum) {
  // Define the spectrum-to-degrees lookup dictionary
  const int spectrumValues[] = {1, 2, 3,   8,  16, 24};   // Spectrum values
  const int degreeValues[]   = {10, 2, 20, -2, -10, -20};   // Corresponding degree values

  const int dictionarySize = sizeof(spectrumValues) / sizeof(spectrumValues[0]);

  // Loop through the lookup dictionary to find the matching spectrum
  for (int i = 0; i < dictionarySize; i++) {
    if (spectrum == spectrumValues[i]) {
      return degreeValues[i]; // Return corresponding degree
    }
  }

  return 0; // no match is found (default case - no line)
}

// Setup function runs once when you press reset
void setup() {
  Serial.begin(9600);

  // Configure motor pins as outputs
  pinMode(mRpwmPin, OUTPUT);
  pinMode(mRphasePin, OUTPUT);
  pinMode(mLpwmPin, OUTPUT);
  pinMode(mLphasePin, OUTPUT);

  blinkLED(3); // Blink the LED 3 times to confirm setup

  delay(2000); // Short delay before line
  blinkLED(3);
}

void loop() {
  int whiteThreshold = 2700; // Example threshold value
  int spectrum = readSensors(whiteThreshold, AnalogPin); // Get spectrum from sensors
  int degrees = directionController(spectrum); // Get degrees based on spectrum

  /*
  //Serial.print("Spectrum: ");
  //Serial.println(spectrum);
  //Serial.print("Degrees: ");
  //Serial.println(degrees);
  */
  
  // Adjust movement based on the detected spectrum
  if (spectrum == 0) {
    // If no line is detected, stop and blink LED
    analogWrite(mRpwmPin, 0);
    analogWrite(mLpwmPin, 0);
    blinkLED(2);
  } else if (degrees == 0) {
    // If the robot is aligned with the line, drive forward
    drive(topSpeed, 50, false); // Drive forward at speed 80, no stop condition
  } else {
    turnForward(topSpeed, degrees); // Turn with speed 80 and the degrees value
  }
}
