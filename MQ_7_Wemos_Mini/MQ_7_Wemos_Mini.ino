#ifdef ARDUINO_ARCH_ESP32
#include <WiFi.h>
#else
#include <ESP8266WiFi.h>
#endif
#include "MQ7.h"
#include "hassio_mqtt.h"
#define BLYNK_PRINT Serial
#include <BlynkSimpleEsp8266.h>

MQ7 mq7(A0,5.0);
boolean wifiConnected = false;
WiFiClient wifiClient;
//##################################################
char auth[] = "auth_code";
BlynkTimer timer;
//##################################################
// Change this!!
const char* ssid = "ssid name";
const char* password = "ssid passwod";
//##################################################
//##################################################
void myTimerEvent()
{
  // You can send any value at any time.
  // Please don't send more that 10 values per second.
  Blynk.virtualWrite(V5, mq7.getPPM());
}


void setup() {
    Serial.begin(115200);
    // Initialise wifi connection
    wifiConnected = connectWifi();
    hassioMqttSetup();
    Blynk.begin(auth, ssid, password);
    timer.setInterval(1000L, myTimerEvent);
}

void loop() {
    if(WiFi.status() != WL_CONNECTED){
      connectWifi();
    }else{
      float co= mq7.getPPM();
      Serial.println(co);
      publishLightState(co);
      delay(3000);
    }
    hassioMqttloop();
    Blynk.run();
    timer.run(); // Initiates BlynkTimer
}

// connect to wifi – returns true if successful or false if not
boolean connectWifi(){
  boolean state = true;
  int i = 0;

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("");
  Serial.println("Connecting to WiFi");

  // Wait for connection
  Serial.print("Connecting...");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    if (i > 20){
      state = false; break;
    }
    i++;
  }
  Serial.println("");
  if (state){
    Serial.print("Connected to ");
    Serial.println(ssid);
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
  }
  else {
    Serial.println("Connection failed.");
  }
  return state;
}
