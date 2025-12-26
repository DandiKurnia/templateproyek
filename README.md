# ESP32 Firebase Realtime Database Template

Template proyek untuk integrasi ESP32 dengan Firebase Realtime Database.

## 📚 Library yang Digunakan

Untuk menggunakan template ini, Anda memerlukan library berikut di Arduino IDE:

### Library Utama

1. **WiFi.h** (Built-in ESP32)

   - Library bawaan ESP32 untuk koneksi WiFi
   - Tidak perlu instalasi tambahan

2. **FirebaseESP32.h**

   - Library untuk integrasi Firebase dengan ESP32
   - Install via Arduino Library Manager: `Firebase ESP32 Client`
   - Author: Mobizt
   - Versi minimum: 4.x.x

3. **addons/TokenHelper.h**

   - Helper untuk autentikasi Firebase
   - Otomatis terinstall bersama FirebaseESP32

4. **addons/RTDBHelper.h**
   - Helper untuk Firebase Realtime Database
   - Otomatis terinstall bersama FirebaseESP32

### Cara Instalasi Library

1. Buka Arduino IDE
2. Pergi ke **Sketch** → **Include Library** → **Manage Libraries**
3. Cari "Firebase ESP32 Client" oleh Mobizt
4. Klik **Install**

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

Buka file `esp32.ino` dan edit bagian berikut:

```cpp
// WiFi Credentials
#define WIFI_SSID "NAMA_WIFI_ANDA"
#define WIFI_PASSWORD "PASSWORD_WIFI_ANDA"

// Firebase Credentials
#define API_KEY "YOUR_FIREBASE_API_KEY"
#define DATABASE_URL "YOUR_DATABASE_URL"
#define USER_EMAIL "your-email@example.com"
#define USER_PASSWORD "your-password"
```

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

### Inisialisasi WiFi

```cpp
void setup() {
  Serial.begin(115200);

  // Koneksi WiFi
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    delay(300);
    Serial.print(".");
  }
  Serial.println("\nWiFi Connected!");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());
}
```

### Konfigurasi Firebase

```cpp
// Konfigurasi autentikasi
config.api_key = API_KEY;
auth.user.email = USER_EMAIL;
auth.user.password = USER_PASSWORD;
config.database_url = DATABASE_URL;

// Callback untuk token
config.token_status_callback = tokenStatusCallback;

// Mulai koneksi Firebase
Firebase.begin(&config, &auth);
Firebase.reconnectWiFi(true);
```

### Menulis Data ke Realtime Database

```cpp
// Menulis data integer
if (Firebase.RTDB.setInt(&fbdo, "/sensor/temperature", 28)) {
  Serial.println("Temperature updated!");
} else {
  Serial.println("FAILED: " + fbdo.errorReason());
}

// Menulis data float
Firebase.RTDB.setFloat(&fbdo, "/sensor/humidity", 65.5);

// Menulis data string
Firebase.RTDB.setString(&fbdo, "/status/message", "System OK");

// Menulis data boolean
Firebase.RTDB.setBool(&fbdo, "/control/led", true);
```

### Membaca Data dari Realtime Database

```cpp
// Membaca data integer
if (Firebase.RTDB.getInt(&fbdo, "/sensor/temperature")) {
  if (fbdo.dataType() == "int") {
    int temp = fbdo.intData();
    Serial.println("Temperature: " + String(temp));
  }
}

// Membaca data float
if (Firebase.RTDB.getFloat(&fbdo, "/sensor/humidity")) {
  if (fbdo.dataType() == "float") {
    float hum = fbdo.floatData();
    Serial.println("Humidity: " + String(hum));
  }
}

// Membaca data boolean
if (Firebase.RTDB.getBool(&fbdo, "/control/led")) {
  if (fbdo.dataType() == "boolean") {
    bool ledState = fbdo.boolData();
    Serial.println("LED State: " + String(ledState));
  }
}
```

### Menulis Multiple Data (JSON)

```cpp
FirebaseJson json;
json.set("temperature", 28.5);
json.set("humidity", 65.2);
json.set("pressure", 1013.25);

Firebase.RTDB.setJSON(&fbdo, "/sensor", &json);
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

## 📱 Contoh Penggunaan

### 1. IoT Monitoring Sensor

- Baca sensor (suhu, kelembaban, dll)
- Kirim ke Firebase setiap X detik
- Monitor via Firebase Console atau Mobile App

### 2. Remote Control

- Baca status control dari Firebase
- Kontrol LED, relay, motor, dll
- Update dari web dashboard atau mobile app

### 3. Data Logging

- Simpan log data dengan timestamp
- Analisis data historis
- Export ke CSV atau visualisasi

## 📚 Referensi

- [Firebase ESP32 Client Documentation](https://github.com/mobizt/Firebase-ESP32)
- [Firebase Realtime Database Documentation](https://firebase.google.com/docs/database)
- [ESP32 Arduino Core](https://github.com/espressif/arduino-esp32)

## 📄 Lisensi

Template ini bebas digunakan untuk keperluan pendidikan dan proyek pribadi.

---

**Dibuat untuk**: Universitas Gunadarma - Asisten Laboratorium Sistem Komputer Lanjus
**Tahun**: 2025
