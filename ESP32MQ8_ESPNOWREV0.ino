//Librerias de comunicación
#include <WiFi.h>
#include <esp_now.h>

//Libreria el sensor
#include <MQUnifiedsensor.h>
/************************Hardware Related Macros************************************/
#define         Board                   ("ESP-32") 


#define         Pin                     (34) //pin analogico en el ESP32

/***********************Software Related Macros************************************/
#define         Type                    ("MQ-8") //MQ8 
#define         Voltage_Resolution      (3.3) // 3V3 
#define         ADC_Bit_Resolution      (12) // ESP-32 bit resolution. 
#define         RatioMQ8CleanAir        (15.95) //RS / R0 = 20 ppm nuevo calculo
/*****************************Globals***********************************************/
MQUnifiedsensor MQ8(Board, Voltage_Resolution, ADC_Bit_Resolution, Pin, Type);
/*****************************Globals***********************************************/


// ESP-NOW
//-------------------------------------------------------------------------------------
uint8_t RxMACaddress[] = {0xAC, 0x67, 0xB2, 0x37, 0x00, 0xDC}; // verificar con codigo 84:CC:A8:2C:1C:8C Esta es la dirección mac del otro ESP32


typedef struct TxStruct
{
  float H2; 
}TxStruct;
TxStruct sentData;

void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) //Funcion de retorno.
{
  Serial.print("\r\nLast Packet Send Status:\t");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
}



void setup()
{

  
  Serial.begin(115200); //inicialización del puerto serial
  WiFi.mode(WIFI_STA);  // Lo inicio en modo estación
   //Valores de calibración del sensor MQ8
  MQ8.init();
     
  MQ8.setRL(1); // RESISTENCIA DE CARGA
  MQ8.setR0(0.14); // VALOR DE R0 CACULADO EN LA CALIBRACION 
 
  //------------------------------------------------------------------------------------
  if(esp_now_init() != ESP_OK)
  {
    Serial.println("Error initializing ESP-NOW");
    return;
  }
  // Definir la función de callback para recibir confirmaciones de entrega
  esp_now_register_send_cb(OnDataSent);
  // Registrar el dispositivo receptor con su dirección MAC
  esp_now_peer_info_t peerInfo;
  memcpy(peerInfo.peer_addr, RxMACaddress, 6);
  peerInfo.channel = 0;  
  peerInfo.encrypt = false;
  //-------------------------------------------------------------------------------------
  if(esp_now_add_peer(&peerInfo) != ESP_OK)
  {
    Serial.println("Error al registrar el dispositivo receptor");
    return;
  }  
  
  
  
      
  }

void loop()
{
  MQ8.update(); // Actualiza los datos, lee el voltaje desde el pin analogico.
  MQ8.setRegressionMethod(1); //_PPM =  a*ratio^b
  MQ8.setA(976.99); MQ8.setB(-0.688); // Configure the equation to to calculate H2 concentration
  
  sentData.H2 = MQ8.readSensor();
  
  Serial.println(sentData.H2);
    
    
  esp_err_t result = esp_now_send(RxMACaddress, (uint8_t *) &sentData, sizeof(sentData));
  //-------------------------------------------------------------------------------------
  if (result == ESP_OK) Serial.println("Sent with success");
  else Serial.println("Error sending the data");
  //-------------------------------------------------------------------------------------
  delay(10000);  
  //Serial.print("Hidrogeno (PPM): ");    
  //Serial.println(H2);
  //delay(2000); 
}
