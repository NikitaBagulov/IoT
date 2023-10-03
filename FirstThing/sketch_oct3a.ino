#define led_pin 13

void setup() {
  pinMode(led_pin, OUTPUT);
  Serial.begin(9600);

}

void loop() {
  if (Serial.available() > 0){
    char cmd = Serial.read();
    if (cmd == 'u'){ //u for up/on
      digitalWrite(led_pin, HIGH); //use LOW on
      Serial.print("LED on");
    }
    if (cmd == 'd'){
      digitalWrite(led_pin, LOW);// d for down/off
      Serial.print("LED off");
    }

  }

}
