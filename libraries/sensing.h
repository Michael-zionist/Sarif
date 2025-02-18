/*****************************************************************************
sensing.h - Contains "parking()", "readDistanceSensor()" and "readSensors()"
*****************************************************************************/

#include "constants.h"
#include "motors.h"

#ifndef SENSING_H
#define SENSING_H

class Sensing{
    Motors motors;

    public:
        // Parking Function
         void park(){  
            int time = 0;
        
            while (true)
            {
                int distanceValue = readDistanceSensor(); // Read the sensor value
                
                Serial.print("Time Counter: "); //for debugging
                Serial.println(time);
        
                // if object detected before time = 40 
                if (distanceValue > parkDistance && time < 40) { 
                    Serial.println("Obstacle!!! Turning right");
                    motors.rotate(topSpeed, 90); // Turn right in place
                    delay(500);  // Turning duration (adjust as needed)
                } 
                    //after time = 40 park if obstacle
                else if (distanceValue > parkDistance) { 
                    Serial.println("Parking condition met. Stopping motors.");
                    motors.drive(0, 0, false); // Stops the mobot
                    break;
                } 
                else {
                    motors.drive(topSpeed, 25, false);
                    delay(100);
                    time += 1; 
                }
            }
        }

        // Obstacle Detection Function (0 - free, 1 - blocked)
        bool obstacleAhead(int obsDistance = 2000){
            int distanceValue = readDistanceSensor(); // Read the sensor value
            Serial.println(distanceValue);  // Print value in serial for debugging

            return (distanceValue > obsDistance);
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
