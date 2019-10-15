
/**************************************************************
   This example runs directly on ESP8266 chip.
   Note: This requires ESP8266 support package:
   https://github.com/esp8266/Arduino

   Please be sure to select the right ESP8266 module
   in the Tools -> Board menu!

   NOTE: SmartConfig might not work in your environment.
         Please try basic ESP8266 SmartConfig examples
         before using this sketch!

   Change Blynk auth token to run :)
   Plus WiFi-config function into Blynk Mighty D1
 **************************************************************/

#define BLYNK_PRINT Serial    // Comment this out to disable prints and save space
#include <ESP8266WiFi.h>      // Include WIFI function
#include <BlynkSimpleEsp8266.h>// Include BlynkSimple
#include <SoftwareSerial.h>   // Arduino IDE <1.6.6
#include <PZEM004T.h>

/**************************************************************/
// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
char auth[] = "706726f17900420599b39dbde7be548b"; //imtbtest02;      interface code
char ssid[] = "Guest";                         //at I40 Center;    WIFI Name
char pass[] = "61828630";                      //at I40 Center;    WIFI password

PZEM004T pzem(4,5);                         //current meter PIN
IPAddress ip(192, 168, 1, 1);                  //PZEM004T IP address

/**************************************************************/
//initail
void setup()
{
    Serial.begin(9600);                        //current meter baudrate
    Blynk.begin(auth, ssid, pass);        
    pzem.setAddress(ip);
    WiFi.mode(WIFI_STA);
  
    int cnt = 0;
    while (WiFi.status() != WL_CONNECTED) {     //Check Wifi status
        delay(500);
        Serial.print(".");
        if (cnt++ >= 10) {
            WiFi.beginSmartConfig();                    // WiFi begin 
            while (1) {
                delay(1000);
                if (WiFi.smartConfigDone()) {
                  Serial.println();
                  Serial.println("SmartConfig: Success");   // Print connect Success
                  break;
              }
             Serial.print("|");                            
            }
        }
    }
    WiFi.printDiag(Serial);
    Blynk.config(auth);
}

/**************************************************************/
//Loop
void loop()
{
    Blynk.run();
    //  digitalWrite(LED_BUILTIN, HIGH);      // turn the LED on (HIGH is the voltage level)
    //  delay(1000);                       // wait for a second
    //  digitalWrite(LED_BUILTIN, LOW);      // turn the LED off by making the voltage LOW
    //  delay(1000);                       // wait for a second
}

//////////////////////////////////////////////////////////////////////////
//Show voltage
BLYNK_READ(V0) {
    float v = pzem.voltage(ip);
    if (v < 0.0) v = 0.0;
    Blynk.virtualWrite(V0, v);
}

//Show current
BLYNK_READ(V1) {
    float i = pzem.current(ip);
    if (i < 0.0) i = 0.0;
    Blynk.virtualWrite(V1, i);
}

//Show power
BLYNK_READ(V2) {
    float p = pzem.power(ip);
    if (p < 0.0) p = 0.0;
    Blynk.virtualWrite(V2, p);
}

//Show energy
BLYNK_READ(V3) {
    float e = pzem.energy(ip);
    if (e < 0.0) e = 0.0;
    Blynk.virtualWrite(V3, e);
}
