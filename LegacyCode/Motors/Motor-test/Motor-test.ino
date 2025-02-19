int motor1PWM = 37;
int motor1Phase = 38;
// the setup routine runs once when you press reset:
void blinkLED(int times) {
  for (int i = 0; i < times; i++) {
    digitalWrite(LED_BUILTIN, HIGH); // Turn the LED on
    delay(300);                      // Wait for 0.3 seconds
    digitalWrite(LED_BUILTIN, LOW);  // Turn the LED off
    delay(300);                      // Wait for 0.3 seconds
  }
}

void setup() {
  Serial.begin(9600);

  // Configure pins as outputs
  pinMode(motor1PWM, OUTPUT);
  pinMode(motor1Phase, OUTPUT);
  blinkLED(3); // Blinks 3 times to confirm setup was done
}

// the loop routine runs over and over again continuously:
void loop() {
  digitalWrite(motor1Phase, HIGH); //forward
  analogWrite(motor1PWM, 100); // set speed of motor
  //Serial.println("Forward"); // Display motor direction
  delay(2000); //2 seconds
  blinkLED(1); Blinks once after setting to go forward
  digitalWrite(motor1Phase, LOW); //Backward
  analogWrite(motor1PWM, 100); // set speed of motor
  //Serial.println("Backward"); // Display motor direction
  delay(2000); //2 seconds
  blinkLED(2); Blinks 2 times after setting to go back
}