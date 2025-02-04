#include "../../libraries/constants.h"
#include "../../libraries/cosmetics.h"
#include "../../libraries/motors.h"
#include "../../libraries/sensing.h"
int whiteThreshold = 2700; // Calibrate here

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
    drive(topSpeed, 50, false); // Drive forward at speed 80, no stop condition
  } else {
    turnForward(topSpeed, degrees); // Turn with speed 80 and the degrees value
  }
}
