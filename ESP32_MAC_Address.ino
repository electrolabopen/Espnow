
//------------------------
// MAC Adress del ESP32
//-----------------------

#include <WiFi.h>



void setup() {
  Serial.begin(9600);
  WiFi.mode(WIFI_MODE_STA);
  
  Serial.println(WiFi.macAddress());
}

void loop() {
  

}
