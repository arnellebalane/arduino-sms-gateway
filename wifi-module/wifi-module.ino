#include <Base64.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClientSecureBearSSL.h>

#define SSID ""
#define PASSWORD ""

#define URL ""
#define FINGERPRINT ""

const String API_USERNAME = "";
const String API_PASSWORD = "";

void setup() {
    Serial.begin(115200);
    ensureNetworkConnection();
}

void loop() {
    String message = Serial.readStringUntil('\n');
    message.trim();

    if (message.length() > 0) {
        Serial.println(">>> " + message);

        ensureNetworkConnection();
        WiFiClientSecure client;
        client.setFingerprint(FINGERPRINT);

        HTTPClient http;
        http.begin(client, URL);
        http.addHeader("Authorization", getAuthorizationHeader());

        String payload = "{\"input\":\"" + message + "\"}";
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

void ensureNetworkConnection() {
    if (WiFi.status() != WL_CONNECTED) {
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
}

String getAuthorizationHeader() {
    return "Basic " + base64::encode(API_USERNAME + ":" + API_PASSWORD);
}
