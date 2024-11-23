#include <DFRobot_DHT11.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

DFRobot_DHT11 DHT;
#define DHT11_PIN 12

const char* ssid = "----";
const char* password = "-----";

ESP8266WebServer server(8080);

void setup() {
  Serial.begin(115200);
  Serial.print("Connecting to: ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while(WiFi.status() != WL_CONNECTED){
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  server.on("/", handleRoot);
  server.begin();
}
//DHT.read(DHT11_PIN);
  /*Serial.print("temp:");
  Serial.print(DHT.temperature);
  Serial.print("  humi:");
  Serial.println(DHT.humidity);
  delay(1000);*/
void loop() {
  server.handleClient();
}
void handleRoot(){
  DHT.read(DHT11_PIN);
  float humidity = DHT.humidity;
  Serial.println(humidity);
  float temp = DHT.temperature;
  Serial.println(temp);
  if(isnan(humidity) || isnan(temp)){
    server.send(500, "text/plain", "Failed to read from DHT sensor");
    return;
  }
  String html = "<html><body>";
  html += "<h1>ESP8266 Web Server</h1>";
  html += "<p>Temperature: " + String(temp) + " &deg;C</p>";
  html += "<p>Humidity: " + String(humidity) + " %</p>";
  html += "</body></html>";

  // Send the HTML response
  server.send(200, "text/html", html);
}
