/*
*  NTP Scrolling Weather ( FC16 + ESP8266 )
*/

#include <SPI.h>
#include <MD_Parola.h>
#include <MD_MAX72xx.h>
#include <ESP8266WiFi.h>
#include <ArduinoJson.h>

#include "Weather_Fontx.h"

#define HARDWARE_TYPE MD_MAX72XX::FC16_HW

#define MAX_DEVICES 8
#define DATA_PIN  	13
#define CS_PIN    	15
#define CLK_PIN   	14

MD_Parola P = MD_Parola(HARDWARE_TYPE, CS_PIN, MAX_DEVICES);
textEffect_t FX[4] ={PA_PRINT,PA_SCROLL_LEFT,PA_OPENING_CURSOR, PA_SCROLL_UP};

const char* ssid                  = "Arduino Wifi";     
const char* password              = "101010101";  

char servername[]	          = "api.openweathermap.org"; 
String weatherLang 	          = "&lang=en";
String APIKEY 		          = "e876574712e8cd87bad58bc226831414";
String CityID 		          = "1732811";                            // Kluang,Malaysia.

const char* ntpServer 		  = "pool.ntp.org";
const long  gmtOffset_sec 	  = 28800;                                // 60 sec * 60 min * local UTC
const int   daylightOffset_sec 	  = 0;

const uint16_t One_SEC = 1000, Half_SEC = 500;
int i=1,j;      

char Tx[15],Txx[15],Txy[15],DDtx[22],Weatherx[200],T2x[15];
const char *NTP[5]={Tx,DDtx,Weatherx,Txx,Txy};

WiFiClient client;
String Connect = "NTP >",D,Dt,DDt,icon,k,l;

void setup(void)
{
  Serial.begin(115200);
  P.begin();
  P.setIntensity(0);
  P.setFont(ExtASCII); 
  P.setTextAlignment(PA_LEFT);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED)
  {delay(500);Serial.print('>');P.print(Connect); Connect += ">";}
  Serial.println();P.displayClear();
  delay(1000);
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
  getWeatherData();
  }
  
void printLocalTime()
{
  char timex[45];
  time_t rawtime;
  struct tm * timeinfo;
  time (&rawtime);
  timeinfo = localtime (&rawtime);
  strftime (timex,45,"%p %I:%M:%S %d.%m.%y %p %I %M %S %A",timeinfo);         
  String TDDt   = timex;
  String T2     = TDDt.substring(21,32);if (T2.substring(3,4) == "0") {T2.remove(3,1);} 
  String T      = TDDt.substring(0,11) ;if (T.substring(3,4)   == "0") {T.remove(3,1);} 
         D      = TDDt.substring(32);
         Dt     = TDDt.substring(12,20);if (Dt.substring(0,1) == "0") {Dt.remove(0,1);}
         DDt    = D + "    " + Dt;
         k      = String(char(j))+ T ;
         l      = String(char(j))+ T2 ;
         T.toCharArray(Txx,15);l.toCharArray(Txy,15);T2.toCharArray(T2x,15);
         k.toCharArray(Tx,15);
         DDt.toCharArray(DDtx,22);
         Serial.println(T + " " + D + " " + Dt);
         //delay(One_SEC);
         delay(Half_SEC);   // Blink seperator every 1/2 sec
}

