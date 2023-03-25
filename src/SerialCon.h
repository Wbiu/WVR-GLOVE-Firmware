#include <Arduino.h>
#include "Properties.h"

class SerialCon {
public:
    SerialCon(){}
    bool connect()
    {   
        Serial.begin(BAUNDRATE);
        Serial.setRxBufferSize(SERIAL_SIZE_RX);
        return true;
    }
    void write(String strg)
    {
        Serial.println(strg);
    }
};