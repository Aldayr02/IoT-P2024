#include <Arduino.h>
#include <functions.h>



void setup()
{
  // 3.2 Leer información del sensor de temperatura y humedad (DHT 11)
  setup2();

  // 3.3 Leer información del sensor de ultrasónico (HC-SR04)
  // setup3();

  // 3.4 Lectura de tarjetas RFID por medio del lector RC522
  // setup4(); 

  // 3.5 Programa para control de accesos con el RC522
  // setup5();
}

void loop()
{
  // 3.2 Leer información del sensor de temperatura y humedad (DHT 11)
  TH_sensor();

  // 3.3 Leer información del sensor de ultrasónico (HC-SR04)
  // ultrasonic();

  // 3.4 Lectura de tarjetas RFID por medio del lector RC522
  // RFID();
  
  // 3.5 Programa para control de accesos con el RC522
  // access_control_RFID();
}