#include "../../libraries/constants.h"
#include "../../libraries/cosmetics.h"
#include "../../libraries/motors.h"
#include "../../libraries/sensing.h"
#include "../../libraries/navigation.h"

/* IMPLIED CALIBRATED VALUES:
const float slowingCoeff = 0.92;  // Makes more efficient L motor slower to match R
const int topSpeed = 180;
const int step = 25;
const int parkDistance = 2000; //distance at which is stops before wall 
int whiteThreshold = 2700; // Calibrate here for light level
*/

// Setup function runs once when you press reset
void setup() {
  Serial.begin(9600);
  Cosmetics cosmetics;

  // Configure motor pins as outputs
  pinMode(mRpwmPin, OUTPUT);
  pinMode(mRphasePin, OUTPUT);
  pinMode(mLpwmPin, OUTPUT);
  pinMode(mLphasePin, OUTPUT);

  cosmetics.blinkLED(3); // Blink the LED 3 times to confirm setup
}

void loop() {
  Cosmetics cosmetics;
  Motors motors;
  Sensing sensing;
  Navigation navigation;

  int spectrum = sensing.readSensors(whiteThreshold, AnalogPin); // Get spectrum from sensors
  float turnCoeff = navigation.directionController(spectrum); // Get degrees based on spectrum

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
  } else if (turnCoeff == 0) {
    // If the robot is aligned with the line, drive forward
    motors.drive(topSpeed, step, false); // Drive forward at speed 80, no stop condition
  } else if (turnCoeff == 666) {
    motors.drive(topSpeed, step, false); // Drive forward at speed 80, no stop condition
    cosmetics.blinkLED(2);
  } else {
    motors.slideForward(topSpeed, turnCoeff); // Turn with speed 80 and the degrees value
  }
}