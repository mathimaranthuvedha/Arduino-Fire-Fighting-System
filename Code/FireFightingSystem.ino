#include <Servo.h>

// ---------------- PIN DEFINITIONS ----------------
const int flamePin = A0;        // A0 from flame sensor
const int relayPin = 8;         // Relay IN
const int servoFlamePin = 9;    // Scan servo
const int servoWaterPin = 10;   // Water servo

// ---------------- OBJECTS ----------------
Servo servoFlame;
Servo servoWater;

// ---------------- SETTINGS ----------------
const int flameThreshold = 500;   // Flame detected below this value

int scanAngle = 20;
int direction = 1;   // 1 = right, -1 = left

void setup() {

  Serial.begin(9600);

  pinMode(relayPin, OUTPUT);
  digitalWrite(relayPin, HIGH);   // Pump OFF (Active LOW relay)

  servoFlame.attach(servoFlamePin);
  servoWater.attach(servoWaterPin);

  servoFlame.write(scanAngle);
  servoWater.write(scanAngle);

  Serial.println("Fire Fighting System Ready (Analog Mode)");
}

void loop() {

  int flameValue = analogRead(flamePin);

  Serial.print("Flame Value: ");
  Serial.println(flameValue);

  // ---------------- FLAME DETECTED ----------------
  if (flameValue < flameThreshold) {
,;
    Serial.println("FLAME DETECTED!");

    // Stop servos at current angle
    servoFlame.write(scanAngle);
    servoWater.write(scanAngle);

    // Turn ON pump
    digitalWrite(relayPin, LOW);

    // Keep spraying until flame disappears
    while (analogRead(flamePin) < flameThreshold) {
      Serial.println("Spraying Water...");
      delay(200);
    }

    // Flame gone -> Turn OFF pump
    digitalWrite(relayPin, HIGH);
    Serial.println("Flame Extinguished");

    delay(1000);   // Small pause before scanning again
  }

  // ---------------- SCANNING MODE ----------------
  else {

    digitalWrite(relayPin, HIGH);   // Ensure pump OFF

    servoFlame.write(scanAngle);
    servoWater.write(scanAngle);

    delay(30);

    scanAngle += direction;

    if (scanAngle >= 160 || scanAngle <= 20) {
      direction = -direction;
    }
  }
}
