#include "SPIFFS_Helper.h"

bool SPIFFS_Helper::begin()
{   
    bool b = SPIFFS.begin(true);
    if(b)
    {   
        readFile( getFile(FILENAME), spiffs_fileContent);
    }
    return b;
}

File SPIFFS_Helper::getFile(const char *filename)
{
    return SPIFFS.open(filename);
}

void SPIFFS_Helper::readFile(File file,char* data)
{
    int i = 0;
    while (file.available())
    {
        data[i++] = file.read();
    }
    data[i] = '\0'; // Null-terminated / mark end of line  
}

bool SPIFFS_Helper::fileExists()
{
    return SPIFFS.exists(FILENAME);
}

void SPIFFS_Helper::createDefaultSettings()
{   
    File f = SPIFFS.open(FILENAME,"r");
    if(!f){return;}
    f.print(defaultSettings);
    f.close();
}

void SPIFFS_Helper::parse_mac_addr(const char* macAddr, uint8_t* destAddr)
{
    int count = 0;
    char* token = strtok((char*)macAddr, ":");
    while (token != NULL)
    {
        destAddr[count++] = strtol(token, 0, 16);
        token = strtok(NULL, ":");
    }
}