#include <Arduino.h>
#include <MD_Parola.h>
#include <MD_MAX72xx.h>
#include <SPI.h>
#include <WiFi.h>
#include <NTPClient.h>
#include <time.h>
#include <Font_Data.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <esp_task_wdt.h>  


// WiFi SETUP
// Replace with your network credentials
const char *ssid = "shree";
const char *password = "Leelamma@157=";

const char *ntpServer = "pool.ntp.org";
const long gmtOffset_sec = 19800;
const int daylightOffset_sec = 0;

String openWeatherMapApiKey = "bc9993ab8d9bcfc98a94eac5846ee0b4";
String Lat = "17.406644548114567";
String Lon = "78.55805746746056";

String jsonBuffer;

void WiFi_init()
{
  Serial.printf("Connecting to %s ", ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println(" CONNECTED");

  // init and get the time
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);

  DTime();
  Wupdate();

  // disconnect WiFi as it's no longer needed
  WiFi.disconnect(true);
  WiFi.mode(WIFI_OFF);
}


String httpGETRequest(const char *serverName)
{
  WiFiClient client;
  HTTPClient http;

  // Your Domain name with URL path or IP address with path
  http.begin(client, serverName);

  // Send HTTP POST request
  int httpResponseCode = http.GET();

  String payload = "{}";

  if (httpResponseCode > 0)
  {
    Serial.print("HTTP Response code: ");
    Serial.println(httpResponseCode);
    payload = http.getString();
  }
  else
  {
    Serial.print("Error code: ");
    Serial.println(httpResponseCode);
  }
  // Free resources
  http.end();

  return payload;
}

void strToJson()
{
  JsonDocument doc;
  DeserializationError error = deserializeJson(doc, jsonBuffer);

  // Test if parsing succeeds.
  if (error)
  {
    Serial.print(F("deserializeJson() failed: "));
    Serial.println(error.f_str());
    return;
  }

  sprintf(Weather, "t;%d h;%d", (uint8_t)((float)(doc["main"]["feels_like"]) - 273.15), (uint8_t)(doc["main"]["humidity"]));
  Serial.println(Weather);
}

void Wupdate()
{
  if (WiFi.status() == WL_CONNECTED)
  {
    configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);

    DTime();

    String serverPath = "http://api.openweathermap.org/data/2.5/weather?lat=" + Lat + "&lon=" + Lon + "&appid=" + openWeatherMapApiKey;

    jsonBuffer = httpGETRequest(serverPath.c_str());
 //   Serial.println(jsonBuffer);
  }
  else
  {
    Serial.println("WiFi Disconnected - Reconnecting to WiFi");

    Serial.printf("Connecting to %s ", ssid);
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED)
    {
      delay(500);
      Serial.print(".");
    }
    Serial.println(" CONNECTED");

    configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);

    DTime();

    String serverPath = "http://api.openweathermap.org/data/2.5/weather?lat=" + Lat + "&lon=" + Lon + "&appid=" + openWeatherMapApiKey;

    jsonBuffer = httpGETRequest(serverPath.c_str());

  //  Serial.println(jsonBuffer);

    WiFi.disconnect(true);
    WiFi.mode(WIFI_OFF);
  }

  strToJson();
}