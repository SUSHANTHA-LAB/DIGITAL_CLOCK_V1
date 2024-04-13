#include <Arduino.h>
#include <WiFi.h>
#include <NTPClient.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <..\src\app\Disp.h>
#include <time.h>





void WiFi_init();

String httpGETRequest(const char *serverName);


void strToJson();

void Weather_update(char *Wdata);
