#include <ESP8266WiFi.h>
#include <WiFiUdp.h>

#include "NTPClient.h"

#define NTP_PACKET_SIZE 48
#define NTP_LOCAL_PORT 2390

NTPClient::NTPClient(const char* serverName): _serverName(serverName){
    _udp.begin(NTP_LOCAL_PORT);
};

unsigned long NTPClient::getTime(){

    WiFi.hostByName(_serverName, _ip);
    sendNTPRequest();
    delay(1000);

    if (!_udp.parsePacket()){
        return 0;
    }

    return parseNTPResponse();
};

void NTPClient::sendNTPRequest(){
  //Serial.println("sending NTP packet...");
  unsigned char packet[NTP_PACKET_SIZE];
  // set all bytes in the buffer to 0
  memset(packet, 0, NTP_PACKET_SIZE);
  // Initialize values needed to form NTP request
  // (see URL above for details on the packets)
  packet[0] = 0b11100011;   // LI, Version, Mode
  packet[1] = 0;     // Stratum, or type of clock
  packet[2] = 6;     // Polling Interval
  packet[3] = 0xEC;  // Peer Clock Precision
  // 8 bytes of zero for Root Delay & Root Dispersion
  packet[12]  = 49;
  packet[13]  = 0x4E;
  packet[14]  = 49;
  packet[15]  = 52;

  // all NTP fields have been given values, now
  // you can send a packet requesting a timestamp:
  _udp.beginPacket(_ip, 123); //NTP requests are to port 123
  _udp.write(packet, NTP_PACKET_SIZE);
  _udp.endPacket();
}

unsigned long NTPClient::parseNTPResponse(){
    unsigned char packet[NTP_PACKET_SIZE];

    _udp.read(packet, NTP_PACKET_SIZE);

    unsigned long highWord = word(packet[40], packet[41]);
    unsigned long lowWord = word(packet[42], packet[43]);

    unsigned long retval = highWord << 16 | lowWord;
    // subtract seconds between 1900/01/01 and 1970/01/01
    retval -= 2208988800ul;

    return retval;
};
