/******************************************************************
cosmetics.h - Contains "blinkLED()", "jukebox()" and "display()"
******************************************************************/

#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#ifndef COSMETICS_H
#define COSMETICS_H

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define SDA_PIN 17  // Your custom SDA pin
#define SCL_PIN 18  // Your custom SCL pin

class Cosmetics{
    public:

        // LED Blink Function
        void blinkLED(int times){
            for (int i = 0; i < times; i++){
                digitalWrite(LED_BUILTIN, HIGH); // Turn the LED on
                delay(50);                      // Wait for 0.3 seconds
                digitalWrite(LED_BUILTIN, LOW);  // Turn the LED off
                delay(50);                      // Wait for 0.3 seconds
            }
        }

        // Piezo Buzzer Jukebox Function
        void jukebox(int index){
            // To be developed at INNOVATION STAGE!
        }

        // OLED Display Function - Hello World demo for now
        void display(){

        }
};

#endif // COSMETICS_H
