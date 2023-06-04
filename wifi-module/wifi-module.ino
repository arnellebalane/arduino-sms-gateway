#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClientSecureBearSSL.h>

#define SSID ""
#define PASSWORD ""

#define URL ""
#define FINGERPRINT ""

void setup() {
    Serial.begin(115200);

    Serial.print("Connecting to network");
    WiFi.begin(SSID, PASSWORD);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println();
    Serial.println("Connected to network");

    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
}

void loop() {
    String line = Serial.readStringUntil('\n');
    if (line.length() > 0) {
        Serial.println(">>> " + line);

        WiFiClientSecure client;
        client.setFingerprint(FINGERPRINT);

        HTTPClient http;
        http.begin(client, URL);

        String payload = "{\"input\":\"" + line + "\"}";
        int httpCode = http.POST(payload);

        if (httpCode == HTTP_CODE_OK) {
            String response = http.getString();
            Serial.println("<<< " + response);
        } else {
            Serial.println("Request failed: " + http.errorToString(httpCode));
        }

        http.end();
    }
}
