/******************************************************************
server.h - Contains "wiFiConnect()" and "destinationFetch()"
******************************************************************/

#include <WiFi.h>
#include <HTTPClient.h>
#include <constants.h>

#ifndef SERVER_H
#define SERVER_H

class server{
    public:
        // WiFi Connection Function
        void wiFiConnect(){
            // Connect to Wifi
            WiFi.begin(ssid, password);
            Serial.println("Connecting...");
            while(WiFi.status() != WL_CONNECTED){   // Wait for Connection
                    delay(500);
                    Serial.print(".");
            }
            Serial.println("");
            delay(500);
        }

        // Server Communication Function
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
};

#endif // SERVER_H
