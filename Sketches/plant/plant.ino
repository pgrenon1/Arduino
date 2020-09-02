#include <NTPClient.h>
#include <WiFiUdp.h>

#include "FirebaseESP8266.h"
#include <ESP8266WiFi.h>

#define FIREBASE_HOST "plant-eed41.firebaseio.com"
#define FIREBASE_AUTH "DnrFaWiCaJreAB5JeLR7YbiqEJ5KKCwKr0kJEbaa"
#define WIFI_SSID "Poil"
#define WIFI_PASSWORD "poildepoche"

FirebaseData firebaseData;

const long utcOffsetInSeconds = -14400;
char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", utcOffsetInSeconds);
String formattedDate;

void setup()
{
  pinMode(5, OUTPUT);
  digitalWrite(5, HIGH);

  Serial.begin(115200);

  setup_wifi();

  setup_firebase();

  setup_time();

  //5. Try to set int data to Firebase
  //The set function returns bool for the status of operation
  //firebaseData requires for sending the data
  if (Firebase.setInt(firebaseData, "/LED_Status", 1))
  {
    //Success
    Serial.println("Set int data success");

  } else {
    //Failed?, get the error reason from firebaseData

    Serial.print("Error in setInt, ");
    Serial.println(firebaseData.errorReason());
  }


  //6. Try to get int data from Firebase
  //The get function returns bool for the status of operation
  //firebaseData requires for receiving the data
  if (Firebase.getInt(firebaseData, "/LED_Status"))
  {
    //Success
    Serial.print("Get int data success, int = ");
    Serial.println(firebaseData.intData());

  } else {
    //Failed?, get the error reason from firebaseData

    Serial.print("Error in getInt, ");
    Serial.println(firebaseData.errorReason());
  }

  /*

    In case where you want to set other data types i.e. bool, float, double and String, you can use setBool, setFloat, setDouble and setString.
    If you want to get data which you known its type at specific node, use getInt, getBool, getFloat, getDouble, getString.
    If you don't know the data type at specific node, use get and check its type.

    The following shows how to get the variant data

  */

  if (Firebase.get(firebaseData, "/LED_Status"))
  {
    //Success
    Serial.print("Get variant data success, type = ");
    Serial.println(firebaseData.dataType());

    if (firebaseData.dataType() == "int") {
      Serial.print("data = ");
      Serial.println(firebaseData.intData());
    } else if (firebaseData.dataType() == "bool") {
      if (firebaseData.boolData())
        Serial.println("data = true");
      else
        Serial.println("data = false");
    }

  } else {
    //Failed?, get the error reason from firebaseData

    Serial.print("Error in get, ");
    Serial.println(firebaseData.errorReason());
  }

  /*

    If you want to get the data in realtime instead of using get, see the stream examples.
    If you want to work with JSON, see the FirebaseJson, jsonObject and jsonArray examples.

    If you have questions or found the bugs, feel free to open the issue here https://github.com/mobizt/Firebase-ESP8266

  */




}

void setup_firebase() {
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);

  // Enable auto reconnect the WiFi when connection lost
  Firebase.reconnectWiFi(true);
}

void setup_wifi() {
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

void setup_time(){
  timeClient.begin();
}


void loop()
{
  timeClient.update();
  Serial.println(timeClient.getFullFormattedTime());

  delay(1000);
//  int sensorValue = analogRead(A0);
//
//  if (Firebase.setInt(firebaseData, "/sensor_value", sensorValue))
//  {
//    //Success
//    Serial.println("Set int data success");
//
//  } else {
//    //Failed?, get the error reason from firebaseData
//
//    Serial.print("Error in setInt, ");
//    Serial.println(firebaseData.errorReason());
//  }
//
//  delay(3000);
}
//
//class Log {
//    int temperature;
//    int humidity;
//
//}
