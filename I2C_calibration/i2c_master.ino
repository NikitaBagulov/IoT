#include <Wire.h>
#define sensor_pin A0
bool flag = false;

void setup() {
  Serial.begin(9600);
  Wire.begin();
}

void loop() {
  int val = analogRead(sensor_pin);
  Serial.print("Master: ");
  Serial.println(val);
  Wire.requestFrom(8, 1);
  if (Wire.available()){
    char c = Wire.read();
    if (c == 'f')
    {
      Wire.beginTransmission(8); /* begin with device address 8 */
      Wire.write((uint8_t*)&val, sizeof(val));
      Wire.endTransmission();  /* stop transmitting */
      flag=true;
    }
    if (c == 't' && flag){
      Serial.println("Calibrate complete!");
      flag=false;
    }
  }
  delay(1000);
}
