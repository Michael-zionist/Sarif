/* This script takes the destination node from the server
and adjusts the lastNode, nextNode and targetNode variables
accordingly. */

#include <WiFi.h>
#include <HTTPClient.h>

// WiFi Info
const char* ssid = "iot";
const char* password = "athyria67hyperflexibleness";

// Target Domain
const char* server = "http://3.250.38.184:8000/api/arrived/wsdu8873";

// Map Array
int mapArray[12] = {0, 7, 2, 3, 6, 4, 5, 1, 0, 0, 0, 0};

// WiFi Setup Function
void wiFiConnect(){
  // WiFi setup
  Serial.begin(115200);

  // Connect to Wifi
  WiFi.begin(ssid, password);
  //Serial.println("Connecting...");
  while(WiFi.status() != WL_CONNECTED){
    delay(500);
    //Serial.print(".");
  }
  //Serial.println("");
  delay(500);
}

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

    //Serial.print("HTTP Response code: ");
    //Serial.println(httpResponseData);
    //Serial.println(payload);

    // Convert to Integer
    int currentPos = payload.toInt();

    // End
    http.end();

    return currentPos;
  }

  else{
    //Serial.print("Error - WiFi Disconnected...");   // If no connection, print error
  }
}

// Function for updating Map Array (GPS)
int* GPS(int mapArray[]){
  if(mapArray[8] == mapArray[10]){  // CurrentNode == TargetNode, Pone Server
    //Serial.println("Arrived! Fetching next node...");
    int destNode = destinationFetch(mapArray[mapArray[8]]); // Next Target

    for(int i=0; i<8; i++){
      if(mapArray[i] == destNode){  // Search array for destination node
        mapArray[10] = i;     // TargetNode assigned destNode index
        //Serial.println("Destination at index: ");
        //Serial.println(i);
      }
    }

    if(mapArray[8] == 7){     // If at Node 1
      if(mapArray[10] == 3 || mapArray[10] == 5 || mapArray[10] == 6){   // Target Node is 3 or 4 or 5
        mapArray[9] = 4;      // Next Node is 6
        //Serial.println("NextNode index is: ");
        //Serial.println(mapArray[9]);
        return mapArray;
      }

      else if(mapArray[10] == 0 || mapArray[10] == 2){    // Target Node is 0 or 2
        mapArray[9] = 1;      // Next Node is 7
        //Serial.println("NextNode index is: ");
        //Serial.println(mapArray[9]);
        return mapArray;
      }
    }

    else if(mapArray[8] == 0 && mapArray[10] == 5){    // If at Node 0 & Target is Node 4 (Optimisation Case)
      mapArray[9] = 5;    // Next Node is 4
      //Serial.println("NextNode index is: ");
      //Serial.println(mapArray[9]);
      return mapArray;
    }

    else if(mapArray[8] == 5 && mapArray[10] == 0){    // If at Node 4 & Target is Node 0 (Special Case)
      mapArray[9] = 0;    // Next Node is 0
      //Serial.println("NextNode index is: ");
      //Serial.println(mapArray[9]);
      return mapArray;
    }

    else if(mapArray[8] == 5 && mapArray[10] == 6){    // If at Node 4 & Target is Node 5 (Special Case)
      mapArray[9] = 4;    // Next Node is 6
      //Serial.println("NextNode index is: ");
      //Serial.println(mapArray[9]);
      return mapArray;
    }

    else if(mapArray[8] == 5 && mapArray[10] == 7){    // If at Node 4 & Target is Node 1 (Special Case)
      mapArray[9] = 4;    // Next Node is 6
      //Serial.println("NextNode index is: ");
      //Serial.println(mapArray[9]);
      return mapArray;
    }

    else if(mapArray[8] == 3 && mapArray[10] == 2){     // If at Node 3 & Target is Node 2 (Optimisation Case)
      mapArray[9] = 2;    // Next Node is 2
      //Serial.println("NextNode index is: ");
      //Serial.println(mapArray[9]);
      return mapArray;
    }

    else{   // Starting at all other Nodes
      if(mapArray[11] == 0){    // Counter Clockwise
        mapArray[9] = mapArray[8] + 1;    // NextNode index is LastNode incremented
        //Serial.println("NextNode index is: ");
        //Serial.println(mapArray[9]);
        return mapArray;
      }

      else{     // Clockwise
        mapArray[9] = mapArray[8] - 1;    // NextNode index is LastNode decremented
        //Serial.println("NextNode index is: ");
        //Serial.println(mapArray[9]);
        return mapArray;
      }
    }
  }

  else{   // CurrentNode == NextNode
    if(mapArray[8] == 1){   // Scary Node 7 (Junction)
      if(mapArray[10] == 2 || mapArray[10] == 3){   // Target is Node 2 or 3
        mapArray[9] = 2;
        //Serial.println("NextNode index is: ");
        //Serial.println(mapArray[9]);
        return mapArray;
      }

      else if(mapArray[10] == 7 || mapArray[10] == 4 || mapArray[10] == 6){   // Target is Node 1 or 6 or 5
        mapArray[9] = 7;
        //Serial.println("NextNode index is: ");
        //Serial.println(mapArray[9]);
        return mapArray;
      }

      else if(mapArray[10] == 0 || mapArray[10] == 5){    // Target is Node 0 or 4
        mapArray[9] = 0;
        //Serial.println("NextNode index is: ");
        //Serial.println(mapArray[9]);
        return mapArray;
      }
    }

    else if(mapArray[8] == 4){    // Node 6 (Junction)
      if(mapArray[10] == 2 || mapArray[10] == 3){   // Target is Node 2 or 3
        mapArray[9] = 3;
        //Serial.println("NextNode index is: ");
        //Serial.println(mapArray[9]);
        return mapArray;
      }
      
      else if(mapArray[10] == 1 || mapArray[10] == 7){    // Target is Node 1 or 7
        mapArray[9] = 7;
        //Serial.println("NextNode index is: ");
        //Serial.println(mapArray[9]);
        return mapArray;
      }

      else if(mapArray[10] == 0 || mapArray[10] == 5){    // Target is Node 0 or 4
        mapArray[9] = 5;
        //Serial.println("NextNode index is: ");
        //Serial.println(mapArray[9]);
        return mapArray;
      }

      else if(mapArray[10] == 6){    // Target is Node 5
        mapArray[9] = 6;
        //Serial.println("NextNode index is: ");
        //Serial.println(mapArray[9]);
        return mapArray;
      }
    }

    else if(mapArray[8] == 7){    // Node 1 (Junction)
      if(mapArray[10] == 3 || mapArray[10] == 5 || mapArray[10] == 6){   // Target Node is 3 or 4 or 5
        mapArray[9] = 4;
        //Serial.println("NextNode index is: ");
        //Serial.println(mapArray[9]);
        return mapArray;
      }

      else if(mapArray[10] == 0 || mapArray[10] == 2){    // Target Node is 0 or 2
        mapArray[9] = 1;
        //Serial.println("NextNode index is: ");
        //Serial.println(mapArray[9]);
        return mapArray;
      }
    }

    else if(mapArray[8] == 5 && mapArray[10] == 0){    // If at Node 4 & Target is Node 0 (Special Case)
      mapArray[9] = 0;    // Next Node is 0
      //Serial.println("NextNode index is: ");
      //Serial.println(mapArray[9]);
      return mapArray;
    }

    else if(mapArray[8] == 0 && mapArray[10] == 5){    // If at Node 0 & Target is Node 4 (Special Case)
      mapArray[9] = 5;    // Next Node is 4
      //Serial.println("NextNode index is: ");
      //Serial.println(mapArray[9]);
      return mapArray;
    }

    else if(mapArray[8] == 5 && mapArray[10] == 6){    // If at Node 4 & Target is Node 5 (Special Case)
      mapArray[9] = 4;    // Next Node is 6
      //Serial.println("NextNode index is: ");
      //Serial.println(mapArray[9]);
      return mapArray;
    }

    else if(mapArray[8] == 3 && mapArray[10] == 2){     // If at Node 3 & Target is Node 2 (Optimisation Case)
      mapArray[9] = 2;    // Next Node is 2
      //Serial.println("NextNode index is: ");
      //Serial.println(mapArray[9]);
      return mapArray;
    }

    else{    // Not A Junction or Special Case - Normal Execution
      if(mapArray[11] == 0){    // Counter Clockwise
        mapArray[9] = mapArray[8] + 1;    // NextNode index is LastNode incremented
        //Serial.println("NextNode index is: ");
        //Serial.println(mapArray[9]);
        return mapArray;
      }

      else{     // Clockwise
        mapArray[9] = mapArray[8] - 1;    // NextNode index is LastNode decremented
        //Serial.println("NextNode index is: ");
        //Serial.println(mapArray[9]);
        return mapArray;
      }
    }
  }
}

