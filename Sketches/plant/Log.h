#ifndef MY_LOG_H
#define MY_LOG_H

#include "TempHu.h"
#include "Arduino.h"

class Log
{
    public:
    // dateTime is stored 2020-09-05 09:26:21
        String dateTime = "";
        TempHu tempHu = TempHu();
        TempHu targetTempHu = TempHu();
        Log(String datetime, TempHu tempHu, TempHu targetTempHu);
};

#endif
