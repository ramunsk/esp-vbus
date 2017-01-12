#define DEBUG

#include <dbg.h>            // DEBUG_PRINT macro definitions
#include "config.h"         // Configuration file with SSID and password


#include <Arduino.h>
#include <VBusBuffer.h>
#include <SoftwareSerial.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>

#define VBUS_RX_PIN 2
#define VBUS_TX_PIN 3

ESP8266WebServer server(80);

String webPage = "";
MDNSResponder mdns;


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

  webPage = "Hi!";

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.println("");

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  if (mdns.begin("iot01", WiFi.localIP())) {
    Serial.println("MDNS responder started");
  }

  server.on("/", [](){
    server.send(200, "text/html", webPage);
  });
  server.on("/data", [](){
    server.send(200, "text/html", "This should be data");
  });

    server.begin();
}

void loop() {
  if (vbus.available() > 0){
      vbusBuffer.push(vbus.read());
  }

  server.handleClient();
}
