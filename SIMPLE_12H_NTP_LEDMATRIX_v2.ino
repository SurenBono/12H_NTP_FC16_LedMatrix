
#include <MD_Parola.h>
#include <MD_MAX72xx.h>
#include <SPI.h>
#include <ESP8266WiFi.h>

const uint16_t sec_1 = 1000;
const uint16_t sec_2 = 2000;
const uint16_t sec_15 = 15000;

#define HARDWARE_TYPE MD_MAX72XX::FC16_HW

#define MAX_DEVICES 8
#define DATA_PIN  13
#define CS_PIN    15
#define CLK_PIN   14


MD_Parola P = MD_Parola(HARDWARE_TYPE, CS_PIN, MAX_DEVICES);

const char* ssid     = "Arduino Wifi";     
const char* password = "tm1010101";    

const char* ntpServer = "pool.ntp.org";
const long  gmtOffset_sec = 28800;       
const int   daylightOffset_sec = 0;

char buffer[80];
int  counter = 60;

void setup(void)

{
  P.begin();
  P.setIntensity(0);
  P.write("ArduinoWifi");
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500); 
  }
  P.write("     Get NTP");
  delay(1000);
  
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
 
  unsigned timeout = 5000;
  unsigned start = millis();
  while (!time(nullptr))
  {
    delay(1000);
  }
  delay(1000);
}

void printLocalTime()

{
  time_t rawtime;
  struct tm * timeinfo;
  time (&rawtime);
  timeinfo = localtime (&rawtime);
  strftime (buffer,80,"  %I:%M:%S %p",timeinfo); 
  //Serial.println(buffer);
  P.print(buffer);
  delay(sec_1);
}

void printDateOnly()
{
  time_t rawtime;
  struct tm * timeinfo;
  time (&rawtime);
  timeinfo = localtime (&rawtime);
  strftime (buffer,80," %a %d.%m.%y",timeinfo);
  P.print(buffer);
}

void printDayOnly()
{
  time_t rawtime;
  struct tm * timeinfo;
  time (&rawtime);
  timeinfo = localtime (&rawtime);
  strftime (buffer,80,"      %a",timeinfo);
  P.print(buffer);
}

void loop(void)
{
  printLocalTime();
  printLocalTime();
  printLocalTime();
  printLocalTime();
  printLocalTime();
  printLocalTime();
  printLocalTime();
  printLocalTime();
  printLocalTime();
  printLocalTime();
  printLocalTime();
  printLocalTime();
  printLocalTime();
  printLocalTime();
  printLocalTime();
  printLocalTime();
  printLocalTime();
  printLocalTime();
  printLocalTime();
  printLocalTime();
  //printDayOnly();
  //delay(sec_2);
  printDateOnly();
  delay(sec_2);
  delay(sec_2);
}
