#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>
#include <MD_Parola.h>        // Add via LIBRARY MANAGER or GOOGLE it
#include <MD_MAX72xx.h>       // Add via LIBRARY MANAGER or GOOGLE it
#include <SPI.h>

#define HARDWARE_TYPE MD_MAX72XX::FC16_HW   // Mirrored display Fix 
#define MAX_DEVICES 8      

// GND = Common Ground (-)
#define DATA_PIN  13   // D7 = MOSI
#define CS_PIN    15   // D8 = CS
#define CLK_PIN   14   // D5 = CLK

#ifndef STASSID
#define STASSID "Arduino Wifi"
#define STAPSK  "101010101"
#endif

const char* ssid = STASSID;
const char* password = STAPSK;
const char* host = "NTP_D1MINIv2";

const char* ntpServer = "pool.ntp.org";
const long  gmtOffset_sec = 28800;       //Malaysian,Singapore GMT = +8UTC*60*60 , GREENWICH = 0 UTC + (60*60)
const int   daylightOffset_sec = 0;

MD_Parola P = MD_Parola(HARDWARE_TYPE, CS_PIN, MAX_DEVICES);

char buffer[80];
const uint16_t SEC_1 = 1000;

void printLocalTime()
{
  time_t rawtime;
  struct tm * timeinfo;
  time (&rawtime);
  timeinfo = localtime (&rawtime);
  strftime (buffer,80,"%I:%M:%S %p",timeinfo );
  P.print(buffer);  
  delay(SEC_1);  
}

void printDayOnly()
{
  time_t rawtime;
  struct tm * timeinfo;
  time (&rawtime);
  timeinfo = localtime (&rawtime);
  strftime(buffer,80,"%a %d.%m.%y",timeinfo);          
  P.print(buffer);    
  delay(SEC_1);                                            
}

void setup()
{
  Serial.begin(115200);
  Serial.println("Booting");
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.waitForConnectResult() != WL_CONNECTED) {
    Serial.println("Connection Failed! Rebooting...");
    delay(5000);
    ESP.restart();
  }

  ArduinoOTA.onStart([]() {
    String type;
    if (ArduinoOTA.getCommand() == U_FLASH) {
      type = "sketch";
    } else { // U_SPIFFS
      type = "filesystem";
    }

    Serial.println("Start updating " + type);
  });
  ArduinoOTA.onEnd([]() {
    Serial.println("\nEnd");
  });
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
  });
  ArduinoOTA.onError([](ota_error_t error) {
    Serial.printf("Error[%u]: ", error);
    if (error == OTA_AUTH_ERROR) {
      Serial.println("Auth Failed");
    } else if (error == OTA_BEGIN_ERROR) {
      Serial.println("Begin Failed");
    } else if (error == OTA_CONNECT_ERROR) {
      Serial.println("Connect Failed");
    } else if (error == OTA_RECEIVE_ERROR) {
      Serial.println("Receive Failed");
    } else if (error == OTA_END_ERROR) {
      Serial.println("End Failed");
    }});
  ArduinoOTA.begin();
  P.begin();                        // Init FC16 LED MATRIX display 
  P.setIntensity(0);                // Lowest brightness = 0.5 Watt/hour , MAX=15
  P.setTextAlignment(PA_CENTER);    // Centering Text Display
  Serial.println("Ready");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
}

void loop() 
{
  ArduinoOTA.handle(); 
  for (int Ti = 0; Ti < 25; Ti++)     	  // until 25 counts = 25sec
      {printLocalTime();}                 // Display centered 12h time, Minute with Second updates & AM/PM status
      for (int Dy = 0; Dy < 3; Dy++)      // the next each 1 sec until 3 count = 3sec 
      {printDayOnly();} 
}
