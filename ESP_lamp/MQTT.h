#include <PubSubClient.h>

PubSubClient mqtt_client(wifiClient);

void callback(char *topic, byte *payload, unsigned int length){
  Serial.print("There is message on topic");
  Serial.println(topic);
  Serial.println("Mesage is: ");
  char cmd = (char) payload[0];
  if (cmd == 'd'){ //d for up/on
      digitalWrite(LED_BUILTIN, LOW);
      Serial.print("LED on");
    }
    if (cmd == 'u'){
      digitalWrite(LED_BUILTIN, HIGH);// u for down/off
      Serial.print("LED off");
    }
  Serial.println("");
}

bool init_MQTT(){
  mqtt_client.setServer(mqtt_broker, mqtt_port);//подключение к брокеру
  mqtt_client.setCallback(callback);
  while(!mqtt_client.connected()){
    String client_id = "esp8266" + id();
    if(mqtt_client.connect(client_id.c_str())){
      Serial.println("MQTT client connected with id: " + client_id);
    } 
  }
  return true;
}