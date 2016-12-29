#include <Arduino.h>
#include <VBusBuffer.h>
// #include <ESP8266WiFi.h>
// #include <PubSubClient.h>


//#include <NTPClient.h>
//#include "config.h"


// #ifndef WIFI_SSID
// #error "[WIFI_SSID] is not #define'd"
// #endif
//
// #ifndef WIFI_PASSWORD
// #error "[WIFI_PASSWORD] is not #define'd"
// #endif
//
// #ifndef NTP_SERVER_NAME
// #define NTP_SERVER_NAME "lt.pool.ntp.org"
// #endif
//
//
//
//
//
// NTPClient* ntp;

// void setup(){
//     Serial.begin(115200);
//     WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
//
//     while (WiFi.status() != WL_CONNECTED) {
//         delay(500);
//         Serial.print(".");
//     }
//
//     ntp = new NTPClient(NTP_SERVER_NAME);
// };
//
// void loop(){
//     Serial.print("Time is: ");
//     Serial.println(ntp->getTime());
//
//     delay(5000);
// };

// #include <SoftwareSerial.h>
//
// #define VBUS_RX_PIN 2
// #define VBUS_TX_PIN 3
//
// SoftwareSerial vbus(VBUS_RX_PIN, VBUS_TX_PIN);
//
// void setup() {
//   delay(500);
//   Serial.begin(115200);
//   while (!Serial);
//
//   vbus.begin(9600);
//   Serial.println("-------------");
//
// }
//
// void loop() {
//
//   if (vbus.available() > 0){
//     Serial.println("");
//     Serial.print(millis());Serial.print(": ");
//     while (vbus.available() > 0) {
//       uint8_t inByte = vbus.read();
//       if (inByte < 0x10){
//         Serial.print("0");
//       }
//       Serial.print(inByte, HEX);
//       Serial.print(" ");
//     }
//     Serial.println("");
//   } else {
//     Serial.print(".");
//     delay(200);
//   }
//
// }

bool proceed = false;
uint8_t data[] = {0xAA, 0x10, 0x00, 0x21, 0x77, 0x10, 0x00, 0x01, 0x11, 0x35, 0x88, 0x22, 0x38, 0x22, 0x05, 0x46, 0x2C, 0x02, 0x66, 0x02, 0x01};
int datalength = sizeof(data);
int pos = 0;
VBusBuffer vbus;

void printHEX(const uint8_t byte){
    if (byte < 16){
        Serial.print("0");
    }
    Serial.print(byte, HEX);
    Serial.print(" ");
}

void setup(){
    Serial.begin(115200);
    while(!proceed){
        if (Serial.available() > 0){
            char in = Serial.read();
            if (in == 'a'){
                proceed = true;
            } else {
                Serial.println("Waiting for command to start...");
                delay(500);
            }
        }  else {
            Serial.println("Waiting for command to start...");
            delay(500);
        }
    }

}

void loop() {
    for (pos = 0; pos < datalength; pos++){
        printHEX(data[pos]);
        vbus.push(data[pos]);
        delay(200);
    }
}
