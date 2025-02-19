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
#define SCREEN_ADDRESS 0x3C
#define SDA_PIN 17  // Your custom SDA pin
#define SCL_PIN 18  // Your custom SCL pin


// Sarif Logo Bitmap Image (Inverted)
static const uint8_t image_data_Sarif[1024] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x06, 0x20, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 0x20, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0c, 0x40, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x40, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0xc0, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x31, 0x80, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x31, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x63, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xc6, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x8c, 0x40, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x18, 0x80, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x06, 0x31, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1c, 0x62, 0x22, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x38, 0xcc, 0x44, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x73, 0x98, 0x88, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0xc6, 0x31, 0x10, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x9c, 0x66, 0x60, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0e, 0x31, 0x8c, 0x80, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3c, 0xe3, 0x11, 0x20, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x71, 0x8e, 0x64, 0x40, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0xc7, 0x39, 0x89, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0x1c, 0x63, 0x26, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1c, 0x71, 0xcc, 0xc8, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xf1, 0xc7, 0x33, 0x30, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0xc7, 0x1c, 0xe4, 0xc0, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0f, 0x1c, 0x73, 0x93, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7c, 0xf1, 0xce, 0x66, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0xe3, 0xcf, 0x39, 0x98, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0f, 0x9e, 0x3c, 0xe6, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7c, 0x78, 0xe7, 0x98, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0xe3, 0xc7, 0x9c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x1f, 0x9f, 0x3c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x01, 0xfc, 0xf8, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x0f, 0xe7, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0xfc, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x41, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x01, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x01, 0x80, 0x00, 0x07, 0x00, 0x07, 0x07, 0x00, 0x38, 0x03, 0x81, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0xe0, 0x00, 0x0b, 0x80, 0x07, 0x01, 0x80, 0x38, 0x03, 0x80, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x3c, 0x00, 0x19, 0xc0, 0x07, 0x03, 0x00, 0x38, 0x03, 0x80, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x0f, 0x80, 0x30, 0xe0, 0x07, 0x04, 0x00, 0x38, 0x03, 0x82, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x01, 0xc0, 0x60, 0x70, 0x07, 0x0e, 0x00, 0x38, 0x03, 0x80, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0xc0, 0xc0, 0x38, 0x07, 0x03, 0x80, 0x38, 0x03, 0x80, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x03, 0x01, 0x81, 0x80, 0x1c, 0x07, 0x00, 0xe0, 0x38, 0x03, 0x80, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x01, 0x80, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x0c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00,
    0x0c, 0x0e, 0x01, 0x03, 0x03, 0x83, 0x01, 0x84, 0x00, 0x18, 0x02, 0x18, 0x18, 0x08, 0x01, 0x80,
    0x0c, 0x09, 0xc1, 0x03, 0x00, 0xc3, 0x00, 0x87, 0x00, 0x18, 0x02, 0x08, 0x08, 0x08, 0x00, 0xf0,
    0x0c, 0x08, 0x71, 0x03, 0x00, 0x43, 0x00, 0x80, 0xe0, 0x18, 0x03, 0x30, 0x08, 0x0c, 0x00, 0x1c,
    0x0c, 0x08, 0x0f, 0x03, 0x00, 0xc3, 0x01, 0x00, 0x30, 0x18, 0x02, 0x0c, 0x08, 0x08, 0x00, 0x0c,
    0x0c, 0x08, 0x03, 0x03, 0x00, 0x00, 0x80, 0x00, 0x00, 0x18, 0x02, 0x02, 0x08, 0x0c, 0x01, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

