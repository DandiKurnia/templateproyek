#include <WiFi.h>
#include <Firebase_ESP_Client.h>

/* ================= WIFI ================= */
#define WIFI_SSID "ISI_WIFI"
#define WIFI_PASSWORD "ISI_PASSWORD"

/* ================= FIREBASE ================= */
#define API_KEY "ISI_API_KEY_FIREBASE"
#define DATABASE_URL "https://NAMA-PROJECT-default-rtdb.firebaseio.com/"
#define USER_EMAIL "EMAIL_FIREBASE"
#define USER_PASSWORD "PASSWORD_FIREBASE"

/* ================= FIREBASE OBJECT ================= */
FirebaseData fbdo;
FirebaseConfig config;
FirebaseAuth auth;

/* ================= SETUP ================= */
void setup() {
  Serial.begin(115200);

  // Connect WiFi
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\n✅ WiFi Connected");
  Serial.println(WiFi.localIP());

  // Firebase Config
  auth.user.email = USER_EMAIL;
  auth.user.password = USER_PASSWORD;
  config.api_key = API_KEY;
  config.database_url = DATABASE_URL;

  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);

  Serial.println("✅ Firebase Initialized");
}

/* ================= LOOP ================= */
void loop() {
  // Kosong
  // Tambahkan logic Firebase di sini (set/get data)
}


nanti lu tingal cari cara ngambil apikey fire base sama link https yang ada di realtime database