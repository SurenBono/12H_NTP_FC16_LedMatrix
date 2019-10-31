
//   SIMPLE_12H_NTP_LEDMATRIX_v6 Developed by Sroto&Gargees (OpenSourced)
//   http://srotogargees.business.site/
//   Keeps running even when network down for a day until software resets in loop .
//   ** Version 6 Bugfix **
// - Added nested looping count for functions ,( finally figure dit daut..yeeeay)
// - Precise Day & Date update if changes during display than delaying till next loop.
// - Added ESP reset & sync function in loop since crystal oscil offsets +22 sec/day.

#include <MD_Parola.h>				// Add via LIBRARY MANAGER or GOOGLE it
#include <MD_MAX72xx.h>				// Add via LIBRARY MANAGER or GOOGLE it
#include <SPI.h>					    // Included in Arduino IDE
#include <ESP8266WiFi.h>			// GOOGLE it

#define HARDWARE_TYPE MD_MAX72XX::FC16_HW   // Mirrored display Fix 
#define MAX_DEVICES 8                       // DEFAULT FC16 = 4 COLOMN/ROW , 4< EXTENDED,

// VCC = 3v3  
// GND = Common Ground (-)
#define DATA_PIN  13   // D7 = MOSI
#define CS_PIN    15	 // D8 = CS
#define CLK_PIN   14	 // D5 = CLK

MD_Parola P = MD_Parola(HARDWARE_TYPE, CS_PIN, MAX_DEVICES);

const char* ssid     = "Arduino Wifi";    // Hotspot Access Point/SSID on Smartphone until sync is archieved...etc
const char* password = "10101010";        // Pre-defined once every sketch upload

const char* ntpServer = "pool.ntp.org";
const long  gmtOffset_sec = 28800;        // +8UTC = My,Sg, Find your GMT/UTC/TIMEZONE *60*60
const int   daylightOffset_sec = 0;		    // Non daylight offset Zone

char buffer[80];                   // Allocate static temp memory = buffer for time variables ,max=2000
const uint16_t SEC_1 = 1000;       // 1000 millisec = 1 Second
int  counter = 0;              

void setup()
{
  P.begin();							          // Init FC16 LED MATRIX display 
  P.setIntensity(0);					      // Lowest brightness = 0.5 Watt/hour , MAX=15
  P.setTextAlignment(PA_CENTER);		// Centering Text Display
  
  P.write("Connecting");
  WiFi.mode(WIFI_STA);             // A wifi client, without this line it will broadcast esp#### ,energy efficient for Solar battery setup...etc
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) 
  {
    delay(500); 
  }
  P.write("Get NTP");
  delay(1000);
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
  unsigned timeout = 5000;
  unsigned start = millis();
  while (!time(nullptr))
  {delay(1000);}
  delay(1000);
}

void printLocalTime()
{
  time_t rawtime;
  struct tm * timeinfo;
  time (&rawtime);
  timeinfo = localtime (&rawtime);
  strftime(buffer,80,"%I:%M:%S %p",timeinfo); // custom format time into buffer
  P.print(buffer);                            // display current content in buffer 
  delay(SEC_1);                               // set int Ti++ count 1 / sec
}

void printDayOnly()
{
  time_t rawtime;
  struct tm * timeinfo;
  time (&rawtime);
  timeinfo = localtime (&rawtime);
  strftime(buffer,80,"%A",timeinfo);          // custom format full Day name into buffer, abbrieviated Day = %a
  P.print(buffer);                            // display current content in buffer
  delay(SEC_1);                               // set int Dy++ count 1 / sec
}

void printDateOnly()
{
  time_t rawtime;
  struct tm * timeinfo;
  time (&rawtime);
  timeinfo = localtime (&rawtime);
  strftime(buffer,80,"%d.%m.%Y",timeinfo);
  P.print(buffer);
  delay(SEC_1);                               // set int Dt++ count 1 / sec
}

void loop()
{
  if(counter == 86400)               // re-sync/day,counter++ = 31 sec/loop,
    {
      counter = 0;                    
      P.write("Sync NTP");
      delay(SEC_1); 
      setup();                        // Software Reset, reconnect and sync./ESP.reset 
    }
    else
    { 
      for (int Ti = 0; Ti < 25; Ti++) 	  // until 25 counts = 25sec
      {printLocalTime();}				          // Display centered 12h time, Minute with Second updates & AM/PM status
      for (int Dy = 0; Dy < 3; Dy++) 	    // the next each 1 sec until 3 count = 3sec 
      {printDayOnly();}					          // Display full Day name centered ,update Day name if changed inbetween these 3sec
      for (int Dt = 0; Dt < 3; Dt++) 	    // the next each 1 sec until 3 count = 3 sec 
      {printDateOnly();}				          // Display Date centered,update Date if changed inbetween these 3sec
       counter++;
     }
}