void setup() {
  wiFiConnect();
}

// Testing garbage (IGNORE FOR NOW)
void loop() {
  delay(10000);

  GPS(mapArray);
  mapArray[8] = 1;
  delay(3000);

  GPS(mapArray);
  mapArray[8] = 7;
  delay(3000);

  GPS(mapArray);
  mapArray[8] = 4;
  mapArray[11] = 1;
  delay(3000);

  GPS(mapArray);
  mapArray[8] = 3;
  delay(3000);

  GPS(mapArray);
  mapArray[8] = 2;
  delay(3000);

  GPS(mapArray);
  mapArray[8] = 1;
  delay(3000);

  GPS(mapArray);
  mapArray[8] = 0;
  delay(3000);

  GPS(mapArray);
  mapArray[8] = 5;
  delay(3000);

  GPS(mapArray);
  mapArray[8] = 4;
  delay(3000);

  GPS(mapArray);
  mapArray[8] = 3;
  delay(3000);

  GPS(mapArray);
  mapArray[8] = 2;
  delay(3000);

  GPS(mapArray);
  mapArray[8] = 1;
  delay(3000);

  GPS(mapArray);
  mapArray[8] = 7;
  delay(3000);

  GPS(mapArray);
  mapArray[8] = 4;
  delay(3000);

  GPS(mapArray);
  mapArray[8] = 6;
  delay(3000);

  GPS(mapArray);

  delay(60000);
}
