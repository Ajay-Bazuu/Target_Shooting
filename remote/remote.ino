#include <WiFi.h>

const char *ssid = "Target_shooting"; // Change to the receiver's SSID
const char *password = "pass123456";   // Change to the receiver's password

const char *receiverIP = "192.168.4.1"; // IP address of the receiver

WiFiClient client;

int joystick1X, joystick2Y;

const int sw_Pin = 18; // input for detecting whether the joystick/button is pressed
const int sw_Pin2 = 19;  //for other
int Status_sw_Pin = 0; // variable to store the button's state => 1 if not pressed
int Status_sw_Pin2 = 0;

int joy1XPin = 32;  // Example Pin for Joystick 1 X-Axis
int joy2YPin = 33;  // Example Pin for Joystick 2 Y-Axis

const int ledPin1 = 12;//INDICATE lED cONTROL
const int ledPin2 = 13;

void setup() {
  Serial.begin(115200);
  pinMode(ledPin1, OUTPUT);
  pinMode(ledPin2, OUTPUT);

  WiFi.begin(ssid, password);

  pinMode(joy1XPin, INPUT); // Set the joystick pins as INPUT
  pinMode(joy2YPin, INPUT);
  pinMode(sw_Pin, INPUT_PULLUP); // we use a pullup-resistor for the button functionality
  pinMode(sw_Pin2, INPUT_PULLUP);

  while (WiFi.status() != WL_CONNECTED) {
    Serial.println("Connecting to WiFi...");
    digitalWrite(ledPin1, HIGH);
    delay(500);
    digitalWrite(ledPin1, LOW);
    delay(500);
  }

  Serial.println("Connected to WiFi");
  digitalWrite(ledPin2, HIGH);
}

void loop() {
  client = WiFiClient();

  if (client.connect(receiverIP, 80)) {
    joystick1X = analogRead(joy1XPin);
    joystick2Y = analogRead(joy2YPin);

    Status_sw_Pin = digitalRead(sw_Pin); // reading button state: 1 = not pressed, 0 = pressed
    Status_sw_Pin2 = digitalRead(sw_Pin2);

    client.print(joystick1X);
    client.print(",");
    client.print(joystick2Y);
    client.print(",");
    client.print(Status_sw_Pin);
    client.print(",");
    client.println(Status_sw_Pin2);
    client.stop();
  }

  delay(10); // Adjust delay if needed
}
