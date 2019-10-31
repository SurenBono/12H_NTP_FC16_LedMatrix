//https://forum.arduino.cc/index.php?topic=452680.0

#include "time.h"		  // strftime works without it (flash updated esp micropyton) 
#include <ESP8266WiFi.h>  //esp8266

const char* ssid     = "Arduino Wifi";                    // your network SSID (name)
const char* password = "tm1010101";                       // your network password


const char* ntpServer = "pool.ntp.org";
const long  gmtOffset_sec = 28800;       //Malaysian,Singapore GMT = +8UTC*60*60 , GREENWICH = 0 UTC + (60*60)
const int   daylightOffset_sec = 0;

/*
  %a Abbreviated weekday name
  %A Full weekday name
  %b Abbreviated month name
  %B Full month name
  %c Date and time representation for your locale
  %d Day of month as a decimal number (01-31)
  %H Hour in 24-hour format (00-23)
  %I Hour in 12-hour format (01-12)
  %j Day of year as decimal number (001-366)
  %m Month as decimal number (01-12)
  %M Minute as decimal number (00-59)
  %p Current locale's A.M./P.M. indicator for 12-hour clock
  %S Second as decimal number (00-59)
  %U Week of year as decimal number,  Sunday as first day of week (00-51)
  %w Weekday as decimal number (0-6; Sunday is 0)
  %W Week of year as decimal number, Monday as first day of week (00-51)
  %x Date representation for current locale
  %X Time representation for current locale
  %y Year without century, as decimal number (00-99)
  %Y Year with century, as decimal number
  %z %Z Time-zone name or abbreviation, (no characters if time zone is unknown)
  %% Percent sign
  You can include text literals (such as spaces and colons) to make a neater display or for padding between adjoining columns.
  You can suppress the display of leading zeroes  by using the "#" character  (%#d, %#H, %#I, %#j, %#m, %#M, %#S, %#U, %#w, %#W, %#y, %#Y)
*/
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
 
  // We start by connecting to a WiFi network
  Serial.print("\n\nConnecting to ");
  Serial.println(ssid);
 
  /* Explicitly set the ESP8266 to be a WiFi-client, otherwise, it by default,
    would try to act as both a client and an access-point and could cause
  network-issues with your other WiFi-devices on your WiFi-network. */
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
  //time_t now = time(nullptr);
  //Serial.print(ctime(&now));
  printLocalTime();
  delay(1000);
}
