/****************************************
 * Include Libraries
 ****************************************/
#include <WiFi.h>
#include <PubSubClient.h>

/****************************************
 * Define Constants  
 ****************************************/
#define WIFISSID "YOUR_WIFI_SSID" // WIFI SSID here
#define PASSWORD "Your_WIFI_Pwd" // WIFI password
#define TOKEN "YourUBIDOTSTokenID" // Ubidots TOKEN ID, same as used in class, available from Ubidots
#define MQTT_CLIENT_NAME "123456AbCde" // Client ID, 8 to 12 alphanumeric characters (ASCII), must be random and unique, different from other devices
#define VARIABLE_LABEL_temp "temperatura" // Temperature variable
#define VARIABLE_LABEL_hum "humedad" // Humidity variable
#define DEVICE_LABEL "MQTT-Publish-esp32" // Device name to create

char mqttBroker[] = "industrial.api.ubidots.com";
char payload[200]; // Read and understand the payload, reference: https://techterms.com/definition/payload
char topic[150]; // Space for topic name

// Space to store values to send
char str_temp[10];
char str_hum[10];

#include "DHT.h"
#define pin1 15
DHT dht1(pin1, DHT11); // Blue temperature and humidity sensor

/****************************************
 * Helper Functions
 ****************************************/
WiFiClient ubidots;
PubSubClient client(ubidots);

void callback(char* topic, byte* payload, unsigned int length) {
    char p[length + 1];
    memcpy(p, payload, length);
    p[length] = '\0';
    String message(p);
    Serial.write(payload, length);
    Serial.println(topic);
}

void reconnect() {
    // Loop until reconnected
    while (!client.connected()) {
        Serial.println("Attempting MQTT connection...");
        // Attempt to connect
        if (client.connect(MQTT_CLIENT_NAME, TOKEN, "")) {
            Serial.println("Connected");
        } else {
            Serial.print("Failed, rc=");
            Serial.print(client.state());
            Serial.println(" try again in 2 seconds");
            // Wait 2 seconds before retrying
            delay(2000);
        }
    }
}

/****************************************
 * Main Functions
 ****************************************/
void setup() {
    Serial.begin(115200);
    WiFi.begin(WIFISSID, PASSWORD);
    Serial.print("Wait for WiFi...");
    while (WiFi.status() != WL_CONNECTED) {
        Serial.print(".");
        delay(500);
    }
    Serial.println("\nWiFi Connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
    client.setServer(mqttBroker, 1883);
    client.setCallback(callback);
    dht1.begin(); // Initialize temperature and humidity sensor
}

void loop() {
    if (!client.connected()) {
        reconnect();
    }

    Serial.println("\nSensor data:");

    // Publish temperature
    sprintf(topic, "%s%s", "/v1.6/devices/", DEVICE_LABEL);
    sprintf(payload, ""); // Clean the payload
    sprintf(payload, "{\"%s\":", VARIABLE_LABEL_temp); // Add temperature variable label
    float t1 = dht1.readTemperature();
    Serial.print(" 1. Temperature: ");
    Serial.println(t1);
    dtostrf(t1, 4, 2, str_temp); // Convert temperature to string
    sprintf(payload, "%s {\"value\": %s", payload, str_temp); // Format payload
    sprintf(payload, "%s } }", payload); // Close payload
    Serial.println(" Sending temperature to Ubidots via MQTT...");
    client.publish(topic, payload);

    Serial.println();

    // Publish humidity
    sprintf(topic, "%s%s", "/v1.6/devices/", DEVICE_LABEL);
    sprintf(payload, ""); // Clean the payload
    sprintf(payload, "{\"%s\":", VARIABLE_LABEL_hum); // Add humidity variable label
    float h1 = dht1.readHumidity();
    Serial.print(" 2. Humidity: ");
    Serial.println(h1);
    dtostrf(h1, 4, 2, str_hum); // Convert humidity to string
    sprintf(payload, "%s {\"value\": %s", payload, str_hum); // Format payload
    sprintf(payload, "%s } }", payload); // Close payload
    Serial.println(" Sending humidity to Ubidots via MQTT...");
    client.publish(topic, payload);

    client.loop();

    Serial.println("\nWaiting 12 seconds to read sensors again...");
    Serial.println(".......................................................");
    delay(12000); // 12 seconds between Ubidots publications
}
