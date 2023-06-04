#include <SoftwareSerial.h>

#define TX 2
#define RX 3

const int InputPin = 8;

SoftwareSerial esp8266(RX, TX);

void setup() {
  Serial.begin(115200);
  esp8266.begin(115200);
}

void loop() {
  if (digitalRead(InputPin) == HIGH) {
    esp8266.println("send request");
    delay(1000);
  }
}
