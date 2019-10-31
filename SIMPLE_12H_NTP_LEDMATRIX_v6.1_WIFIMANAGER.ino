
// SIMPLE_12H_NTP_LEDMATRIX_v6.1 via WIFIMANAGER Developed by Sroto&Gargees (OpenSourced)
// http://srotogargees.business.site/
// ** Version 6.1 Bugfix **
// Portability  ( connect to any authorised available wifi via a webinterface )
// Secure,Connect to esp.AP with pw,open browser address 192.168.4.1,scanAP(SSID)+ keyin pw with internet connection

#include <FS.h>                     // Included in Arduino IDE / esp8266 setup ..google it
#include <ESP8266WiFi.h>            // http://arduino.esp8266.com/stable/package_esp8266com_index.json
#include <DNSServer.h>              // Included on esp8266 platform setup
#include <ESP8266WebServer.h>       // Included on esp8266 platform setup
#include <WiFiManager.h>            // https://github.com/tzapu/WiFiManager
#include <MD_Parola.h>              // https://github.com/MajicDesigns/MD_Parola
#include <MD_MAX72xx.h>             // https://github.com/MajicDesigns/MD_MAX72XX
#include <SPI.h>                    // Included in Arduino IDE 

const uint16_t SEC_1 = 1000;  

#define HARDWARE_TYPE MD_MAX72XX::FC16_HW

#define MAX_DEVICES 8
#define DATA_PIN  13
#define CS_PIN    15
#define CLK_PIN   14

MD_Parola P = MD_Parola(HARDWARE_TYPE, CS_PIN, MAX_DEVICES);

const char* ntpServer = "pool.ntp.org";
const long  gmtOffset_sec = 28800;       // My,SG=+8(UTC=GMT=TIMEZONE) *60*60
const int   daylightOffset_sec = 0;

char buffer[80];

void setup()
{
  Serial.begin(115200);
  Serial.println();
  WiFiManager wifiManager;

  IPAddress _ip = IPAddress(10, 0, 1, 78);
  IPAddress _gw = IPAddress(10, 0, 1, 1);
  IPAddress _sn = IPAddress(255, 255, 255, 0);
  
  if (!wifiManager.autoConnect("WifiManager","password")) // scan wifi on smartphone..etc...Open browser 192.168.4.1
  {
    Serial.println("failed, reset reconnect");
    delay(3000);
    ESP.reset();
    delay(5000);
  }
  Serial.println("connected...");
  Serial.println("local ip");
        
  P.begin();
  P.setIntensity(0);
  P.setTextAlignment(PA_CENTER);
  Serial.println(WiFi.localIP());
  WiFi.mode(WIFI_STA);      
  while (WiFi.status() != WL_CONNECTED) {delay(500);}
  P.write("Get NTP");
  delay(SEC_1); 
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
}

void printLocalTime()
{
  time_t rawtime;
  struct tm * timeinfo;
  time (&rawtime);
  timeinfo = localtime (&rawtime);
  strftime (buffer,80,"%I:%M:%S %p",timeinfo); 
  P.print(buffer);
  delay(SEC_1); 
}

void printDayOnly()
{
  time_t rawtime;
  struct tm * timeinfo;
  time (&rawtime);
  timeinfo = localtime (&rawtime);
  strftime (buffer,80,"%A",timeinfo);
  P.print(buffer);
  delay(SEC_1); 
}

void printDateOnly()
{
  time_t rawtime;
  struct tm * timeinfo;
  time (&rawtime);
  timeinfo = localtime (&rawtime);
  strftime (buffer,80,"%d.%m.%Y",timeinfo);
  P.print(buffer);
  delay(SEC_1); 
}

void loop()
{
   for (int Ti = 0; Ti < 25; Ti++)        // until 25 counts = 25sec
      {printLocalTime();}                 // Display centered 12h time, Minute with Second updates & AM/PM status
      for (int Dy = 0; Dy < 3; Dy++)      // the next each 1 sec until 3 count = 3sec 
      {printDayOnly();}                   // Display full Day name centered ,update Day name if changed inbetween these 3sec
      for (int Dt = 0; Dt < 3; Dt++)      // the next each 1 sec until 3 count = 3 sec 
      {printDateOnly();}    
}
