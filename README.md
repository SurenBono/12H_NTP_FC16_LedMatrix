  # SIMPLE_12H_NTP_LEDMATRIX
  a simple complex strftme NTP Timekeeping on FC16_HW LED MATRIX

- 4 or more Column FC-16 LED MATRIX powered by phone 5 DCV charger.
- Once synced wifi can be turned off until module re-connect . Only Time , scrolling Date & Dayname displays until wifi made available again for full weather forecast.
- Continuous Precise Time keeping via UTC init.
- Precision Tested on 3 different esp8266 module ( D1R1+OLED & LCD,D1 MINI,NODEMCU and Wifi with different SSID"s.
- Minimalist 7x5 Font & LED brightness for Independent power efficiency if solar powered 2 x 18650 battery shield possible.
- Define LED MATRIX Type =  FC16_HW if similar or choose 7 other LED MATRIX Type, Default = PAROLA_HW
- If you notice the #include <time.h> is not included in this sketch..... strftime comes with esp8266 lib
- latest version has scrolling and weather addition.
- Also detail on removing Hour leading zero via substring conversion.
- Added Weather cloud icon font to sync with openweathermap webicon pointers.
- RSSI Strenght, Dayname, Date, Cloud Condition, Temperature , Atmospheric Pressure , Wind Speed / direction and Visibility reports every 40 seconds.
- Time refresh atleast once every 500 millis blinky seperator with cloud icon.(rain,sunny..etc) later scrolls out other descriptions.
- Tested working for more than a year.
- Get & use the free Joining apikey token from openweathermap.org.Find your city code as in pic folder suggests.
- Requires Internet & router accesability .ISP Charges may apply.
- Edit Font - https://pjrp.github.io/MDParolaFontEditor

Update Note 30.01.2021
Programmed using Arduino ver 1.8.9 , esp8266 ver 2.5.0 on XP SP3

---------------------------------------------------------------------------------------------------------------
   strftime - C++ Reference

 - %a Abbreviated weekday name
 - %A Full weekday name
 - %b Abbreviated month name
 - %B Full month name
 - %c Date and time representation for your locale
 - %d Day of month as a decimal number (01-31)
 - %H Hour in 24-hour format (00-23)
 - %I Hour in 12-hour format (01-12)
 - %j Day of year as decimal number (001-366)
 - %m Month as decimal number (01-12)
 - %M Minute as decimal number (00-59)
 - %p Current locale's A.M./P.M. indicator for 12-hour clock
 - %S Second as decimal number (00-59)
 - %U Week of year as decimal number,  Sunday as first day of week (00-51)
 - %w Weekday as decimal number (0-6; Sunday is 0)
 - %W Week of year as decimal number, Monday as first day of week (00-51)
 - %x Date representation for current locale
 - %X Time representation for current locale
 - %y Year without century, as decimal number (00-99)
 - %Y Year with century, as decimal number
 - %z %Z Time-zone name or abbreviation, (no characters if time zone is unknown)
 - %% Percent sign
 - You can include text literals (such as spaces and colons) to make a neater display or for padding between adjoining columns.
 - You can suppress the display of leading zeroes  by using the "#" character  (%#d, %#H, %#I, %#j, %#m, %#M, %#S, %#U, %#w, %#W, %#y,%#Y) *this didnt work,convert char to      string and del the "0" column via substring.

   Source https://forum.arduino.cc/index.php?topic=452680.0 developed by Sroto&Gargees™ 2019 
   
   -n-
