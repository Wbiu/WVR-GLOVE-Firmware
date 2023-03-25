#include <Arduino.h>
#include <WiFi.h>
#include <esp_wifi.h>
#include <esp_now.h>

#define ESP_NOW_CH 1


typedef struct esp_now_msg {
  char msg[50];
} esp_now_msg;

esp_now_msg esp_now_data;

//uint8_t broadcastAddress[] = {0x44, 0x17, 0x93, 0xE3, 0x8E, 0xCC};

uint8_t broadcastAddress[] = {0xC0, 0x49, 0xEF, 0xE4, 0xC1, 0x0C};
esp_now_peer_info_t  peerInfo;

String success;

String test = "";

void onRecvData(const uint8_t *mac_addr, const uint8_t *data, int data_len)
{
    memcpy(&esp_now_data, data, sizeof(esp_now_data));
    Serial.println(esp_now_data.msg);
}   

void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  Serial.println("send");
}
 
void serialOnReceive()
{
  test = Serial.readString().toInt();
  esp_now_send(broadcastAddress,(uint8_t *)&test,sizeof(test));
}
void setup()
{
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);

  //Serial.onReceive(serialOnReceive,true);
  // Init ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  // Once ESPNow is successfully Init, we will register for Send CB to
  // get the status of Trasnmitted packet
  esp_now_register_send_cb(OnDataSent);
  
  // Register peer
  for (int i = 0; i < 6; ++i ) {
    peerInfo.peer_addr[i] = 255;
  } 
  //memcpy(peerInfo.peer_addr, broadcastAddress, 6);
  peerInfo.channel = 0;  
  peerInfo.encrypt = false;
  

  // Register for a callback function that will be called when data is received
  esp_now_register_recv_cb(onRecvData);

    wifi_init_config_t my_config = WIFI_INIT_CONFIG_DEFAULT();
  my_config.ampdu_tx_enable = 0;
   esp_wifi_init(&my_config);
  esp_wifi_start();
  esp_wifi_set_channel(0, WIFI_SECOND_CHAN_NONE);
  esp_wifi_config_espnow_rate(WIFI_IF_STA,WIFI_PHY_RATE_LORA_500K);
  esp_now_init();

  // Add peer        
  if (esp_now_add_peer(&peerInfo) != ESP_OK){
    Serial.println("Failed to add peer");
    return;
  }

}

void loop()
{
   test = Serial.readString().toInt();
  esp_now_send(broadcastAddress,(uint8_t *)&test,sizeof(test));
 // delay(1500);
}