#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <PubSubClient.h>
#include <WiFi.h>

// Pin yang digunakan untuk sensor DS18B20
const int pinSensor = 4;

// Inisialisasi objek OneWire dan DallasTemperature
OneWire oneWire(pinSensor);
DallasTemperature sensors(&oneWire);

// Inisialisasi Adafruit_MPU6050
Adafruit_MPU6050 mpu;

// Konfigurasi koneksi WiFi
const char* ssid = "Raja Kegelapan";
const char* password = "kegelapan";
const char* mqttServer = "159.223.249.103";
const int mqttPort = 1883;
const char* mqttUser = "onedev";
const char* mqttPassword = "#12345678";

// Inisialisasi objek WiFiClient dan PubSubClient
WiFiClient espClient;
PubSubClient client(espClient);

void setup(void) {
  Serial.begin(115200);
  while (!Serial)
    delay(10); // akan menunda Zero, Leonardo, dll. hingga konsol serial terbuka

  Serial.println("TES SENSOR MPU6050 & DS18B20 !!!");

  // Menghubungkan ke jaringan WiFi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");

  // Konfigurasi koneksi MQTT
  client.setServer(mqttServer, mqttPort);
  client.setCallback(callback);

  // Try to initialize MPU6050
  if (!mpu.begin()) {
    Serial.println("Failed to find MPU6050 chip");
    while (1) {
      delay(10);
    }
  }
  Serial.println("MPU6050 Found!");

  // ... kode lainnya ...
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  /* Get new sensor events with the readings */
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);

  /* Print out the values */
  Serial.print("Acceleration X: ");
  Serial.print(a.acceleration.x);
  Serial.print(", Y: ");
  Serial.print(a.acceleration.y);
  Serial.print(", Z: ");
  Serial.print(a.acceleration.z);
  Serial.println(" m/s^2");

  Serial.print("Rotation X: ");
  Serial.print(g.gyro.x);
  Serial.print(", Y: ");
  Serial.print(g.gyro.y);
  Serial.print(", Z: ");
  Serial.print(g.gyro.z);
  Serial.println(" rad/s");

  Serial.print("Temperature MPU6050: ");
  Serial.print(temp.temperature);
  Serial.println(" C");

   // Ambil pembacaan suhu DS18B20
  sensors.requestTemperatures();

  // Baca suhu dalam derajat Celsius
  float suhu = sensors.getTempCByIndex(0);

  // Tampilkan suhu ke Serial Monitor
  Serial.print("Temperature DS18B20: ");
  Serial.print(suhu);
  Serial.println(" °C");
  Serial.println("");

  // Kirim data sensor melalui MQTT
  char payload1[100];
  snprintf(payload1, sizeof(payload1),"%f",
           a.acceleration.x);
  client.publish("/gyro_x", payload1);

  char payload2[100];
  snprintf(payload2, sizeof(payload2),"%f",
           a.acceleration.y);
  client.publish("/gyro_y", payload2);

  char payload3[100];
  snprintf(payload3, sizeof(payload3),"%f",
           a.acceleration.z);
  client.publish("/gyro_z", payload3);

  char payload4[100];
  snprintf(payload4, sizeof(payload4),"%f",
           suhu);
  client.publish("/ds18b20", payload4);

  char payload5[100];
  snprintf(payload5, sizeof(payload5),"%f, %f, %f}",
           a.acceleration.x, a.acceleration.y, a.acceleration.z);
  client.publish("/gyro_sensor", payload5);

  delay(300000); // Tunggu 5 menit sebelum mengambil pembacaan berikutnya
}

void callback(char* topic, byte* payload, unsigned int length) {
  // Callback untuk menerima pesan MQTT (jika diperlukan)
}

void reconnect() {
  // Fungsi untuk melakukan reconnect ke broker MQTT
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    if (client.connect("arduinoClient", mqttUser, mqttPassword)) {
      Serial.println("connected");
      client.subscribe("command_topic"); // Subscribe ke topik perintah (jika diperlukan)
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
}
