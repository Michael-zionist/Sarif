#include "../../libraries/constants.h"
#include "../../libraries/cosmetics.h"
#include "../../libraries/motors.h"
#include "../../libraries/sensing.h"
#include "../../libraries/navigation.h"
#include "../../libraries/online.h"

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
  Online online;

  // Configure motor pins as outputs
  pinMode(mRpwmPin, OUTPUT);
  pinMode(mRphasePin, OUTPUT);
  pinMode(mLpwmPin, OUTPUT);
  pinMode(mLphasePin, OUTPUT);

  online.wiFiConnect();
  cosmetics.blinkLED(3); // Blink the LED 3 times to confirm setup
}

int node = 0;  // BEGIN THE SCRIPT BETWEEN 4 & 0 (FACING 0)
void loop() {
  Cosmetics cosmetics;
  Navigation navigation;
  Motors motors;
  Sensing sensing;

  int spectrum = sensing.readSensors(whiteThreshold, AnalogPin); // Get spectrum from sensors
  float turnCoeff = navigation.directionController(spectrum); // Get degrees based on spectrum
  
  
  // Adjust movement based on the detected spectrum
  if (spectrum == 0) {  // If no line is detected, stop and blink LED
    analogWrite(mRpwmPin, 0);
    analogWrite(mLpwmPin, 0);

  } else if (turnCoeff == 0) {  // If the robot is aligned with the line, drive forward
    motors.drive(topSpeed, step, false); // Drive forward at top speed, no stop condition

  } else if (turnCoeff == 666) {
    motors.driveDistance(topSpeed, 5); // Drive forward at top speed, no stop condition
    cosmetics.blinkLED(1);
    mapArray[8] = mapArray[9]; // node reached, so: lastNode = nextNode;
    navigation.GPS(mapArray);  // fetching new nextNode from GPS
    navigation.crossJunction(mapArray, topSpeed);

  } else {
    motors.slideForward(topSpeed, turnCoeff); // Turn with top speed and the degrees value
  }
} 


    

    

    
  