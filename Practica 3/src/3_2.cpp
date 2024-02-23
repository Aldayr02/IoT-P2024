#include <Arduino.h>
#include <DHT.h>

DHT dht15(15, DHT11);

void setup2()
{
    Serial.begin(9600);
    delay(2000);
    dht15.begin();
}

void TH_sensor()
{
    Serial.println("Temperatura en el sensor");
    Serial.println(dht15.readTemperature());
    Serial.println("Humedad del sensor:");
    Serial.println(dht15.readHumidity());
    delay(5000);
}