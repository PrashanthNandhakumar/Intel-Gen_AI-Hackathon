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

  Serial.begin(9600);
}

void loop() {
  int leftSensor = digitalRead(LEFT_IR);
  int centerSensor = digitalRead(CENTER_IR);
  int rightSensor = digitalRead(RIGHT_IR);
  
  // Forward movement if center sensor detects black line
  if (centerSensor == LOW && leftSensor == HIGH && rightSensor == HIGH) {
    moveForward();
  } 
  // Left turn if left sensor detects black line
  else if (leftSensor == LOW) {
    turnLeft();
  } 
  // Right turn if right sensor detects black line
  else if (rightSensor == LOW) {
    stopRightTurn();
    if (digitalRead(BUTTON_PIN) == LOW) {  // Wait for button press to continue
      moveForward();
    }
  }
  // Stop if no line detected
  else {
    stopMotors();
  }
}

void moveForward() {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  analogWrite(ENA, 200);  // Set speed for left motor

  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  analogWrite(ENB, 200);  // Set speed for right motor

  Serial.println("Forward");  // Print forward direction to Serial Monitor
}

void turnLeft() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  analogWrite(ENA, 150);  // Slow down for turning

  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  analogWrite(ENB, 150);

  Serial.println("Left");  // Print left direction to Serial Monitor
}

void stopRightTurn() {
  // Stop for right turn detection
  stopMotors();
  Serial.println("Right");  // Print right direction to Serial Monitor
}

void stopMotors() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
  analogWrite(ENA, 0);
  analogWrite(ENB, 0);
}
