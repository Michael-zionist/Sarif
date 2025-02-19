#ifndef CONSTANTS_H
#define CONSTANTS_H

// this follows the Pinout given and suggested in the Getting Started Guide
const int mRpwmPin = 37;    // RIGHT Motor PWM
const int mRphasePin = 38;  // RIGHT Motor Phase
const int mLpwmPin = 39;    // LEFT Motor PWM
const int mLphasePin = 20;  // LEFT Motor Phase
const int A1pin = 4;        // Analog pin 1 (line-follow B/W 5-sensor)
const int A2pin = 5;        // Analog pin 2 (line-follow B/W 5-sensor)
const int A3pin = 6;        // Analog pin 3 (line-follow B/W 5-sensor)
const int A4pin = 7;        // Analog pin 4 (line-follow B/W 5-sensor)
const int A5pin = 15;       // Analog pin 5 (line-follow B/W 5-sensor)
const int A16pin = 16;      // Analog pin 16 (short distance sensor)
int AnalogPin[5] = {A1pin, A2pin, A3pin, A4pin, A5pin};

// ...motor calibration (native to script):
const float slowingCoeff = 0.92;  // Makes more efficient L motor slower to match R
const int topSpeed = 180;
const int step = 60;
const int parkDistance = 2000; //distance at which is stops before wall
int whiteThreshold = 1900; // Calibrate here for light level


// Navigation array has set structure: [N, Nf, N, N, Nf, N, Np, N, I, I, I, B, O, Bl, Nb, Sb, Cb]
// N - Navigation node (Nf - fictional, Np - parking)
// I - Index (LastNodeIndex, NextNodeIndex, TargetNodeIndex)
// B - Orientation boolean (0 - counter-clockwise, 1 - clockwise)
// O - Obstacle memory (100 - Obstacle False, <8 - Target Node index -> Obstacle True)
// B1 - Blocked node index (for obstacles)
// Nb - North bridge status (0 - open, 1 - blocked)
// Cb - Central bridge status (0 - open, 1 - blocked)
// Sb - South bridge status (0 - open, 1 - blocked)
// Starting position: LastNode -> 0; NextNode -> 0; TargetNode -> 0; Orientation -> 0 (counter-clockwise)
int mapArray[16] = {0, 7, 2, 3, 6, 4, 5, 1, 0, 0, 0, 0, 100, 0, 0, 0};

// Wifi Info
//const char* ssid = "Sarif Industries";
//const char* password = "amongsus";
//const char* ssid = "Nathan's Hotspot";
//const char* password = "hello1234";

const char* ssid = "iot";
const char* password = "athyria67hyperflexibleness";   // For ESP in Mobot
//const char* password = "unwrinkleable66abrogative";    // For test ESP

// Target Domain
const char* server = "http://3.250.38.184:8000/api/arrived/wsdu8873";

#endif  // CONSTANTS_H
