#include  "IMU.h"
#include <WiFi.h>
#include "esp_now.h"
class WirelessCon
{
public:

    esp_now_peer_info_t peerInfo;

    typedef struct struct_message
    {
        char msg[50];
    }esp_msg;

    WirelessCon(){}
    bool init();
    void setPeerAddr(uint8_t destAddr[],uint8_t channel);
    void send(const uint8_t* data);
    void getMacAddr(char* macAddr);
    ~WirelessCon(){}

};