// Finishing Flag Bitmap Image (Inverted)
static const uint8_t image_data_Flag[1024] = {
    0xfe, 0x07, 0xe0, 0x3f, 0x03, 0xf8, 0x1f, 0xc0, 0xfe, 0x03, 0xf0, 0x1f, 0x81, 0xfc, 0x0f, 0xc0,
    0xfe, 0x07, 0xe0, 0x3f, 0x03, 0xf8, 0x1f, 0xc0, 0xfe, 0x03, 0xf0, 0x1f, 0x81, 0xfc, 0x0f, 0xc0,
    0xfe, 0x07, 0xe0, 0x3f, 0x03, 0xf8, 0x1f, 0xc0, 0xfe, 0x03, 0xf0, 0x1f, 0x81, 0xfc, 0x0f, 0xc0,
    0xfe, 0x07, 0xe0, 0x3f, 0x03, 0xf8, 0x1f, 0xc0, 0xfe, 0x03, 0xf0, 0x1f, 0x81, 0xfc, 0x0f, 0xc0,
    0xfe, 0x07, 0xe0, 0x3f, 0x03, 0xf8, 0x1f, 0xc0, 0xfe, 0x03, 0xf0, 0x1f, 0x81, 0xfc, 0x0f, 0xc0,
    0xfe, 0x07, 0xe0, 0x3f, 0x03, 0xf8, 0x1f, 0xc0, 0x7e, 0x03, 0xf0, 0x1f, 0x81, 0xfc, 0x0f, 0xc0,
    0xfe, 0x07, 0xe0, 0x3f, 0x03, 0xf8, 0x1f, 0xc0, 0x7e, 0x03, 0xf0, 0x1f, 0x81, 0xfc, 0x0f, 0xc0,
    0x03, 0xf8, 0x1f, 0xc0, 0xfc, 0x07, 0xe0, 0x3f, 0x81, 0xfc, 0x0f, 0xe0, 0x7e, 0x03, 0xf0, 0x3f,
    0x03, 0xf8, 0x1f, 0xc0, 0xfc, 0x07, 0xe0, 0x3f, 0x81, 0xfc, 0x0f, 0xe0, 0x7e, 0x03, 0xf0, 0x3f,
    0x03, 0xf8, 0x1f, 0xc0, 0xfc, 0x07, 0xe0, 0x3f, 0x81, 0xfc, 0x0f, 0xe0, 0x7e, 0x03, 0xf0, 0x3f,
    0x03, 0xf8, 0x1f, 0xc0, 0xfc, 0x07, 0xe0, 0x3f, 0x81, 0xfc, 0x0f, 0xe0, 0x7e, 0x03, 0xf0, 0x3f,
    0x03, 0xf8, 0x1f, 0xc0, 0xfc, 0x07, 0xe0, 0x3f, 0x81, 0xfc, 0x0f, 0xe0, 0x7e, 0x03, 0xf0, 0x3f,
    0x01, 0xf8, 0x1f, 0xc0, 0xfc, 0x07, 0xe0, 0x3f, 0x81, 0xfc, 0x0f, 0xe0, 0x7e, 0x03, 0xf0, 0x3f,
    0x03, 0xf8, 0x1f, 0xc0, 0xfc, 0x07, 0xe0, 0x3f, 0x81, 0xfc, 0x0f, 0xe0, 0x7e, 0x03, 0xf0, 0x3f,
    0xfe, 0x07, 0xe0, 0x3f, 0x03, 0xf8, 0x1f, 0xc0, 0xfe, 0x03, 0xf0, 0x1f, 0x81, 0xfc, 0x0f, 0xc0,
    0xfe, 0x07, 0xe0, 0x3f, 0x03, 0xf8, 0x1f, 0xc0, 0xfe, 0x03, 0xf0, 0x1f, 0x81, 0xfc, 0x0f, 0xc0,
    0xfe, 0x07, 0xe0, 0x3f, 0x03, 0xf8, 0x1f, 0xc0, 0xfe, 0x03, 0xf0, 0x1f, 0x81, 0xfc, 0x0f, 0xc0,
    0xfe, 0x07, 0xe0, 0x3f, 0x03, 0xf8, 0x1f, 0xc0, 0xfe, 0x03, 0xf0, 0x1f, 0x81, 0xfc, 0x0f, 0xc0,
    0xfe, 0x07, 0xe0, 0x3f, 0x03, 0xf8, 0x1f, 0xc0, 0xfe, 0x03, 0xf0, 0x1f, 0x81, 0xfc, 0x0f, 0xc0,
    0xfe, 0x07, 0xe0, 0x3f, 0x03, 0xf8, 0x1f, 0xc0, 0x7e, 0x03, 0xf0, 0x1f, 0x81, 0xfc, 0x0f, 0xc0,
    0x03, 0xf8, 0x1f, 0xc0, 0xfc, 0x07, 0xe0, 0x3f, 0x83, 0xfc, 0x0f, 0xe0, 0x7e, 0x03, 0xf0, 0x3f,
    0x01, 0xf8, 0x1f, 0xc0, 0xfc, 0x07, 0xe0, 0x3f, 0x81, 0xfc, 0x0f, 0xe0, 0x7e, 0x03, 0xf0, 0x3f,
    0x03, 0xf8, 0x1f, 0xc0, 0xfc, 0x07, 0xe0, 0x3f, 0x81, 0xfc, 0x0f, 0xe0, 0x7e, 0x03, 0xf0, 0x3f,
    0x03, 0xf8, 0x1f, 0xc0, 0xfc, 0x07, 0xe0, 0x3f, 0x81, 0xfc, 0x0f, 0xe0, 0x7e, 0x03, 0xf0, 0x3f,
    0x03, 0xf8, 0x1f, 0xc0, 0xfc, 0x07, 0xe0, 0x3f, 0x81, 0xfc, 0x0f, 0xe0, 0x7e, 0x03, 0xf0, 0x3f,
    0x01, 0xf8, 0x1f, 0xc0, 0xfc, 0x07, 0xe0, 0x3f, 0x81, 0xfc, 0x0f, 0xe0, 0x7e, 0x03, 0xf0, 0x3f,
    0x01, 0xf8, 0x1f, 0xc0, 0xfc, 0x07, 0xe0, 0x3f, 0x81, 0xfc, 0x0f, 0xe0, 0x7e, 0x03, 0xf0, 0x3f,
    0xfe, 0x07, 0xe0, 0x3f, 0x03, 0xf8, 0x1f, 0xc0, 0xfe, 0x03, 0xf0, 0x1f, 0x81, 0xfc, 0x0f, 0xc0,
    0xfe, 0x07, 0xe0, 0x3f, 0x03, 0xf8, 0x1f, 0xc0, 0x7e, 0x03, 0xf0, 0x1f, 0x81, 0xfc, 0x0f, 0xc0,
    0xfe, 0x07, 0xe0, 0x3f, 0x03, 0xf8, 0x1f, 0xc0, 0xfe, 0x03, 0xf0, 0x1f, 0x81, 0xfc, 0x0f, 0xc0,
    0xfe, 0x07, 0xe0, 0x3f, 0x03, 0xf8, 0x1f, 0xc0, 0xfe, 0x03, 0xf0, 0x1f, 0x81, 0xfc, 0x0f, 0xc0,
    0xfe, 0x07, 0xe0, 0x3f, 0x03, 0xf8, 0x1f, 0xc0, 0xfe, 0x03, 0xf0, 0x1f, 0x81, 0xfc, 0x0f, 0xc0,
    0xfe, 0x07, 0xe0, 0x3f, 0x03, 0xf8, 0x1f, 0xc0, 0xfe, 0x03, 0xf0, 0x1f, 0x81, 0xfc, 0x0f, 0xc0,
    0xfe, 0x07, 0xe0, 0x3f, 0x03, 0xf8, 0x1f, 0xc0, 0xfe, 0x03, 0xf0, 0x1f, 0x81, 0xfc, 0x0f, 0xc0,
    0x01, 0xf8, 0x1f, 0xc0, 0xfc, 0x07, 0xe0, 0x3f, 0x81, 0xfc, 0x0f, 0xe0, 0x7e, 0x03, 0xf0, 0x3f,
    0x01, 0xf8, 0x1f, 0xc0, 0xfc, 0x07, 0xe0, 0x3f, 0x81, 0xfc, 0x0f, 0xe0, 0x7e, 0x03, 0xf0, 0x3f,
    0x03, 0xf8, 0x1f, 0xc0, 0xfc, 0x07, 0xe0, 0x3f, 0x81, 0xfc, 0x0f, 0xe0, 0x7e, 0x03, 0xf0, 0x3f,
    0x03, 0xf8, 0x1f, 0xc0, 0xfc, 0x07, 0xe0, 0x3f, 0x81, 0xfc, 0x0f, 0xe0, 0x7e, 0x03, 0xf0, 0x3f,
    0x03, 0xf8, 0x1f, 0xc0, 0xfc, 0x07, 0xe0, 0x3f, 0x81, 0xfc, 0x0f, 0xe0, 0x7e, 0x03, 0xf0, 0x3f,
    0x01, 0xf8, 0x1f, 0xc0, 0xfc, 0x07, 0xe0, 0x3f, 0x81, 0xfc, 0x0f, 0xe0, 0x7e, 0x03, 0xf0, 0x3f,
    0x01, 0xf8, 0x1f, 0xc0, 0xfc, 0x07, 0xe0, 0x3f, 0x81, 0xfc, 0x0f, 0xe0, 0x7e, 0x03, 0xf0, 0x3f,
    0xfe, 0x07, 0xe0, 0x3f, 0x03, 0xf8, 0x1f, 0xc0, 0xfe, 0x03, 0xf0, 0x1f, 0x81, 0xfc, 0x0f, 0xc0,
    0xfe, 0x07, 0xe0, 0x3f, 0x03, 0xf8, 0x1f, 0xc0, 0xfe, 0x03, 0xf0, 0x1f, 0x81, 0xfc, 0x0f, 0xc0,
    0xfe, 0x07, 0xe0, 0x3f, 0x03, 0xf8, 0x1f, 0xc0, 0xfe, 0x03, 0xf0, 0x1f, 0x81, 0xfc, 0x0f, 0xc0,
    0xfe, 0x07, 0xe0, 0x3f, 0x03, 0xf8, 0x1f, 0xc0, 0xfe, 0x03, 0xf0, 0x1f, 0x81, 0xfc, 0x0f, 0xc0,
    0xfe, 0x07, 0xe0, 0x3f, 0x03, 0xf8, 0x1f, 0xc0, 0xfe, 0x03, 0xf0, 0x1f, 0x81, 0xfc, 0x0f, 0xc0,
    0xfe, 0x07, 0xe0, 0x3f, 0x03, 0xf8, 0x1f, 0xc0, 0x7e, 0x03, 0xf0, 0x1f, 0x81, 0xfc, 0x0f, 0xc0,
    0xfe, 0x07, 0xe0, 0x3f, 0x03, 0xf8, 0x1f, 0xc0, 0xfe, 0x03, 0xf0, 0x1f, 0x81, 0xfc, 0x0f, 0xc0,
    0x03, 0xf8, 0x1f, 0xc0, 0xfc, 0x07, 0xe0, 0x3f, 0x81, 0xfc, 0x0f, 0xe0, 0x7e, 0x03, 0xf0, 0x3f,
    0x03, 0xf8, 0x1f, 0xc0, 0xfc, 0x07, 0xe0, 0x3f, 0x81, 0xfc, 0x0f, 0xe0, 0x7e, 0x03, 0xf0, 0x3f,
    0x03, 0xf8, 0x1f, 0xc0, 0xfc, 0x07, 0xe0, 0x3f, 0x81, 0xfc, 0x0f, 0xe0, 0x7e, 0x03, 0xf0, 0x3f,
    0x03, 0xf8, 0x1f, 0xc0, 0xfc, 0x07, 0xe0, 0x3f, 0x81, 0xfc, 0x0f, 0xe0, 0x7e, 0x03, 0xf0, 0x3f,
    0x03, 0xf8, 0x1f, 0xc0, 0xfc, 0x07, 0xe0, 0x3f, 0x81, 0xfc, 0x0f, 0xe0, 0x7e, 0x03, 0xf0, 0x3f,
    0x03, 0xf8, 0x1f, 0xc0, 0xfc, 0x07, 0xe0, 0x3f, 0x81, 0xfc, 0x0f, 0xe0, 0x7e, 0x03, 0xf0, 0x3f,
    0xfe, 0x07, 0xe0, 0x3f, 0x03, 0xfc, 0x1f, 0xe0, 0xfe, 0x03, 0xf0, 0x1f, 0x81, 0xfc, 0x0f, 0xe0,
    0xfe, 0x07, 0xe0, 0x3f, 0x03, 0xf8, 0x1f, 0xc0, 0xfe, 0x03, 0xf0, 0x1f, 0x81, 0xfc, 0x0f, 0xe0,
    0xfe, 0x07, 0xe0, 0x3f, 0x03, 0xf8, 0x1f, 0xc0, 0xfe, 0x03, 0xf0, 0x1f, 0x81, 0xfc, 0x0f, 0xc0,
    0xfe, 0x07, 0xe0, 0x3f, 0x03, 0xf8, 0x1f, 0xc0, 0xfe, 0x03, 0xf0, 0x1f, 0x81, 0xfc, 0x0f, 0xc0,
    0xfe, 0x07, 0xe0, 0x3f, 0x03, 0xf8, 0x1f, 0xc0, 0xfe, 0x03, 0xf0, 0x1f, 0x81, 0xfc, 0x0f, 0xc0,
    0xfe, 0x07, 0xe0, 0x3f, 0x03, 0xf8, 0x1f, 0xc0, 0xfe, 0x03, 0xf0, 0x1f, 0x81, 0xfc, 0x0f, 0xc0,
    0xfe, 0x07, 0xe0, 0x3f, 0x03, 0xf8, 0x1f, 0xc0, 0xfe, 0x03, 0xf0, 0x1f, 0x81, 0xfc, 0x0f, 0xe0,
    0x03, 0xf8, 0x1f, 0xc0, 0xfc, 0x07, 0xe0, 0x3f, 0x81, 0xfc, 0x0f, 0xe0, 0x7e, 0x03, 0xf0, 0x3f,
    0x03, 0xf8, 0x1f, 0xc0, 0xfc, 0x07, 0xe0, 0x3f, 0x81, 0xfc, 0x0f, 0xe0, 0x7e, 0x03, 0xf0, 0x3f,
    0x03, 0xf8, 0x1f, 0xc0, 0xfc, 0x07, 0xe0, 0x3f, 0x81, 0xfc, 0x0f, 0xe0, 0x7e, 0x03, 0xf0, 0x3f
};

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

        // OLED Display Sarif Logo Function
        void displayLogo(){
            Wire.begin(SDA_PIN, SCL_PIN);   // Set SDA and SCL Pins
            Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);   // Declare display object

            if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {  // Check for connection
                //Serial.println(F("SSD1306 allocation failed"));
                for(;;);
            }

            // Display Sarif Logo
            display.clearDisplay();  // Clear buffer
            display.drawBitmap(0, 0, image_data_Sarif, 128, 64, 1);  // Draw Sarif Logo Bitmap
            display.display();  // Display
            delay(3000);
        }

        // OLED Generic taximeter stats display
        void displayStats(int journeyTime, float journeyFare){
            Wire.begin(SDA_PIN, SCL_PIN);   // Set SDA and SCL Pins
            Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);   // Declare display object

            if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {  // Check for connection
                //Serial.println(F("SSD1306 allocation failed"));
                for(;;);
            }

            // Display Taximeter Stats
            display.clearDisplay();            // Clear buffer
            display.setTextSize(2);            // Set text size
            display.setTextColor(WHITE);       // Set text color
            display.setCursor(5, 5);           // Define position
            display.println("");               // Newline
            display.print("Wait: ");           // Display Journey Time
            display.print(journeyTime);
            display.print("s");

            display.setCursor(5, 25);          // Redefine position
            display.println("");               // Newline
            display.print("Fare: ");           // Display Fare
            display.print(journeyFare);

            display.display();      // Display on screen
        }

        void displayNextNode(){
            Wire.begin(SDA_PIN, SCL_PIN);   // Set SDA and SCL Pins
            Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);   // Declare display object

            if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {  // Check for connection
                //Serial.println(F("SSD1306 allocation failed"));
                for(;;);
            }

            // Display Next Node
            display.clearDisplay();            // Clear buffer
            display.setTextSize(7);            // Set text size
            display.setTextColor(WHITE);       // Set text color
            display.setCursor(50, 16);         // Define Position
            display.print(mapArray[mapArray[9]]);    // Display Next Node
            display.display();
            delay(3000);
        }

        void displayJourneyEnd(float journeyFare){
            Wire.begin(SDA_PIN, SCL_PIN);   // Set SDA and SCL Pins
            Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);   // Declare display object

            if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {  // Check for connection
                //Serial.println(F("SSD1306 allocation failed"));
                for(;;);
            }

            // Display Finishing Flag
            display.clearDisplay();  // Clear buffer
            display.drawBitmap(0, 0, image_data_Flag, 128, 64, 1);  // Draw Flag Bitmap
            display.display();  // Display
            delay(3000);

            // Display Fare
            display.clearDisplay();            // Clear buffer
            display.setTextSize(4);            // Set text size
            display.setTextColor(WHITE);       // Set text color
            display.setCursor(5, 16);          // Define Position
            display.print(journeyFare);
            display.print(" Credits");
            display.display();
            delay(3000);

            // Ending Motto
            display.clearDisplay();            // Clear buffer
            display.setTextSize(4);            // Set text size
            display.setTextColor(WHITE);       // Set text color
            display.setCursor(5, 16);          // Define Position
            display.print("You Can't");
            display.setCursor(5, 25);          // Redefine position
            display.println("");               // Newline
            display.print("Kill Progress");
            display.display();
            delay(3000);
        }
};

#endif // COSMETICS_H
