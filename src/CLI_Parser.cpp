#include "CLI_Parser.h"
/*
The CLI parser will be call on serial recieve call back.
*/
int parseCLI(char* cmd,char* cmd_buff[])
{
    int idx = -1; //returns -1 if cmd is not found
    int localBuffer_Idx = 0;
    
    char locabuff[10];
    char* foundCMD_buff;
    char* foundCMD[CMDSIZE]; // contains cmds in every index
    int foundCMD_Idx = 0;

    int cmdCount = 0;
    bool creatCMD = false;

    for (int i = 0; i < CLI_BUFFER_SIZE; i++)
    {
        // char tmp = cmd[i];                       // if store to tmp then value can´t be check for NULL!
        if (cmd[i] != NULL && cmd[i] != WHITESPACE) // -> So it must be directly checked
        {
            locabuff[localBuffer_Idx++] = cmd[i];
            creatCMD = true;
        }
        else
        {
            if (creatCMD)
            {
                foundCMD_buff = new char[localBuffer_Idx];
                for (int k = 0; k < localBuffer_Idx; k++)
                {
                    foundCMD_buff[k] = locabuff[k];
                    foundCMD_buff[k + 1] = '\0';
                }
                foundCMD[foundCMD_Idx++] = foundCMD_buff;
                cmdCount++;
                localBuffer_Idx = 0;
                creatCMD = false;
            }
        }
    }

    memccpy(cmd_buff,foundCMD,0,CMDSIZE);

    for (int i = 0; i < cmdCount; i++)
    {
        if (strcasecmp(foundCMD[i], "#macaddr") == 0)
        {
            idx = MACADDR_CMD;
        }
        else if (strcasecmp(foundCMD[i], "#hold") == 0)
        {
            idx = HOLD_CMD;
        }
        else if (strcasecmp(foundCMD[i], "#home") == 0)
        {
            idx = HOME_POS;
        }
        else if (strcasecmp(foundCMD[i], "#continue") == 0)
        {
            idx = CONTINUE;
        }
        else if (strcasecmp(foundCMD[i], "#test") == 0)
        {
            idx = FFB_Spin;
        }
    }
    return idx;
}

void parseFFB(char* val,volatile int* tmb_ffb ,volatile int* idx_ffb,volatile int* mdl_ffb,volatile int* rng_ffb,volatile int* pky_ffb)
{
    int count = 0;
    int fv[5];
    char *token = strtok(val, ";");
    while (token != NULL)
    {
        fv[count++] = atoi(token);
        token = strtok(NULL, ";");
    }
    *tmb_ffb = fv[0];
    *idx_ffb = fv[1];
    *mdl_ffb = fv[2];
    *rng_ffb = fv[3];
    *pky_ffb = fv[4];
}