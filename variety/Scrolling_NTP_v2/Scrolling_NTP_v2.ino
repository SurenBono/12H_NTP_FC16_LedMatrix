//Simple FC-16 LED MATRIX CLOCK

#include <ESP8266WiFi.h>
#include <MD_MAX72xx.h>
#include <MD_Parola.h>
#include <SPI.h>

#define HARDWARE_TYPE MD_MAX72XX::FC16_HW

#define MAX_DEVICES 8
#define DATA_PIN  	13
#define CS_PIN    	15
#define CLK_PIN   	14

MD_Parola P = MD_Parola(HARDWARE_TYPE, CS_PIN, MAX_DEVICES);

const char* ssid     = "Arduino Wifi";     
const char* password = "101010101";  

textPosition_t scrollAlign  = PA_CENTER;

const char* ntpServer 			= "pool.ntp.org";
const long  gmtOffset_sec 		= 28800;       
const int   daylightOffset_sec 	= 0;

short i=0;
short counter=0;
char timex[80];
char dayx[80] ;
char datex[80] ;

const uint16_t ONE_SEC = 1000;
const char *NTP[4]={dayx,timex,datex};
textEffect_t FX[] ={PA_SCROLL_LEFT,PA_SCROLL_LEFT,PA_SCROLL_LEFT};


WiFiClient client;
String Connect = "NTP >";

void setup(void)
{
  Serial.begin(115200);
  P.begin();
  P.setIntensity(0);
  P.setTextAlignment(PA_CENTER);
  Serial.print("Connecting");
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED)
  {delay(1000); P.print(Connect); Connect += ">";}
  delay(1000);
  
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
  delay(1000);
  P.displayClear();
}

void printLocalTime()
{
  time_t rawtime;
  struct tm * timeinfo;
  time (&rawtime);
  timeinfo = localtime (&rawtime);
  strftime (timex,80,"%I:%M:%S %p",timeinfo);
}

void printDateOnly()
{
  time_t rawtime;
  struct tm * timeinfo;
  time (&rawtime);
  timeinfo = localtime (&rawtime);
  strftime (datex,80,"%d.%m.%Y",timeinfo);
}

void printDayOnly()
{
  time_t rawtime;
  struct tm * timeinfo;
  time (&rawtime);
  timeinfo = localtime (&rawtime);
  strftime (dayx,80,"%A",timeinfo);
}

void loop()
{
  printDayOnly();printLocalTime();printDateOnly();
  if(P.displayAnimate()){P.displayText(NTP[i],scrollAlign,5,2000,FX[i],FX[i]);}
  if(counter>3750){i++;counter=0;}if(i>2){ i=0;counter=0;}
  Serial.print(i);Serial.print("="); Serial.println(counter);counter++;
 }
	
