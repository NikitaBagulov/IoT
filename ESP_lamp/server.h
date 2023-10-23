#include <ESP8266WebServer.h>

ESP8266WebServer server(WEB_SERVER_PORT);
unsigned char* buf1 = new unsigned char[100];
unsigned char* buf2 = new unsigned char[100];
void handle_root(){
  String html = "<html><body>";
  html += "<h1>Wi-Fi lamp settings</h1>";
  html += "<form action='/save' method='post'>";
  html += "SSID:<br><input type='text' name='ssid'><br>";
  html += "Password:<br><input type='password' name='password'><br>";
  html += "<input type='submit' value='Save'></form>";
  html += "</body></html>";
  server.send(200, "text/html", html);
}

void handle_save(){
  if (server.hasArg("ssid") && server.hasArg("password")) {
    String ssid = server.arg("ssid").c_str();
    String password = server.arg("password").c_str();
    ssid.getBytes(buf1 ,100, 0);
    password.getBytes(buf2 ,100, 0);
    Serial.print(ssid);
    Serial.print(password);
    CLI_SSID = (char*)buf1;
    CLI_PASS = (char*)buf2;
    delete[] buf1;
    delete[] buf2;

    start_client_mode();
    if(wifiMulti.run() == WL_CONNECTED){
      flag=true;
      WiFi.softAPdisconnect();
      wifi_on = false;
    }
    server.send(200, "text/plain", "Wi-Fi credentials saved!");
  } else {
    server.send(400, "text/plain", "Bad Request");
  }
}


void handle_not_found(){
  server.send(404, "text/html", "404: check URL");
}

void server_init(){
  server.on("/", HTTP_GET, handle_root);
  server.on("/save", HTTP_POST, handle_save);
  server.onNotFound(handle_not_found);

  server.begin();
  Serial.print("Server start on port: ");
  Serial.println(WEB_SERVER_PORT);
}