#include <Arduino.h>

#ifdef __cplusplus
extern "C" {
#endif
uint8_t temprature_sens_read();
#ifdef __cplusplus
}
#endif

void temperature_reading()
{
    Serial.println("la temperatura interna del ESP32 es: ");
    Serial.println((( temprature_sens_read() - 32 ) / 1.8));
    Serial.println("");
}

