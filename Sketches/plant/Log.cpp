#include "Log.h"
#include "TempHu.h"

Log::Log(String dateTime, TempHu tempHu, TempHu targetTempHu)
{
    this->dateTime = dateTime;
    this->tempHu = tempHu;
    this->targetTempHu = targetTempHu;
}
