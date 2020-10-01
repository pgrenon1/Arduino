#ifndef MY_TEMPHU_H
#define MY_TEMPHU_H

class TempHu
{
public:
    TempHu();
    TempHu(float temperature, float humidity, float soilMoisture);
    float temperature;
    float humidity;
    float soilMoisture;
};

#endif
