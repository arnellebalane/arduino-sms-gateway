#include <SoftwareSerial.h>

#define TX 2  // Connect to RX of ESP8266
#define RX 3  // Connect to TX of ESP8266

const int InputPin = 8;

SoftwareSerial esp8266(RX, TX);

void setup() {
  Serial.begin(115200);
  esp8266.begin(115200);
}

void loop() {
  // TODO: This message should be coming from the GSM module
  String message = Serial.readStringUntil('\n');
  message.trim();

  if (message.length() > 0) {
    String payload = "{\"message\":\"" + message + "\"}";
    esp8266.println(message);
  }
}
