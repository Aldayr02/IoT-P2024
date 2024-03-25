/*
#include <WiFi.h>

#include "ThingSpeak.h" // always include thingspeak header file
// your network SSID (name)
const char* ssid = "IoT";
// your network password
const char* pass = "1t3s0IoT23";
WiFiClient client;

unsigned long myChannelNumber = 2481092; //SECRET_CH_ID;
const char* myWriteAPIKey = "DMO59SFP5W3D584K";

// Initialize with random values
int number1 = random(0,50);
int number2 = random(0,50);
String myStatus = "";

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, pass);

  Serial.println();
  Serial.print("Wait for WiFi...");

  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }

  Serial.println("");
  Serial.println("WiFi Connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  ThingSpeak.begin(client); // Initialize ThingSpeak
}

void loop() {
  // set the fields with the values
  ThingSpeak.setField(1, number1);
  ThingSpeak.setField(2, number2);

  // figure out the status message for multi fields
  if(number1 > number2){
    myStatus = String("field1 is greater than field2");
  }
  else if(number1 < number2){
    myStatus = String("field1 is less than field2");
  }
  else{
    myStatus = String("field1 equals field2");
  }

  // set the status
  ThingSpeak.setStatus(myStatus);

  // write to the ThingSpeak channel
  int x = ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);
  if(x == 200){
    Serial.println("ThingSpeak Channel update successful.");
  }
  else{
    Serial.println("Problem updating channel. HTTP error code " + String(x));
  }

  // change the values
  number1++;
  if(number1 > 99){
    number1 = 0;
  }
  number2 = random(0,100);

  delay(20000); // Wait 20 seconds to update the channel again
}
*/

#include <DHT.h>
#include <HTTPClient.h>
#include <ThingSpeak.h>
#include <WiFi.h>

// Temperature Data from sensors
struct TemperatureData {
    float dht; // in Celsius
    String getAllTemperatureData() {
        return "\nDHT: " + (String)dht + "*C";
    }
};

// dht11
const uint8_t dhtPin = 15; // Pin que usaras para el sensor de temp
const uint8_t dhtType = DHT11;
DHT dht(dhtPin, dhtType);

// data
TemperatureData temperatureData;

// timer
uint32_t startMillis;
const uint16_t timerDuration = 10000;

// wifi credentials
const char *ssid = "IoT"; // Tu SSID
const char *password = "1t3s0IoT23"; // Tu PWD del SSID
WiFiClient client;

// OpenWeather
String apiTemperature;
String serverPath = "https://api.openweathermap.org/data/2.5/weather?lat=20.6024&lon=-103.2714&appid=2a8d176a267afd236aa25106e6f80541&units=metric"; //El path que usaste en el web para la tem de la API - TODO el path con http

// ThingSpeak
uint32_t channelNumber = 2481092; // Tu channel ID number de thingspeak
const char *writeAPIKey = "DMO59SFP5W3D584K"; // El API write de thingspeak




// Helper functions

void connect_wifi(){
  Serial.begin(115200);
  WiFi.begin(ssid, password);

  Serial.println();
  Serial.print("Wait for WiFi...");

  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }

  Serial.println("");
  Serial.println("WiFi Connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void sendDataToThingSpeak(){
  // Check connection or reconnect to WiFi
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("No se detectó conexion a Internet");
    connect_wifi();
  }

  // first set the fields
  ThingSpeak.setField(1, temperatureData.dht);
  ThingSpeak.setField(2, apiTemperature);

  // set a status message based on any logic
  float indoorTempAverage = temperatureData.dht; 
  float outdoorTemp = apiTemperature.toFloat();
  String status;

  if (indoorTempAverage > 30)
    status = "Temperatura Relativamente Alta";
  else if (indoorTempAverage <= 30 && indoorTempAverage >= 20)
    status = "Temperatura Normal";
  else if (indoorTempAverage < 20)
    status = "Temperatura Relativamente Baja";

  // set status
  ThingSpeak.setStatus(status);

  // write to the ThingSpeak Channel
  int code = ThingSpeak.writeFields(channelNumber, writeAPIKey);
  if(code == 200) 
    Serial.println("Canal de Thingspeak actualizado correctamente.");
  else
    Serial.println("Hubo un problema actualizando el canal. Error HTTP: " + String(code));
}

float getDHTTemperatureC(){
  float t = dht.readTemperature(false);

  if (isnan(t)){
    t = 0;
    Serial.println("Lectura fallida del sensor DHT");
  }
  return t;
}

String getTemperatureFromAPI(){
  // Check connection or reconnect to WiFi
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("No se detecto conexion a Internet");
    connect_wifi();
  }

  HTTPClient http;

  // Domain + URL path or IP with path
  http.begin(serverPath.c_str());

  String temperature;
  int httpResponseCode = http.GET();

  if (httpResponseCode == 200) {
    String payload = http.getString();

    // filter temperature
    int index = payload.indexOf("\"temp\":");
    temperature = payload.substring(index + 7, payload.indexOf(",\"feels_"));
  } else {
    Serial.print("Peticion Get fallo, codigo de Error: ");
    Serial.println(httpResponseCode);
    temperature = "0.0"; // if fails
  }

  
  // free
  http.end();

  return temperature;
}


void setup() {
  Serial.begin(115200);
  dht.begin();

  ThingSpeak.begin(client); // Initialize ThingSpeak client
  connect_wifi();

  // start timer
  startMillis = millis();
}

void loop() {
  if (millis() >= timerDuration + startMillis) {
    // get sensors readings
    temperatureData.dht = getDHTTemperatureC();

    // get API temp
    apiTemperature = getTemperatureFromAPI();

    // debug on serial monitor
    Serial.println(temperatureData.getAllTemperatureData());
    Serial.println("Temperatura de la API: " + apiTemperature + "*C");
    delay(50);

    sendDataToThingSpeak();

    // set a twittwer
    String tweet = "Proyecto IoT, mensaje AUTOMATICO - La temperatura actual en mi ciudad es: " +
    apiTemperature + "*C";
    ThingSpeak.setTwitterTweet("AldairVP02", tweet); // sustituir por el nombre de tu usuario de twitter

    // restart
    startMillis = millis();
  }
}

