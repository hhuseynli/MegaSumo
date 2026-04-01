#include <IRremote.hpp>
#include <QTRSensors.h>


// --START MODULE (embedded)--
#define START_MODULE 11

// --DIP SWITCHES FOR STRATEGY SELECTION (embedded)--
#define PIN_1 A4
#define PIN_2 A5
#define PIN_3 A6
#define PIN_4 A7

// --QTR SENSORS FOR DOHYO EDGE DETECTION--
#define RIGHT_QTR = A2;
#define LEFT_QTR = A3;

// --OPTICAL IR SENSORS FOR OPPONENT DETECTION-- 
// Named based on the direction of projection
#define LEFT_IR = 2;
#define FRONT_RIGHT_IR = 3;
#define FRONT_IR = 4;
#define FRONT_LEFT_IR = 7;
#define FRONT_RIGHT = 8; 

// --MOTORS--
const int M1E = 0;
const int M2E = 1;

const int M1A = 5;
const int M1B = 6;

const int M2A = 9;
const int M2B = 10;

// -- Parameters --
int startStatus = 0;
const int ON = 0; // reading of 0 corresponds to ON position in a dip switch
int mode;

QTRSensors qtr;
const uint8_t SENSOR_COUNT = 2;
uint16_t sensorValues[SENSOR_COUNT];
uint16_t minValues[SENSOR_COUNT];
uint16_t maxValues[SENSOR_COUNT];

// const int THRESHOLD = ;

struct Motor {
  int A;
  int B;
  int E;
};

const int MOTOR_COUNT = 2;
Motor motors[MOTOR_COUNT + 1] = {
  {-1, -1, -1},               // Index 0 not used
  {M1A, M1B, M1E},            // Motor 1
  {M2A, M2B, M2E}             // Motor 2
};

void setup() {
  Serial.begin(9600);
   
  pinMode(START_MODULE, INPUT);
  IrReceiver.begin(START_MODULE, ENABLE_LED_FEEDBACK); // Start receiver

  pinMode(PIN_1, INPUT);
  pinMode(PIN_2, INPUT);
  pinMode(PIN_3, INPUT);
  pinMode(PIN_4, INPUT);

  // Calculate strategy number based on dip switches
  int bit0 = analogRead(PIN_4) == ON;
  int bit1 = analogRead(PIN_3) == ON;
  int bit2 = analogRead(PIN_2) == ON;
  int bit3 = analogRead(PIN_1) == ON;
  mode = (bit0 * 1) + (bit1 * 2) + (bit2 * 4) + (bit3 * 8);
  Serial.println(mode);

  pinMode(RIGHT_QTR, INPUT);
  pinMode(LEFT_QTR, INPUT);
  qtr.setTypeAnalog();
  qtr.setSensorPins((const uint8_t[]){LEFT_QTR, RIGHT_QTR}, SENSOR_COUNT);  

  pinMode(LEFT_IR, INPUT);
  pinMode(FRONT_RIGHT_IR, INPUT);
  pinMode(FRONT_IR, INPUT);
  pinMode(FRONT_LEFT_IR, INPUT);
  pinMode(FRONT_RIGHT, INPUT);

  pinMode(M1A, OUTPUT);
  pinMode(M1B, OUTPUT);
  pinMode(M1E, OUTPUT);

  pinMode(M2A, OUTPUT);
  pinMode(M2B, OUTPUT);
  pinMode(M2E, OUTPUT);

}

void loop() {
 if (IrReceiver.decode()) {
    uint32_t irCode = IrReceiver.decodedIRData.decodedRawData;

    switch (irCode) {
      case 0xBA45FF00: 
        Serial.println("Pending");
        break;
      case 0xB946FF00:
        Serial.println("Start");
        moveMotor(1,25,true);
        moveMotor(2,255,true);
        delay(3000);
        stopAllMotors();
        break;
      case 0xB847FF00:
        Serial.println("Stop");
        stopAllMotors();
        break;
      default:
        Serial.print("Received code: 0x");
        Serial.println(irCode, HEX);
        break;
    }

    IrReceiver.resume(); // Prepare to receive next
  }
}

void moveMotor(int motorNumber, int motorSpeed, bool isForward){
  if (motorNumber < 1 || motorNumber > MOTOR_COUNT) {
    Serial.println("Warning: Inexistent motor used!");
    return; 
  }

  Motor M = motors[motorNumber];
  motorSpeed = constrain(motorSpeed, 0, 255);

  analogWrite(M.A, isForward ? motorSpeed : 0);
  analogWrite(M.B, isForward ? 0 : motorSpeed); 
  digitalWrite(M.E, HIGH);
}

void stopMotor(int motorNumber){
  if (motorNumber < 1 || motorNumber > MOTOR_COUNT) {
    Serial.println("Warning: Inexistent motor used!");
    return;
  }

  Motor M = motors[motorNumber];
  analogWrite(M.A, 0);
  analogWrite(M.B, 0);
  digitalWrite(M.E, LOW);
}

void stopAllMotors() {
  for (int i = 1; i <= MOTOR_COUNT; i++) {
    stopMotor(i);
  }
}





