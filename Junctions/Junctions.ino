// this follows the Pinout given and suggested in the Getting Started Guide
const int mRpwmPin = 37;    // RIGHT Motor PWM
const int mRphasePin = 38;  // RIGHT Motor Phase
const int mLpwmPin = 39;    // LEFT Motor PWM
const int mLphasePin = 20;  // LEFT Motor Phase
const int A1pin = 4;        // Analog pin 1 (line-follow B/W 5-sensor)
const int A2pin = 5;        // Analog pin 2 (line-follow B/W 5-sensor)
const int A3pin = 6;        // Analog pin 3 (line-follow B/W 5-sensor)
const int A4pin = 7;        // Analog pin 4 (line-follow B/W 5-sensor)
const int A5pin = 15;       // Analog pin 5 (line-follow B/W 5-sensor)
const int A16pin = 16;      // Analog pin 16 (short distance sensor)
int AnalogPin[5] = {A1pin, A2pin, A3pin, A4pin, A5pin};

// ...motor calibration (native to script):
const float slowingCoeff = 0.92;  // Makes more efficient L motor slower to match R
const int topSpeed = 250;

// Navigation array has set structure: [N, Nf, N, N, Nf, N, Np, N, I, I, I, B]
// N - Navigation node (Nf - fictional, Np - parking)
// I - Index (LastNodeIndex, NextNodeIndex, TargetNodeIndex)
// B - Orientation boolean (0 - counter-clockwise, 1 - clockwise)
// Starting position: LastNode -> 4; NextNode -> 0; TargetNode -> 5; Orientation -> 0 (counter-clockwise)
int mapArray[12] = {0, 7, 2, 3, 6, 4, 5, 1, 5, 0, 6, 0};

//Function to read short distance sensor 
int readDistanceSensor() {
  int distanceValue = analogRead(A16pin); // Reads the sensor value from pin 16
  return distanceValue;                   // Returns distance value
}

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
  Serial.print("Values: ");
  for (int i = 0; i < 5; i++) {
    Serial.print(AnalogValue[i]); // Print sensor values
    Serial.print(" ");
  }
  Serial.println("");

  // Convert binary array to spectrum value (decimal)
  int spectrum = 0;
  for (int i = 0; i < 5; i++) {
    spectrum = (spectrum << 1) | BinaryArray[i];
  }
  return spectrum;
}

// Function to convert the spectrum value to the corresponding degrees
// Returns 666 at a junction
int directionController(int spectrum, int* mapArray, int speed) {
  // Define the spectrum-to-degrees lookup dictionary
  const int spectrumValues[] = {1, 2, 3,  8,   16, 24, 31};   // Spectrum values
  const int degreeValues[]   = {8, 3, 20, -3, -8, -20, 666};  // Corresponding deg (666 -> junction)

  const int dictionarySize = sizeof(spectrumValues) / sizeof(spectrumValues[0]);

  // Loop through the lookup dictionary to find the matching spectrum
  for (int i = 0; i < dictionarySize; i++) {
    if (spectrum == spectrumValues[i]) {
      return degreeValues[i]; // Return corresponding degree
    }
  }

  return 0; // no match is found (default case - no line)
}

// Function to traverse a junction and report current Node position to server
void crossJunction(
    int* mapArray, int speed, int turnDegrees = 90, 
    int forwardDistance = 5, float coeff = 0.92
    ) {
  // Extract indices and orientation from the mapArray
  int& LastNodeIndex = mapArray[8];
  int& NextNodeIndex = mapArray[9];
  int& TargetNodeIndex = mapArray[10];
  int& Orientation = mapArray[11];

  // Determine behavior at junctions
  if (NextNodeIndex == 7 || NextNodeIndex == 6) {
    // Nodes 7 and 6 are true junctions
    if (TargetNodeIndex > NextNodeIndex) {
      // Target node is ahead: Turn 90 degrees right
      rotate(speed, turnDegrees, coeff);
    } else if (TargetNodeIndex < NextNodeIndex) {
      // Target node is behind: Turn 90 degrees left
      rotate(-speed, turnDegrees, coeff);
    }
    // Drive forward 5 cm after turning
    driveDistance(speed, forwardDistance, coeff);
  } else if ((NextNodeIndex == 3 || NextNodeIndex == 4) &&
             (TargetNodeIndex == 6 || TargetNodeIndex == 4 || TargetNodeIndex == 3)) {
    // Nodes 3 and 4 with a target node leading through 3-6 or 4-6
    // Reverse 180 degrees to avoid less advantageous paths
    rotate(speed, 180, coeff);
    driveDistance(speed, forwardDistance, coeff);
  } else {
    // Default: Drive straight over the node for 5 cm
    driveDistance(speed, forwardDistance, coeff);
  }

  // Update navigation indices after crossing the node
  LastNodeIndex = NextNodeIndex;

  if (Orientation == 0) {
    // Clockwise loop direction
    if (LastNodeIndex == 4) {
      NextNodeIndex = 0;
    } else {
      NextNodeIndex = LastNodeIndex + 1;
    }
  } else {
    // Counterclockwise loop direction
    if (LastNodeIndex == 0) {
      NextNodeIndex = 4;
    } else {
      NextNodeIndex = LastNodeIndex - 1;
    }
  }

  // Check if the robot has arrived at the target node
  if (NextNodeIndex == TargetNodeIndex) {
    Serial.println("Hooray");
  }
}



// Setup function runs once when you press reset
void setup() {
  Serial.begin(9600);
  // Set map between 4 and 0, facing counter-clockwise, target: 2 (target index = 2)
  int mapArray[12] = {0, 7, 2, 3, 6, 4, 5, 1, 5, 0, 2, 0}; 

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
  Serial.print("Spectrum: ");
  Serial.println(spectrum);
  Serial.print("Degrees: ");
  Serial.println(degrees);
  */
  
  // Adjust movement based on the detected spectrum
  if (spectrum == 0) {
    // If no line is detected, stop and blink LED
    analogWrite(mRpwmPin, 0);
    analogWrite(mLpwmPin, 0);
    blinkLED(2);
  } else if (degrees == 0) {
    // If the robot is aligned with the line, drive forward
    drive(100, 50, false); // Drive forward at speed 80, no stop condition
  } else if (degrees == 666) {
    // if the robot reaches a junction, cross junction:
    crossJunction(mapArray, 100);
  } else {
    turnForward(100, degrees); // Turn with speed 80 and the degrees value
  }
}
