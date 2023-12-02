const int led_pin = 13;
const int sensor_pin = A0;
float V;

const int trig = 9;
const int echo = 10;
float slope = 0;
float intercept = 0;
int lower = 0;
int upper = 0;
bool flag = false;

  // int data

void setup() {
  Serial.begin(9600);
  pinMode(trig, OUTPUT);
  pinMode(echo, INPUT);
  Serial.println("Введите диапазон:");
  while (lower == 0) {
    if (Serial.available() > 0) {
      lower = Serial.parseInt();
    }
  }
  while (upper == 0) {
    if (Serial.available() > 0) {
      upper = Serial.parseInt();
    }
  }
}

void loop() {
  // Serial.println(get_dist());
  // V = analogRead(sensor_pin) * 5 / 1024;
  // Serial.println(analogRead(sensor_pin));
  // delay(1000);
  Serial.println(lower);
  Serial.println(upper);
  if (flag == false) {
    calibrate(lower, upper);
  } else {
    Serial.println("Distance real: ");
    Serial.println(get_dist());
    distance();
    delay(1000);
  }
}

void calibrate(int lower, int upper) {
  const int requiredDataPercentage = 90;
  const int maxGap = 2;

  bool enoughData = false;
  int dataCount = 0;
  int dataX[100];
  float dataY[100];  // Массив для хранения данных
  int index = 0;

  digitalWrite(led_pin, HIGH);

  while (!enoughData) {
    int sensorValue = analogRead(sensor_pin) * 5 / 1024;
    float distance = get_dist();  // Преобразуем значение сенсора в расстояние
    // Serial.println("gtdist");
    // Serial.println(distance);
    if (distance >= lower && distance <= upper) {
      if (index == 0 || (distance - dataY[index - 1] <= maxGap)) {
        dataY[index] = distance;
        dataX[index] = sensorValue;
          index++;
          dataCount++;
          Serial.println("dataCount");
      }
    }

    if (dataCount > 0 && (dataCount * 100) / (upper - lower + 1) >= requiredDataPercentage) {
      enoughData = true;
    }

    delay(500);  // Задержка для сбора данных
  }
  digitalWrite(led_pin, LOW);  // Выключаем светодиод после сбора данных

  buildModel(dataX, dataY, dataCount);
}

float calculateSlope(int x[], float y[], int n) {
  float sumX = 0, sumY = 0, sumXY = 0, sumX2 = 0;

  for (int i = 0; i < n; ++i) {
    sumX += x[i];
    sumY += y[i];
    sumXY += x[i] * y[i];
    sumX2 += x[i] * x[i];
  }

  float slope = (n * sumXY - sumX * sumY) / (n * sumX2 - sumX * sumX);
  return slope;
}

float calculateIntercept(int x[], float y[], int n, float slope) {
  float sumX = 0, sumY = 0;

  for (int i = 0; i < n; ++i) {
    sumX += x[i];
    sumY += y[i];
  }

  float intercept = (sumY - slope * sumX) / n;
  return intercept;
}

void buildModel(int signalValues[], float distanceValues[], int dataSize) {
  slope = calculateSlope(signalValues, distanceValues, dataSize);
  intercept = calculateIntercept(signalValues, distanceValues, dataSize, slope);
  Serial.print("Модель: y = ");
  Serial.print(slope);
  Serial.print(" * x + ");
  Serial.println(intercept);
  flag = true;
}

float get_dist() {
  digitalWrite(trig, LOW);
  delayMicroseconds(2);
  digitalWrite(trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig, LOW);
  // Wait ultrasonic pulse to return
  unsigned long duration = pulseIn(echo, HIGH);
  // Calculate the distance in centimeters using the speed of sound
  // (343 meters per second or 0.0343 centimeters per microsecond)
  float distance_cm = duration * 0.0343 / 2.0;
  // Serial.print("Distance: ");
  // Serial.print(distance_cm);
  // Serial.println("Distance real: ");
  // Serial.println(distance_cm);
  return distance_cm;
}

void distance() {
  int sensorValue = analogRead(sensor_pin) * 5 / 1024;
  float dist_cm = sensorValue * slope + intercept;
  Serial.println("Distance:");
  Serial.println(dist_cm);
}
