/******************************************************************
navigation.h - Contains "getIndex()", "directionController()", "crossJunction()" and "GPS()"
******************************************************************/
#include "constants.h"
#include "cosmetics.h"
#include "motors.h"
#include "sensing.h"
#include "online.h"

class Navigation {
    Cosmetics cosmetics;
    Motors motors;
    Sensing sensing;
    Online online;

    public:
        // Function to get the index of a value in an array
        int getIndex(int array[], int arraySize, int number) {
            for (int i = 0; i < arraySize; i++) {
                if (array[i] == number) {
                    return i;
                }
            }
            return -1; // Return -1 if not found
        }

        // Function to map spectrum values to direction coefficients
        float directionController(int spectrum) {
            const int spectrumValues[] = {1, 2, 3, 6, 7, 8, 12, 16, 24, 28, 31, 30, 15};
            const float slowCoeffs[]   = {0.02, 0.15, 0.1, 0.9, 0.1, -0.15, -0.9, -0.02, -0.1, 0.1, 666, 666, 666};
            const int dictionarySize = sizeof(spectrumValues) / sizeof(spectrumValues[0]);

            for (int i = 0; i < dictionarySize; i++) {
                if (spectrum == spectrumValues[i]) {
                    return slowCoeffs[i];
                }
            }
            return 0; // Default case
        }

        // Function to traverse a junction in the correct way based on nextNode
        void crossJunction(int* mapArray, int speed, int turnDegrees = 90, int forwardDistance = 5, float coeff = 0.92) {
            int& lastNodeIndex = mapArray[8];
            int& nextNodeIndex = mapArray[9];
            int& targetNodeIndex = mapArray[10];
            int& orientation = mapArray[11];
            int& nextNode = mapArray[nextNodeIndex];
            int& lastNode = mapArray[lastNodeIndex];

            // mapArray: [0, 7, 2, 3, 6, 4, 5, 1, 0, 0, 0, 0] (... note: 0723640 are the "main loop")
            // Main loop: next node succeeds last node in mapArray:
            bool succeedsOnMain =
                (nextNodeIndex < 6 && nextNodeIndex == (lastNodeIndex + 1)) ||
                (lastNodeIndex == 5 && nextNodeIndex == 0); // array border case: nodes 4 -> 0
            // Main loop: next node preceeds last node in mapArray:
            bool preceedssOnMain =
                (nextNodeIndex < 6 && nextNodeIndex == (lastNodeIndex - 1)) ||
                (lastNodeIndex == 0 && nextNodeIndex == 5); // array border case: nodes 0 -> 4

            // Turning & updating orientation:
            if (succeedsOnMain) {
                if (orientation == 1) {motors.rotate(speed, 180, coeff); orientation = 0;}
                else if (orientation > 1) {motors.rotate(speed, -90, coeff); orientation = 0;} 
                else ; // default case -> drive forward

            } else if (preceedssOnMain) {
                if (orientation == 0) {motors.rotate(speed, 180, coeff); orientation = 1;}
                else if (orientation > 1) {motors.rotate(speed, 90, coeff); orientation = 1;} 
                else ; // default case -> drive forward

            } else if (nextNode == 1) {  // Next node is off main loop
                if (orientation == 0) motors.rotate(speed, -90, coeff); // counter-clockwise
                else motors.rotate(speed, 90, coeff); // clockwise
                
                if (lastNode == 7) orientation = 2;
                if (lastNode == 6) orientation = 3;
            
            } else if (lastNode == 1) {  // Car is off main loop
                if (nextNode == 6 && orientation == 3) {motors.rotate(speed, 180, coeff); orientation = 2;}
                if (nextNode == 7 && orientation == 2) {motors.rotate(speed, 180, coeff); orientation = 3;}
                else ; // default case -> drive forward

            } else if (nextNode == 5) { // Parking
                if (orientation == 0) {motors.rotate(speed, 90, coeff); orientation = 2;}
                if (orientation == 1) {motors.rotate(speed, -90, coeff); orientation = 2;}
            }
            
            Serial.print("Moving from ");
            Serial.print(lastNode);
            Serial.print(" towards ");
            Serial.println(nextNode);
            motors.driveDistance(speed, forwardDistance, coeff); // move off juction at the end
        }

