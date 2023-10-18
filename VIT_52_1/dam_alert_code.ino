#include <NewPing.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266SMTP.h>

// Wi-Fi credentials
const char* ssid = "yourSSID";
const char* password = "yourPASSWORD";

// Email configuration
const char* smtpServer = "smtp.gmail.com";
const char* smtpUser = "your@gmail.com";
const char* smtpPassword = "yourPASSWORD";
const int smtpPort = 465;
const char* toEmail = "to@example.com";

// Ultrasonic sensor
#define TRIGGER_PIN 2
#define ECHO_PIN 3
#define MAX_DISTANCE 400
NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE);

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");
}

void loop() {
  delay(1000);
  int distance = sonar.ping_cm();

  if (distance > 100) { // Adjust this threshold based on your dam's specifications
    sendEmail("Water level alert", "Water level is too high!");
    delay(3600000); // Send email once per hour to avoid spamming
  }
}

void sendEmail(String subject, String body) {
  WiFiClient client;
  SMTPClient smtp;
  
  if (smtp.sendStart(client, smtpServer, smtpPort, smtpUser, smtpPassword, smtpUser)) {
    smtp.sendHeader(client, smtpUser, toEmail, subject);
    smtp.sendBody(client, body);
    smtp.sendEnd(client);
  }
  
  client.stop();
}
