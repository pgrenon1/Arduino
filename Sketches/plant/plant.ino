#include <DHT.h>
#include <DHT_U.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
#include "Log.h"
#include "TempHu.h"
#include "FirebaseESP8266.h"
#include <ESP8266WiFi.h>

// Firebase
#define FIREBASE_HOST "plant-eed41.firebaseio.com"
#define FIREBASE_AUTH "DnrFaWiCaJreAB5JeLR7YbiqEJ5KKCwKr0kJEbaa"
#define WIFI_SSID "Poil"
#define WIFI_PASSWORD "poildepoche"

FirebaseData firebaseData;
FirebaseJsonArray firebaseJsonArray;

// Firebase navigation
const String divider = "/";
const String logsPath = "logs";
int nextIndex = 0;

// Time and Date
const long utcOffsetInSeconds = -14400;
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", utcOffsetInSeconds);
String formattedDate;

// Temp and Humidity sensor
#define DHTPIN 4
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);
TempHu tempHu;

bool testLog = false;

void setup()
{
    Serial.begin(115200);

    setupDHT();

    setupWifi();

    setupFirebase();

    setupTime();

    getNextLogIndex();
}

void setupFirebase()
{
    Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);

    // Enable auto reconnect the WiFi when connection lost
    Firebase.reconnectWiFi(true);

    //Set the size of WiFi rx/tx buffers in the case where we want to work with large data.
    firebaseData.setBSSLBufferSize(1024, 1024);

    //Set the size of HTTP response buffers in the case where we want to work with large data.
    firebaseData.setResponseSize(1024);
}

void setupDHT()
{
    dht.begin();
}

void setupWifi()
{
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    Serial.print("Connecting to Wi-Fi");
    while (WiFi.status() != WL_CONNECTED)
    {
        Serial.print(".");
        delay(300);
    }
    Serial.println();
    Serial.print("Connected with IP: ");
    Serial.println(WiFi.localIP());
    Serial.println();
}

void setupTime()
{
    timeClient.begin();
}

int getNextLogIndex()
{
    nextIndex = 0;

    Serial.println("Getting last Index");
    while (Firebase.get(firebaseData, divider + logsPath + divider + nextIndex) && firebaseData.dataType() != "null")
    {
        Serial.println("PASSED");
        Serial.println("PATH: " + firebaseData.dataPath());
        Serial.println("TYPE: " + firebaseData.dataType());
        nextIndex++;
    }
    Serial.println(nextIndex);
}

void sendLog(Log log)
{
    // create json object to share with database
    FirebaseJson logJson;

    // set dateTime
    logJson.set("time", log.dateTime);

    // set current Temp and Humidity
    logJson.set("current" + divider + "temperature", log.tempHu.temperature);
    logJson.set("current" + divider + "humidity", log.tempHu.humidity);
    logJson.set("Current" + divider + "soil_moisture", log.tempHu.soilMoisture);

    // set target Temp Humidity
    logJson.set("target" + divider + "temperature", log.targetTempHu.temperature);
    logJson.set("target" + divider + "humidity", log.targetTempHu.humidity);
    logJson.set("target" + divider + "soil_moisture", log.targetTempHu.soilMoisture);

    Serial.println("SET");
    if (Firebase.set(firebaseData, logsPath + divider + nextIndex, logJson))
    {
        Serial.println("PASSED");
    }
    else
    {
        firebaseFail();
    }

    nextIndex++;
}

void firebaseFail()
{
    Serial.println("FAILED");
    Serial.println("REASON: " + firebaseData.errorReason());
    Serial.println("------------------------------------");
    Serial.println();
}

void printPretty(FirebaseJson json)
{
    String jsonStr;
    json.toString(jsonStr, true);
    Serial.println(jsonStr);
}

void loop()
{
    timeClient.update();
    formattedDate = timeClient.getFullFormattedTime();

    getTempHu();

    if (testLog)
        sendTestLog();

    delay(3000);
}

void sendTestLog()
{
    float hum = 3;
    float temp = 24;
    float soil = 19;
    float targetTemp = 20;
    float targetHum = 2;
    float targetSoil = 11;

    TempHu t1 = TempHu(temp, hum, soil);
    TempHu t2 = TempHu(targetTemp, targetHum, targetSoil);

    Log log = Log(formattedDate, t1, t2);

    sendLog(log);
}

void getTempHu()
{
    // Reading temperature or humidity takes about 250 milliseconds!
    // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
    float h = dht.readHumidity();
    float t = dht.readTemperature();

    // Check if any reads failed and exit early.
    if (isnan(h) || isnan(t))
    {
        Serial.println(F("Failed to read from DHT sensor!"));
        return;
    }

    float s = getSoilMoisture();
    // float hic = dht.computeHeatIndex(t, h, false);

    // Serial.print(F("Humidity: "));
    // Serial.print(h);
    // Serial.print(F("%  Temperature: "));
    // Serial.print(t);
    // Serial.print(F("°C "));
    // Serial.print(F("Heat index: "));
    // Serial.print(hic);
    // Serial.print(F("°C "));

    tempHu.temperature = t;
    tempHu.humidity = h;
    tempHu.soilMoisture = s;
}

void calibrateSoilMoistureSensor()
{
}

float getSoilMoisture()
{
}