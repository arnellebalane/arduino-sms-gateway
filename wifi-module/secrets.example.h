// Wifi network SSID and password
#define SSID ""
#define PASSWORD ""

// URL where messages will be sent to
#define URL ""

// Credentials to authorize requests to URL
const String API_USERNAME = "";
const String API_PASSWORD = "";

// Root CA certificate for the URL's SSL certificate
// https://www.dzombak.com/blog/2021/10/HTTPS-Requests-with-a-Small-Set-of-Root-Certificates-on-ESP8266-Arduino.html
const char CA_CERTIFICATE [] PROGMEM = R"CERT(
-----BEGIN CERTIFICATE-----
-----END CERTIFICATE-----
)CERT";
