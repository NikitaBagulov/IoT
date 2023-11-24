// Latch pin (ST_CP) of 74HC595 to pin 5
int latchPin = 5;      
// Latch pin (SH_CP) of 74HC595 to pin 6
int clockPin = 3;
// Data pin (DS of 74HC595 to pin 6
int dataPin = 7;       
// Make sure MR is HIGH (connect ot Vcc)

// 2 3 4 5 6 7 8 9 
// A B . C D E G F
//bool digits[10][8] = {
  //{1,1,0,1,1,1,0,1},  // 0
  //{0,1,0,1,0,0,0,0},  // 1
  //{1,1,0,0,1,1,1,0},  // 2
  //{1,1,0,1,1,0,1,0},  // 3
  //{0,1,0,1,0,0,1,1},  // 4
  //{1,0,0,1,1,0,1,1},  // 5
  //{1,0,1,1,1,1,1,1},  // 6
  //{1,1,0,1,0,0,0,0},  // 7
  //{1,1,0,1,1,1,1,1},  // 8
  //{1,1,1,1,1,0,1,1}   // 9
//};
byte digits[10] = {
  B11011101, // 0
  B01010000, // 1
  B11001110, // 2
  B11011010, // 3
  B01010011, // 4
  B10011011, // 5
  B10111111, // 6
  B11010000, // 7
  B11011111, // 8
  B11111011  // 9
};

void setup() 
{
  pinMode(latchPin, OUTPUT);
  pinMode(dataPin, OUTPUT);  
  pinMode(clockPin, OUTPUT);
  digitalWrite(clockPin, LOW);
  //digitalWrite(latchPin, HIGH);
  for (int i=0;i<10;i++){
  	show_number(i);
    delay(500);
  }
  Serial.begin(9600);
}

void show_number(int digit) {
  digitalWrite(latchPin, LOW);
  if(digit <0 or digit >9) {
    return;
  }
  shiftOut(dataPin, clockPin, LSBFIRST, digits[digit]);
  //for(int i = 7; i >=0; i--) {
    //shift_and_set(digits[digit][i]);
    //}
  digitalWrite(latchPin, HIGH);
}

void loop() 
{
  if (Serial.available()) {
    char user_input = Serial.read(); 
    int digit = (int)(user_input - '0');
    show_number(digit);
  }
}


void shift_and_set(bool val) {
  digitalWrite(dataPin, val);
  digitalWrite(clockPin, HIGH);
  delay(10);
  digitalWrite(clockPin, LOW);
}
