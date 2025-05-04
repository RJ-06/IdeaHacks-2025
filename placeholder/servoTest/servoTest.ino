#include <Servo.h>
#include <string.h>

// Initialize servo
Servo servo1; // Low E

// System variables
int reset = 0;
int wait = 10;
int base_fret = 45;
String inByte = ""; // <-- Moved outside loop so it's persistent

void setup() {
  Serial.begin(9600);
  servo1.attach(9); // Use a valid PWM pin (e.g., 9)
  servo1.write(reset);
  Serial.println("Ready to receive commands. Type 'r' to reset.");
}

void loop() {
  // Read serial input
  if (Serial.available()) {
    inByte = Serial.readStringUntil('\n'); // safer than readString()
    inByte.trim(); // removes any \r or spaces
    Serial.print("Received: ");
    Serial.println(inByte);

    if (inByte == "r") {
      Serial.println("Resetting...");
      servo1.write(reset);
      delay(500);
      servo1.write(180);
    }

    inByte = ""; // Clear input so it doesn't re-trigger
  }

  // Optional: move the servo gradually
  static int val = reset;
  if (val < 80) {
    val++;
    servo1.write(val);
    delay(20);  // Slow down motion
  }
}
