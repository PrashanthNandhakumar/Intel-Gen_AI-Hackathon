#include <WiFi.h>
#include <HTTPClient.h>

// Pin Definitions
#define LDR_PIN 34  // LDR connected to GPIO34 on ESP32 (Analog pin)

// WiFi Settings
const char* ssid = "Redmi Note 10 Pro";
const char* password = "12345786";
const String serverUrl = "https://7188-2409-408d-339f-b4ec-c901-464a-4327-a55e.ngrok-free.app/artifact_recognized";  // Change to your server IP

WiFiClient client;  // Create a WiFiClient object

// Time Variables
unsigned long previousMillis = 0;
unsigned long artifact1Time = 1000;  // 1 second
unsigned long artifact2Time = 2000;  // 2 seconds
int artifactId = 0;

void setup() {
  Serial.begin(115200);
  pinMode(LDR_PIN, INPUT);

  // Connect to WiFi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");
}

void loop() {
  int ldrValue = analogRead(LDR_PIN);
  unsigned long currentMillis = millis();

  // Check for drastic light changes (laser interception)
  if (ldrValue < 500) {  // Assuming < 500 indicates laser intercepted
    unsigned long timeDifference = currentMillis - previousMillis;
    
    if (timeDifference < artifact1Time) {
      artifactId = 1;  // Artifact 1 recognized
      Serial.println("Artifact 1 recognized");
      sendArtifactData(artifactId);
    } else if (timeDifference >= artifact1Time && timeDifference < artifact2Time) {
      artifactId = 2;  // Artifact 2 recognized
      Serial.println("Artifact 2 recognized");
      sendArtifactData(artifactId);
    }

    previousMillis = currentMillis;  // Reset timer for next detection
  }
  delay(100);  // Small delay to avoid rapid readings
}

// Function to send artifact data to server
void sendArtifactData(int artifactId) {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;

    // Update the 'begin' method with WiFiClient
    http.begin(client, serverUrl);
    http.addHeader("Content-Type", "application/json");

    String postData = "{\"artifact_id\": " + String(artifactId) + "}";
    int httpResponseCode = http.POST(postData);

    if (httpResponseCode > 0) {
      String response = http.getString();
      Serial.println("Server response: " + response);
    } else {
      Serial.println("Error sending artifact data");
    }

    http.end();  // End HTTP connection
  } else {
    Serial.println("WiFi not connected");
  }
}
