//sydney
#include "WiFi.h"
#include "secrets.h"
#include <WiFiClientSecure.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
 
#define AWS_IOT_PUBLISH_TOPIC   "esp32/pub"
#define AWS_IOT_SUBSCRIBE_TOPIC "esp32/sub"
 
float rawvalue ;
float tvoc;

WiFiClientSecure net = WiFiClientSecure();
PubSubClient client(net);

void connectAWS() {
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.println("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  // Configure WiFiClientSecure to use the AWS IoT device credentials
  net.setCACert(AWS_CERT_CA);
  net.setCertificate(AWS_CERT_CRT);
  net.setPrivateKey(AWS_CERT_PRIVATE);
  // Connect to the MQTT broker on the AWS endpoint we defined earlier
  client.setServer(AWS_IOT_ENDPOINT, 8883);
  // Create a message handler
  client.setCallback(messageHandler);
  Serial.println("Connecting to AWS IOT");
  while (!client.connect(THINGNAME))  {
    Serial.print(".");
    delay(100);
  }
  if (!client.connected())  {
    Serial.println("AWS IoT Timeout!");
    return;
  }
  // Subscribe to a topic
  client.subscribe(AWS_IOT_SUBSCRIBE_TOPIC);
  Serial.println("AWS IoT Connected!");
}
 
void publishMessage() {
  StaticJsonDocument<200> doc;
  doc["Raw Value"] = rawvalue;
  doc["TVOC"] = tvoc;
  char jsonBuffer[512];
  serializeJson(doc, jsonBuffer); // print to client
  client.publish(AWS_IOT_PUBLISH_TOPIC, jsonBuffer);
}
 
void messageHandler(char* topic, byte* payload, unsigned int length)  {
  Serial.print("incoming: ");
  Serial.println(topic);
  StaticJsonDocument<200> doc;
  deserializeJson(doc, payload);
  const char* message = doc["message"];
  Serial.println(message);
}

void setup() {
  Serial.begin(115200);
  pinMode(36, INPUT);
  
  // Attempt to connect to WiFi and AWS IoT continuously
  while (true) {
    connectToWiFi();
    if (WiFi.status() == WL_CONNECTED) {
      connectAWS();
      if (client.connected()) {
        break; // Break the loop if successfully connected to WiFi and AWS IoT
      }
    }
    delay(1000); // Delay before retrying
  }
}

void connectToWiFi() {
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.println("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nConnected to Wi-Fi");
}

void loop() {
  if (!client.connected()) {
    reconnectAWS(); // Reconnect to AWS IoT if the connection is lost
  }
  rawvalue = analogRead(36);
  tvoc = map(rawvalue,  100, 2880, 0, 100);
  Serial.print(F("Raw Value: "));
  Serial.print(rawvalue);
  Serial.print(F("  TVOC: "));
  Serial.println(tvoc);
  publishMessage();
  client.loop();
  delay(1000);
}

void reconnectAWS() {
  while (!client.connected()) {
    if (WiFi.status() != WL_CONNECTED) {
      connectToWiFi(); // Reconnect to WiFi if the connection is lost
    }
    connectAWS(); // Reconnect to AWS IoT
    delay(1000); // Delay before retrying
  }
}
