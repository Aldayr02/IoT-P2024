#include <Arduino.h>

void setup_1(){
    pinMode(2, OUTPUT);
    Serial.begin(9600);
}

void led_on_off()
{
digitalWrite(2,HIGH);
Serial.println("Led prendido");
delay(2500);
digitalWrite(2,LOW);
Serial.println("Led apagado");
delay(2500);
}

