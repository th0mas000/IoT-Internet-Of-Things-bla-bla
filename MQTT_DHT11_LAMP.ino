#include <WiFi.h>
#include <PubSubClient.h>
#include <Wire.h>
#include "DHT.h"
const char* ssid = "NICO";
const char* password = "twk14169";
//MQTT broker
const char* mqtt_server = "mqtt-dashboard.com";
WiFiClient espClient;
PubSubClient client(espClient);
//DHT Sensor type
#define DHT_SENSOR_TYPE DHT_TYPE_11
//#define DHT_SENSOR_TYPE DHT_TYPE_21
//#define DHT_SENSOR_TYPE DHT_TYPE_22
// LED Pin
#define DHTPIN 2
#define DHTTYPE DHT11   // DHT 22  (AM2302), AM2321
//#define DHTTYPE DHT21   // DHT 21 (AM2301)
DHT dht(DHTPIN, DHTTYPE);
#define ledPin  4
#define FPin  5
long lastMsg = 0;
char msg[50];
int value = 0;
int valueF = 0;
String lamp = "";
void setup() {
  Serial.begin(115200);
  // delete old config
  WiFi.disconnect(true);
  pinMode(FPin, INPUT);
  delay(1000);
  // WiFi.onEvent(WiFiStationConnected, SYSTEM_EVENT_STA_CONNECTED);
  // WiFi.onEvent(WiFiGotIP, SYSTEM_EVENT_STA_GOT_IP);
  // WiFi.onEvent(WiFiStationDisconnected, SYSTEM_EVENT_STA_DISCONNECTED);
  /* Remove WiFi event
      Serial.print("WiFi Event ID: ");
      Serial.println(eventID);
      WiFi.removeEvent(eventID);*/
  WiFi.begin(ssid, password);
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
  pinMode(ledPin, OUTPUT);
  dht.begin();
  Serial.println();
  Serial.println();
  Serial.println("Wait for WiFi... ");
}
void loop() {
  if (!client.connected()) {
    reconnect_mqtt();
  }
  client.loop();
  long now = millis();
  valueF = digitalRead(FPin);
  // Serial.println(valueF);
  if (now - lastMsg > 10000) {
    lastMsg = now;
    float temperature = dht.readTemperature();
    float humidity = dht.readHumidity();
    // Temperature in Celsius
    char FString[8];
    dtostrf(valueF, 1, 2, FString);
    char tempString[8];
    dtostrf(temperature, 1, 2, tempString);
    char humString[8];
    dtostrf(humidity, 1, 2, humString);
    char strbuf[100];
    sprintf(strbuf, "{\"temperature\": %s, \"humidity\": %s, \"flame\": %s}", tempString, humString, FString);
    if (valueF == 0){
      digitalWrite(ledPin, HIGH);
    } else {
      digitalWrite(ledPin, LOW);
    }
    Serial.println(strbuf);
    client.publish("nicky/temp", strbuf);
  }
}
void callback(char* topic, byte* message, unsigned int length) {
  Serial.print("Message arrived on topic: ");
  Serial.print(topic);
  Serial.print(". Message: ");
  String messageTemp;
  for (int i = 0; i < length; i++) {
    Serial.print((char)message[i]);
    messageTemp += (char)message[i];
  }
  Serial.println();
  // Feel free to add more if statements to control more GPIOs with MQTT
  // If a message is received on the topic esp32/output, you check if the message is either "on" or "off".
  // Changes the output state according to the message

  if (String(topic) == "nicky/lamp") {
    lamp = String(messageTemp);
    if (lamp == "on"){
      digitalWrite(ledPin, HIGH);
    } else {
      digitalWrite(ledPin, LOW);
    }
    
    // delay(1000);
    
  }

}
void reconnect_mqtt() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    Serial.println(millis());
    // Attempt to connect
    if (client.connect("NickyClient")) {
      Serial.println("connected");
      // Subscribe
      client.subscribe("nicky/lamp");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      Serial.print("RRSI: ");
      Serial.println(WiFi.RSSI());
      Serial.println();
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}
// void WiFiStationConnected(WiFiEvent_t event, WiFiEventInfo_t info) {
//   Serial.println("Connected to AP successfully!");
// }
// void WiFiGotIP(WiFiEvent_t event, WiFiEventInfo_t info) {
//   Serial.println("WiFi connected");
//   Serial.println("IP address: ");
//   Serial.println(WiFi.localIP());
// }
// void WiFiStationDisconnected(WiFiEvent_t event, WiFiEventInfo_t info) {
//   Serial.println("Disconnected from WiFi access point");
//   Serial.print("WiFi lost connection. Reason: ");
//   Serial.println(info.disconnected.reason);
//   Serial.println("Trying to Reconnect");
//   WiFi.begin(ssid, password);
// }