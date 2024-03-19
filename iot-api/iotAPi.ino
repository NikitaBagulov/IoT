#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h> 
#include <ArduinoJson.h>

const char* ssid = "zababulin";
const char* password = "12345678";
const char* serverUrl = "http://192.168.215.164:8005";
String location = "room";
String deviceId = "komp111";
unsigned long lastUpdateTime = 0;
float totalValue = 0;
int valueCount = 0;

ESP8266WiFiMulti WiFiMulti;

void setup() {
  Serial.begin(9600);
  delay(100);

  WiFi.mode(WIFI_STA);
  WiFiMulti.addAP(ssid, password);
  Serial.print("Connecting to WiFi");

  while (WiFiMulti.run() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("WiFi connected");
}

void loop() {
  unsigned long currentTime = millis();
  if (currentTime - lastUpdateTime >= 10000) {
    lastUpdateTime = currentTime;
    if (sendGetRequest()) {
      float averageValue = calculateAverageValue();
      
      Serial.print("Average Value: ");
      Serial.println(averageValue);
    }
  }

  if (Serial.available() > 0) {
    float value = Serial.parseFloat();
    if (value != 0) {

      DynamicJsonDocument json(200);
      json["value"] = value;
      json["location"] = location;
      json["device_id"] = deviceId;
      json["timestamp"] = "2024-03-14 21:18:21.0";

      HTTPClient http;
      WiFiClient client;
      http.begin(client, String(serverUrl) + "/submit_temperature");
      http.addHeader("Content-Type", "application/json");
      Serial.println(json.as<String>());
      int httpCode = http.POST(json.as<String>());
      

      if (httpCode > 0) {
        String payload = http.getString();
        Serial.println("Server response: " + payload);
      } else {
        Serial.println(httpCode);
        Serial.println("Error on HTTP request");
      }



      http.end();
    }
  }
  delay(5000);
}
bool sendGetRequest() {
  HTTPClient http;
  WiFiClient client;
  
  if (http.begin(client, String(serverUrl) + "/temperature?location="+location)) {
    int httpCode = http.GET();
    
    if (httpCode == HTTP_CODE_OK) {
      String payload = http.getString();
      Serial.println("Server response: " + payload);
      
      
      DynamicJsonDocument doc(1024);
      deserializeJson(doc, payload);
      
      JsonArray values = doc["measurements"]["Values"];
      
      totalValue = 0;
      valueCount = values.size();
      for (int i = 0; i < valueCount; i++) {
        totalValue += values[i].as<float>();
      }
      
      http.end();
      return true;
    } else {
      Serial.println("Error on HTTP request");
    }
  } else {
    Serial.println("Failed to connect to server");
  }
  
  return false;
}
float calculateAverageValue() {
  if (valueCount == 0) {
    return 0;
  } else {
    return totalValue / valueCount;
  }
}
