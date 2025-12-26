/*
 * ESP32 Firebase Realtime Database Template
 * 
 * Template untuk integrasi ESP32 dengan Firebase Realtime Database
 * Author: Lab Assistant - Universitas Gunadarma
 * Year: 2025
 * 
 * Library yang dibutuhkan:
 * - Firebase ESP32 Client by Mobizt (v4.x.x)
 */

#include <WiFi.h>
#include <FirebaseESP32.h>
#include <addons/TokenHelper.h>
#include <addons/RTDBHelper.h>

// ==================== KONFIGURASI ====================
// WiFi Credentials
#define WIFI_SSID "NAMA_WIFI_ANDA"
#define WIFI_PASSWORD "PASSWORD_WIFI_ANDA"

// Firebase Credentials
#define API_KEY "YOUR_FIREBASE_API_KEY"
#define DATABASE_URL "https://your-project-default-rtdb.firebaseio.com/"

// Firebase Authentication (User Email & Password)
#define USER_EMAIL "your-email@example.com"
#define USER_PASSWORD "your-password"

// ==================== FIREBASE OBJECTS ====================
FirebaseData fbdo;      // Firebase Data object
FirebaseAuth auth;      // Firebase Authentication object
FirebaseConfig config;  // Firebase Configuration object

// ==================== VARIABLES ====================
unsigned long sendDataPrevMillis = 0;
const long sendDataInterval = 5000;  // Kirim data setiap 5 detik
int count = 0;
bool signupOK = false;

// ==================== SETUP ====================
void setup() {
  Serial.begin(115200);
  Serial.println();
  Serial.println("=================================");
  Serial.println("ESP32 Firebase Template");
  Serial.println("=================================");

  // Koneksi ke WiFi
  Serial.print("Connecting to WiFi");
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(300);
    Serial.print(".");
  }
  
  Serial.println();
  Serial.println("WiFi Connected!");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());
  Serial.println();

  // Konfigurasi Firebase
  config.api_key = API_KEY;
  config.database_url = DATABASE_URL;
  
  // Autentikasi dengan Email dan Password
  auth.user.email = USER_EMAIL;
  auth.user.password = USER_PASSWORD;

  // Callback function untuk status token
  config.token_status_callback = tokenStatusCallback;

  // Mulai koneksi Firebase
  Serial.println("Connecting to Firebase...");
  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);
  
  signupOK = true;
  Serial.println("Firebase Connected!");
  Serial.println("=================================");
  Serial.println();
}

// ==================== MAIN LOOP ====================
void loop() {
  // Cek apakah sudah waktunya mengirim data
  if (Firebase.ready() && signupOK && (millis() - sendDataPrevMillis > sendDataInterval || sendDataPrevMillis == 0)) {
    sendDataPrevMillis = millis();
    
    Serial.println("--- Sending Data to Firebase ---");
    
    // ========== CONTOH 1: Menulis Data Integer ==========
    String path1 = "/sensor/count";
    if (Firebase.RTDB.setInt(&fbdo, path1, count)) {
      Serial.println("✓ Count updated: " + String(count));
    } else {
      Serial.println("✗ FAILED to update count");
      Serial.println("REASON: " + fbdo.errorReason());
    }

    // ========== CONTOH 2: Menulis Data Float ==========
    float tempValue = random(200, 350) / 10.0; // Random suhu 20.0 - 35.0
    String path2 = "/sensor/temperature";
    if (Firebase.RTDB.setFloat(&fbdo, path2, tempValue)) {
      Serial.println("✓ Temperature updated: " + String(tempValue) + " °C");
    } else {
      Serial.println("✗ FAILED to update temperature");
      Serial.println("REASON: " + fbdo.errorReason());
    }

    // ========== CONTOH 3: Menulis Data Boolean ==========
    bool ledStatus = (count % 2 == 0);
    String path3 = "/control/led";
    if (Firebase.RTDB.setBool(&fbdo, path3, ledStatus)) {
      Serial.println("✓ LED status updated: " + String(ledStatus ? "ON" : "OFF"));
    } else {
      Serial.println("✗ FAILED to update LED status");
      Serial.println("REASON: " + fbdo.errorReason());
    }

    // ========== CONTOH 4: Menulis Data String ==========
    String statusMsg = "System OK - Count: " + String(count);
    String path4 = "/status/message";
    if (Firebase.RTDB.setString(&fbdo, path4, statusMsg)) {
      Serial.println("✓ Status message updated");
    } else {
      Serial.println("✗ FAILED to update status message");
      Serial.println("REASON: " + fbdo.errorReason());
    }

    // ========== CONTOH 5: Menulis Multiple Data dengan JSON ==========
    FirebaseJson json;
    json.set("humidity", random(400, 800) / 10.0);
    json.set("pressure", random(980, 1040));
    json.set("timestamp", millis() / 1000);
    
    String path5 = "/sensor/environment";
    if (Firebase.RTDB.setJSON(&fbdo, path5, &json)) {
      Serial.println("✓ Environment data updated (JSON)");
    } else {
      Serial.println("✗ FAILED to update environment data");
      Serial.println("REASON: " + fbdo.errorReason());
    }

    Serial.println();
    
    // ========== CONTOH 6: Membaca Data dari Firebase ==========
    if (count % 3 == 0) {  // Baca data setiap 3 kali loop
      Serial.println("--- Reading Data from Firebase ---");
      
      // Baca data integer
      if (Firebase.RTDB.getInt(&fbdo, "/control/threshold")) {
        if (fbdo.dataType() == "int") {
          int threshold = fbdo.intData();
          Serial.println("✓ Threshold value: " + String(threshold));
        }
      }
      
      // Baca data boolean
      if (Firebase.RTDB.getBool(&fbdo, "/control/led")) {
        if (fbdo.dataType() == "boolean") {
          bool led = fbdo.boolData();
          Serial.println("✓ LED state from Firebase: " + String(led ? "ON" : "OFF"));
        }
      }
      
      Serial.println();
    }

    count++;
  }
}

// ==================== HELPER FUNCTIONS ====================

// Fungsi untuk membaca sensor (contoh)
float readTemperature() {
  // Ganti dengan kode pembacaan sensor sebenarnya
  // Contoh: sensor DHT22, DS18B20, dll
  return random(200, 350) / 10.0;
}

float readHumidity() {
  // Ganti dengan kode pembacaan sensor sebenarnya
  return random(400, 800) / 10.0;
}

// Fungsi untuk kontrol output (contoh)
void controlLED(bool state) {
  // Ganti dengan pin LED Anda
  // pinMode(LED_PIN, OUTPUT);
  // digitalWrite(LED_PIN, state ? HIGH : LOW);
  Serial.println("LED set to: " + String(state ? "ON" : "OFF"));
}
