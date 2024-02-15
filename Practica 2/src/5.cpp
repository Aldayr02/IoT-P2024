#include <WiFi.h>
#include <WiFiClient.h>

const char* ssid5 = "Mi_WiFi";
const char* password5 = "Mi_Password";

WiFiServer server5(80);

// Usar el pin 15 GPIO
#define LED 15

String estado5 = "";

void setup_5() {
    Serial.begin(115200);
    pinMode(LED, OUTPUT);
    digitalWrite(LED, LOW);
    // Conectando a WiFi
    WiFi.begin(ssid5, password5);
    // Checa si esta conectado
    while (WiFi.status() != WL_CONNECTED) {
        delay(1000);
        Serial.print(".");
    }
    Serial.println("");
    Serial.println("WiFi conectado");
    Serial.print("La dirección IP es: ");
    Serial.println(WiFi.localIP()); // Muestra IP
    // Inicio del Servidor web.
    server5.begin();
    Serial.println("Servidor web iniciado.");
}

void external_led_webserver() {
    // Consulta si se ha conectado algún cliente.
    WiFiClient client = server5.available();
    if (!client) {
        return;
    }
    Serial.print("Nuevo cliente: ");
    Serial.println(client.remoteIP());
    // Espera hasta que el cliente envie datos.
    while (!client.available()) {
        delay(1);
    }
    // Lee la información enviada por el cliente.
    String req = client.readStringUntil('\r');
    Serial.println(req);
    // Realiza la petición del cliente.
    if (req.indexOf("on2") != -1) {
        digitalWrite(LED, HIGH);
        estado5 = "Encendido";
    } else if (req.indexOf("off2") != -1) {
        digitalWrite(LED, LOW);
        estado5 = "Apagado";
    }
    // Página WEB.
    client.println("HTTP/1.1 200 OK");
    client.println("Content-Type: text/html");
    client.println(""); // Importante.
    client.println("<!DOCTYPE HTML>");
    client.println("<html>");
    client.println("<head><meta charset=utf-8></head>");
    client.println("<body><center><font face='Arial'>");
    client.println("<h1>Servidor web con ESP32.</h1>");
    client.println("<h1>Guadalupe Torres</h1>");
    client.println("<h2><font color='#009900'>Webserver 1.0</font></h2>");
    client.println("<h3>Práctica botón</h3>");
    client.println("<br><br>");
    client.println("<a href='on2'><button>Enciende LED</button></a>");
    client.println("<a href='off2'><button>Apaga LED</button></a>");
    client.println("<br><br>");
    client.println(estado5);
    client.println("</font></center></body></html>");
    Serial.println("Cliente desconectado.");
    Serial.println(client.remoteIP());
    client.flush();
    client.stop();
}
