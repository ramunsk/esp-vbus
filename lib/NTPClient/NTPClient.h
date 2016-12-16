#ifndef ntpclient_h
#define ntpclient_h


#include <WiFiUdp.h>

class NTPClient {
public:
    NTPClient(const char* serverName);
    unsigned long getTime();

private:
    const char* _serverName;
    WiFiUDP _udp;
    IPAddress _ip;
    
    void sendNTPRequest();
    unsigned long parseNTPResponse();
};

#endif
