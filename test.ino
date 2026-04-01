#define M1E 5 // deytisme 
#define M1A 1
#define M1B 2

#define M2E 6 // deyisme 
#define M2A 3
#define M2B 4



void setup() {
  
  pinMode(M1A, OUTPUT);
  pinMode(M1B, OUTPUT);
  pinMode(M1E, OUTPUT);

  pinMode(M2A, OUTPUT);
  pinMode(M2B, OUTPUT);
  pinMode(M2E, OUTPUT);

  // move(100, 100, 100);  // 100 ms maksimum suretle ireli
  // move(0, 0, 1); //  stop kodu
  // move(-100, -100, 100); // 100ms maksimum suretle geri
  move(10, 10, 1000);
  move(0, 0, 10000000);
}

void loop() {
  // put your main code here, to run repeatedly:

}



void move(int Lspeed, int Rspeed, int Time) {
  int LspeedValue = map(Lspeed, -100, 100, 0, 255);
  int RspeedValue = map(Rspeed, -100, 100, 0, 255);

  LspeedValue = constrain(LspeedValue, 0, 255);
  RspeedValue = constrain(RspeedValue, 0, 255);
  
  if (Lspeed == 0) {
    digitalWrite(M1E, 0);
    digitalWrite(M1A, 0);
    digitalWrite(M1B, 0);
  } else if (Lspeed > 0) {
    analogWrite(M1E, LspeedValue);
    digitalWrite(M1A, 1);
    digitalWrite(M1B, 0);  
  } else if (Lspeed < 0) {
    analogWrite(M1E, LspeedValue);
    digitalWrite(M1A, 0);
    digitalWrite(M1B, 1);
  }  
  
  if (Rspeed == 0) {
    digitalWrite(M2E, 0);
    digitalWrite(M2A, 0);
    digitalWrite(M2B, 0);
  } else if (Rspeed > 0) {
    analogWrite(M2E, RspeedValue);
    digitalWrite(M2A, 1);
    digitalWrite(M2B, 0);  
  } else if (Rspeed < 0) {
    analogWrite(M2E, RspeedValue);
    digitalWrite(M2A, 0);
    digitalWrite(M2B, 1);
  }

  delay(Time);
}