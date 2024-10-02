#include <WiFi.h>

// Definición de la clase WiFiManager
class WiFiManager {
  const char* ssid;
  const char* password;

public:
  WiFiManager(const char* wifi_ssid, const char* wifi_password)
    : ssid(wifi_ssid), password(wifi_password) {}

  void connect() {
    Serial.print("Connecting to: ");
    Serial.print(ssid);
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
      delay(200);
      Serial.print(".");
    }
    Serial.print("\nIP address: ");
    Serial.println(WiFi.localIP());
  }
};

// Definición de la clase UltrasonicSensor
class UltrasonicSensor {
  int triggerPin;
  int echoPin;

public:
  UltrasonicSensor(int trigger, int echo)
    : triggerPin(trigger), echoPin(echo) {
    pinMode(triggerPin, OUTPUT);
    pinMode(echoPin, INPUT);
  }

  long readDistance() {
    digitalWrite(triggerPin, LOW);
    delayMicroseconds(2);
    digitalWrite(triggerPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(triggerPin, LOW);
    return pulseIn(echoPin, HIGH);
  }
};

const int triggerPin = 19, echoPin = 18;
const char* WIFI_SSID = "iPhone de Naty";
const char* WIFI_PASS = "123456NAP";
const char* SERVER_ADDRESS = "172.20.10.2";
const int SERVER_PORT =  65432 ;

WiFiManager wifiManager(WIFI_SSID, WIFI_PASS);
UltrasonicSensor ultrasonicSensor(triggerPin, echoPin);

void setup() {
  Serial.begin(115200);
  wifiManager.connect();
}

void loop() {
  long duration = ultrasonicSensor.readDistance();
  int cm = 0.01723 * duration;
  Serial.println(cm);
  delay(100); // Si necesitas cambiar este valor, cambialo aquí

  Serial.print("Connecting to server: ");
  Serial.println(SERVER_ADDRESS);

  WiFiClient client;

  if (!client.connect(SERVER_ADDRESS, SERVER_PORT)) {
    Serial.println("Connection failed");
    return;
  }

  client.println("distance=" + String(cm));

  String line = client.readStringUntil('\n');
  Serial.println(line);
  Serial.println("Closing connection.");
  client.stop();
}