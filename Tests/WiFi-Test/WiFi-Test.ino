#include "../../libraries/online.h"
#include "../../libraries/navigation.h"

void setup() {
  Online online;

  Serial.begin(115200);
  online.wiFiConnect();
}

int pos = 0;
void loop() {
  Navigation navigation;

  delay(10000);
  Serial.print("Starting position: ");
  Serial.println(pos);    // Print starting position
  navigation.GPS(mapArray);
  mapArray[8] = 1;
  delay(3000);

  navigation.GPS(mapArray);
  mapArray[8] = 7;
  delay(3000);

  navigation.GPS(mapArray);
  mapArray[8] = 4;
  delay(3000);

  navigation.GPS(mapArray);
  mapArray[11] = 1; // Orientation flips
  mapArray[8] = 3;
  delay(3000);

  navigation.GPS(mapArray);
  mapArray[8] = 2;
  delay(3000);

  navigation.GPS(mapArray);
  mapArray[8] = 1;
  delay(3000);

  navigation.GPS(mapArray);
  mapArray[8] = 0;
  delay(3000);

  navigation.GPS(mapArray);
  mapArray[8] = 5;
  delay(3000);

  navigation.GPS(mapArray);
  mapArray[8] = 4;
  delay(3000);

  navigation.GPS(mapArray);
  mapArray[8] = 3;
  delay(3000);

  navigation.GPS(mapArray);
  mapArray[8] = 2;
  delay(3000);

  navigation.GPS(mapArray);
  mapArray[8] = 1;
  delay(3000);

  navigation.GPS(mapArray);
  mapArray[11] = 0; 
  mapArray[8] = 7;
  delay(3000);

  navigation.GPS(mapArray);
  mapArray[8] = 4;
  delay(3000);

  navigation.GPS(mapArray);
  mapArray[8] = 6;
  delay(3000);

  navigation.GPS(mapArray);

  delay(60000);
}