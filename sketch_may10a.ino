// === Motor Pins ===
#define M1A_PIN 9
#define M1B_PIN 10
#define M1E_PIN 8

#define M2A_PIN 5
#define M2B_PIN 6
#define M2E_PIN 7

// === Sensor Pin ===
#define RF_PIN 2  // Front-right enemy sensor (digital)

// === Time Config ===
unsigned long startTime;
unsigned long lastEnemySeenTime = 0;

const int AGGRESSIVE_TIME = 3000;       // First 3 sec: aggressive forward
const int PUSH_DURATION = 1500;         // Max push duration
const int LOST_ENEMY_TIMEOUT = 500;     // Recovery if enemy disappears
const int MATCH_DURATION = 15000;       // Total match time = 15 seconds

bool isPushing = false;
bool matchOver = false;

void setup() {
  pinMode(RF_PIN, INPUT);

  pinMode(M1A_PIN, OUTPUT);
  pinMode(M1B_PIN, OUTPUT);
  pinMode(M1E_PIN, OUTPUT);

  pinMode(M2A_PIN, OUTPUT);
  pinMode(M2B_PIN, OUTPUT);
  pinMode(M2E_PIN, OUTPUT);

  stopMotors();
  startTime = millis();
}

void loop() {
  unsigned long currentTime = millis();

  if (digitalRead(startPin) == HIGH) {  // or HIGH depending on module behavior
    Serial.println("Start triggered!");
  // === Stop everything after 15 seconds ===
    if (currentTime - startTime > MATCH_DURATION) {
      if (!matchOver) {
        stopMotors();
        matchOver = true; // Only stop once
      }
      return;
    }

    bool enemyDetected = digitalRead(RF_PIN) == HIGH;

    // === AGGRESSIVE START ===
    if (currentTime - startTime < AGGRESSIVE_TIME) {
      forward();
      return;
    }

    // === ENEMY DETECTED ===
    if (enemyDetected) {
      lastEnemySeenTime = currentTime;
      if (!isPushing) {
        isPushing = true;
        forward(); // Begin push
      }
      // Safety: stop pushing if too long
      if (currentTime - lastEnemySeenTime > PUSH_DURATION) {
        reverseAndSpin();
        isPushing = false;
      }
      return;
    }

    // === LOST ENEMY RECOVERY ===
    if (isPushing && (currentTime - lastEnemySeenTime > LOST_ENEMY_TIMEOUT)) {
      reverseAndSpin();
      isPushing = false;
      return;
    }

    // === DEFAULT SEARCH MODE ===
    cautiousZigzag();
  }
  }

  // === Motor Control ===

  void setMotor(int A, int B, int E, bool forward) {
    digitalWrite(A, forward ? HIGH : LOW);
    digitalWrite(B, forward ? LOW : HIGH);
    digitalWrite(E, HIGH); // Enable
  }

  void forward() {
    setMotor(M1A_PIN, M1B_PIN, M1E_PIN, true);
    setMotor(M2A_PIN, M2B_PIN, M2E_PIN, true);
  }

  void reverse() {
    setMotor(M1A_PIN, M1B_PIN, M1E_PIN, false);
    setMotor(M2A_PIN, M2B_PIN, M2E_PIN, false);
  }

  void spinRight() {
    setMotor(M1A_PIN, M1B_PIN, M1E_PIN, true);   // Left forward
    setMotor(M2A_PIN, M2B_PIN, M2E_PIN, false);  // Right backward
  }

  void stopMotors() {
    digitalWrite(M1E_PIN, LOW);
    digitalWrite(M2E_PIN, LOW);
  }

  // === Behaviors ===

  void reverseAndSpin() {
    reverse();
    delay(500);
    stopMotors();
    delay(100);
    spinRight();
    delay(500);
    stopMotors();
  }

  void cautiousZigzag() {
    forward();
    delay(600);
    spinRight();
    delay(300);
    stopMotors();
    delay(100);
  }
