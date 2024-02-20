// PRUEBA MANUAL/AUTOMATICO Y SALIDA CON ESP32 PLC

//-----------------------------------------------------------
//ESP-NOW: Receiver

//-----------------------------------------------------------
#include <esp_now.h>
#include <WiFi.h>




#define BUTTON_PIN_AUTO 15           //AUTOMATICO 
#define BUTTTON_PIN_MANUAL 2        //MANUAL

const int LED_PIN = 13;                //defino el pin de salida

//Configuracion ESPNOW

typedef struct RxStruct
{
  float H2;
} RxStruct;
RxStruct receivedData;
//-------------------------------------------------------------------------------------
void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len)
{
  memcpy(&receivedData, incomingData, sizeof(receivedData));
}






void setup() {

   Serial.begin(115200); // Inicializa la comunicación serial
  pinMode(BUTTON_PIN_AUTO, INPUT_PULLUP);  // Configura el pin como entrada con pull-up
  pinMode(BUTTTON_PIN_MANUAL, INPUT_PULLUP); //Connfigura el pin como entrada con pull-up
  pinMode( LED_PIN, OUTPUT);

  // aseguro que se inicialice apagado
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

void loop() {


  //LECTURA DE ESTADO AUTOMATICO
  int buttonState_AUTO = digitalRead(BUTTON_PIN_AUTO);

  // LECTURA DE ESTADO MANUAL
  int buttonState_MANUAL = digitalRead(BUTTTON_PIN_MANUAL);

  //EVALUACION DE ESTADO AUTOMATICO
  if (buttonState_AUTO == 1){

    Serial.println("AUTOMATICO");
    Serial.println(receivedData.H2);
    if(receivedData.H2 > 100){
      //delay(1000);
       digitalWrite(LED_PIN, HIGH);
       //Serial.println("activo");
       
      delay(15000); // Esperar un segundo 
    }

  else{
        
  digitalWrite(LED_PIN, LOW);
  //Serial.println("desactivado");
  
  delay(10); // Esperar un segundo

  }  
    //delay(1000); 
  }



  // EVALUACION DE ESTADO MANUAL
  if (buttonState_MANUAL == 1){

    Serial.println("MANUAL");
    digitalWrite(LED_PIN, HIGH);

    delay(2000);
  }

  else{
    digitalWrite(LED_PIN, LOW);
    delay(1000);
  }


  }
