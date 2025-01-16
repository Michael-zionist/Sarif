#include <WiFi.h>
#include <HTTPClient.h>

// Wifi Info
const char* ssid = "iot";
const char* password = "athyria67hyperflexibleness";

// Target Domain
const char* server = "http://3.250.38.184:8000/api/arrived/3";


void setup() {
  Serial.begin(115200);

  // Connect to Wifi
  WiFi.begin(ssid, password);
  Serial.println("Connecting...");
  while(WiFi.status() != WL_CONNECTED){
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
}

void loop() {
  // Send post request
  if(WiFi.status()==WL_CONNECTED){
    WiFiClient client;
    HTTPClient http;

    http.begin(client, server);

    // Content Header
    http.addHeader("");
  }
}
