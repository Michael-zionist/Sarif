/******************************************************************
cosmetics.h - Contains "blinkLED()", "jukebox()" and "display()"
******************************************************************/

#ifndef COSMETICS_H
#define COSMETICS_H

class cosmetics{
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

        // OLED Display Function
        void display(){
            // To be developed at INNOVATION STAGE!
        }
};

#endif // COSMETICS_H
