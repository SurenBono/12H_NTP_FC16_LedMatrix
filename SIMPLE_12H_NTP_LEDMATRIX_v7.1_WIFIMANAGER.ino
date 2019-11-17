
// SIMPLE_12H_NTP_LEDMATRIX_v6.1 via WIFIMANAGER Developed by Sroto&Gargees (OpenSourced)
// http://srotogargees.business.site/
// ** Version 7.1 Bugfix ** 101119
// Display refresh x 10 /second for precision timekeeping.
// Runs once synced,resyncs and run on crystal if wifi unavailable until reset.
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
const uint16_t deciSec =100;
const uint16_t centiSec =10;

#define HARDWARE_TYPE MD_MAX72XX::FC16_HW

#define MAX_DEVICES 8
#define DATA_PIN  13
#define CS_PIN    15
#define CLK_PIN   14

MD_Parola P = MD_Parola(HARDWARE_TYPE, CS_PIN, MAX_DEVICES);

const char* ntpServer = "pool.ntp.org";
const long  gmtOffset_sec = 28800;       // My,SG=+8(UTC=GMT=TIMEZONE) *60*60
const int   daylightOffset_sec = 0;
int  counter 	= 55;
char buffer[80];

void setup()
{
  Serial.begin(115200);
  Serial.println();
  WiFiManager wifiManager;

  IPAddress _ip = IPAddress(10, 0, 1, 78);
  IPAddress _gw = IPAddress(10, 0, 1, 1);
  IPAddress _sn = IPAddress(255, 255, 255, 0);
  
  if (!wifiManager.autoConnect("autoConnect","password")) // scan wifi on smartphone..etc...Open browser 192.168.4.1
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
  while (WiFi.status() != WL_CONNECTED) {delay(500);}
  P.write("Get NTP");
  delay(SEC_1); 
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
  unsigned timeout = 5000;
  unsigned start = millis();
  while (!time(nullptr))
  {
    delay(1000);
  }
  delay(1000);
  WiFi.mode(WIFI_STA);
}

void printLocalTime()
{
  time_t rawtime;
  struct tm * timeinfo;
  time (&rawtime);
  timeinfo = localtime (&rawtime);
  strftime (buffer,80,"%I:%M:%S %p",timeinfo); 
  P.print(buffer);
  delay(deciSec); 		//	1/10 Sec Display Precision
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
if(counter == 60 && (WiFi.status() == WL_CONNECTED)){counter = 0;P.write("Sync NTP");delay(SEC_1);configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);}	
if(counter == 60 && (WiFi.status() != WL_CONNECTED)){counter = 0;P.write("Quartz Run");delay(SEC_1);}	
else  {	for (int Ti = 0; Ti < 540; Ti++)  // for 540 counts *0.1 sec = 54 sec 
      {printLocalTime();}                 // Display centered 12h time, Minute with Second updates & AM/PM status
      for (int Dy = 0; Dy < 3; Dy++)      // the next each 1 sec until 3 count = 3sec 
      {printDayOnly();}                   // Display full Day name centered ,update Day name if changed inbetween these 3sec
      for (int Dt = 0; Dt < 3; Dt++)      // the next each 1 sec until 3 count = 3 sec 
      {printDateOnly();} counter++;} 
}
