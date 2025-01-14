int m1PWMPin = 37;
int m1PhasePin = 38;
int m2PWMPin = 39;
int m2PhasePin = 20;

// blinks onboard LED x times with a 0.6s total on-off period
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
  pinMode(motor1PWM, OUTPUT);
  pinMode(motor1Phase, OUTPUT);
}
// the loop routine runs over and over again continuously:
void loop() {
  digitalWrite(motor1Phase, HIGH); //forward
  analogWrite(motor1PWM, 100); // set speed of motor
  Serial.println("Forward"); // Display motor direction
  delay(2000); //2 seconds
  digitalWrite(motor1Phase, LOW); //Backward
  analogWrite(motor1PWM, 100); // set speed of motor
  Serial.println("Backward"); // Display motor direction
  delay(2000); //2 seconds
}