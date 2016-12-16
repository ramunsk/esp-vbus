#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>


#include <NTPClient.h>
#include "config.h"


#ifndef WIFI_SSID
#error "[WIFI_SSID] is not #define'd"
#endif

#ifndef WIFI_PASSWORD
#error "[WIFI_PASSWORD] is not #define'd"
#endif

#ifndef NTP_SERVER_NAME
#define NTP_SERVER_NAME "lt.pool.ntp.org"
#endif





NTPClient* ntp;

void setup(){
    Serial.begin(115200);
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }

    ntp = new NTPClient(NTP_SERVER_NAME);
};

void loop(){
    Serial.print("Time is: ");
    Serial.println(ntp->getTime());

    delay(5000);
};
