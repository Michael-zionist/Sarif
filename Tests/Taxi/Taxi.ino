#include "../../libraries/constants.h"
#include "../../libraries/cosmetics.h"
#include "../../libraries/motors.h"
#include "../../libraries/sensing.h"
#include "../../libraries/navigation.h"
#include "../../libraries/online.h"

/* IMPLIED CALIBRATED VALUES:
const float slowingCoeff = 0.92;  // Makes more efficient L motor slower to match R
const int topSpeed = 220;
const int step = 25;
const int parkDistance = 2000; //distance at which is stops before wall 
int whiteThreshold = 2700; // Calibrate here for light level
*/

// Setup function runs once when you press reset
void setup() {
  Serial.begin(9600);
  Cosmetics cosmetics;
  Online online;
  Navigation navigation;

  // Configure motor pins as outputs
  pinMode(mRpwmPin, OUTPUT);
  pinMode(mRphasePin, OUTPUT);
  pinMode(mLpwmPin, OUTPUT);
  pinMode(mLphasePin, OUTPUT);

  online.wiFiConnect();
  navigation.GPS(mapArray);

  cosmetics.blinkLED(3); // Blink the LED 3 times to confirm setup
}

void loop() {
  Cosmetics cosmetics;
  Motors motors;
  Sensing sensing;
  Navigation navigation;
  Online online;

  int spectrum = sensing.readSensors(whiteThreshold, AnalogPin); // Get spectrum from sensors
  int degrees = navigation.directionController(spectrum); // Get degrees based on spectrum

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
    cosmetics.blinkLED(2);
  } else if (degrees == 0) {
    // If the robot is aligned with the line, drive forward
    motors.drive(topSpeed, step, false); // Drive forward at speed 80, no stop condition
  } else if (degrees == 666) { // Junction!
    
    mapArray[8] = mapArray[9]; // node reached, so: lastNode = nextNode;
    navigation.GPS(mapArray);  // fetching new nextNode from GPS
    navigation.crossJunction(mapArray, topSpeed);
    
  } else{
    motors.turnForward(topSpeed, degrees); // Turn with speed 80 and the degrees value
  }
}