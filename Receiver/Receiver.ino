#include <WiFi.h>
#include <WiFiClient.h>
#include <WiFiAP.h>
#include <ESP32Servo.h>

const char *ssid = "Target_shooting";
const char *password = "pass123456";

WiFiServer server(80);

int Xvalue1, Yvalue2;
int Status_sw_Pin, Status_sw_Pin2;

const int motor_Pin = 23;
Servo servo1;
Servo servo2;
Servo esc;//for the brushless motor

void setup() {
  Serial.begin(115200);
  WiFi.softAP(ssid, password);Serial.println("Wifi On");
  server.begin();
  servo1.attach(2); // Attach the servo to pin 2
  servo2.attach(4); // Attach the servo to pin 4
  esc.attach(motor_Pin, 1000, 2000);
}


void loop() {
  WiFiClient client = server.available();

  if (client) {
    String request = client.readStringUntil('\r');
    client.flush();

    client.println("HTTP/1.1 200 OK");
    client.println("Content-type:text/html");
    client.println();

    int comma1 = request.indexOf(',');
    int comma2 = request.indexOf(',', comma1 + 1);
    int comma3 = request.indexOf(',', comma2 + 1);
    int comma4 = request.indexOf(',', comma3 + 1);

    Xvalue1 = request.substring(0, comma1).toInt(); Serial.println(Xvalue1);
    Yvalue2 = request.substring(comma1 + 1, comma2).toInt(); Serial.println(Yvalue2);
    Status_sw_Pin = request.substring(comma2 + 1, comma3).toInt(); Serial.println(Status_sw_Pin);
    Status_sw_Pin2 = request.substring(comma3 + 1, comma4).toInt(); Serial.println(Status_sw_Pin2);

      if (Xvalue1 < 1850 || Xvalue1 > 1950) {
    Xvalue1 = map(Xvalue1, 0, 4095, 50, 110);
    servo2.write(Xvalue1);
    delay(10);
  }
    if (Yvalue2 < 1850 || Yvalue2 > 1950) {
    Yvalue2 = map(Yvalue2, 0, 4095, 50, 140);
    servo1.write(Yvalue2);
    delay(10);
  }

    if (Status_sw_Pin == LOW || Status_sw_Pin2 == LOW) {
    shoot();
  } else {
    shootStop();
  }
  
  

  }
}

void shoot() {
  esc.write(300); Serial.println("Rotating at high speed"); // set the speed range from 0-499)
}
void shootStop() {
  esc.write(0); Serial.println("stop");
}
