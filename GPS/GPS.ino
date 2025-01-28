/* This script takes the destination node from the server
and adjusts the lastNode, nextNode and targetNode variables
accordingly. */

#include <WiFi.h>
#include <HTTPClient.h>

// WiFi Info
const char* ssid = "iot";
const char* password = "unwrinkleable66abrogative";

// Target Domain
const char* server = "http://3.250.38.184:8000/api/arrived/wsdu8873";

// Map Array
int mapArray[12] = {0, 7, 2, 3, 6, 4, 5, 1, 0, 0, 0, 0};

// Function to fetch target node
int destinationFetch(int currentPos){
  // Ensure  WiFi Connected
  if(WiFi.status()==WL_CONNECTED){
    WiFiClient client;
    HTTPClient http;

    // Start creating post
    http.begin(client, server);

    // Content Header
    http.addHeader("Content-Type", "application/x-www-form-urlencoded");
    http.addHeader("Content-Length", "10");
    
    // Content Body
    String pos = String(currentPos);
    String httpRequestData = "position=" + pos;

    // Post Content
    int httpResponseData = http.POST(httpRequestData);
    
    // Receive & Print Response
    String payload = "{}";
    payload = http.getString();

    Serial.print("HTTP Response code: ");
    Serial.println(httpResponseData);
    Serial.println(payload);

    // Convert to Integer
    int currentPos = payload.toInt();

    // End
    http.end();

    return currentPos;
  }

  else{
    Serial.print("Error - WiFi Disconnected...");   // If no connection, print error
  }
}

// Function for updating Map Array (GPS)
void GPS(int mapArray[]){
  if(mapArray[8] == mapArray[10]){  // Reached Destination Node, Pone Server
    int destNode = destinationFetch(mapArray[mapArray[8]]); // Next destination
    Serial.println("Arrived! Fetching next node...");

    for(int i=0; i<8; i++){
      if(mapArray[i] == destNode){  // Search array for destination node
        mapArray[10] = i;     // TargetNode assigned destNode index
        Serial.println("Destination at index: ");
        Serial.println(i);
      }
    }

    if(mapArray[11] == 0){    // Counter Clockwise
      mapArray[9] = mapArray[8] + 1;    // NextNode index is LastNode incremented
      Serial.println("NextNode index is: ");
      Serial.println(mapArray[9]);
    }

    else{     // Clockwise
      mapArray[9] = mapArray[8]--;    // NextNode index is LastNode decremented
      Serial.println("NextNode index is: ");
      Serial.println(mapArray[9]);
    }
  }

  else{   // LastNode == NextNode
    if(mapArray[11] == 0){    // Counter Clockwise
      mapArray[9] = mapArray[8] + 1;    // NextNode index is LastNode incremented
      Serial.println("NextNode index is: ");
      Serial.println(mapArray[9]);
    }

    else{     // Clockwise
      mapArray[9] = mapArray[8]--;    // NextNode index is LastNode decremented
      Serial.println("NextNode index is: ");
      Serial.println(mapArray[9]);
    }
  }
}

void setup() {
// WiFi setup
Serial.begin(115200);

  // Connect to Wifi
  WiFi.begin(ssid, password);
  Serial.println("Connecting...");
  while(WiFi.status() != WL_CONNECTED){
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  delay(500);
}

// Testing garbage (IGNORE FOR NOW)
void loop() {
  delay(10000);
  GPS(mapArray);
  mapArray[8] = 1;
  delay(1000);
  GPS(mapArray);
  mapArray[8] = 2;
  delay(1000);
  GPS(mapArray);
  mapArray[8] = 3;
}
