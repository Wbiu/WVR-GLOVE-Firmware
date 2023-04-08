/*
    Main file of the WVR Glove firmware.
    https://github.com/Wbiu/WVR-GLOVE-Firmware

    Main file is only use for the initialization. There for please see Dev_Controll.h and Dev_Controll.cpp
*/
#include "Dev_Controll.h"  

/*Toggle Debuging,these will only show up in serial COM port*/
#define DEBUG  
#ifdef  DEBUG
#define PRINT_DEBUG(x) Serial.println(x)
#else
#define PRINT_DEBUG(x) 
#endif

/*if greater then 0 initialization was not successful */
int devStatus = DEVICE_STATUS_RDY;

void setup()
{
    devStatus = initDev();
    
#ifdef DEBUG
    if (devStatus == DEVICE_STATUS_ERR)
    {
        while (1)
        {
            PRINT_DEBUG("initialization error");
            delay(2000);
        }
    }
    else
    {
        PRINT_DEBUG("initialize successful");
        initTasks();
    }
#elif
    if (devStatus = !DEVICE_STATUS_RDY)
    {
        while (1)
        {
            /*spin*/
            PRINT_DEBUG("initialization error");
            delay(2000);
        }
    }
    initTasks();
#endif
}
/*
Discarding task loop to create a controllable function.
See Dev_Controll.initTasks
*/
void loop()
{
    vTaskDelete(NULL);
}