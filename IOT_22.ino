#include <WiFi.h>

// Clase para manejar la conexión WiFi
class WiFiManager {
  const char* ssid;
  const char* password;

public:
  WiFiManager(const char* wifi_ssid, const char* wifi_password) : ssid(wifi_ssid), password(wifi_password) {}

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

// Clase para manejar los LEDs
class LEDManager {
  int bluePin;
  int redPin;
  int whitePin;
  int bluePin2;

public:
  LEDManager(int bPin, int rPin, int wPin, int b2Pin) : bluePin(bPin), redPin(rPin), whitePin(wPin), bluePin2(b2Pin) {
    pinMode(bluePin, OUTPUT);
    pinMode(redPin, OUTPUT);
    pinMode(whitePin, OUTPUT);
    pinMode(bluePin2, OUTPUT);
  }

  void turnOffAll() 
  {
    digitalWrite(bluePin, LOW);
    digitalWrite(redPin, LOW);
    digitalWrite(whitePin, LOW);
    digitalWrite(bluePin2, LOW);
  }

  void turnOnBlue()
   {
    turnOffAll();
    digitalWrite(bluePin, HIGH);
    digitalWrite(redPin, LOW);
    digitalWrite(whitePin, LOW);
    digitalWrite(bluePin2, LOW);
  }

  void turnOnRed()
   {
    turnOffAll();
    digitalWrite(redPin, HIGH);
    digitalWrite(bluePin, LOW);
    digitalWrite(whitePin, LOW);
    digitalWrite(bluePin2, LOW);
  }

  void turnOnWhite()
   {
    turnOffAll();
    digitalWrite(whitePin, HIGH);
    digitalWrite(redPin, LOW);
    digitalWrite(bluePin, LOW);
    digitalWrite(bluePin2, LOW);
  }

  void turnOnBlue2()
   {
    turnOffAll();
    digitalWrite(bluePin2, HIGH);
    digitalWrite(redPin, LOW);
    digitalWrite(whitePin, LOW);
    digitalWrite(bluePin, LOW);
  }
  
};

// Configuración de red WiFi y pines de LEDs
const char* WIFI_SSID = "iPhone de Naty";
const char* WIFI_PASS = "123456NAP";
const char* SERVER_ADDRESS = "172.20.10.2";
const int SERVER_PORT =  65432 ;

WiFiManager wifiManager(WIFI_SSID, WIFI_PASS);
LEDManager ledManager(13, 12, 14, 27); // bluePin, redPin, whitePin, bluePin2

void setup() {
  Serial.begin(115200);
  wifiManager.connect();
}

void loop() {
  delay(300);
  ledManager.turnOffAll();

  WiFiClient client;
  if (!client.connect(SERVER_ADDRESS, SERVER_PORT)) {
    Serial.println("Connection failed");
    return;
  }

  // Enviar solicitud GET al servidor
  client.println("GET");

  // Esperar por respuesta con un timeout de 1 segundo
  unsigned long previousMillis = millis();
  while (!client.available() && millis() - previousMillis < 1000);

  if (client.available() > 0) {
    String line = client.readStringUntil('\n');
    int pos = line.toInt();
    Serial.println(line);

    // Encender el LED correspondiente
    switch(pos) {
      case 0: ledManager.turnOnBlue(); break;
      case 1: ledManager.turnOnRed(); break;
      case 2: ledManager.turnOnWhite(); break;
      case 3: ledManager.turnOnBlue2(); break;
    }
  } else {
    Serial.println("Server timeout.");
  }

  Serial.println("Closing connection.");
  client.stop();
}