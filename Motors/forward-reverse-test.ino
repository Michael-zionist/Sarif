int m1PWMPin = 37;
int m1PhasePin = 38;
int m2PWMPin = 39;
int m2PhasePin = 20;

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
  pinMode(m1PWMPin, OUTPUT);
  pinMode(m1PhasePin, OUTPUT);
  pinMode(m2PWMPin, OUTPUT);
  pinMode(m2PhasePin, OUTPUT);
  blinkLED(3); // Blinks 3 times to confirm setup was done
}

// the loop routine runs over and over again continuously:
void loop() {
  digitalWrite(m1PhasePin, HIGH); // motor 1 forward
  digitalWrite(m2PhasePin, HIGH); // motor 2 forward
  analogWrite(m1PWMPin, 50); // set speed of motor 1
  analogWrite(m2PWMPin, 50); // set speed of motor 2
  Serial.println("Forward"); // Display motor direction
  delay(2000); //2 seconds
  blinkLED(1); Blinks once after setting to go forward
  
  digitalWrite(m1PhasePin, LOW); // motor 1 forward
  digitalWrite(m2PhasePin, LOW); // motor 2 forward
  analogWrite(m1PWMPin, 50); // set speed of motor 1
  analogWrite(m2PWMPin, 50); // set speed of motor 2
  Serial.println("Forward"); // Display motor direction
  delay(2000); // 2 seconds
  blinkLED(2); Blinks 2 times after setting to go back
}