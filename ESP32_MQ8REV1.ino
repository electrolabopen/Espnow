//Programa para leer sensor MQ-8


#include <WiFi.h>

//Include the library
#include <MQUnifiedsensor.h>
/************************Hardware Related Macros************************************/
#define         Board                   ("ESP-32") 


#define         Pin                     (36) // Entrada Analogica en el microcontrolador

/***********************Software Related Macros************************************/
#define         Type                    ("MQ-8") //MQ8 
#define         Voltage_Resolution      (3.3) // 3.3 volt, valor del microcontrolador ESP32
#define         ADC_Bit_Resolution      (12) // ESP-32 Bit de resolución
#define         RatioMQ2CleanAir        (15.95) //RS / R0 = 70 dato en la hoja del sensor
/*****************************Globals***********************************************/
MQUnifiedsensor MQ8(Board, Voltage_Resolution, ADC_Bit_Resolution, Pin, Type);
/*****************************Globals***********************************************/


float H2 =0;

void setup()
{

  
  Serial.begin(115200); //Init serial port
  delay(10);

  
  


  /*****************************  MQ Init ********************************************/ 
  //Remarks: Configure the pin of arduino as input.
  /************************************************************************************/ 
  MQ8.init(); 
    
  MQ8.setRL(1);
  MQ8.setR0(0.14); // 1kohm
  //MQ8.setR0(1.73); // 10 Kohm
    
 
}

void loop()
{
  MQ8.update(); // actualiza los datos leido por la entrada analogica.
  
  MQ8.setRegressionMethod(1); //_PPM =  a*ratio^b metodo de regesion
  MQ8.setA(976.97); MQ8.setB(-0.688); // Datos del fabricante.
  float H2 = MQ8.readSensor();  
  //H2 =  MQ8.readSensor("H2");  
  //Serial.println(" PPM");
  Serial.print("Hidrogeno (PPM): ");
  Serial.println(H2);
  
  delay(500); 
}
