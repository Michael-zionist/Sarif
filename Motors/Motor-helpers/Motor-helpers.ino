int mRpwmPin = 37; // RIGHT Motor
int mRphasePin = 38; 
int mLpwmPin = 39; // LEFT Motor
int mLphasePin = 20;

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