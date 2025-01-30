int mRPWMPin = 37; // RIGHT Motor
int mRPhasePin = 38; 
int mLPWMPin = 39; // LEFT Motor
int mLPhasePin = 20;

void blinkLED(int times) {
  for (int i = 0; i < times; i++) {
    digitalWrite(LED_BUILTIN, HIGH); // Turn the LED on
    delay(300);                      // Wait for 0.3 seconds
    digitalWrite(LED_BUILTIN, LOW);  // Turn the LED off
    delay(300);                      // Wait for 0.3 seconds
  }
}

// the setup routine runs once when you press reset:
void setup() {
  Serial.begin(9600);

  // Configure pins as outputs
  pinMode(mRPWMPin, OUTPUT);
  pinMode(mRPhasePin, OUTPUT);
  pinMode(mLPWMPin, OUTPUT);
  pinMode(mLPhasePin, OUTPUT);
  blinkLED(3); // Blinks 3 times to confirm setup was done
}

// the loop routine runs over and over again continuously:
void loop() {
  digitalWrite(mRPhasePin, HIGH); // motor 1 forward
  digitalWrite(mLPhasePin, HIGH); // motor 2 forward
  analogWrite(mRPWMPin, 55); // set speed of motor 1
  analogWrite(mLPWMPin, 50); // set speed of motor 2
  Serial.println("Forward"); // Display motor direction
  delay(2000); //2 seconds
  blinkLED(1); //Blinks once after setting to go forward
  
  digitalWrite(mRPhasePin, LOW); // motor 1 forward
  digitalWrite(mLPhasePin, LOW); // motor 2 forward
  analogWrite(mRPWMPin, 55); // set speed of motor 1
  analogWrite(mLPWMPin, 50); // set speed of motor 2
  Serial.println("Forward"); // Display motor direction
  delay(2000); // 2 seconds
  blinkLED(2); //Blinks 2 times after setting to go back
}