        // Function for updating Map Array (GPS - Pathfinding)
        int* GPS(int mapArray[]){
            if(mapArray[8] == mapArray[10]){  // CurrentNode == TargetNode, Pone Server
                Serial.println("Arrived! Fetching next node...");
                int destNode = online.destinationFetch(mapArray[mapArray[8]]); // Next Target

                for(int i = 0; i < 8; i++){
                    if(mapArray[i] == destNode){  // Search array for destination node
                        mapArray[10] = i;         // TargetNode assigned destNode index
                        Serial.print("Destination Node: ");
                        Serial.println(mapArray[mapArray[10]]);
                    }
                }

                if(mapArray[8] == 7){     // If at Node 1
                    if (mapArray[10] == 3 || mapArray[10] == 5 || mapArray[10] == 6){   // Target Node is 3, 4, or 5
                        mapArray[9] = 4;      // Next Node is 6
                    
                    } else if(mapArray[10] == 0 || mapArray[10] == 2){    // Target Node is 0 or 2
                        mapArray[9] = 1;      // Next Node is 7
                    }
                }
                else if(mapArray[8] == 0 && mapArray[10] == 5){    // Node 0 -> 4 (Optimisation)
                    mapArray[9] = 5;    // Next Node is 4
                }
                else if(mapArray[8] == 5 && mapArray[10] == 0){    // Node 4 -> 0 (Special Case)
                    mapArray[9] = 0;    // Next Node is 0
                }
                else if(mapArray[8] == 5 && mapArray[10] == 6){    // Node 4 -> 6 (Special Case)
                    mapArray[9] = 4;    // Next Node is 6
                }
                else if(mapArray[8] == 3 && mapArray[10] == 2){    // Node 3 -> 2 (Optimisation)
                    mapArray[9] = 2;    // Next Node is 2
                }
                else{   // Increment/Decrement mapArray for NextNode (Standard)
                    if(mapArray[11] == 0){    // Counter Clockwise
                        mapArray[9] = mapArray[8] + 1;
                    } else {                  // Clockwise
                        mapArray[9] = mapArray[8] - 1;
                    }
                }
                Serial.print("Next Node: ");
                Serial.println(mapArray[mapArray[9]]);
                return mapArray;
            }

            else {   // CurrentNode == NextNode
                if(mapArray[8] == 1){   // Junction 7 (Virtual)
                    if(mapArray[10] == 2 || mapArray[10] == 3){ // Target 2 or 3
                        mapArray[9] = 2;    // Next Node is 2
                    } else if(mapArray[10] == 7 || mapArray[10] == 4 || mapArray[10] == 6){ // Target 1 or 6 or 5
                        mapArray[9] = 7;    // Next Node is 1
                    } else if(mapArray[10] == 0 || mapArray[10] == 5){  // Target 0 or 4
                        mapArray[9] = 0;    // Next Node is 0
                    }
                }
                else if(mapArray[8] == 4){    // Junction 6 (Virtual)
                    if(mapArray[10] == 2 || mapArray[10] == 3){ // Target 2 or 3
                        mapArray[9] = 3;    // Next Node is 3
                    } else if(mapArray[10] == 1 || mapArray[10] == 7){  // Target 7 or 1
                        mapArray[9] = 7;    // Next Node is 1
                    } else if(mapArray[10] == 0 || mapArray[10] == 5){  // Target 0 or 4
                        mapArray[9] = 5;    // Next Node is 4
                    } else if(mapArray[10] == 6){   // Target 5
                        mapArray[9] = 6;    // Next Node is 5
                    }
                }
                else if(mapArray[8] == 7){    // Junction 1
                    if(mapArray[10] == 3 || mapArray[10] == 5 || mapArray[10] == 6){    // Target 3 or 4 or 6
                        mapArray[9] = 4;    // Next Node is 6
                    }
                    else if(mapArray[10] == 0 || mapArray[10] == 2){  // Target 0 or 2
                        mapArray[9] = 1;    // Next Node is 7
                    }
                    else if(mapArray[10] == 6){
                        mapArray[9] = 4;
                        mapArray[11] = 2;
                    }
                }
                else if(mapArray[8] == 5 && mapArray[10] == 0){    // Node 4 -> 0 (Special Case)
                    mapArray[9] = 0;    // Next Node is 0
                }
                else if(mapArray[8] == 0 && mapArray[10] == 5){    // Node 0 -> 4 (Special Case)
                    mapArray[9] = 5;    // Next Node is 4
                }
                else if(mapArray[8] == 5 && mapArray[10] == 6){    // Node 4 -> 5 (Special Case)
                    mapArray[9] = 4;    // Next Node is 6
                }
                else if(mapArray[8] == 3 && mapArray[10] == 2){    // Node 3 -> 2 (Optimisation)
                    mapArray[9] = 2;    // Next Node is 2
                }
                else {    // Normal Execution
                    if(mapArray[11] == 0){    // Counter Clockwise
                        mapArray[9] = mapArray[8] + 1;
                    } else {                  // Clockwise
                        mapArray[9] = mapArray[8] - 1;
                    }
                }
                Serial.print("Next Node: ");
                Serial.println(mapArray[mapArray[9]]);
                return mapArray;
            }
        }

};
