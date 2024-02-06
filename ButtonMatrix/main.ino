#define PULLUP_PIN2 2
#define PULLUP_PIN3 3
#define CHANGING_PIN4 4
#define CHANGING_PIN5 5
int btn = 0;
int pins[] = {2, 3};
int states[2][2]={{1, 0}, {0, 1}};
  
void setup()
{
  pinMode(PULLUP_PIN2, INPUT_PULLUP);
  pinMode(PULLUP_PIN3, INPUT_PULLUP);
  pinMode(CHANGING_PIN4, OUTPUT);
  pinMode(CHANGING_PIN5, OUTPUT);
  Serial.begin(9600);
}

void loop()
{
  
  check_state();
}

void check_state(){
  Serial.print("Buttons: ");
  for (int i=0; i< 2; i++){
    for (int j=0; j<2; j++){
      	btn++;
    	check_button(pins[i], j);
      Serial.print(" ");
      	delay(100);
    }
  }
  Serial.println("");
  btn=0;
}
void check_button(int pullup_pin, int state){
	digitalWrite(CHANGING_PIN4, states[state][0]);
	digitalWrite(CHANGING_PIN5, states[state][1]);
  	if (digitalRead(pullup_pin) == LOW){
      	Serial.print(btn);
  }
}
