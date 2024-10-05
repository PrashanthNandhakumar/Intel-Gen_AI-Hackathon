#include <SoftwareSerial.h>

// Pin Definitions
#define LEFT_IR A0
#define CENTER_IR A1
#define RIGHT_IR A2

#define ENA 9  // Left motor speed
#define IN1 4
#define IN2 5

#define ENB 10  // Right motor speed
#define IN3 6
#define IN4 7

#define BUTTON_PIN 8  // Button to return to straight path

// Bluetooth module connections
SoftwareSerial BTSerial(11, 12);  // RX, TX

void setup() {
  pinMode(LEFT_IR, INPUT);
  pinMode(CENTER_IR, INPUT);
  pinMode(RIGHT_IR, INPUT);

  pinMode(ENA, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);

  pinMode(ENB, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  pinMode(BUTTON_PIN, INPUT_PULLUP);  // Button with internal pull-up resistor

  Serial.begin(9600);  // For Serial Monitor
  BTSerial.begin(9600);  // For Bluetooth module

  Serial.println("Rover is ready for Bluetooth commands...");
}

void loop() {
  if (BTSerial.available()) {
    char command = BTSerial.read();  // Read command from Bluetooth

    switch (command) {
      case 'F':  // Move forward
        moveForward();
        Serial.println("Forward");
        break;
      case 'B':  // Move backward
        moveBackward();
        Serial.println("Backward");
        break;
      case 'L':  // Turn left
        turnLeft();
        Serial.println("Left");
        break;
      case 'R':  // Turn right
        turnRight();
        Serial.println("Right");
        break;
      case 'S':  // Stop
        stopMotors();
        Serial.println("Stop");
        break;
      default:
        stopMotors();
        break;
    }
  }
}

void moveForward() {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  analogWrite(ENA, 200);  // Speed for left motor

  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  analogWrite(ENB, 200);  // Speed for right motor
}

void moveBackward() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  analogWrite(ENA, 200);  // Speed for left motor

  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
  analogWrite(ENB, 200);  // Speed for right motor
}

void turnLeft() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  analogWrite(ENA, 150);  // Slow down for left motor

  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  analogWrite(ENB, 150);  // Right motor forward
}

void turnRight() {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  analogWrite(ENA, 150);  // Left motor forward

  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
  analogWrite(ENB, 150);  // Slow down for right motor
}

void stopMotors() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
  analogWrite(ENA, 0);
  analogWrite(ENB, 0);
}
