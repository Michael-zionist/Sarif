#include <WiFi.h>
#include <HTTPClient.h>

// Wifi Info
const char* ssid = "iot";
const char* password = "unwrinkleable66abrogative";

// Target Domain
const char* server = "http://3.250.38.184:8000/api/arrived/wsdu8873";

void destinationFetch(){
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
    String httpRequestData = "position=0";

    // Post Content
    int httpResponseData = http.POST(httpRequestData);
    
    // Receive & Print Response
    String payload = "{}";
    payload = http.getString();

    Serial.print("HTTP Response code: ");
    Serial.println(httpResponseData);
    Serial.println(payload);

    // End
    http.end();

    delay(10000);
  }
}

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
  delay(500);
}

void loop() {
  destinationFetch();
}
