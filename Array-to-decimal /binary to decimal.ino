#include <iostream>
#include <vector>
#include <cmath>
#include <string>

int arrayToDecimal(const std::vector<double>& arr, double threshold) {
    std::string binaryValue = ""; // Creates empty string to store binary value
    
    for (double num : arr) {
        if (num >= threshold) {
            binaryValue += '1'; // Add '1' to string if reading is above threshold
        } else {
            binaryValue += '0'; // Add '0' to string if reading is below threshold
        }
    }
    
    int decimalValue = std::stoi(binaryValue, nullptr, 2); // Converts binary string to decimal

    std::cout << decimalValue << std::endl; // Prints decimal value
    return decimalValue;
}

int main() {
    std::vector<double> numbers = {0.5, 1.3, 0.7, 2.9, 3.2}; // Sample array
    double thresholdValue = 2.0; // Threshold value

    arrayToDecimal(numbers, thresholdValue); // Call the function

    return 0;
}
