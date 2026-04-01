#include <QTRSensors.h>

QTRSensors qtr;

const int LEFT_QTR = A3;
const int RIGHT_QTR = A2;

const int THRESHOLD = 875;

const uint8_t SENSOR_COUNT = 2;
uint16_t sensorValues[SENSOR_COUNT];

void setup() {
  pinMode(LEFT_QTR, INPUT);
  pinMode(RIGHT_QTR, INPUT);

  qtr.setTypeAnalog();
  qtr.setSensorPins((const uint8_t[]){LEFT_QTR, RIGHT_QTR}, SENSOR_COUNT);

  // Then start the calibration phase and move the sensors over both reflectance
  // extremes they will encounter in your application. This calibration should
  // take about 5 seconds (250 iterations * 20 ms per iteration).


  delay(500);
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH); // turn on Arduino's LED to indicate we are in calibration mode

  // analogRead() takes about 0.1 ms on an AVR.
  // 0.1 ms per sensor * 4 samples per sensor read (default) * sensorCount sensors
  // * 10 reads per calibrate() call = ~4 ms * sensorCount per calibrate() call.
  // Call calibrate() 2000/sensorCount times to make calibration take about 8 seconds.
  for (uint16_t i = 0; i < (2000/SENSOR_COUNT); i++)
  {
    // try on and off mode
    qtr.calibrate();
  }

  digitalWrite(LED_BUILTIN, LOW); // turn off Arduino's LED to indicate we are through with calibration
  Serial.begin(9600);

  for (uint8_t i = 0; i < SENSOR_COUNT; i++)
  {
    Serial.print(qtr.calibrationOn.minimum[i]);
    Serial.print(' ');
  }
  Serial.println();

  // print the calibration maximum values measured when emitters were on
  for (uint8_t i = 0; i < SENSOR_COUNT; i++)
  {
    Serial.print(qtr.calibrationOn.maximum[i]);
    Serial.print(' ');
  }
  Serial.println();
  Serial.println();
  delay(1000);
}

void loop() {
  // read calibrated sensor values and obtain a measure of the line position
  // from 0 to 5000 (for a white line, use readLineWhite() instead)
  uint16_t position = qtr.readLineBlack(sensorValues);
  
  // print the sensor values as numbers from 0 to 1000, where 0 means maximum
  // reflectance and 1000 means minimum reflectance, followed by the line
  // position
  for (uint8_t i = 0; i < SENSOR_COUNT; i++)
  {
    Serial.print(sensorValues[i]);
    Serial.print('\t');
  }
  Serial.println(position);
  delay(2500);
}
