#define PIN_1 A4
#define PIN_2 A5
#define PIN_3 A6
#define PIN_4 A7

// define on and off readings
#define OFF 1023
#define ON 0

void setup() {
  pinMode(PIN_1, INPUT);
  pinMode(PIN_2, INPUT);
  pinMode(PIN_3, INPUT);
  pinMode(PIN_4, INPUT);

  Serial.begin(9600);
}

void loop() {
  int bit0 = analogRead(PIN_4) == ON;
  int bit1 = analogRead(PIN_3) == ON;
  int bit2 = analogRead(PIN_2) == ON;
  int bit3 = analogRead(PIN_1) == ON;
  int tactic = bit0 * 1 + bit1 * 2 + bit2 * 4 + bit3 * 8;
  Serial.println(tactic);
  delay(200);
}
