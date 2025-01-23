// ...pins taken from PINOUT.txt
const int mRpwmPin = 37; // RIGHT Motor PWM
const int mRphasePin = 38; // RIGHT Motor Phase
const int mLpwmPin = 39; // LEFT Motor PWM
const int mLphasePin = 20; // LEFT Motor Phase

// ...motor calibration (native to script):
const float slowingCoeff = 0.92;  // Makes more efficient L motor slower to match R


void setup() {
  Serial.begin(9600);

  // Configure pins as outputs
  pinMode(motor1PWM, OUTPUT);
  pinMode(motor1Phase, OUTPUT);
  const int step = 50; // Step duration in milliseconds
  bool stop = false; // Variable to control the loop
  blinkLED(3); // Blinks 3 times to confirm setup was done
}

// the loop routine runs over and over again continuously:
void loop() {
  while (!stop) {
    int spectrum = analogRead(A0); // Example sensor input (adjust as needed)
    int turnDeg = directionController(spectrum); // Get the turn direction and magnitude
    
    if (turnDeg == 0) {
      drive(80, step, stop); // Move forward if no turn needed
    } else if (turnDeg > 0) {
      turnForward(80, turnDeg); // Adjust to the right
    } else {
      turnForward(80, turnDeg); // Adjust to the left (negative degrees)
    }
    
    // Optional: Add a condition to break the loop if needed
  }
}