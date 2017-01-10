#include <Arduino.h>
#include <VBusBuffer.h>
#include <PrintEx.h>

/* **********************************************************

    NTP time

************************************************************ */
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

/* **********************************************************

    Reading from Deltasol E

************************************************************ */

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

/* **********************************************************

    Parsing VBus stream

************************************************************ */

// void onFrameReceived(const uint8_t frameIndex, const uint8_t data[]);
//
//
//
// bool proceed = false;
// uint8_t data[] = {0xAA, 0x10, 0x00, 0x21, 0x77, 0x10, 0x00, 0x01, 0x11, 0x35, 0x38, 0x22, 0x38, 0x22, 0x05, 0x46, 0x0F, 0x01, 0x0F, 0x01, 0x00, 0x5F, 0x38, 0x22, 0x30, 0x01, 0x01, 0x73, 0x31, 0x7F, 0x38, 0x22, 0x06, 0x6F, 0x38, 0x22, 0x38, 0x22, 0x05, 0x46, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7F, 0x01, 0x00, 0x00, 0x00, 0x00, 0x7E, 0x64, 0x64, 0x64, 0x00, 0x00, 0x53, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7F, 0x01, 0x00, 0x00, 0x00, 0x00, 0x7E, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7F, 0x0B, 0x01, 0x09, 0x00, 0x01, 0x69, 0x01, 0x0F, 0x25, 0x21, 0x00, 0x29, 0x61, 0x07, 0x01, 0x07, 0x01, 0x0E};
// int datalength = sizeof(data);
// int pos = 0;
// VBusBuffer vbus(onFrameReceived);
// PrintEx printEx(&Serial);
//
// #define DEBUG
// #ifdef DEBUG
// #define DEBUG_PRINT(fmt, args...)    Serial.printf(fmt, ## args)
// #else
// #define DEBUG_PRINT(fmt, args...)    /* Don't do anything in release builds */
// #endif
//
// template<typename TProxy, typename TRet> TRet parseBuffer(const uint8_t* buffer, const uint8_t start, const float factor = 1.0){
//     TProxy result = 0;
//
//     for (uint8_t i = 0; i < sizeof(TProxy); i++) {
//         result |= buffer[i + start] << 8 * i;
//     }
//
//     return (TRet)(result * factor);
// }
//
// int16_t parseInt(const uint8_t* buffer, const uint8_t start){
//     return buffer[start] | buffer[start + 1] << 8;
// }
//
// float parseFloat(const uint8_t* buffer, const uint8_t start, const float factor = 0.1){
//     return parseInt(buffer, start) * factor;
// }
//
//
//
//
//
// void onFrameReceived(const uint8_t frameIndex, const uint8_t data[]){
//     //DEBUG_PRINT("DATA: %02X %02X %02X %02X\n", data[0], data[1], data[2], data[3]);
//
//     if (frameIndex == 0){
//         DEBUG_PRINT("==============================\n");
//         DEBUG_PRINT("Temperature  1:\t%s °C\n", String(parseFloat(data, 0), 1).c_str());
//         DEBUG_PRINT("Temperature  2:\t%s °C\n", String(parseFloat(data, 2), 1).c_str());
//         return;
//     }
//
//     if (frameIndex == 1){
//         DEBUG_PRINT("Temperature  3:\t%s °C\n", String(parseFloat(data, 0), 1).c_str());
//         DEBUG_PRINT("Temperature  4:\t%s °C\n", String(parseFloat(data, 2), 1).c_str());
//         return;
//     }
//
//     if (frameIndex == 2){
//         DEBUG_PRINT("Temperature  5:\t%s °C\n", String(parseFloat(data, 0), 1).c_str());
//         DEBUG_PRINT("Temperature  6:\t%s °C\n", String(parseFloat(data, 2), 1).c_str());
//         return;
//     }
//
//     if (frameIndex == 3){
//         DEBUG_PRINT("Temperature  7:\t%s °C\n", String(parseFloat(data, 0), 1).c_str());
//         DEBUG_PRINT("Temperature  8:\t%s °C\n", String(parseFloat(data, 2), 1).c_str());
//         return;
//     }
//
//     if (frameIndex == 4){
//         DEBUG_PRINT("Temperature  9:\t%s °C\n", String(parseFloat(data, 0), 1).c_str());
//         DEBUG_PRINT("Temperature 10:\t%s °C\n", String(parseFloat(data, 2), 1).c_str());
//         return;
//     }
//
//     if (frameIndex == 5){
//         DEBUG_PRINT("Irradiation CS:\t%d W/m²\n", parseInt(data, 0));
//         DEBUG_PRINT("Impulse 1 V40: \t%d\n", parseInt(data, 2));
//         return;
//     }
//
//     if (frameIndex == 6){
//         DEBUG_PRINT("Digital Input:\t%d\n", parseInt(data, 0));
//         DEBUG_PRINT("Pump speed relay 1: \t%d %\n", data[2]);
//         DEBUG_PRINT("Pump speed relay 2: \t%d %\n", data[3]);
//         return;
//     }
//
//     if (frameIndex == 7){
//         DEBUG_PRINT("Pump speed relay 3: \t%d %\n", data[0]);
//         DEBUG_PRINT("Pump speed relay 4: \t%d %\n", data[1]);
//         DEBUG_PRINT("Pump speed relay 5: \t%d %\n", data[2]);
//         DEBUG_PRINT("Pump speed relay 6: \t%d %\n", data[3]);
//         return;
//     }
//
//     if (frameIndex == 8){
//         DEBUG_PRINT("Pump speed relay 7: \t%d %\n", data[0]);
//         return;
//     }
//
//     if (frameIndex == 9){
//         DEBUG_PRINT("Error mask: \t%d\n", parseInt(data, 0));
//         DEBUG_PRINT("Messages: \t%d\n", parseInt(data, 2));
//         return;
//     }
//
//     if (frameIndex == 10){
//         DEBUG_PRINT("System: \t%d\n", data[0]);
//         DEBUG_PRINT("Scheme: \t%d\n", parseInt(data, 2));
//         return;
//     }
//
//     if (frameIndex == 11){
//         DEBUG_PRINT("Flow set HC1 module sensor 18: \t%s °C\n", String(parseFloat(data, 0), 1).c_str());
//         DEBUG_PRINT("Status HC1 module: \t%d\n", parseInt(data, 2));
//         return;
//     }
//
//     if (frameIndex == 12){
//         DEBUG_PRINT("Flow set HC2 module sensor 25: \t%s °C\n", String(parseFloat(data, 0), 1).c_str());
//         DEBUG_PRINT("Status HC2 module: \t%d\n", parseInt(data, 2));
//         return;
//     }
//
//     if (frameIndex == 13){
//         DEBUG_PRINT("Flow set HC3 module sensor 32: \t%s °C\n", String(parseFloat(data, 0), 1).c_str());
//         DEBUG_PRINT("Status HC3 module: \t%d\n", parseInt(data, 2));
//         return;
//     }
//
//     if (frameIndex == 14){
//         DEBUG_PRINT("Flow set heating circuit Sensor 11: \t%s °C\n", String(parseFloat(data, 0), 1).c_str());
//         DEBUG_PRINT("Heating circuit status: \t%d\n", parseInt(data, 2));
//         return;
//     }
//
//     if (frameIndex == 15){
//         DEBUG_PRINT("Version: \t%d.%d\n", data[0], data[1]);
//         DEBUG_PRINT("Time: \t%04X\n", parseInt(data, 2));
//         return;
//     }
//
//     if (frameIndex == 16){
//         DEBUG_PRINT("Year: \t%d\n", parseInt(data, 0));
//         DEBUG_PRINT("Month: \t%d\n", data[2]);
//         DEBUG_PRINT("Day: \t%d\n", data[3]);
//         return;
//     }
// }
//
// void setup(){
//     Serial.begin(115200);
//     while(!proceed){
//         if (Serial.available() > 0){
//             char in = Serial.read();
//             if (in == 'a'){
//                 proceed = true;
//             } else {
//                 Serial.println("Waiting for command to start...");
//                 delay(500);
//             }
//         }  else {
//             Serial.println("Waiting for command to start...");
//             delay(500);
//         }
//     }
// }
//
// void loop() {
//     for (pos = 0; pos < datalength; pos++){
//         //printEx.printHEX(data[pos]);printEx.print(" ");
//         vbus.push(data[pos]);
//         delay(10);
//     }
// }
/* **********************************************************

    Live reading Deltasol E
    parsing and
    writing to serial

************************************************************ */
#include <SoftwareSerial.h>

