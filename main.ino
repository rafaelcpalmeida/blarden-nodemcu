#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

/*Put your SSID & Password*/
const char* ssid = "";  // Enter SSID here
const char* password = "";  //Enter Password here

ESP8266WebServer server(80);

uint8_t LEDpin = D4;
uint8_t OUTSIDEpin = D7;
uint8_t GARAGEpin = D6;

void setup() {
  Serial.begin(115200);
  delay(100);

  pinMode(LEDpin, OUTPUT);
  pinMode(OUTSIDEpin, OUTPUT);
  pinMode(GARAGEpin, OUTPUT);

  digitalWrite(LEDpin, HIGH);

  connectWifi();

  server.on("/", handle_OnConnect);
  server.on("/outside-gate/7w4hTAYZzFwJrmxsu7C63gfE7vkLfs", handle_outside_gate);
  server.on("/garage-gate/7w4hTAYZzFwJrmxsu7C63gfE7vkLfs", handle_garage_gate);
  server.onNotFound(handle_NotFound);

  server.begin();
  Serial.println("HTTP server started");
}

void loop() {
  server.handleClient();
  connectWifi();
}

void connectWifi() {
  if (WiFi.status() == WL_CONNECTED) {
    return;
  }

  Serial.println("Connecting to ");
  Serial.println(ssid);

  //connect to your local wi-fi network
  WiFi.begin(ssid, password);

  //check wi-fi is connected to wi-fi network
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected..!");
  Serial.print("Got IP: ");  Serial.println(WiFi.localIP());
}

void handle_OnConnect() {
  server.send(200, "text/html", "ALIVE");

  digitalWrite(LEDpin, LOW);
  delay(1000);
  digitalWrite(LEDpin, HIGH);
  delay(1000);
}

void handle_outside_gate() {
  digitalWrite(OUTSIDEpin, HIGH);
  delay(1000);
  digitalWrite(OUTSIDEpin, LOW);
  server.send(200, "text/html", "OK");
}

void handle_garage_gate() {
  digitalWrite(GARAGEpin, HIGH);
  delay(1000);
  digitalWrite(GARAGEpin, LOW);
  server.send(200, "text/html", "OK");
}

void handle_NotFound() {
  server.send(404, "text/plain", "Not found");
}
