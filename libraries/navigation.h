/******************************************************************
navigation.h - Contains "getIndex()", "directionController()", "crossJunction()" and "GPS()"
******************************************************************/

#include "constants.h"
#include "cosmetics.h"
#include "motors.h"
#include "sensing.h"
#include "online.h"

#ifndef NAVIGATION_H
#define NAVIGATION_H

class Navigation{
    Cosmetics cosmetics;
    Motors motors;
    Sensing sensing;
    Online online;

    public:
        // testing function to get the index
        int getIndex(int array[], int arraySize, int number) {
            int index = -1;  // Default to -1 if the number is not found
            for (int i = 0; i < arraySize; i++) {
                if (array[i] == number) {
                index = i;
                break;
                }
            }
            return index;
        } 

        // Function to convert the spectrum value to the corresponding degrees
        // Returns 666 at a junction : corresponding deg (666 -> junction)
        float directionController(int spectrum) {
            // Define the spectrum-to-degrees lookup dictionary
            // Spectrum values -> 
            const int spectrumValues[] = {1,    2,    3,   6,    7,    8,     12,   16,     24,  28,  31,  30,  15};   
            const float slowCoeffs[]   = {0.02, 0.15, 0.1, 0.9,  0.1, -0.15, -0.9, -0.02,  -0.1, 0.1, 666, 666, 666};

            const int dictionarySize = sizeof(spectrumValues) / sizeof(spectrumValues[0]);

            // Loop through the lookup dictionary to find the matching spectrum
            for (int i = 0; i < dictionarySize; i++) {
                if (spectrum == spectrumValues[i]) {
                    return slowCoeffs[i]; // Return corresponding degree
                }
            }
            //delay(25);

            return 0; // no match is found (default case - no line)
            }

        // Function to traverse a junction and report current Node position to server.
        void crossJunction(
            int* mapArray, int speed, 
            int turnDegrees = 90, int forwardDistance = 2, float coeff = 0.92
        ) {
            // Extract key indices and orientation from mapArray
            int& lastNodeIndex = mapArray[8];
            int& nextNodeIndex = mapArray[9];
            int& targetNodeIndex = mapArray[10];
            int& orientation = mapArray[11];

            // Blink LED the number of times corresponding to the current node
            cosmetics.blinkLED(nextNodeIndex);

            // Determine behavior based on the current node
            if (nextNodeIndex == 7) { // Junction 7 behavior
                if (targetNodeIndex == 2 || targetNodeIndex == 0) {
                // Drive straight at node 7
                motors.driveDistance(speed, forwardDistance, coeff);
                } else if (targetNodeIndex == 1 && lastNodeIndex == 0) {
                // Turn left at node 7 (path 0 -> 7 -> 1)
                motors.rotate(-speed, turnDegrees, coeff); // 90-degree left turn
                motors.driveDistance(speed, forwardDistance, coeff);
                orientation = 0; // Update orientation for 7 -> 1 edge
                } else if (targetNodeIndex == 1 && lastNodeIndex == 2) {
                // Turn right at node 7 (path 2 -> 7 -> 1)
                motors.rotate(speed, turnDegrees, coeff); // 90-degree right turn
                motors.driveDistance(speed, forwardDistance, coeff);
                orientation = 1; // Update orientation for 7 -> 1 edge
                }
            } else if (nextNodeIndex == 6) { // Junction 6 behavior
                if (targetNodeIndex == 3 || targetNodeIndex == 4) {
                // Drive straight at node 6
                motors.driveDistance(speed, forwardDistance, coeff);
                } else if (targetNodeIndex == 1 && lastNodeIndex == 3) {
                // Turn left at node 6 (path 3 -> 6 -> 1)
                motors.rotate(-speed, turnDegrees, coeff); // 90-degree left turn
                motors.driveDistance(speed, forwardDistance, coeff);
                orientation = 0; // Update orientation for 6 -> 1 edge
                } else if (targetNodeIndex == 1 && lastNodeIndex == 4) {
                // Turn right at node 6 (path 4 -> 6 -> 1)
                motors.rotate(speed, turnDegrees, coeff); // 90-degree right turn
                motors.driveDistance(speed, forwardDistance, coeff);
                orientation = 1; // Update orientation for 6 -> 1 edge
                }
            } else if (nextNodeIndex == 3 || nextNodeIndex == 4) {
                // If at nodes 3 or 4 and the target involves edge 3 -> 6 or 4 -> 6
                if ((nextNodeIndex == 3 && targetNodeIndex == 6) || 
                    (nextNodeIndex == 4 && targetNodeIndex == 6)) {
                motors.rotate(speed, 180, coeff); // Reverse direction (180 degrees)
                motors.driveDistance(speed, 1, coeff); // Drive 1 cm after turn
                orientation = !orientation; // Toggle orientation
                } else {
                // Drive straight for other cases at 3 or 4
                motors.driveDistance(speed, forwardDistance, coeff);
                }
            } else {
                // Drive straight for all other nodes
                motors.driveDistance(speed, forwardDistance, coeff);
            }

            // Update indices in the mapArray
            lastNodeIndex = nextNodeIndex;

            if (lastNodeIndex == 1) {
                // Handle orientation changes at node 1 edges
                if (nextNodeIndex == 7) {
                orientation = 0; // For edge 7 -> 1
                } else if (nextNodeIndex == 6) {
                orientation = 1; // For edge 6 -> 1
                }
            } else {
                // Update nextNodeIndex based on orientation
                if (orientation == 0) {
                nextNodeIndex = (lastNodeIndex == 4) ? 0 : (lastNodeIndex + 1) % 7;
                } else {
                nextNodeIndex = (lastNodeIndex == 0) ? 4 : (lastNodeIndex - 1 + 7) % 7;
                }
            }

            // Print "Hooray" when reaching the target node
            if (nextNodeIndex == targetNodeIndex) {
                Serial.println("Hooray");
            }
        }

        // Function for updating Map Array (GPS)
        int* GPS(int mapArray[]){
            if(mapArray[8] == mapArray[10]){  // Reached Destination Node, Pone Server
                int destNode = online.destinationFetch(mapArray[mapArray[8]]); // Next destination
                Serial.println("Arrived! Fetching next node...");

                for(int i=0; i<8; i++){
                    if(mapArray[i] == destNode){  // Search array for destination node
                        mapArray[10] = i;     // TargetNode assigned destNode index
                        Serial.println("Destination at index: ");
                        Serial.println(i);
                    }
                }

                if(mapArray[11] == 0){    // Counter Clockwise
                    mapArray[9] = mapArray[8] + 1;    // NextNode index is LastNode incremented
                    Serial.println("NextNode index is: ");
                    Serial.println(mapArray[9]);
                    return mapArray;
                }

                else{     // Clockwise
                    mapArray[9] = mapArray[8]--;    // NextNode index is LastNode decremented
                    Serial.println("NextNode index is: ");
                    Serial.println(mapArray[9]);
                    return mapArray;
                }
            }

            else{   // LastNode == NextNode
                if (mapArray[11] == 0){    // Counter Clockwise
                    mapArray[9] = mapArray[8] + 1;    // NextNode index is LastNode incremented
                    Serial.println("NextNode index is: ");
                    Serial.println(mapArray[9]);
                    return mapArray;
                }

                else {     // Clockwise
                    mapArray[9] = mapArray[8]--;    // NextNode index is LastNode decremented
                    Serial.println("NextNode index is: ");
                    Serial.println(mapArray[9]);
                    return mapArray;
                }
            }
        }
};

#endif // NAVIGATION_H