#define VBUS_RX_PIN 2
#define VBUS_TX_PIN 3

#define DEBUG
#ifdef DEBUG
#define DEBUG_PRINT(fmt, args...)    Serial.printf(fmt, ## args)
#else
#define DEBUG_PRINT(fmt, args...)    /* Don't do anything in release builds */
#endif

int16_t parseInt(const uint8_t* buffer, const uint8_t start){
    return buffer[start] | buffer[start + 1] << 8;
}

float parseFloat(const uint8_t* buffer, const uint8_t start, const float factor = 0.1){
    return parseInt(buffer, start) * factor;
}

void onFrameReceived(const uint8_t frameIndex, const uint8_t data[]){
    //DEBUG_PRINT("DATA: %02X %02X %02X %02X\n", data[0], data[1], data[2], data[3]);

    if (frameIndex == 0){
        DEBUG_PRINT("==============================\n");
        DEBUG_PRINT("Temperature  1:\t%s °C\n", String(parseFloat(data, 0), 1).c_str());
        DEBUG_PRINT("Temperature  2:\t%s °C\n", String(parseFloat(data, 2), 1).c_str());
        return;
    }

    if (frameIndex == 1){
        DEBUG_PRINT("Temperature  3:\t%s °C\n", String(parseFloat(data, 0), 1).c_str());
        DEBUG_PRINT("Temperature  4:\t%s °C\n", String(parseFloat(data, 2), 1).c_str());
        return;
    }

    if (frameIndex == 2){
        DEBUG_PRINT("Temperature  5:\t%s °C\n", String(parseFloat(data, 0), 1).c_str());
        DEBUG_PRINT("Temperature  6:\t%s °C\n", String(parseFloat(data, 2), 1).c_str());
        return;
    }

    if (frameIndex == 3){
        DEBUG_PRINT("Temperature  7:\t%s °C\n", String(parseFloat(data, 0), 1).c_str());
        DEBUG_PRINT("Temperature  8:\t%s °C\n", String(parseFloat(data, 2), 1).c_str());
        return;
    }

    if (frameIndex == 4){
        DEBUG_PRINT("Temperature  9:\t%s °C\n", String(parseFloat(data, 0), 1).c_str());
        DEBUG_PRINT("Temperature 10:\t%s °C\n", String(parseFloat(data, 2), 1).c_str());
        return;
    }

    if (frameIndex == 5){
        DEBUG_PRINT("Irradiation CS:\t%d W/m²\n", parseInt(data, 0));
        DEBUG_PRINT("Impulse 1 V40: \t%d\n", parseInt(data, 2));
        return;
    }

    if (frameIndex == 6){
        DEBUG_PRINT("Digital Input:\t%d\n", parseInt(data, 0));
        DEBUG_PRINT("Pump speed relay 1: \t%d %\n", data[2]);
        DEBUG_PRINT("Pump speed relay 2: \t%d %\n", data[3]);
        return;
    }

    if (frameIndex == 7){
        DEBUG_PRINT("Pump speed relay 3: \t%d %\n", data[0]);
        DEBUG_PRINT("Pump speed relay 4: \t%d %\n", data[1]);
        DEBUG_PRINT("Pump speed relay 5: \t%d %\n", data[2]);
        DEBUG_PRINT("Pump speed relay 6: \t%d %\n", data[3]);
        return;
    }

    if (frameIndex == 8){
        DEBUG_PRINT("Pump speed relay 7: \t%d %\n", data[0]);
        return;
    }

    if (frameIndex == 9){
        DEBUG_PRINT("Error mask: \t%d\n", parseInt(data, 0));
        DEBUG_PRINT("Messages: \t%d\n", parseInt(data, 2));
        return;
    }

    if (frameIndex == 10){
        DEBUG_PRINT("System: \t%d\n", data[0]);
        DEBUG_PRINT("Scheme: \t%d\n", parseInt(data, 2));
        return;
    }

    if (frameIndex == 11){
        DEBUG_PRINT("Flow set HC1 module sensor 18: \t%s °C\n", String(parseFloat(data, 0), 1).c_str());
        DEBUG_PRINT("Status HC1 module: \t%d\n", parseInt(data, 2));
        return;
    }

    if (frameIndex == 12){
        DEBUG_PRINT("Flow set HC2 module sensor 25: \t%s °C\n", String(parseFloat(data, 0), 1).c_str());
        DEBUG_PRINT("Status HC2 module: \t%d\n", parseInt(data, 2));
        return;
    }

    if (frameIndex == 13){
        DEBUG_PRINT("Flow set HC3 module sensor 32: \t%s °C\n", String(parseFloat(data, 0), 1).c_str());
        DEBUG_PRINT("Status HC3 module: \t%d\n", parseInt(data, 2));
        return;
    }

    if (frameIndex == 14){
        DEBUG_PRINT("Flow set heating circuit Sensor 11: \t%s °C\n", String(parseFloat(data, 0), 1).c_str());
        DEBUG_PRINT("Heating circuit status: \t%d\n", parseInt(data, 2));
        return;
    }

    if (frameIndex == 15){
        DEBUG_PRINT("Version: \t%d.%d\n", data[0], data[1]);
        DEBUG_PRINT("DATA: %02X %02X %02X %02X\n", data[0], data[1], data[2], data[3]);
        DEBUG_PRINT("Time: \t%02d:%02d\n", parseInt(data, 2) / 60, parseInt(data, 2) % 60);
        return;
    }

    if (frameIndex == 16){
        DEBUG_PRINT("Year: \t%d\n", parseInt(data, 0));
        DEBUG_PRINT("Month: \t%d\n", data[2]);
        DEBUG_PRINT("Day: \t%d\n", data[3]);
        return;
    }
}

SoftwareSerial vbus(VBUS_RX_PIN, VBUS_TX_PIN);
VBusBuffer vbusBuffer(onFrameReceived);

void setup() {
  Serial.begin(115200);
  vbus.begin(9600);
}

void loop() {
  if (vbus.available() > 0){
      vbusBuffer.push(vbus.read());
  }
}
