#include <Arduino.h>
#include <VBusBuffer.h>
#include <PrintEx.h>
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

void onFrameReceived(const uint8_t frameIndex, const uint8_t data[]);



bool proceed = false;
uint8_t data[] = {0xAA, 0x10, 0x00, 0x21, 0x77, 0x10, 0x00, 0x01, 0x11, 0x35, 0x38, 0x22, 0x38, 0x22, 0x05, 0x46, 0x2B, 0x02, 0x67, 0x02, 0x01, 0x68, 0x38, 0x22, 0x02, 0x01, 0x01, 0x21, 0x2E, 0x00, 0x38, 0x22, 0x04, 0x73, 0x38, 0x22, 0x38, 0x22, 0x05, 0x46, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7F, 0x01, 0x00, 0x00, 0x00, 0x00, 0x7E, 0x00, 0x64, 0x00, 0x00, 0x00, 0x1B, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7F, 0x01, 0x00, 0x00, 0x00, 0x00, 0x7E, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7F, 0x0C, 0x01, 0x09, 0x00, 0x00, 0x69, 0x01, 0x0F, 0x35, 0x02, 0x04, 0x34, 0x60, 0x07, 0x0C, 0x1A, 0x01, 0x71};
int datalength = sizeof(data);
int pos = 0;
VBusBuffer vbus(onFrameReceived);
PrintEx printEx(&Serial);

uint8_t b1;
uint16_t b2;
float f;

void onFrameReceived(const uint8_t frameIndex, const uint8_t data[]){
    // if (frameIndex < 16){
    //     return;
    // }
    //
    // printEx
    //     .newln()
    //     .print("Frame received: ").print(frameIndex).print(", data:");
    //
    // for (size_t i = 0; i < 4; i++) {
    //     printEx.print(" 0x").printHEX(data[i]);
    // }
    //

    if (frameIndex >= 0 && frameIndex < 5){
        if (frameIndex == 0){
            printEx.newln().println("==================================================");
        }
        //TODO: optimize float printing
        b2 = data[0] | data[1] << 8;
        printEx.print("Temperature ").print((uint8_t)((frameIndex + 1) * 2 - 1)).print(": \t").println((float)(b2 * 0.1));
        b2 = data[2] | data[3] << 8;
        printEx.print("Temperature ").print((uint8_t)((frameIndex + 1) * 2)).print(": \t").println((float)(b2 * 0.1));
        return;
    }

    if (frameIndex == 5){
        b2 = data[0] | data[1] << 8;
        printEx.print("Irradiation CS: \t").println(b2);
        b2 = data[2] | data[3] << 8;
        printEx.print("Impulse 1 V40: \t").println(b2);
        return;
    }

    if (frameIndex == 6){
        b2 = data[0] | data[1] << 8;
        printEx.print("Digital input: \t").println(b2);
        printEx.print("Pump speed relay 1: \t").println(data[2]);
        printEx.print("Pump speed relay 2: \t").println(data[3]);
        return;
    }

    if (frameIndex == 7){
        printEx.print("Pump speed relay 3: \t").println(data[0]);
        printEx.print("Pump speed relay 4: \t").println(data[1]);
        printEx.print("Pump speed relay 5: \t").println(data[2]);
        printEx.print("Pump speed relay 6: \t").println(data[3]);
        return;
    }

    if (frameIndex == 8){
        printEx.print("Pump speed relay 7: \t").println(data[0]);
        b2 = data[1] | data[2] << 8;
        printEx.print("Error mask: \t").println(b2);
        b1 = data[3];   // this is f*&^@d up as word overlaps frames
        return;
    }

    if (frameIndex == 9){
        b2 = data[0] | b1 << 8;
        printEx.print("Messages: \t").println(b2);
        printEx.print("System: \t").println(data[1]);
        b2 = data[2] | data[3] << 8;
        printEx.print("Scheme: \t").println(b2);
        return;
    }

    if (frameIndex == 10){
        b2 = data[0] | b1 << 8;
        printEx.print("Flow set HC1 module sensor 18: \t").println((float)((float)b2 * 0.1));
        b2 = data[2] | data[3] << 8;
        printEx.print("Status HC1 module: \t").println(b2);
        return;
    }

    if (frameIndex == 11){
        b2 = data[0] | b1 << 8;
        printEx.print("Flow set HC2 module sensor 25: \t").println((float)((float)b2 * 0.1));
        b2 = data[2] | data[3] << 8;
        printEx.print("Status HC2 module: \t").println(b2);
        return;
    }

    if (frameIndex == 12){
        b2 = data[0] | b1 << 8;
        printEx.print("Flow set HC3 module sensor 32: \t").println((float)((float)b2 * 0.1));
        b2 = data[2] | data[3] << 8;
        printEx.print("Status HC3 module: \t").println(b2);
        return;
    }

    if (frameIndex == 13){
        b2 = data[0] | b1 << 8;
        printEx.print("Flow set heating circuit Sensor 11: \t").println((float)((float)b2 * 0.1));
        b2 = data[2] | data[3] << 8;
        printEx.print("Heating circuit status: \t").println(b2);
        return;
    }

    if (frameIndex == 15){
        f = data[0] + data[1] * 0.01;
        printEx.print("Version: \t").println(f);
        b2 = data[2] | data[3] << 8;
        printEx.print("Time: \t").printHEXln(b2);
        return;
    }


    if (frameIndex == 16){
        // correct
        b2 = data[0] | data[1] << 8;
        printEx.print("Year: \t").println(b2);
        printEx.print("Month: \t").println(data[2]);
        printEx.print("Day: \t").println(data[3]);
        return;
    }
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
        //printEx.printHEX(data[pos]);printEx.print(" ");
        vbus.push(data[pos]);
        delay(10);
    }
}
