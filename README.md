# ESP8266-things
some esp8266 related things :O)

<b>BNO055 folder:</b> </br>
.ino file: connecting an HUZZAH (https://www.adafruit.com/product/2471) by i2c to a BNO055 (https://www.adafruit.com/product/2472) 
and sending xyz info (also calibration info and analog input A0) by OSC to PD. </br>

ESP8266 core for Arduino (https://github.com/esp8266/Arduino)</br>
BNO055 adafruit library (https://github.com/adafruit/Adafruit_BNO055) </br>
CNMAT OSC library (https://github.com/CNMAT/OSC)</br>

.pd file: using vanilla 0.47.1 and installing mrpeach (for OSC), ggee (for shell) and zexy (for s2l)</br>
pd file connect to 255.255.255.255 and send the ip to HUZZAH so it starts sending all info to that ip. </br>
im just sending "ifconfig en0" to shell to get my ip , yes im lazy :O) </br>

