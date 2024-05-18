int ledpins[] = {8, 9, 10, 11};
int groundpins[] = {2, 3};

void setup() {
  for (int i = 0; i < 4; i++) {
    pinMode(ledpins[i], OUTPUT);
  }
  for (int i = 0; i < 2; i++) {
    pinMode(groundpins[i], OUTPUT);
  }
}

void led(int pin2, int pin3, int pin8, int pin9, int pin10, int pin11) {
  digitalWrite(groundpins[0], pin2);
  digitalWrite(groundpins[1], pin3);

  digitalWrite(ledpins[0], pin8);
  digitalWrite(ledpins[1], pin9);
  digitalWrite(ledpins[2], pin10);
  digitalWrite(ledpins[3], pin11);
}

void delay_time(int milliseconds = 500) {
  unsigned long time = millis();
  while (millis() - time < milliseconds) {}
}

void blink(int pattern) {
  for (int i = 0; i < 8; i++) {
    digitalWrite(ledpins[i % 4], (pattern >> i) & 0x01);
  }
  delay_time();
}

void loop() {
  int time = 5;

  for (int pattern = 1; pattern <= 255; pattern++) {
    blink(pattern);
    delay_time(time);
  }

  led(1, 1, 0, 0, 0, 0);
  delay_time(time);
}
