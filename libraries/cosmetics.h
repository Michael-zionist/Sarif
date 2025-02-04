/******************************************************************
cosmetics.h - Contains "blinkLED()", "jukebox()" and "display()"
******************************************************************/

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#ifndef COSMETICS_H
#define COSMETICS_H

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define SDA_PIN 17  // Your custom SDA pin
#define SCL_PIN 18  // Your custom SCL pin

class cosmetics{
    public:
        // Create an I2C display object
        Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

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
            // Manually set I2C pins before initializing the display
            Wire.begin(SDA_PIN, SCL_PIN);
            
            // Scan I2C to check if the OLED is detected
            Serial.println("Scanning for I2C devices...");
            byte error, address;
            int nDevices = 0;
            for (address = 1; address < 127; address++) {
                Wire.beginTransmission(address);
                error = Wire.endTransmission();
                if (error == 0) {
                    Serial.print("I2C device found at 0x");
                    Serial.println(address, HEX);
                    nDevices++;
                }
            }
            if (nDevices == 0) {
                Serial.println("No I2C devices found. Check wiring!");
                while (1);
            }

            // Initialize the OLED display
            if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Try 0x3D if this fails
                Serial.println("SSD1306 allocation failed");
                while (1);
            }

            // Display "Hello, World!"
            display.clearDisplay();
            display.setTextSize(2);
            display.setTextColor(SSD1306_WHITE);
            display.setCursor(10, 25);
            display.println("Hello, World!");
            display.display();
        }
};

#endif // COSMETICS_H
