/******************************************************************
motors.h - Contains "driveDistance()", "drive()", "turnForward()" and "rotate()"
******************************************************************/
#include "constants.h"
#include "cosmetics.h"

#ifndef MOTORS_H
#define MOTORS_H

class Motors{
    Cosmetics cosmetics;

    public:
        // Function to drive the robot forward/backward a given distance in cm
        void driveDistance(int speed, int distance, const float slowingCoeff = 0.92) {
            bool direction = speed < 0; // Determine direction from speed
            digitalWrite(mRphasePin, direction); // Set direction for RIGHT motor
            digitalWrite(mLphasePin, direction); // Set direction for LEFT motor

            analogWrite(mRpwmPin, abs(speed)); // Set speed for RIGHT motor
            analogWrite(mLpwmPin, abs(speed) * slowingCoeff); // Set speed for LEFT motor

            delay(abs(round((distance * 9000 / speed)))); // Simulate distance covered (adjust factor as needed)

            analogWrite(mRpwmPin, 0); // Stop RIGHT motor
            analogWrite(mLpwmPin, 0); // Stop LEFT motor
        }

        // Function to drive the robot forward/backward until stopped, sampled every 50ms
        void drive(int speed, int step, bool stop, const float slowingCoeff = 0.92) {
            bool direction = speed < 0; // Determine direction from speed
            digitalWrite(mRphasePin, direction); // Set direction for RIGHT motor
            digitalWrite(mLphasePin, direction); // Set direction for LEFT motor

            analogWrite(mRpwmPin, abs(speed)); // Set speed for RIGHT motor
            analogWrite(mLpwmPin, abs(speed) * slowingCoeff); // Set speed for LEFT motor

            int coveredDistance = 0;
            if (!stop) {
                delay(step); // Simulate distance increments;
            } else {
                analogWrite(mRpwmPin, 0); // Stop RIGHT motor
                analogWrite(mLpwmPin, 0); // Stop LEFT motor
            }
        }

        // Function to turn the robot forward on the outer wheel
        void turnForward(int speed, int deg, const float slowingCoeff = 0.92) {
            bool clockwise = deg > 0; // Determine direction of rotation from degrees
            if (clockwise) {
                analogWrite(mRpwmPin, 0);         // Stop RIGHT motor
                analogWrite(mLpwmPin, abs(speed) * slowingCoeff);    // Set speed for LEFT motor
            } else {
                analogWrite(mLpwmPin, 0);        // Stop LEFT motor
                analogWrite(mRpwmPin, abs(speed));    // Set speed for RIGHT motor
            }

            delay(round(abs(deg) * 2500 / speed)); // Simulate turn duration (adjust factor as needed)

            analogWrite(mRpwmPin, 0); // Stop RIGHT motor
            analogWrite(mLpwmPin, 0); // Stop LEFT motor
        }

        // Function to turn the robot on both wheels with a speed differential
        void slideForward(int speed, float turnCoeff, const float slowingCoeff = 0.92)
        {
            Serial.print("Sliding ");
            Serial.println(turnCoeff);
            bool clockwise = turnCoeff > 0; // Determine direction of rotation from degrees
            if (clockwise) {
                analogWrite(mRpwmPin, round(abs(speed) * turnCoeff));     // Stop RIGHT motor
                analogWrite(mLpwmPin, round(abs(speed) * slowingCoeff));  // Set speed for LEFT motor
            } else {
                analogWrite(mLpwmPin, round(abs(speed) * slowingCoeff * turnCoeff)); // Stop LEFT motor
                analogWrite(mRpwmPin, abs(speed)); // Set speed for RIGHT motor
            }
            delay(step);

            analogWrite(mRpwmPin, 0); // Stop RIGHT motor
            analogWrite(mLpwmPin, 0); // Stop LEFT motor
        }

        // Function to rotate the robot in place on both wheels
        void rotate(int speed, int deg, const float slowingCoeff = 0.92) {
            Serial.println("Rotating 90!");
            bool clockwise = deg > 0; // Determine direction of rotation from degrees
            if (clockwise) {
                digitalWrite(mRphasePin, LOW); // RIGHT motor backward
                digitalWrite(mLphasePin, HIGH); // LEFT motor forward
            } else {
                digitalWrite(mRphasePin, HIGH); // RIGHT motor forward
                digitalWrite(mLphasePin, LOW);  // LEFT motor backward
            }

            analogWrite(mRpwmPin, abs(speed)); // Set speed for RIGHT motor
            analogWrite(mLpwmPin, abs(speed) * slowingCoeff); // Set speed for LEFT motor

            delay(round(abs(deg) * 1000 / speed)); // Simulate turn duration (adjust factor as needed)

            analogWrite(mRpwmPin, 0); // Stop RIGHT motor
            analogWrite(mLpwmPin, 0); // Stop LEFT motor
            delay(50);
        }
};

#endif // MOTORS_H
