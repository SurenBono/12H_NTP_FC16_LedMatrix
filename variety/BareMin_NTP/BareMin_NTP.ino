//https://forum.arduino.cc/index.php?topic=452680.0

#include "time.h"		  // strftime works without it (flash updated esp micropyton) 
#include <ESP8266WiFi.h>  //esp8266

const char* ssid     = "Android Wifi" ";                 // your network SSID (name) / Smartphone cellular Hotspot / AP
const char* password = "10101010";                       // your network password


const char* ntpServer = "pool.ntp.org";
const long  gmtOffset_sec = 28800;       //Malaysian,Singapore GMT = +8UTC*60*60 , GREENWICH = 0 UTC + (60*60)
const int   daylightOffset_sec = 0;

char buffer[80];

void printLocalTime()
{
  time_t rawtime;
  struct tm * timeinfo;

  time (&rawtime);
  timeinfo = localtime (&rawtime);

  strftime (buffer,80,"%a %d/%m/%y %I:%M:%S %p ",timeinfo);
  Serial.println(buffer);
}

void setup()
{
  Serial.begin(115200);
  delay(10);
 
  Serial.print("\n\nConnecting to ");
  Serial.println(ssid);
 
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
 
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
 
  Serial.println();
  Serial.println("WiFi connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
 
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
 
  Serial.println("\nWaiting for time");
  unsigned timeout = 5000;
  unsigned start = millis();
  while (!time(nullptr))
  {
    Serial.print(".");
    delay(1000);
  }
  delay(1000);
 
  Serial.println("Time...");
}

void loop()
{
  printLocalTime();
  delay(1000);
}
