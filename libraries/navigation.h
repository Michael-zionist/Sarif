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

        // Function to determine turning behavior at a junction
        void turnRobot(int currentNode, int targetNode, int lastNode, int turnSpeed, int junctionOffset = 2) {
            if (targetNode == lastNode) {
                motors.driveDistance(turnSpeed, junctionOffset);
            } else if (targetNode < lastNode) {
                motors.rotate(-turnSpeed, 90);
                motors.driveDistance(turnSpeed, junctionOffset);
            } else {
                motors.rotate(turnSpeed, 90);
                motors.driveDistance(turnSpeed, junctionOffset);
            }
        }

        // Function to traverse a junction and update position
        void crossJunction(int* mapArray, int speed, int turnDegrees = 90, int forwardDistance = 2, float coeff = 0.92) {
            int& lastNodeIndex = mapArray[8];
            int& nextNodeIndex = mapArray[9];
            int& targetNodeIndex = mapArray[10];
            int& orientation = mapArray[11];

            cosmetics.blinkLED(nextNodeIndex);

            switch (nextNodeIndex) {
                case 7:
                case 6:
                    turnRobot(nextNodeIndex, targetNodeIndex, lastNodeIndex, speed);
                    orientation = (targetNodeIndex == 1 && lastNodeIndex == 0) ? 0 : 1;
                    break;
                case 3: case 4:
                    if (targetNodeIndex == 6) {
                        motors.rotate(speed, 180);
                        motors.driveDistance(speed, 1);
                        orientation = !orientation;
                    } else {
                        motors.driveDistance(speed, forwardDistance);
                    }
                    break;
                default:
                    motors.driveDistance(speed, forwardDistance);
            }

            lastNodeIndex = nextNodeIndex;
            nextNodeIndex = (orientation == 0) ? (lastNodeIndex + 1) % 7 : (lastNodeIndex - 1 + 7) % 7;

            if (nextNodeIndex == targetNodeIndex) {
                Serial.println("Hooray");
            }
        }

        // Function for updating Map Array (GPS)
        int* GPS(int mapArray[]){
            if(mapArray[8] == mapArray[10]){  // CurrentNode == TargetNode, Pone Server
                Serial.println("Arrived! Fetching next node...");
                int destNode = online.destinationFetch(mapArray[mapArray[8]]); // Next Target
            
                for(int i = 0; i < 8; i++){
                    if(mapArray[i] == destNode){  // Search array for destination node
                        mapArray[10] = i;     // TargetNode assigned destNode index
                        Serial.print("Destination Node: ");
                        Serial.println(mapArray[mapArray[10]]);
                    }
                }
            
                if(mapArray[8] == 7){     // If at Node 1
                    if(mapArray[10] == 3 || mapArray[10] == 5 || mapArray[10] == 6){   // Target Node is 3, 4, or 5
                        mapArray[9] = 4;      // Next Node is 6
                    } else if(mapArray[10] == 0 || mapArray[10] == 2){    // Target Node is 0 or 2
                        mapArray[9] = 1;      // Next Node is 7
                    }
                }
                else if(mapArray[8] == 0 && mapArray[10] == 5){    // Optimisation Case
                    mapArray[9] = 5;
                }
                else if(mapArray[8] == 5 && mapArray[10] == 0){    // Special Case
                    mapArray[9] = 0;
                }
                else if(mapArray[8] == 5 && mapArray[10] == 6){    // Special Case
                    mapArray[9] = 4;
                }
                else if(mapArray[8] == 3 && mapArray[10] == 2){    // Optimisation Case
                    mapArray[9] = 2;
                }
                else{   // Normal Execution
                    if(mapArray[11] == 0){    // Counter Clockwise
                        mapArray[9] = mapArray[8] + 1;
                    } else {    // Clockwise
                        mapArray[9] = mapArray[8] - 1;
                    }
                }
                Serial.print("Next Node: ");
                Serial.println(mapArray[mapArray[9]]);
                return mapArray;
            }
            
            else {   // CurrentNode == NextNode
                if(mapArray[8] == 1){   // Junction
                    if(mapArray[10] == 2 || mapArray[10] == 3){
                        mapArray[9] = 2;
                    } else if(mapArray[10] == 7 || mapArray[10] == 4 || mapArray[10] == 6){
                        mapArray[9] = 7;
                    } else if(mapArray[10] == 0 || mapArray[10] == 5){
                        mapArray[9] = 0;
                    }
                }
                else if(mapArray[8] == 4){    // Junction
                    if(mapArray[10] == 2 || mapArray[10] == 3){
                        mapArray[9] = 3;
                    } else if(mapArray[10] == 1 || mapArray[10] == 7){
                        mapArray[9] = 7;
                    } else if(mapArray[10] == 0 || mapArray[10] == 5){
                        mapArray[9] = 5;
                    } else if(mapArray[10] == 6){
                        mapArray[9] = 6;
                    }
                }
                else if(mapArray[8] == 7){    // Junction
                    if(mapArray[10] == 3 || mapArray[10] == 5 || mapArray[10] == 6){
                        mapArray[9] = 4;
                    } else if(mapArray[10] == 0 || mapArray[10] == 2){
                        mapArray[9] = 1;
                    }
                }
                else if(mapArray[8] == 5 && mapArray[10] == 0){    // Special Case
                    mapArray[9] = 0;
                }
                else if(mapArray[8] == 0 && mapArray[10] == 5){    // Special Case
                    mapArray[9] = 5;
                }
                else if(mapArray[8] == 5 && mapArray[10] == 6){    // Special Case
                    mapArray[9] = 4;
                }
                else if(mapArray[8] == 3 && mapArray[10] == 2){    // Optimisation Case
                    mapArray[9] = 2;
                }
                else {    // Normal Execution
                    if(mapArray[11] == 0){    // Counter Clockwise
                        mapArray[9] = mapArray[8] + 1;
                    } else {    // Clockwise
                        mapArray[9] = mapArray[8] - 1;
                    }
                }
                Serial.print("Next Node: ");
                Serial.println(mapArray[mapArray[9]]);
                return mapArray;
            }
        }

};