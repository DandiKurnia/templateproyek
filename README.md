# ESP32 Firebase Realtime Database Template

Template proyek untuk integrasi ESP32 dengan Firebase Realtime Database.

## 📚 Library yang Digunakan

Untuk menggunakan template ini, Anda memerlukan library berikut di Arduino IDE:

### Library Utama

1. **WiFi.h** (Built-in ESP32)

   - Library bawaan ESP32 untuk koneksi WiFi
   - Tidak perlu instalasi tambahan

2. **Firebase_ESP_Client.h**
   - Library untuk integrasi Firebase dengan ESP32
   - Install via Arduino Library Manager: `Firebase ESP Client`
   - Author: Mobizt
   - Versi minimum: 3.x.x atau lebih baru
   - **PENTING**: Library ini sudah include semua yang dibutuhkan (Auth, RTDB, dll)

### Cara Instalasi Library

1. Buka Arduino IDE
2. Pergi ke **Sketch** → **Include Library** → **Manage Libraries**
3. Cari "**Firebase ESP Client**" oleh Mobizt
4. Klik **Install**
5. Tunggu hingga instalasi selesai (library ini cukup besar ~5-10MB)

## 🔧 Konfigurasi Firebase

### 1. Buat Project Firebase

1. Kunjungi [Firebase Console](https://console.firebase.google.com/)
2. Klik **Add Project**
3. Ikuti wizard untuk membuat project baru

### 2. Aktifkan Realtime Database

1. Di Firebase Console, pilih project Anda
2. Klik **Realtime Database** di menu sebelah kiri
3. Klik **Create Database**
4. Pilih lokasi server (pilih yang terdekat dengan lokasi Anda)
5. Pilih mode **Test Mode** untuk development (atau **Locked Mode** untuk production)

### 3. Dapatkan Credentials

#### Database URL:

- Format: `https://[PROJECT_ID]-default-rtdb.firebaseio.com/`
- Contoh: `https://my-iot-project-default-rtdb.firebaseio.com/`

#### API Key:

1. Di Firebase Console, klik ⚙️ (Settings) → **Project Settings**
2. Scroll ke bawah ke bagian **Web API Key**
3. Copy API Key tersebut

#### User Email & Password (Authentication):

1. Di Firebase Console, pilih **Authentication**
2. Klik **Get Started** (jika belum aktif)
3. Pilih tab **Sign-in method**
4. Enable **Email/Password**
5. Klik tab **Users** → **Add User**
6. Masukkan email dan password untuk ESP32

## 📝 Struktur Data di Realtime Database

Contoh struktur data yang akan dibuat:

```json
{
  "sensor": {
    "temperature": 28.5,
    "humidity": 65.2,
    "timestamp": 1703598245
  },
  "control": {
    "led": true,
    "motor": false
  }
}
```

## 🚀 Step-by-Step Menggunakan Template

### Step 1: Setup Hardware

1. ESP32 Development Board
2. Koneksi USB untuk programming
3. (Opsional) Sensor DHT22 untuk contoh pembacaan data

### Step 2: Edit Konfigurasi WiFi dan Firebase

Buka file `esp32.ino` dan edit bagian konfigurasi ini:

```cpp
/* ================= WIFI ================= */
#define WIFI_SSID "ISI_WIFI"              // Ganti dengan nama WiFi Anda
#define WIFI_PASSWORD "ISI_PASSWORD"       // Ganti dengan password WiFi Anda

/* ================= FIREBASE ================= */
#define API_KEY "ISI_API_KEY_FIREBASE"     // Dari Firebase Project Settings
#define DATABASE_URL "https://NAMA-PROJECT-default-rtdb.firebaseio.com/"  // URL Realtime Database
#define USER_EMAIL "EMAIL_FIREBASE"        // Email yang didaftarkan di Firebase Auth
#define USER_PASSWORD "PASSWORD_FIREBASE"  // Password user Firebase
```

**Cara mendapatkan credentials:**

- **API_KEY**: Firebase Console → ⚙️ Settings → Project Settings → Web API Key
- **DATABASE_URL**: Firebase Console → Realtime Database → URL di bagian atas (contoh: `https://your-project-default-rtdb.firebaseio.com/`)
- **USER_EMAIL & PASSWORD**: Yang Anda buat di Firebase Authentication → Users

### Step 3: Upload ke ESP32

1. Hubungkan ESP32 ke komputer via USB
2. Pilih board: **Tools** → **Board** → **ESP32 Dev Module**
3. Pilih port COM yang sesuai
4. Klik **Upload** (→)

### Step 4: Monitor Serial

1. Buka Serial Monitor: **Tools** → **Serial Monitor**
2. Set baud rate ke **115200**
3. Amati proses koneksi WiFi dan Firebase

## 💻 Penjelasan Code

### 1. Include Libraries

```cpp
#include <WiFi.h>
#include <Firebase_ESP_Client.h>
```

- `WiFi.h`: Library untuk koneksi WiFi ESP32
- `Firebase_ESP_Client.h`: Library utama Firebase (sudah include semua fitur)

### 2. Deklarasi Firebase Objects

```cpp
FirebaseData fbdo;      // Object untuk operasi database
FirebaseConfig config;  // Object untuk konfigurasi Firebase
FirebaseAuth auth;      // Object untuk autentikasi
```

### 3. Inisialisasi WiFi (di `setup()`)

```cpp
WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
while (WiFi.status() != WL_CONNECTED) {
  delay(500);
  Serial.print(".");
}
Serial.println("\n✅ WiFi Connected");
Serial.println(WiFi.localIP());
```

ESP32 akan terus mencoba connect sampai berhasil.

### 4. Konfigurasi Firebase (di `setup()`)

```cpp
// Set autentikasi
auth.user.email = USER_EMAIL;
auth.user.password = USER_PASSWORD;

// Set konfigurasi
config.api_key = API_KEY;
config.database_url = DATABASE_URL;

// Mulai Firebase
Firebase.begin(&config, &auth);
Firebase.reconnectWiFi(true);  // Auto-reconnect jika WiFi putus
```

## 🔥 Cara Menggunakan Firebase di `loop()`

Template ini sudah siap pakai! Anda tinggal tambahkan logic di fungsi `loop()`.

### 📤 Menulis Data ke Firebase

#### Menulis Integer

```cpp
void loop() {
  if (Firebase.ready()) {
    int suhu = 28;
    if (Firebase.RTDB.setInt(&fbdo, "/sensor/suhu", suhu)) {
      Serial.println("✅ Suhu terkirim!");
    } else {
      Serial.println("❌ Gagal: " + fbdo.errorReason());
    }
    delay(5000);  // Kirim setiap 5 detik
  }
}
```

#### Menulis Float

```cpp
float humidity = 65.5;
Firebase.RTDB.setFloat(&fbdo, "/sensor/humidity", humidity);
```

#### Menulis String

```cpp
String status = "System OK";
Firebase.RTDB.setString(&fbdo, "/status/message", status);
```

#### Menulis Boolean

```cpp
bool ledState = true;
Firebase.RTDB.setBool(&fbdo, "/control/led", ledState);
```

### 📥 Membaca Data dari Firebase

#### Membaca Integer

```cpp
if (Firebase.RTDB.getInt(&fbdo, "/control/threshold")) {
  if (fbdo.dataType() == "int") {
    int threshold = fbdo.intData();
    Serial.println("Threshold: " + String(threshold));
  }
}
```

#### Membaca Boolean

```cpp
if (Firebase.RTDB.getBool(&fbdo, "/control/led")) {
  if (fbdo.dataType() == "boolean") {
    bool led = fbdo.boolData();
    digitalWrite(LED_PIN, led);  // Kontrol LED
  }
}
```

### 📦 Menulis Multiple Data (JSON)

```cpp
FirebaseJson json;
json.set("suhu", 28.5);
json.set("humidity", 65.2);
json.set("status", "aktif");

if (Firebase.RTDB.setJSON(&fbdo, "/sensor", &json)) {
  Serial.println("✅ Data JSON terkirim!");
}
```

## 🔐 Firebase Rules untuk Development

Untuk testing, gunakan rules berikut di Firebase Console → Realtime Database → Rules:

```json
{
  "rules": {
    ".read": "auth != null",
    ".write": "auth != null"
  }
}
```

⚠️ **Warning**: Untuk production, gunakan rules yang lebih ketat!

## 📊 Monitoring di Firebase Console

1. Buka Firebase Console
2. Pilih **Realtime Database**
3. Lihat data real-time yang masuk dari ESP32
4. Anda bisa manual edit data di console untuk testing kontrol dari cloud

## 🐛 Troubleshooting

### ESP32 tidak terkoneksi WiFi

- Pastikan SSID dan password benar
- Cek jarak dari router
- Pastikan WiFi menggunakan 2.4GHz (bukan 5GHz)

### Firebase Authentication Failed

- Pastikan API Key benar
- Pastikan Email/Password sudah didaftarkan di Firebase Authentication
- Pastikan Email/Password authentication sudah di-enable

### Data tidak muncul di Firebase

- Cek Firebase Rules (harus allow read/write untuk authenticated users)
- Pastikan Database URL benar
- Cek Serial Monitor untuk error message

### ERROR: Token Generation Failed

- Pastikan koneksi internet stabil
- Cek API Key
- Pastikan waktu ESP32 tersinkronisasi (library akan auto-sync)

## 📱 Contoh Project IoT

### 1. 🌡️ Monitoring Suhu Real-time

```cpp
void loop() {
  if (Firebase.ready()) {
    float suhu = readTemperature();  // Dari sensor DHT22
    Firebase.RTDB.setFloat(&fbdo, "/ruangan/suhu", suhu);
    delay(10000);  // Update tiap 10 detik
  }
}
```

### 2. 💡 Remote Control LED via Firebase

```cpp
void loop() {
  if (Firebase.ready()) {
    if (Firebase.RTDB.getBool(&fbdo, "/kontrol/led")) {
      bool ledStatus = fbdo.boolData();
      digitalWrite(LED_PIN, ledStatus ? HIGH : LOW);
    }
    delay(1000);
  }
}
```

### 3. 📊 Data Logging dengan Timestamp

```cpp
void loop() {
  if (Firebase.ready()) {
    String path = "/log/" + String(millis());
    FirebaseJson data;
    data.set("suhu", readTemp());
    data.set("waktu", getTime());
    Firebase.RTDB.setJSON(&fbdo, path, &data);
    delay(60000);  // Log tiap 1 menit
  }
}
```

## 📚 Referensi & Dokumentasi

- [Firebase ESP Client - GitHub](https://github.com/mobizt/Firebase-ESP-Client)
- [Firebase Realtime Database Docs](https://firebase.google.com/docs/database)
- [ESP32 Arduino Core](https://github.com/espressif/arduino-esp32)

## 🔑 Tips Mendapatkan Credentials

**API Key & Database URL:**

1. Buka [Firebase Console](https://console.firebase.google.com/)
2. Pilih project Anda
3. Klik ⚙️ (Settings) → **Project Settings**
4. **Web API Key** ada di bagian "General"
5. **Database URL** ada di menu **Realtime Database** (bagian atas)

**User Email & Password:**

1. Di Firebase Console → **Authentication**
2. Tab **Users** → **Add User**
3. Masukkan email & password (simpan baik-baik untuk ESP32)

## 📄 Lisensi

Template ini bebas digunakan untuk keperluan pendidikan dan proyek pribadi.

---

**Dibuat untuk**: Universitas Gunadarma - Asisten Laboratorium Sistem Komputer Lanjus
**Tahun**: 2025
