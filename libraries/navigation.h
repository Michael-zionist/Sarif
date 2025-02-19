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
            for (int j = 0; j < arraySize; j++) {
                if (array[j] == number) {
                    return j;
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
            int step = 50; // 50 ms step for parking
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
            bool preceedsOnMain =
                (nextNodeIndex < 6 && nextNodeIndex == (lastNodeIndex - 1)) ||
                (lastNodeIndex == 0 && nextNodeIndex == 5); // array border case: nodes 0 -> 4

            // Turning & updating orientation:
            if (succeedsOnMain) {
                if (orientation == 1) {motors.rotate(speed, 180, coeff); orientation = 0;}
                else if (orientation > 1) {motors.rotate(speed, -90, coeff); orientation = 0;}
                else Serial.println("Should just drive on"); // default case -> drive forward

            } else if (preceedsOnMain) {
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

                // 50-step alignment loop (simplified line-follow loop)
                for (int i = 0; i < 50; i++) {
                    int spectrum = sensing.readSensors(whiteThreshold, AnalogPin); // Get spectrum from sensors
                    float turnCoeff = this->directionController(spectrum); // Get degrees based on spectrum
                    if (turnCoeff == 0) {  // If the robot is aligned with the line, drive forward
                        motors.drive(topSpeed/2, step, false); // Drive forward at top speed, no stop condition
                    } else {
                        motors.slideForward(topSpeed/2, turnCoeff); // Turn with top speed and the degrees value
                    }
                }

                // Begin parking
                sensing.park();
                delay(5);
                mapArray[8] = mapArray[9]; // node reached, so: lastNode = nextNode;
                GPS(mapArray);  // Communicate with server: FINISHED!
                cosmetics.blinkLED(5);
                delay(10000);
            }

            //Serial.print("Moving from ");
            //Serial.print(lastNode);
            //Serial.print(" towards ");
            //Serial.println(nextNode);
            motors.driveDistance(speed, forwardDistance, coeff); // move off juction at the end
        }

        // Function for updating Map Array (GPS - Pathfinding)
        int* GPS(int mapArray[]){
            if(mapArray[8] == mapArray[10]){  // CurrentNode == TargetNode, Pone Server
                //Serial.println("Arrived! Fetching next node...");
                int destNode = online.destinationFetch(mapArray[mapArray[8]]); // Next Target

                for(int i = 0; i < 8; i++){
                    if(mapArray[i] == destNode){  // Search array for destination node
                        mapArray[10] = i;         // TargetNode assigned destNode index
                        //Serial.print("Destination Node: ");
                        //Serial.println(mapArray[mapArray[10]]);
                    }
                }

                if(mapArray[8] == 7){     // If at Node 1
                    if (mapArray[10] == 3 || mapArray[10] == 5 || mapArray[10] == 6){   // Target Node is 3, 4, or 5
                        mapArray[9] = 4;      // Next Node is 6

                    } else if(mapArray[10] == 0 || mapArray[10] == 2){    // Target Node is 0 or 2
                        mapArray[9] = 1;      // Next Node is 7
                    }
                }
                else if(mapArray[8] == 0){  // Node 0 (Special Case)
                    if(mapArray[10] == 5){  // Target 4
                        mapArray[9] = 5;    // Next Node is 4
                    }
                    else{
                        mapArray[9] = 1;    // Next Node is 7
                    }
                }
                else if(mapArray[8] == 5){ // Node 4 (Special Case)
                    if(mapArray[10] == 6 || mapArray[10] == 3){ // Target 3 or 5
                        mapArray[9] = 4;    // Next Node is 6
                    }
                    else if(mapArray[10] == 0 || mapArray[10] == 2 || mapArray[10] == 7){ // Target 0 or 2 or 1
                        mapArray[9] = 0;    // Next Node is 0
                    }
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
                cosmetics.displayNextNode();
                delay(200);
                return mapArray;

            } else {   // CurrentNode == NextNode
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
                    if(mapArray[10] == 3 || mapArray[10] == 5 || mapArray[10] == 6){    // Target 3 or 4 or 5
                        mapArray[9] = 4;    // Next Node is 6
                    }
                    else if(mapArray[10] == 0 || mapArray[10] == 2){  // Target 0 or 2
                        mapArray[9] = 1;    // Next Node is 7
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
                cosmetics.displayNextNode();
                delay(200);
                return mapArray;
            }
        }

        // Function for make all MapArray index changes at the moment obstacle is found
        int* rerouteTarget(int mapArray[], int speed=topSpeed, int forwardDistance = 5, float coeff = 0.92){
            int& lastNodeIndex = mapArray[8];
            int& nextNodeIndex = mapArray[9];
            int& targetNodeIndex = mapArray[10];
            int& blockedNodeIndex = mapArray[12];
            int& orientation = mapArray[11];
            int& nextNode = mapArray[nextNodeIndex];
            int& lastNode = mapArray[lastNodeIndex];
            int& targetNode = mapArray[targetNodeIndex];
            int& blockedNode = mapArray[blockedNodeIndex];

            mapArray[12] = targetNodeIndex;
            
            // update target node to (virtual) Node of Access: 6/7
            if (lastNode == 6 || (nextNode < 3 && lastNode != 7) || nextNode == 7)
                targetNodeIndex = 1; // head to 7
            else
                targetNodeIndex = 4; // head to 6

            // update blocked-bridge flags:
            if ((lastNode > 1 && lastNode < 4) || (nextNode > 1 && nextNode < 4)) 
                mapArray[13] = 1; // northern blocked
            else if (lastNode == 1 || nextNode == 1) mapArray[15] = 1; // central blocked
            else mapArray[14] = 1; // southern blocked

            // update orientation:
            if (orientation < 2) {
                orientation = 1 - orientation; // flipping 0/1 (main loop)
            } else {
                orientation = 5 - orientation; // flipping 2/3 (central bridge)
            }

            // update indices: nextNode, lastNode
            int walledNodeIndex = nextNodeIndex;

            nextNodeIndex = lastNodeIndex;
            lastNodeIndex = walledNodeIndex;
            Serial.print("Next Node: ");
            Serial.println(mapArray[mapArray[9]]);
            cosmetics.displayNextNode();
            delay(200);

            // Result: navigation will resume using GPS() with server reporting once blockedNode is reached
            // targetNode is now the virtual node of access
            motors.driveDistance(speed, forwardDistance, coeff); // move off juction at the end
            delay(200);
            return mapArray;
        }

        // Updating Map Array when path is blocked: resolves nextNode, targetNode, blockedNode
        int* teleport(int mapArray[]){
            int& lastNodeIndex = mapArray[8];
            int& nextNodeIndex = mapArray[9];
            int& targetNodeIndex = mapArray[10];
            int& blockedNodeIndex = mapArray[12];
            int& orientation = mapArray[11];
            int& nextNode = mapArray[nextNodeIndex];
            int& lastNode = mapArray[lastNodeIndex];
            int& targetNode = mapArray[targetNodeIndex];
            int& blockedNode = mapArray[blockedNodeIndex];

            // Arrived? (Ie. currentNode == blockedNode?): pone server, return from teleport() to GPS() navigation
            if (lastNode == nextNode && nextNode == blockedNode) {
                cosmetics.blinkLED(3);
                Serial.println("GPS Resumed");
                blockedNodeIndex = 100; // restore normal GPS wifi navigation
                this->GPS(mapArray); // get next node from GPS
                return mapArray;
            
            }

            // Not arrived? Determine next node; targetNode is Node of Access: can be 6 or 7
            int northBridge[4] = {7, 2, 3, 6};  // default orientation: 0
            int southBridge[4] = {7, 0, 4, 6};  // default orientation: 1
            int centerBridge[3] = {7, 1, 6};    // default orientation: 2
            int* bridges[3] = {northBridge, southBridge, centerBridge};
            int bridgeFlags[3] = {mapArray[13], mapArray[14], mapArray[15]}; // north, south, center

            // Procedure: determine current bridge and find a free bridge to cross (preferring middle)
            int freeBridgeIndex;
            int currentBridgeIndex;
            int nextNodeNumber;
            int nodeBridgeID;
            for (int i = 2; i >= 0; i--) { // searches for current bridge, center as default for knots
                if (i < 2) nodeBridgeID = getIndex(bridges[i], 4, lastNode);
                else nodeBridgeID = getIndex(bridges[i], 3, lastNode);
                
                if (nodeBridgeID != -1) {currentBridgeIndex = i; break;}
            }
            for (int i = 0; i < 3; i++) {
                if (mapArray[13+i] == 0) freeBridgeIndex = i;
                if (freeBridgeIndex == currentBridgeIndex) break; // path forward is free
            }
            Serial.print("freeBridgeIndex ");
            Serial.println(freeBridgeIndex);
            Serial.print("currentBridgeIndex ");
            Serial.println(currentBridgeIndex);
            Serial.print("nodeBridgeID ");
            Serial.println(nodeBridgeID);

            // find next node:
            if (targetNode == 6) { // target is West (6)
                if (freeBridgeIndex == currentBridgeIndex && lastNode == 6) {  // target reached, head to blocked node
                    targetNodeIndex = blockedNodeIndex;
                    if (blockedNode == 0) nextNodeNumber = 4;
                    else if (blockedNode == 2) nextNodeNumber = 3;
                    else nextNodeNumber = blockedNode;

                } else if (freeBridgeIndex == currentBridgeIndex) { // path forward toward target is free
                    if (lastNode == 7) nextNodeNumber = bridges[freeBridgeIndex][1];
                    else nextNodeNumber = bridges[currentBridgeIndex][nodeBridgeID+1];
                    
                } else { // path forward toward target blocked, revert to last junction
                    nextNodeNumber = bridges[currentBridgeIndex][nodeBridgeID-1];
                }

            } else if (targetNode == 7) { // target is East (7)
                if (freeBridgeIndex == currentBridgeIndex && lastNode == 7) {  // target reached, head to blocked node
                    targetNodeIndex = blockedNodeIndex;
                    if (blockedNode == 4) nextNodeNumber = 0;
                    else if (blockedNode == 3) nextNodeNumber = 2;
                    else nextNodeNumber = blockedNode;

                } else if (freeBridgeIndex == currentBridgeIndex) { // path forward toward target is free
                    if (lastNode == 6 && freeBridgeIndex == 2) nextNodeNumber = bridges[freeBridgeIndex][1];
                    else if (lastNode == 6) nextNodeNumber = bridges[freeBridgeIndex][2];
                    else nextNodeNumber = bridges[currentBridgeIndex][nodeBridgeID-1];

                } else { // path forward toward target blocked, revert to last junction
                    nextNodeNumber = bridges[currentBridgeIndex][nodeBridgeID+1];
                }
            }

            nextNodeIndex = getIndex(mapArray, 16, nextNodeNumber);
            Serial.print("Next Node: ");
            Serial.println(mapArray[mapArray[9]]);
            cosmetics.displayNextNode();
            delay(200);
            return mapArray;
        }
};
