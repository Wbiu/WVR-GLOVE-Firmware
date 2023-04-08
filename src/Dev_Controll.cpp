#include "Dev_Controll.h"
#include "..\lib\SafeString\src\SafeStringReader.h"
#include <ArduinoJson.h>
#include "Settings.h"
#define ESP_NOW_CH 1

/*BNO055*/
IMU imu_bno;

/*Connection component*/
SerialCon serialCon;
WirelessCon wirelessCon;
/*
 44:17:93:E3:8E:CC MACADDR of the Collector
 Note** ESP-NOW is used for wireless comunication! 
*/
uint8_t destAddr[6];


uint8_t esp_now_channel = 1;

static char espMsgBuff[50];

typedef struct esp_now__to_glove_msg {
  char msg[25];
} esp_now__to_glove_msg;

WirelessCon::esp_msg esp_now_send_msg;
esp_now__to_glove_msg esp_now_recv_msg;
WirelessCon::esp_msg esp_now_empty_msg;

/*SPIFFS*/
/*File access*/
SPIFFS_Helper sfs_helper;

char* spiffs_buffer = new char[FILE_BUFFER_SIZE]; // with hold the data from the settings file 

dev_conf glove_confs;

/*imu data*/
float ryp_buffer [IMU_BUFFER_SIZE_EU];
float quat_buffer [IMU_BUFFER_SIZE_QUAT];
/*cmd vars*/
char* cli_buffer;
char* cmd_buff[5]; 

/* using SafeString lib instead of Serial.read() */
createSafeString(sfReader,23);

SemaphoreHandle_t smp;

/*arrays of finger pin*/
uint8_t fingers[] = {THUMB_READING_PIN,INDEX_READING_PIN,MIDDLE_READING_PIN,RING_READING_PIN,PINKY_READING_PIN};
int fingerReadings[5];

int DEV_STAT = DEVICE_STATUS_RDY;

/*FFB ctrl values*/
volatile int tmb_ffb = 0;
volatile int idx_ffb = 0;
volatile int mdl_ffb = 0;
volatile int rng_ffb = 0;
volatile int pky_ffb = 0;

TaskHandle_t ffb_task;
TaskHandle_t* sWireless_task;
TaskHandle_t* sSerial_task;

/*Toggle Debuging,these will only show up in serial COM port*/
#define DEBUG  
#ifdef  DEBUG
#define PRINT_DEBUG(x) Serial.println(x)
#else
#define PRINT_DEBUG(x) 
#endif

