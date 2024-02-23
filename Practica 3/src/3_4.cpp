#include <Arduino.h>
#include <SPI.h>
#include <MFRC522.h>

#define RST_PIN4 22 //Pin 22 para el reset del RC522
#define SS_PIN4 21 //pin 21 para el SS(SDA) del RC522

MFRC522 mfrc522_4(SS_PIN4, RST_PIN4); // Ovejto del RC522

void setup4()
{
    Serial.begin(9600);
    SPI.begin();
    mfrc522_4.PCD_Init();
    Serial.println("Lectura del UID");
}

void RFID()
{
    delay(5000);
    if(mfrc522_4.PICC_IsNewCardPresent())
    {
        delay(50);
        if (mfrc522_4.PICC_ReadCardSerial())
        {
            Serial.println("Present");
            Serial.println("Card UID: ");
            for (byte i = 0; i < mfrc522_4.uid.size; i++)
            {
                Serial.print(mfrc522_4.uid.uidByte[i] < 0x10 ? "0" : " ");
                Serial.print(mfrc522_4.uid.uidByte[i], HEX);
            }
            Serial.println();
            
            mfrc522_4.PICC_HaltA();
        } 
    }
    else
    {
         Serial.println("No card... waiting for one");
    }
}