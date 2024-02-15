#include <Arduino.h>
#include <functions.h>

void setup()
{
    setup_5();
}

void loop()
{

    /*
    1. Prender y apagar el led interno del ESP32, 
    escribir la temperatura internadel ESP32 en el monitor serial
    */
    // led_on_off();
    // temperature_reading();

    //2. Usar el ESP32 como servidor web
    // webserver();

    //4. Prender y apagar el led interno del ESP32 desde el servidor web
    // led_webserver();

    // 5. Prender y apagar un led externo desde el servidor web ESP32 
    external_led_webserver();
      
}