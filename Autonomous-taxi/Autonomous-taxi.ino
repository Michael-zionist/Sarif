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
int AnalogPin[5] = {A1pin, A2pin, A3pin, A4pin, A5pin};

// ...motor calibration (native to script):
const float slowingCoeff = 0.92;  // Makes more efficient L motor slower to match R
const int topSpeed = 180;

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
    delay(50);                      // Wait for 0.15 seconds
    digitalWrite(LED_BUILTIN, LOW);  // Turn LED off
    delay(50);                      // Wait for 0.15 seconds
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
int directionController(int spectrum) {
  // Define the spectrum-to-degrees lookup dictionary
  const int spectrumValues[] = {1,  2, 3,   8,  16,  24, 31};   // Spectrum values
  const int degreeValues[]   = {10, 2, 20, -2, -10, -20, 666};  // Corresponding deg (666 -> junction)

  const int dictionarySize = sizeof(spectrumValues) / sizeof(spectrumValues[0]);

  // Loop through the lookup dictionary to find the matching spectrum
  for (int i = 0; i < dictionarySize; i++) {
    if (spectrum == spectrumValues[i]) {
      return degreeValues[i]; // Return corresponding degree
    }
  }

  return 0; // no match is found (default case - no line)
}

// Function to traverse a junction and report current Node position to server.
int* crossJunction(
    int* mapArray, int speed, 
    int turnDegrees = 90, int forwardDistance = 2, float coeff = 0.92
) {
  // Extract key indices and orientation from mapArray
  int& lastNodeIndex = mapArray[8];
  int& nextNodeIndex = mapArray[9];
  int& targetNodeIndex = mapArray[10];
  int& orientation = mapArray[11];

  // Blink LED the number of times corresponding to the current node
  blinkLED(nextNodeIndex);

  // Determine behavior based on the current node
  if (nextNodeIndex == 7) { // Junction 7 behavior
    if (targetNodeIndex == 2 || targetNodeIndex == 0) {
      // Drive straight at node 7
      driveDistance(speed, forwardDistance, coeff);
    } else if (targetNodeIndex == 1 && lastNodeIndex == 0) {
      // Turn left at node 7 (path 0 -> 7 -> 1)
      rotate(-speed, turnDegrees, coeff); // 90-degree left turn
      driveDistance(speed, forwardDistance, coeff);
      orientation = 0; // Update orientation for 7 -> 1 edge
    } else if (targetNodeIndex == 1 && lastNodeIndex == 2) {
      // Turn right at node 7 (path 2 -> 7 -> 1)
      rotate(speed, turnDegrees, coeff); // 90-degree right turn
      driveDistance(speed, forwardDistance, coeff);
      orientation = 1; // Update orientation for 7 -> 1 edge
    }
  } else if (nextNodeIndex == 6) { // Junction 6 behavior
    if (targetNodeIndex == 3 || targetNodeIndex == 4) {
      // Drive straight at node 6
      driveDistance(speed, forwardDistance, coeff);
    } else if (targetNodeIndex == 1 && lastNodeIndex == 3) {
      // Turn left at node 6 (path 3 -> 6 -> 1)
      rotate(-speed, turnDegrees, coeff); // 90-degree left turn
      driveDistance(speed, forwardDistance, coeff);
      orientation = 0; // Update orientation for 6 -> 1 edge
    } else if (targetNodeIndex == 1 && lastNodeIndex == 4) {
      // Turn right at node 6 (path 4 -> 6 -> 1)
      rotate(speed, turnDegrees, coeff); // 90-degree right turn
      driveDistance(speed, forwardDistance, coeff);
      orientation = 1; // Update orientation for 6 -> 1 edge
    }
  } else if (nextNodeIndex == 3 || nextNodeIndex == 4) {
    // If at nodes 3 or 4 and the target involves edge 3 -> 6 or 4 -> 6
    if ((nextNodeIndex == 3 && targetNodeIndex == 6) || 
        (nextNodeIndex == 4 && targetNodeIndex == 6)) {
      rotate(speed, 180, coeff); // Reverse direction (180 degrees)
      driveDistance(speed, 1, coeff); // Drive 1 cm after turn
      orientation = !orientation; // Toggle orientation
    } else {
      // Drive straight for other cases at 3 or 4
      driveDistance(speed, forwardDistance, coeff);
    }
  } else {
    // Drive straight for all other nodes
    driveDistance(speed, forwardDistance, coeff);
  }

  // Update indices in the mapArray
  lastNodeIndex = nextNodeIndex;

  if (lastNodeIndex == 1) {
    // Handle orientation changes at node 1 edges
    if (nextNodeIndex == 7) {
      orientation = 0; // For edge 7 -> 1
    } else if (nextNodeIndex == 6) {
      orientation = 1; // For edge 6 -> 1
    }
  } else {
    // Update nextNodeIndex based on orientation
    if (orientation == 0) {
      nextNodeIndex = (lastNodeIndex == 4) ? 0 : (lastNodeIndex + 1) % 7;
    } else {
      nextNodeIndex = (lastNodeIndex == 0) ? 4 : (lastNodeIndex - 1 + 7) % 7;
    }
  }

  // Print "Hooray" when reaching the target node
  if (nextNodeIndex == targetNodeIndex) {
    Serial.println("Hooray");
  }

  return mapArray; // Return the updated mapArray
}

// testing function to get the index
int getIndex(int array[], int arraySize, int number) {
  int index = -1;  // Default to -1 if the number is not found
  for (int i = 0; i < arraySize; i++) {
    if (array[i] == number) {
      index = i;
      break;
    }
  }
  return index;
} 

//Function to stop motors once reached wall 
void parking(){
  int distanceValue = readDistanceSensor(); // Read the sensor value
  Serial.println(distanceValue);  //Print value in serial to allow debugging

  if (distanceValue > parkDistance){   
     drive(0, 25, false);}  //stops rhino 
    else{
      drive(topSpeed, 25, false);   //drives striaght 
    }
  }

// Set map between 4 and 0, facing counter-clockwise, target: 0 (target index = 0)
int route[5] = {0, 3, 2, 1, 4};
int routeIndex = 0;
bool checkpointReached = false;
int whiteThreshold = 2700; // Example threshold value

// Setup function runs once when you press reset
void setup() {
  Serial.begin(9600);

  // Configure motor pins as outputs
  pinMode(mRpwmPin, OUTPUT);
  pinMode(mRphasePin, OUTPUT);
  pinMode(mLpwmPin, OUTPUT);
  pinMode(mLphasePin, OUTPUT);

  delay(2000); // Short delay before line
  blinkLED(3);
}

void loop() {
  int spectrum = readSensors(whiteThreshold, AnalogPin); // Get spectrum from sensors
  int degrees = directionController(spectrum); // Get degrees based on spectrum

  if (routeIndex >= 5) {spectrum = 0;}  // Stop and blink after route finished

  // Adjust movement based on the detected spectrum
  if (spectrum == 0) {
    // If no line is detected, stop and blink LED
    analogWrite(mRpwmPin, 0);
    analogWrite(mLpwmPin, 0);
    blinkLED(2);
  } else if (degrees == 0) {
    // If the robot is aligned with the line, drive forward
    drive(topSpeed, 50, false); // Drive forward at top speed, no stop condition
  } else if (degrees == 666) {
    // if the robot reaches a junction, cross junction:
    mapArray = crossJunction(mapArray, topSpeed);
    routeIndex++;
  } else {
    turnForward(topSpeed, degrees); // Turn with speed 80 and the degrees value
  }
}
