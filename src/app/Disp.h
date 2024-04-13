
#include <MD_Parola.h>
#include <MD_MAX72xx.h>
#include <Arduino.h>
#include <MD_Parola.h>
#include <MD_MAX72xx.h>
#include <SPI.h>
#include <time.h>
#include <..\src\app\FontData.h>


void Display_Init(); 
void Disp_Time();
void Disp_Date();
void Disp_weather(char* Weather);
void Disp_brightness(); 