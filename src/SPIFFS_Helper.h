#include "SPIFFS.h"
#include "CLI_Parser.h"

class SPIFFS_Helper
{
public:
    char *spiffs_fileContent = new char[FILE_BUFFER_SIZE];
    SPIFFS_Helper() {}
    bool begin();
    File getFile(const char *filename);
    void readFile(File file, char *data);
    bool fileExists();
    void createDefaultSettings();
    void parse_mac_addr(const char* macAddr,uint8_t* destAddr);
private:
    char* defaultSettings = "{\"device_name\":\"wGlove\",\"onboot_servo\":0,\"imu_state\":1,\"imu_data\": \"eu\",\"conType\": \"wired\",\"collector_addr\":\"0x0:0x0:0x0:0x0:0x0:0x0\"}\0";
};