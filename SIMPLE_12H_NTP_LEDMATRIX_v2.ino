
#include <MD_Parola.h>                // add from lib manager or similar
#include <MD_MAX72xx.h>               // add from lib manager or similar
#include <SPI.h>
#include <ESP8266WiFi.h>              // wifi drivers 

const uint16_t sec_1 = 1000;          // delay definitions
const uint16_t sec_2 = 2000;
const uint16_t sec_15 = 15000;

#define HARDWARE_TYPE MD_MAX72XX::FC16_HW       // parola mirrored fix

#define MAX_DEVICES 8               // define Led Matrix Col , 8 pixel * 8 pixel = 1 ,Default = 4 ,extended > 4
#define DATA_PIN  13                // GPIO PIN on NODEMCU , D1 MINI , D1R1 ,lolin ..or similar esp8266
#define CS_PIN    15
#define CLK_PIN   14
// GND = COMMON GROUND
// VCC = 3V OUTPUT PIN

MD_Parola P = MD_Parola(HARDWARE_TYPE, CS_PIN, MAX_DEVICES);

const char* ssid     = "Arduino Wifi";     // Tether identical SSID & PW on smart phone or similar for internet until time sync archieved
const char* password = "10101010";         // Predefined password ...Can only be changed once every upload...

const char* ntpServer = "pool.ntp.org";
const long  gmtOffset_sec = 28800;        // UTC= GMT +8 (My,Sg = Timezone), Find yours * 60 *60 for offset in sec
const int   daylightOffset_sec = 0;       

char buffer[80];
int  counter = 60;

void setup(void)

{
  P.begin();                                // init Parola drivers
  P.setIntensity(0);                        // Set Led Brightness 0 to 7
  P.setTextAlignment(PA_CENTER);            // Centered text
  P.write("Connecting");
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500); 
  }
  P.write("Get NTP");
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
  strftime (buffer,80,"%I:%M:%S %p",timeinfo);        // adds custom Time format into buffer until displayed
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
  strftime (buffer,80,"%a %d.%m.%y",timeinfo);           // adds custom Date format into buffer until displayed
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
  printLocalTime();           // Update Sec for 20 sec until date display ...not learned loop yet 
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
  printDateOnly();              // Date format for extended = 8 Col Led Matrix customise for yours ..
  delay(sec_2);
  delay(sec_2);
}
