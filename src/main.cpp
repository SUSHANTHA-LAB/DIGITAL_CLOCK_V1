#include <Arduino.h>
#include <esp_task_wdt.h>                                         
#include <../src/app/Disp.h>
#include<../src/app/WiFi.h>

// # Defines 
#define WDT_TIMEOUT     50 
#define STATE_UPDATE 85000000 // 1.4 min
#define DATA_UPDATE 3600000000 // 60 min
#define DELAY 1000


enum clockstate
{
  tim = 1,
  date = 2,
  weather = 3
};

// Variables
int BootReason        = 99;
hw_timer_t *timer = NULL;
hw_timer_t *timer2 = NULL;
static uint8_t state = 1;
static bool dataupdate = 0; 
static char Weather[10];

// Prototypes 
void Wdog_Init();
void Timers_Init();
void IRAM_ATTR onTimer();
void IRAM_ATTR onTimer2();

void Wdog_Init(){
    Serial.println("Configuring WatchDogTimeout WDT...");
    esp_task_wdt_init(WDT_TIMEOUT, true);             // enable panic so ESP32 restarts
    esp_task_wdt_add(NULL);                           // add current thread to WDT watch
}

void setup() {

  Serial.begin(115200);

  Wdog_Init();

  Timers_Init();

  Display_Init();

  WiFi_init();

  Weather_update(Weather);

  Disp_brightness();
}

void loop() {
  
    delay(DELAY);

  switch (state)
  {
  case tim:
    
    Disp_Time();
    break;
  case date:
   
    Disp_Date();
    delay(DELAY*3);
    state++;
    break;
  case weather:
    Disp_weather(Weather);
    delay(DELAY*4);
    state = 1;
    break;
  default:
  Serial.println("Something failed in the main loop");
    break;
  }

  if(dataupdate){
    Weather_update(Weather);
    dataupdate = 0; 
  }

  esp_task_wdt_reset(); 
}




void IRAM_ATTR onTimer()
{
  if (state > 3)
    state = 0;
  state++;
}

void IRAM_ATTR onTimer2()
{
  Serial.println("Timeout 2");
  dataupdate = 1;
}

void Timers_Init(){
  timer = timerBegin(0, 80, true);
  timerAttachInterrupt(timer, &onTimer, true);
  timerAlarmWrite(timer, STATE_UPDATE, true);
  timerAlarmEnable(timer);

  timer2 = timerBegin(1, 80, true);
  timerAttachInterrupt(timer2, &onTimer2, true);
  timerAlarmWrite(timer2, DATA_UPDATE, true); //10800000000
  timerAlarmEnable(timer2);
}
