#include <Arduino.h>

#define SOUND_SPEED 0.034
#define CM_TO_INCH 0.393701

const int triPin = 5;
const int echoPin = 18;

long duration;
float distanceCm;
float disctanceInch;

void setup3()
{
    Serial.begin(115200);
    pinMode(triPin, OUTPUT);
    pinMode(echoPin, INPUT);
}

void ultrasonic()
{
    digitalWrite(triPin, LOW);
    delayMicroseconds(2);
    digitalWrite(triPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(triPin, LOW);

    duration = pulseIn(echoPin, HIGH);
    distanceCm = duration * SOUND_SPEED;
    disctanceInch = distanceCm * CM_TO_INCH;

    Serial.print("Distance(cm): ");
    Serial.println(distanceCm);
    Serial.print("Distance(inch): ");
    Serial.println(disctanceInch);

    delay(1000);
}