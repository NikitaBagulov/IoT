#include "Config.h"
#include "wifi.h"
#include "server.h"
#include "MQTT.h"
int tries = 0;
void setup() {
  Serial.begin(9600);
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);
  bool is_wifi_on = init_WIFI(true);
  if (is_wifi_on){
    server_init();
  }
}
void loop() {
  if (flag){
    init_MQTT();
    String topic = "kate/command";
    mqtt_client.subscribe(topic.c_str());
    flag=false;
  }
  server.handleClient();
 if (!wifi_on && wifiMulti.run() != WL_CONNECTED){
    Serial.println("Подключение к Wi-Fi...");
      tries++;
      if (tries >= 5) {
        server.close();
        init_WIFI(true);
        server_init();
        tries=0;
      }
 }

  
  mqtt_client.loop();
  delay(10);
}
