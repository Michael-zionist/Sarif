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
        void parking(int whiteThreshold = 2700){
            int distanceValue = readDistanceSensor(); // Read the sensor value
            Serial.println(distanceValue);  //Print value in serial to allow debugging

            if (distanceValue > parkDistance){
                motors.drive(0, 25, false);}  //stops rhino
            else{
                motors.drive(topSpeed, 25, false);   //drives striaght
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
            Serial.print("Values: ");
            for (int i = 0; i < 5; i++) {
                AnalogValue[i] = analogRead(AnalogPin[i]); // Read the analog value
                Serial.print(AnalogValue[i]); // Print each sensor's value
                Serial.print(" ");
            }
            Serial.println("");

            int spectrum = 0; // variable to store computed decimal value
            for (int i = 0; i < 5; i++){
                spectrum = (spectrum << 1) | BinaryArray[i];
            }
            return spectrum;
        }
};

#endif // SENSING_H
