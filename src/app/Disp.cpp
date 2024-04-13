#include <..\src\app\Disp.h>
#include <..\src\app\WiFi.h>
#include <math.h>

//  DISPLAY SETUP
// Uncomment according to your hardware type
#define HARDWARE_TYPE MD_MAX72XX::FC16_HW
// #define HARDWARE_TYPE MD_MAX72XX::GENERIC_HW

// Defining size, and output pins
#define MAX_DEVICES 4
#define CS_PIN 5
#define BRIGHTNESS 0

static MD_Parola Display = MD_Parola(HARDWARE_TYPE, CS_PIN, MAX_DEVICES);

MD_MAX72XX::fontType_t *tf5x3 = newFont;

static char szDTime[12];
static char szDate[10];
// static char Weather[10];


void Display_Init(){
  Display.begin();
  Display.setIntensity(BRIGHTNESS);
  Display.displayClear();

  Display.setFont(tf5x3);
  Display.setCharSpacing(1);

  Display.setTextAlignment(PA_LEFT);
  Display.print(" MTWRFS");
}



void Disp_Time()
{
  struct tm timeinfo;

  if (!getLocalTime(&timeinfo))
  {
    Serial.println("Failed to obtain time");
    return;
  }
 // Serial.println(&timeinfo, "%A, %B %d %Y %H:%M:%S");

  switch (timeinfo.tm_wday)
  {
  case 0:
    strftime(szDTime, 12, "%I:%M:%SU", &timeinfo);
    break;
  case 1:
    strftime(szDTime, 12, "%I:%M:%SM", &timeinfo);
    break;
  case 2:
    strftime(szDTime, 12, "%I:%M:%ST", &timeinfo);
    break;
  case 3:
    strftime(szDTime, 12, "%I:%M:%SW", &timeinfo);
    break;
  case 4:
    strftime(szDTime, 12, "%I:%M:%SR", &timeinfo);
    break;
  case 5:
    strftime(szDTime, 12, "%I:%M:%SF", &timeinfo);
    break;
  case 6:
    strftime(szDTime, 12, "%I:%M:%SS", &timeinfo);
    break;
  default:
  Serial.println("Unknown time");
    break;
  }

  Display.setTextAlignment(PA_LEFT);
  Display.print(szDTime);
}


void Disp_Date()
{
  struct tm timeinfo;

  if (!getLocalTime(&timeinfo))
  {
    Serial.println("Failed to obtain Date info");
    return;
  }
  sprintf(szDate, "%d:%d:%d", timeinfo.tm_mday, timeinfo.tm_mon + 1, timeinfo.tm_year + 1900);
  
  Display.setTextAlignment(PA_LEFT);
  Display.print(szDate);
}


void Disp_weather(char* Weather)
{ 
    Display.setTextAlignment(PA_LEFT);
    Serial.println(Weather);
    Display.print(Weather);
}


void Disp_brightness(){
    struct tm timeinfo;

  if (!getLocalTime(&timeinfo))
  {
    Serial.println("Failed to obtain time");
    return;
  }

  uint8_t y = ((-28*pow((timeinfo.tm_hour - 12), 2))+25);
  //Serial.println(y);
  Display.setIntensity(y);
}