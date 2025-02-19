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
int AnalogPin[5] = {A1pin, A2pin, A3pin, A4pin, A5pin};

// ...motor calibration (native to script):
const float slowingCoeff = 0.92;  // Makes more efficient L motor slower to match R

/*
A function to convert the input of the 5 analog black/white sensors into a spectrum int value, 
depending on which sensors cross whiteTreshold, eg. [0, 0, 1, 1, 1] is a black line
on the left ...converted to a decimal “spectrum”: 7
[0, 0, 1, 1, 1] → 7
[0, 0, 1, 0, 0] → 4

Input: int whiteThreshold
Output: int spectrum
Uses: no dependency functions
*/
int readSensors(int whiteThreshold, int* AnalogPin) {
    static int BinaryArray[5]; // Static array to ensure it persists after the function returns
    int AnalogValue[5];

    for (int i = 0; i < 5; i++) {
        AnalogValue[i] = analogRead(AnalogPin[i]);

        // Decide to add 0 or 1 based on threshold
        if (AnalogValue[i] < whiteThreshold) {
            BinaryArray[i] = 1;
        } else {
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
    for (int i = 0; i < 5; i++)
    {
        spectrum = (spectrum << 1) | BinaryArray[i];
    }
    return spectrum;
}

/*
an arduino function to naively translate an int spectrum into a number of degrees of turn
Note: the conversion is done through lookup arrays: {int spectrum} -> {int degrees}

Input: int spectrum
Output: int deg (positive = clockwise, Ie. “right-turning”)
Uses: no dependency function
*/
int directionController(int spectrum) {
    // Define the spectrum-to-degrees lookup dictionary
    const int spectrumValues[] = {1,   2,   3,   8,  16, 24};   // Spectrum values
    const int degreeValues[]   = {-25, -20, -45, 20, 25, 45};   // Corresponding degree values

    const int dictionarySize = sizeof(spectrumValues) / sizeof(spectrumValues[0]);

    // Loop through the lookup dictionary to find the matching spectrum
    for (int i = 0; i < dictionarySize; i++) {
        if (spectrum == spectrumValues[i]) {
            return degreeValues[i]; // Return corresponding degree
        }
    }
    return 0; // no match is found (default case - no line)
}

void setup()
{
    Serial.begin(9600);
}

void loop()
{
    int whiteThreshold = 2700; // Example threshold value
    int spectrum = readSensors(whiteThreshold, AnalogPin); // Get spectrum from sensors
    //Serial.print("Spectrum: ");
    //Serial.println(spectrum);

    int degrees = directionController(spectrum); // Get degrees based on spectrum
    //Serial.print("Degrees: ");
    //Serial.println(degrees);

    delay(500); // Delay for 500ms to prevent spamming the Serial Monitor
}
