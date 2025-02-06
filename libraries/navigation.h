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
            const int spectrumValues[] = {2,   1,    3,    8,     16,   24,  31,  30,  15};
            const float slowCoeffs[]   = {0.15, 0.02, 0.9, -0.02, -0.15, -0.9, 666, 666, 666};

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
            if(mapArray[8] == mapArray[10]){  // CurrentNode == TargetNode, Pone Server
                Serial.println("Arrived! Fetching next node...");
                int destNode = online.destinationFetch(mapArray[mapArray[8]]); // Next Target

                for(int i=0; i<8; i++){
                  if(mapArray[i] == destNode){  // Search array for destination node
                    mapArray[10] = i;     // TargetNode assigned destNode index
                    Serial.println("Destination at index: ");
                    Serial.println(i);
                  }
                }

                if(mapArray[8] == 7){     // If at Node 1
                  if(mapArray[10] == 3 || mapArray[10] == 5 || mapArray[10] == 6){   // Target Node is 3 or 4 or 5
                    mapArray[9] = 4;      // Next Node is 6
                    Serial.println("NextNode index is: ");
                    Serial.println(mapArray[9]);
                    return mapArray;
                  }

                  else if(mapArray[10] == 0 || mapArray[10] == 2){    // Target Node is 0 or 2
                    mapArray[9] = 1;      // Next Node is 7
                    Serial.println("NextNode index is: ");
                    Serial.println(mapArray[9]);
                    return mapArray;
                  }
                }

                else if(mapArray[8] == 0 && mapArray[10] == 5){    // If at Node 0 & Target is Node 4 (Optimisation Case)
                  mapArray[9] = 5;    // Next Node is 4
                  Serial.println("NextNode index is: ");
                  Serial.println(mapArray[9]);
                  return mapArray;
                }

                else if(mapArray[8] == 5 && mapArray[10] == 0){    // If at Node 4 & Target is Node 0 (Special Case)
                  mapArray[9] = 0;    // Next Node is 0
                  Serial.println("NextNode index is: ");
                  Serial.println(mapArray[9]);
                  return mapArray;
                }

                else if(mapArray[8] == 5 && mapArray[10] == 6){    // If at Node 4 & Target is Node 5 (Special Case)
                  mapArray[9] = 4;    // Next Node is 6
                  Serial.println("NextNode index is: ");
                  Serial.println(mapArray[9]);
                  return mapArray;
                }

                else if(mapArray[8] == 5 && mapArray[10] == 7){    // If at Node 4 & Target is Node 1 (Special Case)
                  mapArray[9] = 4;    // Next Node is 6
                  Serial.println("NextNode index is: ");
                  Serial.println(mapArray[9]);
                  return mapArray;
                }

                else if(mapArray[8] == 3 && mapArray[10] == 2){     // If at Node 3 & Target is Node 2 (Optimisation Case)
                  mapArray[9] = 2;    // Next Node is 2
                  Serial.println("NextNode index is: ");
                  Serial.println(mapArray[9]);
                  return mapArray;
                }

                else{   // Starting at all other Nodes
                  if(mapArray[11] == 0){    // Counter Clockwise
                    mapArray[9] = mapArray[8] + 1;    // NextNode index is LastNode incremented
                    Serial.println("NextNode index is: ");
                    Serial.println(mapArray[9]);
                    return mapArray;
                  }

                  else{     // Clockwise
                    mapArray[9] = mapArray[8] - 1;    // NextNode index is LastNode decremented
                    Serial.println("NextNode index is: ");
                    Serial.println(mapArray[9]);
                    return mapArray;
                  }
                }
              }

              else{   // CurrentNode == NextNode
                if(mapArray[8] == 1){   // Scary Node 7 (Junction)
                  if(mapArray[10] == 2 || mapArray[10] == 3){   // Target is Node 2 or 3
                    mapArray[9] = 2;
                    Serial.println("NextNode index is: ");
                    Serial.println(mapArray[9]);
                    return mapArray;
                  }

                  else if(mapArray[10] == 7 || mapArray[10] == 4 || mapArray[10] == 6){   // Target is Node 1 or 6 or 5
                    mapArray[9] = 7;
                    Serial.println("NextNode index is: ");
                    Serial.println(mapArray[9]);
                    return mapArray;
                  }

                  else if(mapArray[10] == 0 || mapArray[10] == 5){    // Target is Node 0 or 4
                    mapArray[9] = 0;
                    Serial.println("NextNode index is: ");
                    Serial.println(mapArray[9]);
                    return mapArray;
                  }
                }

                else if(mapArray[8] == 4){    // Node 6 (Junction)
                  if(mapArray[10] == 2 || mapArray[10] == 3){   // Target is Node 2 or 3
                    mapArray[9] = 3;
                    Serial.println("NextNode index is: ");
                    Serial.println(mapArray[9]);
                    return mapArray;
                  }

                  else if(mapArray[10] == 1 || mapArray[10] == 7){    // Target is Node 1 or 7
                    mapArray[9] = 7;
                    Serial.println("NextNode index is: ");
                    Serial.println(mapArray[9]);
                    return mapArray;
                  }

                  else if(mapArray[10] == 0 || mapArray[10] == 5){    // Target is Node 0 or 4
                    mapArray[9] = 5;
                    Serial.println("NextNode index is: ");
                    Serial.println(mapArray[9]);
                    return mapArray;
                  }

                  else if(mapArray[10] == 6){    // Target is Node 5
                    mapArray[9] = 6;
                    Serial.println("NextNode index is: ");
                    Serial.println(mapArray[9]);
                    return mapArray;
                  }
                }

                else if(mapArray[8] == 7){    // Node 1 (Junction)
                  if(mapArray[10] == 3 || mapArray[10] == 5 || mapArray[10] == 6){   // Target Node is 3 or 4 or 5
                    mapArray[9] = 4;
                    Serial.println("NextNode index is: ");
                    Serial.println(mapArray[9]);
                    return mapArray;
                  }

                  else if(mapArray[10] == 0 || mapArray[10] == 2){    // Target Node is 0 or 2
                    mapArray[9] = 1;
                    Serial.println("NextNode index is: ");
                    Serial.println(mapArray[9]);
                    return mapArray;
                  }
                }

                else if(mapArray[8] == 5 && mapArray[10] == 0){    // If at Node 4 & Target is Node 0 (Special Case)
                  mapArray[9] = 0;    // Next Node is 0
                  Serial.println("NextNode index is: ");
                  Serial.println(mapArray[9]);
                  return mapArray;
                }

                else if(mapArray[8] == 0 && mapArray[10] == 5){    // If at Node 0 & Target is Node 4 (Special Case)
                  mapArray[9] = 5;    // Next Node is 4
                  Serial.println("NextNode index is: ");
                  Serial.println(mapArray[9]);
                  return mapArray;
                }

                else if(mapArray[8] == 5 && mapArray[10] == 6){    // If at Node 4 & Target is Node 5 (Special Case)
                  mapArray[9] = 4;    // Next Node is 6
                  Serial.println("NextNode index is: ");
                  Serial.println(mapArray[9]);
                  return mapArray;
                }

                else if(mapArray[8] == 3 && mapArray[10] == 2){     // If at Node 3 & Target is Node 2 (Optimisation Case)
                  mapArray[9] = 2;    // Next Node is 2
                  Serial.println("NextNode index is: ");
                  Serial.println(mapArray[9]);
                  return mapArray;
                }

                else{    // Not A Junction or Special Case - Normal Execution
                  if(mapArray[11] == 0){    // Counter Clockwise
                    mapArray[9] = mapArray[8] + 1;    // NextNode index is LastNode incremented
                    Serial.println("NextNode index is: ");
                    Serial.println(mapArray[9]);
                    return mapArray;
                  }

                  else{     // Clockwise
                    mapArray[9] = mapArray[8] - 1;    // NextNode index is LastNode decremented
                    Serial.println("NextNode index is: ");
                    Serial.println(mapArray[9]);
                    return mapArray;
                  }
                }
              }
        }
};

#endif // NAVIGATION_H
