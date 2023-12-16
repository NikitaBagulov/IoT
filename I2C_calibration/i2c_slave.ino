#include <Wire.h>
#define sensor_pin A0
#define NUM_STEPS 20
int val = 0;
volatile bool newData = false;
int master_val = 0;

int masterValues[NUM_STEPS]; 
int slaveValues[NUM_STEPS];  
int numSamplesCollected = 0;
float conversionFactor = 1.0;
int prev = 0;
bool calibrationStart = false;
bool calibrationInProgress = false;
bool calibrationComplete = false;

bool containsValue(int arr[], int length, int targetValue) {
  for (int i = 0; i < length; i++) {
    if (arr[i] == targetValue) {
      return true;
    }
  }
  return false;
}

float calculateAverage(int arr[], int length) {
  float total = 0.0;
  for (int i = 0; i < length; i++) {
    total += arr[i];
  }
  return total / length;
}

void calibrateSensor() {
  if (!calibrationInProgress) {
    Serial.println("Calibration started...");
    calibrationInProgress = true;
    numSamplesCollected = 0;
    masterValues[numSamplesCollected] = master_val;
    slaveValues[numSamplesCollected] = analogRead(A0);
    numSamplesCollected++;
  }
  int start_time = millis();
  if (start_time - prev > 1000 && numSamplesCollected < NUM_STEPS) {
    int masterReading = master_val;
    int slaveReading = analogRead(A0);

    if (!containsValue(masterValues, sizeof(masterValues) / sizeof(masterValues[0]), masterReading) && !containsValue(slaveValues, sizeof(slaveValues) / sizeof(slaveValues[0]), slaveReading)) {
      masterValues[numSamplesCollected] = master_val;
      slaveValues[numSamplesCollected] = analogRead(A0);
      Serial.println("Add data");
      numSamplesCollected++;
      prev = millis();
    }
  } else if (numSamplesCollected >= NUM_STEPS) {
    calibrationInProgress = false;
    calibrationComplete = true;
    Serial.println("Calibration complete");
    float masterAvg = calculateAverage(masterValues, NUM_STEPS);
    float slaveAvg = calculateAverage(slaveValues, NUM_STEPS);
    if (masterAvg != 0 && slaveAvg != 0) {
      conversionFactor = masterAvg / slaveAvg;
    }
  }
}

void setup() {
  Serial.begin(9600);
  pinMode(LED_BUILTIN, OUTPUT);
  Wire.begin(8);  // join I2C bus with address #8
  Wire.onReceive(receiveEvent);
  Wire.onRequest(requestEvent);
}

void loop() {
  if (Serial.available() > 0) {
    char c = Serial.read();
    if (c == 'c') {
      calibrationStart = true;
    }
  }
  if (calibrationStart && !calibrationComplete) {
    calibrateSensor();
    digitalWrite(LED_BUILTIN, HIGH);  // turn the LED on (HIGH is the voltage level)
    delay(200);                      // wait for a second
    digitalWrite(LED_BUILTIN, LOW);   // turn the LED off by making the voltage LOW
    delay(200);
  }
  if (calibrationComplete) {
    digitalWrite(LED_BUILTIN, HIGH);
    int sensorReading = analogRead(A0);
    int calibratedReading = int(sensorReading * conversionFactor);
    Serial.print("Slave: ");
    Serial.print(sensorReading);
    Serial.print(" | SlaveCalibrated: ");
    Serial.println(calibratedReading);

  } else {
    val = analogRead(sensor_pin);
    Serial.print("Slave:");
    Serial.print(val);
    if (newData) {
      Serial.print(" | Master:");
      Serial.println(master_val);
      newData = false;
    }
  }
  delay(1000);
}

void receiveEvent(int byteCount) {
  if (byteCount >= sizeof(master_val)) {  // loop through all but the last
    Wire.readBytes((uint8_t*)&master_val, sizeof(master_val));
    newData = true;  // print the character
  }
}

void requestEvent() {
  if (calibrationComplete) {
    Wire.write("t");
  } else {
    Wire.write("f");
  }
}
