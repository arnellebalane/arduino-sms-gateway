#include <time.h>
#include <TZ.h>
#include <Base64.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClientSecureBearSSL.h>
#include <BearSSLHelpers.h>

#include "secrets.h"

BearSSL::WiFiClientSecure client;
BearSSL::X509List trustedRoots;

void setup() {
    Serial.begin(115200);

    trustedRoots.append(CA_CERTIFICATE);
    client.setTrustAnchors(&trustedRoots);

    ensureNetworkConnection();
}

void loop() {
    String message = Serial.readStringUntil('\n');
    message.trim();

    if (message.length() > 0) {
        Serial.println(">>> " + message);

        ensureNetworkConnection();

        HTTPClient http;
        http.begin(client, URL);
        http.addHeader("Authorization", getAuthorizationHeader());
        http.addHeader("Content-Type", "application/json");

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

        setClock();
    }
}


// Sets the system time via NTP, as required for x.509 validation
// https://github.com/esp8266/Arduino/blob/master/libraries/ESP8266WiFi/examples/BearSSL_CertStore/BearSSL_CertStore.ino
void setClock() {
    configTzTime(TZ_America_Vancouver, "pool.ntp.org", "time.nist.gov");

    Serial.print("Waiting for NTP time sync");
    time_t now = time(nullptr);
    while (now < 8 * 3600 * 2) {
        delay(250);
        Serial.print(".");
        now = time(nullptr);
    }
    Serial.println();

    struct tm timeinfo;
    gmtime_r(&now, &timeinfo);
    Serial.print("Current time (UTC): ");
    Serial.println(asctime(&timeinfo));
    localtime_r(&now, &timeinfo);
    Serial.print("Current time (Local): ");
    Serial.println(asctime(&timeinfo));
}

String getAuthorizationHeader() {
    return "Basic " + base64::encode(API_USERNAME + ":" + API_PASSWORD);
}
