#include <PubSubClient.h>
#define MQTT_VERSION MQTT_VERSION_3_1_1
#include <ArduinoJson.h>
// MQTT: ID, server IP, port, username and password
const PROGMEM char* MQTT_CLIENT_ID = "Corbon Monoxide";
const PROGMEM char* MQTT_SERVER_IP = "192.168.0.16";
const PROGMEM uint16_t MQTT_SERVER_PORT = 1883;
const PROGMEM char* MQTT_USER = "homeassistant";
const PROGMEM char* MQTT_PASSWORD = "welcome";

// MQTT: topics
const char* MQTT_LIGHT_STATE_TOPIC = "sensor/corbon_monoxide";
extern WiFiClient wifiClient;
PubSubClient client(wifiClient);

// function called to publish the state of the light (on/off)
void publishLightState(float co) {
    StaticJsonBuffer<200> jsonBuffer;
    JsonObject& root = jsonBuffer.createObject();
    root["ppm"] = (String) co+" ppm"; // u000B0 is degree symbol
    char data[200];
    root.printTo(data, root.measureLength() +1);
    client.publish(MQTT_LIGHT_STATE_TOPIC, data, true);
}



void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.println("INFO: Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect(MQTT_CLIENT_ID, MQTT_USER, MQTT_PASSWORD)) {
      Serial.println("INFO: connected");
      publishLightState(00.00);
    } else {
      Serial.print("ERROR: failed, rc=");
      Serial.print(client.state());
      Serial.println("DEBUG: try again in 5 seconds");
      yield();
    }
  }
}

void hassioMqttSetup() {
  // init the MQTT connection
  client.setServer(MQTT_SERVER_IP, MQTT_SERVER_PORT); 
}

void hassioMqttloop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
}
