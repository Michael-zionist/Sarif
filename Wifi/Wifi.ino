#include <WiFi.h>
#include <HTTPClient.h>

// Wifi Info
const char* ssid = "iot";
const char* password = "unwrinkleable66abrogative";

// Target Domain
const char* server = "http://3.250.38.184:8000/api/arrived/wsdu8873";

// Current Position
int pos = 0;

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
  delay(10000);
  do{
    Serial.println(pos);
    pos = destinationFetch(pos);
    delay(5000);
  }
  while(pos != 0);
}