void getWeatherData() 
{   
     String result ="";			// empty last result 
 if (client.connect(servername, 80)) {
     client.println("GET /data/2.5/weather?id="+CityID+"&units=metric&APPID="+APIKEY);
     client.println("Host: api.openweathermap.org");
     client.println("User-Agent: ArduinoWiFi/1.1");
     client.println("Connection: close");client.println();} 
	
 else {Serial.println("connection failed"); Serial.println();}

  while(client.connected() && !client.available()) delay(1); 
  while (client.connected() || client.available()) 
  {char c = client.read(); result = result+c;}

  client.stop(); 
  result.replace('[', ' ');
  result.replace(']', ' ');
  //Serial.println(result);

  char jsonArray [result.length()+1];
  result.toCharArray(jsonArray,sizeof(jsonArray));
  jsonArray[result.length() + 1] = '\0';

  StaticJsonBuffer<1024> json_buf;
  JsonObject &root = json_buf.parseObject(jsonArray);
  if (!root.success())
  {Serial.println("parseObject() failed");}

  String loc            = root["name"];
  String lon            = root["coord"]["lon"];
  String lat            = root["coord"]["lat"];
  String country        = root["sys"]["country"];
  String cloud          = root["weather"]["description"];
  int    temp           = root["main"]["temp"];
  String humid          = root["main"]["humidity"];
  String icon           = root["weather"]["icon"];
  String press          = root["main"]["pressure"];
  float  windSpeed      = root["wind"]["speed"];
  int    windDeg        = root["wind"]["deg"];
  int    vision         = root["visibility"];
  int    rssi           = WiFi.RSSI();
  int    Arrow;
  String Dir;

  if ( icon == "01d"|| icon == "01n" ){ j=186;}
  if ( icon == "02d"|| icon == "02n" ){ j=187;}
  if ( icon == "03d"|| icon == "03n" ){ j=188;}
  if ( icon == "04d"|| icon == "04n" ){ j=189;}
  if ( icon == "09d"|| icon == "09n" ){ j=190;}
  if ( icon == "10d"|| icon == "10n" ){ j=191;}
  if ( icon == "11d"|| icon == "11n" ){ j=192;}
  if ( icon == "13d"|| icon == "13n" ){ j=193;}
  if ( icon == "50d"|| icon == "50n" ){ j=194;}

  if( windDeg >= 338 ||windDeg >= 0 && windDeg<23) {Dir = "N" ; Arrow = 178; }
  if( windDeg >= 23  && windDeg < 68 ){ Dir ="NE" ; Arrow = 179; }
  if( windDeg >= 68  && windDeg < 113){ Dir ="E"  ; Arrow = 180; }
  if( windDeg >= 113 && windDeg < 158){ Dir ="SE" ; Arrow = 181; }
  if( windDeg >= 158 && windDeg < 203){ Dir ="S"  ; Arrow = 182; }
  if( windDeg >= 203 && windDeg < 248){ Dir ="SW" ; Arrow = 183; }
  if( windDeg >= 248 && windDeg < 293){ Dir ="W"  ; Arrow = 184; }
  if( windDeg >= 293 && windDeg < 338){ Dir ="NW" ; Arrow = 185; }

  String Vis              = String(vision/1000);
  String AP               = ssid;
  String Weather          = "    "+AP+" "+rssi+"    "+D+"    "+Dt+"    "+loc+"    "+cloud+" "+(char(j))+"    Temp : "+String(temp)+(char(144))+"c    Hum : "+humid+" %    Prs : "+press+" hPa " ;
         Weather         += "    Wind : "+String(windSpeed, 1)+" m/s " +"  "+ (char(Arrow))+"  "+ String(windDeg) + (char(144)) +" "+Dir ;
	 Weather         += "    Vis : "+ Vis + " km";
         Weather.toCharArray(Weatherx,200);
		 
  Serial.print( loc  + "," + country + " (" + lat + "," + lon + ") ,icon:" ) ;Serial.println(icon);
  Serial.println( cloud  + "," + ", T : " + String(temp) + "°c , H : " + humid + "% , P : " + press + " hPa") ;
  Serial.println( "Wind: " + String(windSpeed, 1) + " m/s , " + String(windDeg) + "° , " + Dir +" ,ascii:" + Arrow) ;
  Serial.println("visibility : " + Vis + "km");Serial.println();
  Serial.print(ssid);Serial.print(": ");Serial.println(rssi);
  P.displayText(NTP[2],PA_CENTER , 20, 0, FX[1], FX[1]); 
}

