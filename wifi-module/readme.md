# ESP8266 WiFi Module

## Hardware requirements

- [ESP8266 WiFi Module](https://www.amazon.ca/dp/B00O34AGSU)
- Arduino Uno board (used as programmer)

## Setup development environment

1. Follow the instructions in the [esp8266/Arduino](https://github.com/esp8266/Arduino) repository to install the libraries using Arduino's Board Manager.
1. Select the serial port
1. Select **Generic ESP8266 Module** for the board

## Define variables

Copy the `secrets.example.h` file into `secrets.h` and define the following variables:

1. `SSID` and `PASSWORD` with the WiFi network name and password
1. `URL` with the URL where the data will be sent to
1. `API_USERNAME` and `API_PASSWORD` with the credentials to use to authenticate to `URL`
1. `CA_CERTIFICATE` with the root CA certificate for `URL`, which can be obtained by running this command:

   ```bash
   openssl s_client -connect <REPLACE_WITH_URL>:443 -showcerts
   ```

## Upload code to ESP8266 module

1. Connect the ESP8266 module to the Arduino board:
   ![](../schematics/ESP8266-Programming-Mode.png)
1. Compile and upload the code
1. Once the IDE tries to connect to the module to upload, press the button to connect RST to GND for 1 second in order for the upload to proceed
