
#include <Arduino.h>
#include <..\src\app\WiFi.h>
#include <..\src\app\Disp.h>

String jsonBuffer;
// WiFi SETUP
// Replace with your network credentials
 const char *ssid = "****************";
 const char *password = "******************";

 const char *ntpServer = "pool.ntp.org";
const long gmtOffset_sec = 19800;
 const int daylightOffset_sec = 0;

String openWeatherMapApiKey = "*********************";
String Lat = "******************";
 String Lon = "***************";
 String openWeatherServerPath = "http://api.openweathermap.org/data/2.5/weather?lat=" + Lat + "&lon=" + Lon + "&appid=" + openWeatherMapApiKey;



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

    Disp_Time();
  

  // disconnect WiFi 
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


void Weather_update(char *Wdata)
{
  if (WiFi.status() == WL_CONNECTED)
  {
    configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);

    Disp_Time();

    String serverPath = openWeatherServerPath;

    jsonBuffer = httpGETRequest(serverPath.c_str());

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

    Disp_Time();

    String serverPath = openWeatherServerPath;

    jsonBuffer = httpGETRequest(serverPath.c_str());

    WiFi.disconnect(true);
    WiFi.mode(WIFI_OFF);
  }

    JsonDocument doc;
  DeserializationError error = deserializeJson(doc, jsonBuffer);

  if (error)
  {
    Serial.print(F("deserializeJson() failed: "));
    Serial.println(error.f_str());
    return;
  }

  sprintf(Wdata, "t;%d h;%d", (uint8_t)((float)(doc["main"]["feels_like"]) - 273.15), (uint8_t)(doc["main"]["humidity"]));
  Serial.println(Wdata);
}
