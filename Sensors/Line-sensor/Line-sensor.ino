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
        Serial.print(AnalogValue[i]); // Print the actual analog reading

        // Decide to add 0 or 1 based on threshold
        if (AnalogValue[i] >= whiteThreshold) {
            BinaryArray[i] = 1;
        } else {
            BinaryArray[i] = 0;
        }
        Serial.print(AnalogValue[i]); // Print the actual analog reading
        Serial.print(AnalogValue[i]); // Print the actual analog reading
    }

    int spectrum = 0; // variable to store computed decimal value
    for (int i = 0; i < 5; i++)
    {
        spectrum = (spectrum << 1) | binaryArray[i];
    }
    Serial.print(spectrum); // Print the actual analog reading
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
    const int spectrumValues[] = {1,  2,  3,   8,   16,  24};   // Spectrum values
    const int degreeValues[]   = {25, 20, 45, -20, -25, -45};   // Corresponding degree values

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
    int whiteThreshold = 512; // Example threshold value
    int spectrum = readSensors(whiteThreshold); // Get spectrum from sensors
    
    // Print the sensor readings and calculated spectrum
    Serial.print("Spectrum: ");
    Serial.println(spectrum);

    int degrees = directionController(spectrum); // Get degrees based on spectrum

    // Print the corresponding degrees for the spectrum
    Serial.print("Degrees: ");
    Serial.println(degrees);

    delay(500); // Delay for 500ms to prevent spamming the Serial Monitor
}
