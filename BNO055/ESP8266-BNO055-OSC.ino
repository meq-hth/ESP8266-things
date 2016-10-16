
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <OSCMessage.h>
#include <OSCBundle.h>
#include <OSCData.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <utility/imumaths.h>


char ssid[] = "ssid";          // your net
char pass[] = "pass"; // your pass


WiFiUDP Udp;                                // A UDP instance to let us send and receive packets over UDP
 
 int ip1 = 1;
 int ip2 = 1;
 int ip3 = 1;
 int ip4 = 1;

 IPAddress outIp(ip1,ip2,ip3,ip4); // remote IP 
 
const unsigned int outPort = 9999;          // port to send OSC
const unsigned int localPort = 8888;    //  port to receive OSC


OSCErrorCode error;
unsigned int ledState = HIGH;  

/*
  Connections
   ===========
   Connect SCL to #5
   Connect SDA to #4
   Connect VDD to 3.3V DC
   Connect GROUND to common ground

   */

Adafruit_BNO055 bno = Adafruit_BNO055(55);










void setup() {
    Serial.begin(115200);

    

       pinMode(BUILTIN_LED, OUTPUT);
  digitalWrite(BUILTIN_LED, ledState); 

    // Connect to WiFi network
    Serial.println();
    Serial.println();
    Serial.print("Connecting to ");
    Serial.println(ssid);
    WiFi.begin(ssid, pass);

    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println("");

    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());

    Serial.println("Starting UDP");
    Udp.begin(localPort);
    Serial.print("Local port: ");
    Serial.println(Udp.localPort());

    if(!bno.begin())
  {
    /* There was a problem detecting the BNO055 ... check your connections */
    Serial.print("Ooops, no BNO055 detected ... Check your wiring or I2C ADDR!");
    while(1);
  }

  delay(1000);

}



  

 



void led(OSCMessage &msg) {
  ledState = msg.getInt(0);
  ip1 = msg.getInt(1);
   ip2 = msg.getInt(2);
    ip3 = msg.getInt(3);
     ip4 = msg.getInt(4);
 
  digitalWrite(BUILTIN_LED,  (int)ledState);
  Serial.print("/led: ");
  Serial.println(msg.getInt(3));
}



void loop() {


   IPAddress outIp(ip1,ip2,ip3,ip4);

  sensors_event_t event;
  bno.getEvent(&event);

int analogsensor = analogRead(A0);
   
   /* Also send calibration data for each sensor. */
  uint8_t sys, gyro, accel, mag = 0;
  bno.getCalibration(&sys, &gyro, &accel, &mag);

      /* Board layout:
         +----------+
         |         *| RST   PITCH  ROLL  HEADING
     ADR |*        *| SCL
     INT |*        *| SDA     ^            /->
     PS1 |*        *| GND     |            |
     PS0 |*        *| 3VO     Y    Z-->    \-X
         |         *| VIN
         +----------+
  */


OSCMessage bundle;
  int size = Udp.parsePacket();

  if (size > 0) {
    while (size--) {
      bundle.fill(Udp.read());
    }
    if (!bundle.hasError()) {
      bundle.dispatch("/led", led);
    } else {
      error = bundle.getError();
      Serial.print("error: ");
      Serial.println(error);
    }
  }




 OSCBundle bndl;
 

    //BOSCBundle's add' returns the OSCMessage so the message's 'add' can be composed together

    bndl.add("/analog").add(analogsensor);
    bndl.add("/heading").add((float)event.orientation.x);
    bndl.add("/pitch").add((float)event.orientation.y);
    bndl.add("/roll").add((float)event.orientation.z);
    //calib
    bndl.add("/sys").add((float)sys);
    bndl.add("/gyro").add((float)gyro);
    bndl.add("/accel").add((float)accel);
    bndl.add("/mag").add((float)mag);
    
   
     

    Udp.beginPacket(outIp, outPort);
        bndl.send(Udp); 
    Udp.endPacket(); 
    bndl.empty(); 


  delay(50);

 

  
}

