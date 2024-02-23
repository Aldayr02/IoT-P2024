#include <Arduino.h>
#include <MFRC522.h>

#define RST_PIN5 22 //Pin 22 para el reset del RC522
#define SS_PIN5 21 //pin 21 para el SS(SDA) del RC522
#define greenLed 12
#define redLed 14

MFRC522 mfrc522_5(SS_PIN5, RST_PIN5); // Ovejto del RC522

void setup5()
{
    Serial.begin(9600);
    SPI.begin();
    mfrc522_5.PCD_Init();
    Serial.println("Control de acceso:");
    pinMode(greenLed, OUTPUT);
    pinMode(redLed, OUTPUT); 
}

byte ActualUID[4];
byte Usuario1[4] = {0x14, 0x1D, 0x78, 0x20};
byte Usuario2[4] = {0xC1, 0x2F, 0xD6, 0x0E};

boolean compareArray(byte array1[], byte array2[])
{
    if(array1[0] != array2[0]) {return false;}
    if(array1[1] != array2[1]) {return false;}
    if(array1[2] != array2[2]) {return false;}
    if(array1[3] != array2[3]) {return false;}

    return true;
}

void access_control_RFID()
{

    if(mfrc522_5.PICC_IsNewCardPresent())
    {
         if(mfrc522_5.PICC_ReadCardSerial())
        {
            Serial.println("Card UID: ");
            for (byte i = 0; i < mfrc522_5.uid.size; i++)
            {
                Serial.print(mfrc522_5.uid.uidByte[i] < 0x10 ? "0" : " ");
                Serial.print(mfrc522_5.uid.uidByte[i], HEX);
                ActualUID[i] = mfrc522_5.uid.uidByte[i];


            }

            Serial.print("  ");

            if(compareArray(ActualUID, Usuario1))
            {
                Serial.println("Acceso concedido");
                digitalWrite(greenLed, HIGH);
                delay(1500);
                digitalWrite(greenLed, LOW);
            }

            else if(compareArray(ActualUID, Usuario2))
            {
                Serial.println("Acceso concedido");
                digitalWrite(greenLed, HIGH);
                delay(1500);
                digitalWrite(greenLed, LOW);
            }

            else
            {
                Serial.println("Acceso denegado...");
                digitalWrite(redLed, HIGH);
                delay(1500);
                digitalWrite(redLed, LOW);
            }
            
            mfrc522_5.PICC_HaltA();
        }
    }

}
