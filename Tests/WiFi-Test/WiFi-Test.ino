#include "../../libraries/online.h"

int pos = 0;
Online test;

void setup() {
  Serial.begin(115200);
  test.wiFiConnect();
}

void loop() {
  delay(10000);
  Serial.println(pos);    // Print starting position
  do{
    pos = test.destinationFetch(pos);  // Fetch destination
    delay(5000);      // Wait 5s
  }
  while(pos != 0);
}