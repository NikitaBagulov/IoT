#define cw_pin1 9 
#define cw_pin2 10 
#define ccw_pin2 11
#define ccw_pin1 8

void setup()
{
  
  pinMode(cw_pin1, OUTPUT);
  pinMode(cw_pin2, OUTPUT);
  pinMode(ccw_pin1, OUTPUT);
  pinMode(ccw_pin2, OUTPUT);
  Serial.begin(9600);
  Serial.println("Enter direction and speed (cw 40 or cc 50):");
}

void loop()
{
  if (Serial.available() > 0) {
    motor_control();
  }
  
}

void motor_control() {
  String input = Serial.readStringUntil('\n');
  String direction = input.substring(0, 2);
  int speed = input.substring(3).toInt();
  
  if (speed < 0 || speed > 255) {
    Serial.println("Invalid speed! (max 255)");
    return;
  }

  if (direction == "cw") {
    digitalWrite(cw_pin1, HIGH);
    digitalWrite(cw_pin2, HIGH);
    digitalWrite(ccw_pin1, LOW); 
    digitalWrite(ccw_pin2, LOW); 
    
    analogWrite(cw_pin1,speed);
  	analogWrite(cw_pin2,speed);

  } else if (direction == "cc"){
 	digitalWrite(ccw_pin1, HIGH);
    digitalWrite(ccw_pin2, HIGH);
    digitalWrite(cw_pin1, LOW); 
    digitalWrite(cw_pin2, LOW); 
    analogWrite(ccw_pin1,speed);
  	analogWrite(ccw_pin2,speed);

  } else {
  	Serial.println("Invalid direction!");
  }
}

