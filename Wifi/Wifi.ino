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
    http.addHeader("Content-Type", "application/x-www-form-urlencoded");
    http.addHeader("Content-Length", "10");
    
    // Content Body
    String httpRequestData = "position=0";

    // Receive & Print Response
    int httpResponseData = http.POST(httpRequestData);

    String payload = "{}";
    payload = http.getString();

    Serial.print("HTTP Response code: ");
    Serial.println(payload);

    // End
    http.end();
  }
}
