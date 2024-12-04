#include <DHT.h>
#include <Firebase_Arduino_WiFiNINA.h>
//#include <WiFiNINA.h>

// Wi-Fi credentials
#define WIFI_SSID "Radar"
#define WIFI_PASSWORD "Daniel123"

// Firebase credentials
#define FIREBASE_HOST "final-project-iot-be6f8-default-rtdb.firebaseio.com"  // Firebase URL
#define FIREBASE_AUTH "q58H7LNF8tdOGzBAgZeCbQnmaOk3ou3x2fzT0qRL"   // Firebase token

// Define sensor pins
#define DHTPIN 8           // Pin connected to DHT11 data pin
#define TRIGPIN 9          // Pin connected to HC-SR04 trigger
#define ECHOPIN 10         // Pin connected to HC-SR04 echo
#define PIRPIN 6           // Pin connected to PIR sensor output

// Define sensor types
#define DHTTYPE DHT11      // DHT11 sensor type

// Initialize DHT sensor
DHT dht(DHTPIN, DHTTYPE);

// Firebase paths
const char* temperaturePath = "/sensor_data/temperature";
const char* humidityPath = "/sensor_data/humidity";
const char* distancePath = "/sensor_data/distance";
const char* motionPath = "/sensor_data/motion";

// Data collection interval in milliseconds
unsigned long lastDataSent = 0;
const unsigned long DATA_SEND_INTERVAL = 5000;  // Send data every 5 seconds

// Initialize WiFi client
WiFiSSLClient client;
FirebaseData firebaseData;

void setup() {
  Serial.begin(9600);
  dht.begin();

  pinMode(TRIGPIN, OUTPUT);
  pinMode(ECHOPIN, INPUT);
  pinMode(PIRPIN, INPUT);

  // Connect to Wi-Fi
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");

  // Initialize Firebase
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH, WIFI_SSID, WIFI_PASSWORD);
  Firebase.reconnectWiFi(true);
}

void loop() {
  // Only send data at defined intervals
  if (millis() - lastDataSent >= DATA_SEND_INTERVAL) {
    lastDataSent = millis();

    // Read temperature and humidity from DHT11 sensor
    float humidity = dht.readHumidity();
    float temperature = dht.readTemperature();

    if (isnan(humidity) || isnan(temperature)) {
      Serial.println("Failed to read from DHT sensor!");
    } else {
      Serial.print("Temperature: ");
      Serial.print(temperature);
      Serial.print(" °C, Humidity: ");
      Serial.print(humidity);
      Serial.println(" %");

      // Send temperature and humidity data to Firebase
      if (!Firebase.setFloat(firebaseData, temperaturePath, temperature)) {
        Serial.println("Failed to upload temperature to Firebase");
      }
      if (!Firebase.setFloat(firebaseData, humidityPath, humidity)) {
        Serial.println("Failed to upload humidity to Firebase");
      }
    }

    // Read distance from ultrasonic sensor
    long duration, distance;
    digitalWrite(TRIGPIN, LOW);
    delayMicroseconds(2);
    digitalWrite(TRIGPIN, HIGH);
    delayMicroseconds(10);
    digitalWrite(TRIGPIN, LOW);
    duration = pulseIn(ECHOPIN, HIGH);
    distance = duration * 0.034 / 2;  // Calculate distance in cm

    Serial.print("Distance: ");
    Serial.print(distance);
    Serial.println(" cm");

    // Send distance data to Firebase
    if (!Firebase.setInt(firebaseData, distancePath, distance)) {
      Serial.println("Failed to upload distance to Firebase");
    }

    // Read motion from PIR sensor
    int motionDetected = digitalRead(PIRPIN);
    if (motionDetected == HIGH) {
      Serial.println("Motion detected!");
      if (!Firebase.setBool(firebaseData, motionPath, true)) {
        Serial.println("Failed to upload motion detection to Firebase");
      }
    } else {
      Serial.println("No motion detected.");
      if (!Firebase.setBool(firebaseData, motionPath, false)) {
        Serial.println("Failed to upload motion detection to Firebase");
      }
    }
  }
}
