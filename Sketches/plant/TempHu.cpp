#include "TempHu.h"

TempHu::TempHu()
{
}

TempHu::TempHu(float temperature, float humidity, float soilMoisture)
{
    this->temperature = temperature;
    this->humidity = humidity;
    this->soilMoisture = soilMoisture;
}
