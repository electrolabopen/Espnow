//-----------------------------------------------------------
//ESP-NOW: Receiver

//-----------------------------------------------------------
#include <esp_now.h>
#include <WiFi.h>

const int LED_PIN = 2;


typedef struct RxStruct
{
  float H2;
}RxStruct;
RxStruct receivedData;
//-------------------------------------------------------------------------------------
void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len)
{
  memcpy(&receivedData, incomingData, sizeof(receivedData));
}
//======================================================================================
void setup()
{
  Serial.begin(115200);
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);
  
  
  //-------------------------------------------------------------------------------------
  WiFi.mode(WIFI_STA);
  if (esp_now_init() != ESP_OK)
  {
    Serial.println("Error initializing ESP-NOW");
    return;
  }
  esp_now_register_recv_cb(OnDataRecv);
}
//======================================================================================
void loop()
{
  
  
  Serial.println(receivedData.H2);
  if(receivedData.H2 >= 0.20){
       digitalWrite(LED_PIN, HIGH);
      delay(1000); // Esperar un segundo 
  }

  else{
        
digitalWrite(LED_PIN, LOW);
  delay(1000); // Esperar un segundo
}  
    //Serial.println(receivedData.potVal);
  
}