bool initDev()
{
    serialSetup();
    Wire.begin();
    delay(2000);
    initSPIFFS();
    espNowSetup();
    pinSetup();
    if(glove_confs.imu_state == 1){imuSetup();}
    if(glove_confs.onboot_servo == 1){checkServo();}
    smp = xSemaphoreCreateMutex();
    return DEV_STAT;
}
void pinSetup()
{
    // configure PWM functionalitites
    ledcSetup(THUMB_PWM_CHN_INA, PWM_FREQ, PWM_RES);
    ledcSetup(THUMB_PWM_CHN_INB, PWM_FREQ, PWM_RES);
    ledcSetup(INDEX_PWM_CHN_INA, PWM_FREQ, PWM_RES);
    ledcSetup(INDEX_PWM_CHN_INB, PWM_FREQ, PWM_RES);
    ledcSetup(MIDDLE_PWM_CHN_INA, PWM_FREQ, PWM_RES);
    ledcSetup(MIDDLE_PWM_CHN_INB, PWM_FREQ, PWM_RES);
    ledcSetup(RING_PWM_CHN_INA, PWM_FREQ, PWM_RES);
    ledcSetup(RING_PWM_CHN_INB, PWM_FREQ, PWM_RES);
    ledcSetup(PINKY_PWM_CHN_INA, PWM_FREQ, PWM_RES);
    ledcSetup(PINKY_PWM_CHN_INB, PWM_FREQ, PWM_RES);

    // attach the channel to the GPIO to be controlled
    ledcAttachPin(THUMB_CTL_PIN_INA, THUMB_PWM_CHN_INA);
    ledcAttachPin(THUMB_CTL_PIN_INB, THUMB_PWM_CHN_INB);
    ledcAttachPin(INDEX_CTL_PIN_INA, INDEX_PWM_CHN_INA);
    ledcAttachPin(INDEX_CTL_PIN_INB, INDEX_PWM_CHN_INB);
    ledcAttachPin(MIDDLE_CTL_PIN_INA, MIDDLE_PWM_CHN_INA);
    ledcAttachPin(MIDDLE_CTL_PIN_INB, MIDDLE_PWM_CHN_INB);
    ledcAttachPin(RING_CTL_PIN_INA, RING_PWM_CHN_INA);
    ledcAttachPin(RING_CTL_PIN_INB, RING_PWM_CHN_INB);
    ledcAttachPin(PINKY_CTL_PIN_INA, PINKY_PWM_CHN_INA);
    ledcAttachPin(PINKY_CTL_PIN_INB, PINKY_PWM_CHN_INB);

    analogReadResolution((uint8_t)ANALOG_READ_RES);
}
void imuSetup()
{
    if (!imu_bno.init())
    {
        PRINT_DEBUG("Could not connect to BNO055");
        DEV_STAT = DEVICE_STATUS_ERR;
    }
    else
    {   
        PRINT_DEBUG("IMU found");
    }
}
bool SPIFFSSetup()
{
    if(!sfs_helper.begin())
    {
        PRINT_DEBUG("Error: Filed to init SPIFFS");
        DEV_STAT = DEVICE_STATUS_ERR;
        return false;
    }
    return true;
}
void initSPIFFS()
{
    if (SPIFFSSetup())
    {
        if (sfs_helper.fileExists())
        {
            validateSettings();
        }
        else
        {
            PRINT_DEBUG("no settings file was found!\nCreating a default setting file !");
            sfs_helper.createDefaultSettings();
        }
    }
}
void validateSettings()
{   
    /*
    retrieving data from JSON doc
    */
    PRINT_DEBUG("settings found!");
    DynamicJsonDocument doc(1024);
    deserializeJson(doc, sfs_helper.spiffs_fileContent);

    glove_confs.dev_name = doc["device_name"];
    glove_confs.onboot_servo = doc["onboot_servo"];
    glove_confs.imu_state = doc["imu_state"];

    const char *imu_data = doc["imu_data"];
    const char *conType = doc["conType"];
    const char* macAddr = doc["collector_addr"];
    
    sfs_helper.parse_mac_addr(macAddr,destAddr);

    if(strcmp(imu_data, "eu") == 0)
    {
        glove_confs.gyrodata = IMU::DATATYPE::EU;
    }
    else if (strcmp(imu_data, "quat") == 0)
    {
        glove_confs.gyrodata = IMU::DATATYPE::QUAT;
    }

    if (strcmp(conType, "wired") == 0)
    {
        glove_confs.conType = connectionType::WIRED;
    }
    else if (strcmp(conType, "wireless") == 0)
    {
        glove_confs.conType = connectionType::WIRELESS;
    }
//    delete[] sfs_helper.spiffs_fileContent;
//   sfs_helper.spiffs_fileContent = nullptr;
}
void serialSetup()
{
    serialCon.connect();
    Serial.onReceive(serialOnRecv_Cb);
}
void serialOnRecv_Cb()
{
    sfReader.read(Serial);
    if (sfReader.c_str()[0] == '#')
    {
        int cmd = parseCLI((char *)sfReader.c_str(), cmd_buff);
        switch (cmd)
        {
        case HOLD_CMD:
            Serial.printf("\n%x\0", 0x01);
            vTaskSuspend(*sSerial_task);
            break;
        case CONTINUE:
            Serial.printf("\n%x\0", 0x01);
            vTaskResume(*sSerial_task);
            break;
        case MACADDR_CMD:
            char macArrd[18];
            wirelessCon.getMacAddr(macArrd);
            Serial.println(macArrd);
            break;
        case HOME_POS:
            Serial.printf("\n%x\0", 0x01);
            for (int i = 0; i < 100; i++)
            {
                homePos();
            }
            break;
        case FFB_Spin:
            Serial.printf("\n%x\0", 0x01);
            for (int i = 0; i < 3; i++)
            {
                servoTest();
            }
            break;
        }
    }
    else if (sfReader.c_str()[0] != NULL)
    {
        xSemaphoreTake(smp, portMAX_DELAY);
        parseFFB((char *)sfReader.c_str(), &tmb_ffb, &idx_ffb, &mdl_ffb, &rng_ffb, &pky_ffb);
        xSemaphoreGive(smp);
    }
    sfReader.clear();
}
void ffb(void * parameter)
{   
    for (;;)
    {   
        xSemaphoreTake(smp, portMAX_DELAY);
        setFFB(&tmb_ffb, &idx_ffb, &mdl_ffb, &rng_ffb, &pky_ffb);
        xSemaphoreGive(smp);
    }
}
void sendWireless_with_IMU_EU()
{
    for(;;)
        {   
            readFingerPos();
            readIMU_EU();
            sprintf(espMsgBuff,"%i;%i;%i;%i;%i;%.1f;%.1f;%.1f\0",
                      map(fingerReadings[THB_READING_IDX], 0, 255, 255, 0),
                      map(fingerReadings[IDX_READING_IDX], 0, 255, 255, 0), // reversing value
                      map(fingerReadings[MDL_READING_IDX], 0, 255, 255, 0), // reversing value
                      fingerReadings[RNG_READING_IDX],
                      fingerReadings[PKY_READING_IDX],
                      ryp_buffer[0],
                      ryp_buffer[1],
                      ryp_buffer[2]);

            memcpy(&esp_now_send_msg,espMsgBuff , sizeof(espMsgBuff));
            wirelessCon.send((uint8_t*)&esp_now_send_msg);
        }
}
void sendWireless_with_IMU_QUAT()
{
            for(;;)
        {   
            readFingerPos();
            readIMU_QUAT();
            sprintf(espMsgBuff,"%i;%i;%i;%i;%i;%.3f;%.3f;%.3f;%.3f\0",
                      map(fingerReadings[THB_READING_IDX], 0, 255, 255, 0),
                      map(fingerReadings[IDX_READING_IDX], 0, 255, 255, 0), // reversing value
                      map(fingerReadings[MDL_READING_IDX], 0, 255, 255, 0), // reversing value
                      fingerReadings[RNG_READING_IDX],
                      fingerReadings[PKY_READING_IDX],
                      quat_buffer[0],
                      quat_buffer[1],
                      quat_buffer[2],
                      quat_buffer[2]);

            memcpy(&esp_now_send_msg,espMsgBuff , sizeof(espMsgBuff));
            wirelessCon.send((uint8_t*)&esp_now_send_msg);
        }
}
void sendWireless(void * parameter)
{       

        if(glove_confs.imu_state == 1)
        {
            if(glove_confs.gyrodata == IMU::EU) sendWireless_with_IMU_EU();
            if(glove_confs.gyrodata == IMU::QUAT) sendWireless_with_IMU_QUAT();
        }
        else if(glove_confs.imu_state == 0)
        {
            sendWireless_no_IMU();
        }
}
void sendWireless_no_IMU()
{
        for(;;)
        {   
            readFingerPos();
            sprintf(espMsgBuff,"%i;%i;%i;%i;%i\0",
                      map(fingerReadings[THB_READING_IDX], 0, 255, 255, 0),
                      map(fingerReadings[IDX_READING_IDX], 0, 255, 255, 0), // reversing value
                      map(fingerReadings[MDL_READING_IDX], 0, 255, 255, 0), // reversing value
                      fingerReadings[RNG_READING_IDX],
                      fingerReadings[PKY_READING_IDX]);
            memcpy(&esp_now_send_msg,espMsgBuff , sizeof(espMsgBuff));
            wirelessCon.send((uint8_t*)&esp_now_send_msg);
        }
}
void esp_now_recv_cb(const uint8_t *mac_addr, const uint8_t *data, int data_len)
{   
   memcpy(&esp_now_recv_msg, data, sizeof(esp_now_recv_msg.msg));
   /*
   if(memcmp(mac_addr,destAddr,sizeof(destAddr)) == 0)
   {
   }
   */
    if (esp_now_recv_msg.msg[0] == '-')
        {
            int cmd = parseCLI((char *)esp_now_recv_msg.msg, cmd_buff);
            switch (cmd)
            {
            case HOLD_CMD:
                vTaskSuspend(sWireless_task);
                break;
            case CONTINUE:
                vTaskResume(sWireless_task);
                break;
            case MACADDR_CMD:
                char macArrd[18];
                wirelessCon.getMacAddr(macArrd);
                Serial.println(macArrd);
                break;
            case HOME_POS:
                
                for (int i = 0; i < 100; i++)
                {
                    homePos();
                }
                break;

            case FFB_Spin:
                for (int i = 0; i < 3; i++)
                {
                    servoTest();
                }
                break;
            }
        }
        else if (esp_now_recv_msg.msg[0] != NULL)
        {
            xSemaphoreTake(smp, portMAX_DELAY);
            parseFFB((char *)esp_now_recv_msg.msg, &tmb_ffb, &idx_ffb, &mdl_ffb, &rng_ffb, &pky_ffb);
            xSemaphoreGive(smp);
        }
        memset(esp_now_recv_msg.msg,NULL,sizeof(esp_now_recv_msg.msg));
       // esp_now_recv_msg = esp_now_empty_msg;
}
void initTasks()
{        
        switch (glove_confs.conType)
        {
        case WIRED:
                sSerial_task = new TaskHandle_t;
                xTaskCreatePinnedToCore(
                sendSerial,
                "sendSerial",
                5120,
                NULL,
                1,
                sSerial_task,
                1);
            break;
        case WIRELESS:
            sWireless_task = new TaskHandle_t;
            xTaskCreatePinnedToCore(
                sendWireless,
                "sendWireless",
                5120,
                NULL,
                1,
                sWireless_task,
                1);
            break;
        }

    xTaskCreatePinnedToCore(
      ffb, 
      "ffb", 
      1024,  
      NULL, 
      1,  
      &ffb_task,
      1);
}
void readFingerPos()
{
   for (int i = 0; i < sizeof(fingers); i++)
   {
        int temp = 0;
        for (int j = 0; j < SAMPLE_SIZE; j++)
        {
            temp += analogRead(fingers[i]);
        }
        fingerReadings[i] = temp / SAMPLE_SIZE;
   }
}
void espNowSetup()
{
    if(!wirelessCon.init())
    {
        DEV_STAT == DEVICE_STATUS_ERR;
    }
    else
    {   
        esp_now_register_recv_cb(esp_now_recv_cb);
        wirelessCon.setPeerAddr(destAddr,ESP_NOW_CH);
    }

}
void sendSerial(void *parameter)
{
    if (glove_confs.imu_state == 1)
    {
        if (glove_confs.gyrodata == IMU::EU)sendSerial_with_IMU_EU();
        if (glove_confs.gyrodata == IMU::QUAT)sendSerial_with_IMU_QUAT();
    }

    if (glove_confs.imu_state == 0)
    {
        sendSerial_no_IMU();
    }

}
void sendSerial_with_IMU_EU()
{   
    for (;;)
    {   
        readFingerPos();
        readIMU_EU();
        Serial.printf("\n{%i;%i;%i;%i;%i;%.1f;%.1f;%.1f}\0",
                      map(fingerReadings[THB_READING_IDX], 0, 255, 255, 0), // reversing value
                      map(fingerReadings[IDX_READING_IDX], 0, 255, 255, 0), // reversing value
                      map(fingerReadings[MDL_READING_IDX], 0, 255, 255, 0), // reversing value
                      fingerReadings[RNG_READING_IDX],
                      fingerReadings[PKY_READING_IDX],
                      ryp_buffer[0],
                      ryp_buffer[1],
                      ryp_buffer[2]);
    }
}
void sendSerial_with_IMU_QUAT()
{
        for (;;)
    {   
        readFingerPos();
        readIMU_QUAT();
        Serial.printf("\n{%i;%i;%i;%i;%i;%.3f;%.3f;%.3f;%.3f}\0",
                      map(fingerReadings[THB_READING_IDX], 0, 255, 255, 0), // reversing value
                      map(fingerReadings[IDX_READING_IDX], 0, 255, 255, 0), // reversing value
                      map(fingerReadings[MDL_READING_IDX], 0, 255, 255, 0), // reversing value
                      fingerReadings[RNG_READING_IDX],
                      fingerReadings[PKY_READING_IDX],
                      quat_buffer[0],
                      quat_buffer[1],
                      quat_buffer[2],
                      quat_buffer[3]);
    }
}
void sendSerial_no_IMU()
{
    for (;;)
    {   
        readFingerPos();
        Serial.printf("\n{%i;%i;%i;%i;%i}\0",
                      map(fingerReadings[THB_READING_IDX], 0, 255, 255, 0),
                      map(fingerReadings[IDX_READING_IDX], 0, 255, 255, 0), // reversing value
                      map(fingerReadings[MDL_READING_IDX], 0, 255, 255, 0), // reversing value
                      fingerReadings[RNG_READING_IDX],
                      fingerReadings[PKY_READING_IDX]);
    }
}
void readIMU_EU()
{
    imu_bno.getRoll_Pitch_Yaw(ryp_buffer);
}
void readIMU_QUAT()
{
    imu_bno.getQAUT(quat_buffer);
}

