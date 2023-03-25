#include "WirelessCon.h"

bool WirelessCon::init()
{   
    WiFi.mode(WIFI_STA);
    return esp_now_init() == ESP_OK ? 1 :0;
}

void WirelessCon::setPeerAddr(uint8_t destAddr[],uint8_t channel)
{
    memcpy(peerInfo.peer_addr,destAddr,6);
    peerInfo.channel = channel;
    peerInfo.encrypt = false;
    esp_now_add_peer(&peerInfo);
}

void WirelessCon::getMacAddr(char* macAddr)
{
    WiFi.macAddress().toCharArray(macAddr,18);
}

void WirelessCon::send(const uint8_t* data)
{   
    if(data != NULL)
    {
        esp_now_send(peerInfo.peer_addr,data,50);
    }
}


