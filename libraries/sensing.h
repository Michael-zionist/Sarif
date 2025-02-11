/*****************************************************************************
sensing.h - Contains "park()", "readDistanceSensor()" and "readSensors()"
*****************************************************************************/

#include "constants.h"
#include "motors.h"

#ifndef SENSING_H
#define SENSING_H

class Sensing{
    Motors motors;

    public:
        // Park Function
        void park(){
            int distanceValue = readDistanceSensor(); // Read the sensor value
            Serial.println(distanceValue);  // Print value in serial for debugging

            int traveledDistance = 0; // Track how far the robot has moved
            const int obstacleThreshold = 10; // Define a threshold for obstacle detection
            const int safeDistance = 40; // Minimum distance before detecting obstacle as wall
            const int stepSize = 5; // Small step to move at a time
            const int sideMoveDistance = 10; // Distance to move sideways after detecting obstacle

            while (true) {

               // distanceValue = readDistanceSensor(); //DUPLICATE code i think 
                // Serial.printIn(distanceValue);

                if(distanceValue < obstacleThreshold && traveledDistance < safeDistance){
                    Serial.printIn("Obstacle detected! Executing avoidance maneuver.");
                    motors.drive(0, 0, false);  // Stop the robot
                    motors.rotate(50, 90); // Turn right
                    motors.driveDistance(50, sideMoveDistance); // Move forward 10 cm
                    motors.rotate(-50, 90); // Turn left
                    break; // Exit loop and continue normal parking
                }

                else if(distanceValue > parkDistance) {
                    motors.drive(0,0,false); //stops mobot
                    break; 

                }else{
                    motors.drive(topSpeed, 25, false);
                    traveledDistance += stepSize;
                    delay(100);
                }
            }
        }

        // Distance Sensor Reading Function
        int readDistanceSensor(){
            int distanceValue = analogRead(A16pin); // Reads the sensor value from pin 16
            return distanceValue;                   // Returns distance value
        }

        // Read Sensors Function
        int readSensors(int whiteThreshold, int* AnalogPin){
            static int BinaryArray[5]; // Static array to ensure it persists after the function returns
            int AnalogValue[5];

            for (int i = 0; i < 5; i++) {
                AnalogValue[i] = analogRead(AnalogPin[i]);

                // Decide to add 0 or 1 based on threshold

                if (AnalogValue[i] < whiteThreshold) {
                    BinaryArray[i] = 1;
                }

                else {
                    BinaryArray[i] = 0;
                }
            }
            //Serial.print("Values: ");
            for (int i = 0; i < 5; i++) {
                AnalogValue[i] = analogRead(AnalogPin[i]); // Read the analog value
                //Serial.print(AnalogValue[i]); // Print each sensor's value
                //Serial.print(" ");
            }
            //Serial.println("");

            int spectrum = 0; // variable to store computed decimal value
            for (int i = 0; i < 5; i++){
                spectrum = (spectrum << 1) | BinaryArray[i];
            }
            return spectrum;
        }
};

#endif // SENSING_H
