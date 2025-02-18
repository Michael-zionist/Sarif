#include "../../libraries/constants.h"
#include "../../libraries/cosmetics.h"
#include "../../libraries/motors.h"
#include "../../libraries/sensing.h"
#include "../../libraries/navigation.h"


  Cosmetics cosmetics;
  Motors motors;
  Sensing sensing;
  Navigation navigation;

/* IMPLIED CALIBRATED VALUES:
const float slowingCoeff = 0.92;  // Makes more efficient L motor slower to match R
const int topSpeed = 190;
const int step = 40;
const int parkDistance = 2000; //distance at which is stops before wall 
int whiteThreshold = 2700; // Calibrate here for light level
*/

// Setup function runs once when you press reset
void setup() {
  Serial.begin(115200);
  Cosmetics cosmetics;

  // Configure motor pins as outputs
  pinMode(mRpwmPin, OUTPUT);
  pinMode(mRphasePin, OUTPUT);
  pinMode(mLpwmPin, OUTPUT);
  pinMode(mLphasePin, OUTPUT);
}


void loop() {
  // 50-step alignment loop (simplified line-follow loop)
  for (int i = 0; i < 50; i++) {
    int spectrum = sensing.readSensors(whiteThreshold, AnalogPin); // Get spectrum from sensors
    float turnCoeff = navigation.directionController(spectrum); // Get degrees based on spectrum
    if (turnCoeff == 0) {  // If the robot is aligned with the line, drive forward
      motors.drive(topSpeed/2, step, false); // Drive forward at top speed, no stop condition
    } else {
      motors.slideForward(topSpeed/2, turnCoeff); // Turn with top speed and the degrees value
    }
  }

  sensing.park();
  delay(60000); 
} 