void sequence(){
	P.setTextAlignment(PA_LEFT);
	if(WiFi.status() == WL_CONNECTED) 
	    { 
	        switch (i) {   case 1 : printLocalTime();P.print(Tx); break ; case 2 : printLocalTime();P.print(Txy); break ;
                         case 3 : printLocalTime();P.print(Tx); break ; case 4 : printLocalTime();P.print(Txy); break ;
                         case 5 : printLocalTime();P.print(Tx); break ; case 6 : printLocalTime();P.print(Txy); break ;
                         case 7 : printLocalTime();P.print(Tx); break ; case 8 : printLocalTime();P.print(Txy); break ;
                         case 9 : printLocalTime();P.print(Tx); break ; case 10: printLocalTime();P.print(Txy); break ;
                         case 11: printLocalTime();P.print(Tx); break ; case 12: printLocalTime();P.print(Txy); break ;
                         case 13: printLocalTime();P.print(Tx); break ; case 14: printLocalTime();P.print(Txy); break ;
                         case 15: printLocalTime();P.print(Tx); break ; case 16: printLocalTime();P.print(Txy); break ;
                         case 17: printLocalTime();P.print(Tx); break ; case 18: printLocalTime();P.print(Txy); break ;
                         case 19: printLocalTime();P.print(Tx); break ; case 20: printLocalTime();P.print(Txy); break ;
                         case 21: printLocalTime();P.print(Tx); break ; case 22: printLocalTime();P.print(Txy); break ;
                         case 23: printLocalTime();P.print(Tx); break ; case 24: printLocalTime();P.print(Txy); break ;
                         case 25: printLocalTime();P.print(Tx); break ; case 26: printLocalTime();P.print(Txy); break ;
                         case 27: printLocalTime();P.print(Tx); break ; case 28: printLocalTime();P.print(Txy); break ;
                         case 29: printLocalTime();P.print(Tx); break ; case 30: printLocalTime();P.print(Txy); break ;
                         case 31: printLocalTime();P.print(Tx); break ; case 32: printLocalTime();P.print(Txy); break ;
                         case 33: printLocalTime();P.print(Tx); break ; case 34: printLocalTime();P.print(Txy); break ;
                         case 35: printLocalTime();P.print(Tx); break ; case 36: printLocalTime();P.print(Txy); break ;
                         case 37: printLocalTime();P.print(Tx); break ; case 38: printLocalTime();P.print(Txy); break ;
                         case 39: printLocalTime();P.print(Tx); break ; case 40: printLocalTime();P.print(Txy); break ;
                         case 41: printLocalTime();P.print(Tx); break ; case 42: printLocalTime();P.print(Txy); break ;
                         case 43: printLocalTime();P.print(Tx); break ; case 44: printLocalTime();P.print(Txy); break ;
                         case 45: printLocalTime();P.print(Tx); break ; case 46: printLocalTime();P.print(Txy); break ;
                         case 47: printLocalTime();P.print(Tx); break ; case 48: printLocalTime();P.print(Txy); break ;
                         case 49: printLocalTime();P.print(Tx); break ; case 50: printLocalTime();P.print(Txy); break ;
                         case 51: printLocalTime();P.print(Tx); break ; case 52: printLocalTime();P.print(Txy); break ;
                         case 53: printLocalTime();P.print(Tx); break ; case 54: printLocalTime();P.print(Txy); break ;
                         case 55: printLocalTime();P.print(Tx); break ; case 56: printLocalTime();P.print(Txy); break ;
                         case 57: printLocalTime();P.print(Tx); break ; case 58: printLocalTime();P.print(Txy); break ;
                         case 59: printLocalTime();P.print(Tx); break ; case 60: printLocalTime();P.print(Txy); break ;
                         case 61: printLocalTime();P.print(Tx); break ; case 62: printLocalTime();P.print(Txy); break ;
                         case 63: printLocalTime();P.print(Tx); break ; case 64: printLocalTime();P.print(Txy); break ;
                         case 65: printLocalTime();P.print(Tx); break ; case 66: printLocalTime();P.print(Txy); break ;
                         case 67: printLocalTime();P.print(Tx); break ; case 68: printLocalTime();P.print(Txy); break ;
                         case 69: printLocalTime();P.print(Tx); break ; case 70: printLocalTime();P.print(Txy); break ;
                         case 71: printLocalTime();P.print(Tx); break ; case 72: printLocalTime();P.print(Txy); break ;
                         case 73: printLocalTime();P.print(Tx); break ; case 74: printLocalTime();P.print(Txy); break ;
                         case 75: printLocalTime();P.print(Tx); break ; case 76: printLocalTime();P.print(Txy); break ;
                         case 77: printLocalTime();P.print(Tx); break ; case 78: printLocalTime();P.print(Txy); break ;
                         case 79: printLocalTime();P.print(Tx); break ; case 80: printLocalTime();P.print(Txy); break ;
                         case 81: printLocalTime();P.print(Tx); break ; case 82: printLocalTime();P.print(Txy); break ;
                         case 83: getWeatherData();P.displayText(NTP[2],PA_CENTER , 20, 0, FX[1], FX[1]); break ;
                        }  i++;if(i>83){i=1;};  
	    }
		
    if (WiFi.status() != WL_CONNECTED)
	    {
			 switch (i){ case 1 : printLocalTime();P.print(T2x); break ;
                         case 2 : printLocalTime();P.print(Txx); break ; case 3 : printLocalTime();P.print(T2x); break ;
                         case 4 : printLocalTime();P.print(Txx); break ; case 5 : printLocalTime();P.print(T2x); break ;
                         case 6 : printLocalTime();P.print(Txx); break ; case 7 : printLocalTime();P.print(T2x); break ;
                         case 8 : printLocalTime();P.print(Txx); break ; case 9 : printLocalTime();P.print(T2x); break ;
                         case 10: printLocalTime();P.print(Txx); break ; case 11: printLocalTime();P.print(T2x); break ;
                         case 12: printLocalTime();P.print(Txx); break ; case 13: printLocalTime();P.print(T2x); break ;
                         case 14: printLocalTime();P.print(Txx); break ; case 15: printLocalTime();P.print(T2x); break ;
                         case 16: printLocalTime();P.print(Txx); break ; case 17: printLocalTime();P.print(T2x); break ;
                         case 18: printLocalTime();P.print(Txx); break ; case 19: printLocalTime();P.print(T2x); break ;
                         case 20: printLocalTime();P.print(Txx); break ; case 21: printLocalTime();P.print(T2x); break ;
                         case 22: printLocalTime();P.print(Txx); break ; case 23: printLocalTime();P.print(T2x); break ;
                         case 24: printLocalTime();P.print(Txx); break ; case 25: printLocalTime();P.print(T2x); break ;
                         case 26: printLocalTime();P.print(Txx); break ; case 27: printLocalTime();P.print(T2x); break ;
                         case 28: printLocalTime();P.print(Txx); break ; case 29: printLocalTime();P.print(T2x); break ;
                         case 30: printLocalTime();P.print(Txx); break ; case 31: printLocalTime();P.print(T2x); break ;
                         case 32: printLocalTime();P.print(Txx); break ; case 33: printLocalTime();P.print(T2x); break ;
                         case 44: printLocalTime();P.print(Txx); break ; case 35: printLocalTime();P.print(T2x); break ;
                         case 36: printLocalTime();P.print(Txx); break ; case 37: printLocalTime();P.print(T2x); break ;
                         case 38: printLocalTime();P.print(Txx); break ; case 39: printLocalTime();P.print(T2x); break ;
                         case 40: printLocalTime();P.print(Txx); break ; case 41: printLocalTime();P.print(T2x); break ;
                         case 42: P.displayText(NTP[1],PA_CENTER , 20, 0, FX[1], FX[1]); break ;
    
                        }  i++;if(i>42){i=1;};
		}
		
}

void loop(){if(P.displayAnimate()){sequence();}}

// -N-
