 # SIMPLE_12H_NTP_LEDMATRIX
... a rare simple NTP Timekeeping on FC16_HW LED MATRIX

- 8 Column LED MATRIX VCC runs stable on D1 Mini 3V PIN powered by smart phone 5 DCV charger.
- Onced sync wifi can be turned off until module rebooted.
- Continuous Precise Time keeping on same Timezone = UTC = GMT .
- Precision Tested on 3 different esp8266 module ( D1R1+OLED & LCD,D1 MINI,NODEMCU and Wifi with different SSID"s.
- Minimalist 7x5 Font & LED brightness for Independent power efficiency on solar powered 2 x 18650 battery shield. (untested)
- Define LED MATRIX Type =  FC16_HW if similar or choose 7 other LED MATRIX Type, Default = PAROLA_HW
- If you notice the #include <time.h> did not exist in this sketch..... 

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
 - You can suppress the display of leading zeroes  by using the "#" character  (%#d, %#H, %#I, %#j, %#m, %#M, %#S, %#U, %#w, %#W, %#y,%#Y)

-  **Suppress leading zeroes didnt work in this case ..not know why..

Source https://forum.arduino.cc/index.php?topic=452680.0 developed by Sroto&Gargees™ 2019 
