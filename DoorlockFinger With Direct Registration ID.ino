/*Program Door Lock

  ////////////////////////////////////////////////////////////////

  Konfigurasi Pin

  </>LCD 16x4

  GND         ->    GND
  VCC         ->    5V
  SDA         ->    SDA
  SCL         ->    SCL

  </>Selenoid Door Lock

  Selenoid    ->    4

  </>FingerPrint

  Red (Vin)   ->    5V
  Green (Tx)  ->    2
  White (Rx)  ->    3
  Black (Gnd) ->    Gnd

  </>Touchless Button

  VCC         ->    VCC
  In          ->    5
  GND         ->    GND

  </>Button

  Sw1         ->    6
  Sw2         ->    7
  Sw3         ->    8
*/

//================================================== LIBRARY ==================================================

#include <Wire.h>
#include <SPI.h>
#include "Arduino.h"
#include <Adafruit_Fingerprint.h>
#include <SoftwareSerial.h>
#include <LiquidCrystal_I2C.h>

//================================================== Inisialisasi Pin ==================================================

#define selenoid  13
#define touchless 5
#define Sw1       6
#define Sw2       7
#define Sw3       8
#define SwDoor    9
#define Led1      12


// -------------------- Inisialisasi Serial Modul--------------------
SoftwareSerial FingerScan(2, 3);

// -------------------- Inisialisasi LCD --------------------
LiquidCrystal_I2C lcd(0x27, 16, 4);

// -------------------- Inisialisasi Fingerprint --------------------
Adafruit_Fingerprint finger = Adafruit_Fingerprint(&FingerScan);

//================================================== Variabel ==================================================

// -------------------- Button State --------------------
boolean currentState1 = LOW; //Touchless Button State
boolean lastState1    = LOW; //Touchless Button State

boolean currentState2 = LOW; //Sw1 Button State
boolean lastState2    = LOW; //Sw1 Button State

boolean currentState3 = LOW; //Sw2 Button State
boolean lastState3    = LOW; //Sw2 Button State

boolean currentState4 = LOW; //Sw3 Button State
boolean lastState4    = LOW; //Sw3 Button State

int i, p, x, y;
int waktu = 6;
int noID = 0;

char NmrID;

unsigned long milisterkini  = 0;
unsigned long milisset      = 0;

// -------------------- Variabel Fingerprint --------------------
uint8_t id;
uint8_t akun = 0;

//================================================== Setup ==================================================

void setup() {

  Serial.begin(9600);
  finger.begin(57600); 
  lcd.begin();
  lcd.backlight();

  pinMode(Sw1       , INPUT);
  pinMode(Sw2       , INPUT);
  pinMode(Sw3       , INPUT);
  pinMode(touchless , INPUT);
  pinMode(selenoid  , OUTPUT);

  // -------------------- Monitoring Modul apakah sudah siap digunakan --------------------
  if (finger.verifyPassword()) {
    Serial.println("Perangkat Siap Digunakan");
  
  } else {
    Serial.println("FingerPrint Sensor Tidak Ditemukan! Perangkat Tidak Siap");
    lcd.clear();
    lcd.setCursor(3, 0);
    lcd.printstr("FINGERPRINT");
    lcd.setCursor(6, 1);
    lcd.printstr("TIDAK");
    lcd.setCursor(-1, 2);
    lcd.printstr("TERDETEKSI");
    lcd.setCursor(-3, 3);
    lcd.printstr("CEK INSTALASI!");

    while (1) {
      delay(1);
    }
  }
  
  display1();
}

//================================================== Template Display ==================================================



void display1() { // display

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.printstr("Selamat Datang");
  lcd.setCursor(0, 1);
  lcd.printstr("Silahkan masukan");
  lcd.setCursor(-4, 2);
  lcd.printstr("Sidik jari anda");
  lcd.setCursor(5, 3);
  lcd.printstr("====>");

}

void gagal() {
  lcd.clear();
  lcd.setCursor(1, 0);
  lcd.printstr("Sidik Jari");
  lcd.setCursor(1, 1);
  lcd.printstr("Tidak Terdaftar");
  lcd.setCursor(2, 2);
  lcd.printstr("Silahkan");
  lcd.setCursor(2, 3);
  lcd.printstr("Coba Lagi");
}

void error() {
  lcd.clear();
  lcd.setCursor(2, 1);
  lcd.printstr("Pembacaan");
  lcd.setCursor(-2, 2);
  lcd.printstr("Error!!!");
}

void kurangjelas() {
  lcd.clear();
  lcd.setCursor(1, 1);
  lcd.printstr("Sidik Jari");
  lcd.setCursor(-3, 2);
  lcd.printstr("Kurang Jelas");

}


void konfirmasi() {
  lcd.clear();
  lcd.setCursor(0, 1);
  lcd.printstr("Harap Masuk");
  lcd.setCursor(-4, 2);
  lcd.printstr("Sebagai ADMIN");

}

void gagalmasuk() {
  lcd.clear();
  lcd.setCursor(2, 0);
  lcd.printstr("Bukan ADMIN!!!");
  delay(750);
  lcd.clear();
  lcd.setCursor(2, 0);
  lcd.printstr("Bukan ADMIN!!!");
  lcd.setCursor(-4, 2);
  lcd.printstr("Harap Masuk");
  lcd.setCursor(-4, 3);
  lcd.printstr("Sebagai ADMIN");

}

void cocok() {
  lcd.clear();
  lcd.setCursor(0, 1);
  lcd.printstr("Terdaftar Sebagai");
  lcd.setCursor(-4, 2);
  lcd.printstr("ADMIN");

}

void tampilanID() {
  lcd.clear();
  lcd.setCursor(3, 0);
  lcd.printstr("Pilih Akun");
  lcd.setCursor(-2, 2);
  lcd.printstr("ID : ");
  lcd.setCursor(6, 2);
  lcd.print(noID);

}

void sidikbaru() {
  lcd.clear();
  lcd.setCursor(1, 0);
  lcd.printstr("Masukan");
  lcd.setCursor(1, 1);
  lcd.printstr("Sidik Jari");
  lcd.setCursor(-1, 2);
  lcd.printstr("yang ingin");
  lcd.setCursor(-1, 3);
  lcd.printstr("didaftarkan");

}

void gagaldaftar() {
  lcd.clear();
  lcd.setCursor(2, 0);
  lcd.printstr("Gagal");
  lcd.setCursor(2, 1);
  lcd.printstr("Mendaftarkan");
  lcd.setCursor(-2, 2);
  lcd.printstr("Sidik Jari");

}

void berhasil() {
  lcd.clear();
  lcd.setCursor(1, 1);
  lcd.printstr("Berhasil");
  lcd.setCursor(-3, 2);
  lcd.printstr("Mendaftarkan ");
  lcd.setCursor(-3, 3);
  lcd.printstr("Sidik Jari");

}

void batal(){
  lcd.clear();
  lcd.setCursor(2, 1);
  lcd.printstr("Perintah");
  lcd.setCursor(-2, 2);
  lcd.printstr("Dibatalkan");
  delay(1000);  
}
//================================================== Buka Pintu ==================================================

void Door() {
  digitalWrite  (selenoid,    HIGH);
  digitalWrite  (Led1,        HIGH);
  Serial.println("Pintu Dapat dibuka");
  lcd.clear();
  lcd.setCursor(1, 0);
  lcd.printstr("Sidik Jari");
  lcd.setCursor(1, 1);
  lcd.printstr("Terdaftar");
  lcd.setCursor(-3, 2);
  lcd.printstr("Pintu terbuka");
  delay(2000);
  milisset  = (millis() / 1000);
  display1();
  delay(10);
}

void timerpintu2(){
  // Jika Pintu Terbuka Maka Akan otomatis menghitung
  milisterkini = (millis() / 1000);
  if ((milisterkini - milisset) >= waktu) {
    digitalWrite  (selenoid,    LOW);
    digitalWrite  (Led1,        LOW);
  }
}
//================================================== Main Program ==================================================

void loop() {

  currentState1 = digitalRead(touchless);
  currentState4 = digitalRead(Sw3); //Green

  if (currentState1 == HIGH && lastState1 == LOW) {
    Door();
  }

  lastState1 = currentState1;

  milisterkini = (millis() / 1000);
  if ((milisterkini - milisset) == waktu) {
    digitalWrite  (selenoid,    LOW);
    digitalWrite  (Led1,        LOW);
  }
  
  
  delay(10);
  SetUlang();
  getFingerprintIDez();
  delay(50);

}

//================================================== Set Ulang ==================================================

void SetUlang() {

  if (currentState4 == HIGH && lastState4 == LOW) {
    Serial.println("Menyetel Ulang, Butuh Konfirmasi Sidik Jari");
    Serial.println("Konfirmasi Admin");
    Serial.println("Masukan Sidik Jari");
    konfirmasi();
    //--------------------
    p = -1;
      while (p != FINGERPRINT_OK) {
      currentState2 = digitalRead(Sw1); //Green
      if (currentState2 == HIGH && lastState2 == LOW) {
        Serial.println("Cancel");
        batal();
        display1();
        return p;
      }
      lastState2 = currentState2;  
      timerpintu2();
      p = finger.getImage();
      switch (p) {
        case FINGERPRINT_OK:
          Serial.println("Image taken");
          break;
        case FINGERPRINT_NOFINGER:
          break;
      }
    }
    //--------------------
    p = finger.image2Tz(1);
    switch (p) {
      case FINGERPRINT_OK:
        Serial.println("Image converted");
        break;
      case FINGERPRINT_IMAGEMESS:
        Serial.println("Image too messy");
        kurangjelas();
        delay(1000);
        display1();
        return p;
    }
    //--------------------
    p = finger.fingerFastSearch();
    if (p == FINGERPRINT_OK) {
      Serial.println("Found a print match!");
    }
    else if (p == FINGERPRINT_NOTFOUND) {
      Serial.println("Did not find a match");
      gagal();
      delay(1000);
      display1();
      return p;
    }
    //--------------------

    x = finger.fingerID;
    timerpintu2();

    //Hanya Jika Template ID No. 20 dan 19 baru dianggap sebagai Admin 
    if (x == 20) {
      Serial.println("Masuk Sebagai ADMIN");
      cocok();
    }
    else if (x == 19) {
      Serial.println("Masuk Sebagai ADMIN");
      cocok();
    }
    else {
      Serial.println("Bukan Admin");
      gagalmasuk();
      delay(1000);
      display1();
      return p;
    }
    delay(10);
    tampilanID();

    //--------------------

    y = -1;
    while (y != 0) {
      currentState2 = digitalRead(Sw1); //Green
      currentState3 = digitalRead(Sw2); //Green
      currentState4 = digitalRead(Sw3); //Green
      
      timerpintu2(); // Mengatur Ulang kondisi Pintu yang tertunda selama proses pendaftaran 
    
	// ---------- Memilih No. ID yang akan di daftarkan mulai dari ID template 0 hingga 9 (10 ID)
      if (currentState2 == HIGH && lastState2 == LOW) {
        noID = noID + 1;
        if (noID < 0) {
          noID = 0;
        }
        if (noID > 10) {
          noID = 10;
        }
        akun = noID;
        Serial.println("Pilih Slot ID !");
        Serial.print("Slot ID : ");
        Serial.println(noID);
        tampilanID();
      }
	
      if (currentState3 == HIGH && lastState3 == LOW) {
        noID = noID - 1;

        if (noID < 0) {
          noID = 0;
        }
        if (noID > 10) {
          noID = 10;
        }
        akun = noID;
        Serial.println("Pilih Slot ID !");
        Serial.print("Slot ID : ");
        Serial.println(noID);
        tampilanID();
      }

	//--------------------
	
      if (currentState4 == HIGH && lastState4 == LOW) {
        Serial.println("ID telah Dipilih");
        y = 0;
        id = akun;
      }

      lastState2 = currentState2;
      lastState3 = currentState3;
      lastState4 = currentState4;
      delay(10);
    }
  
  
    //--------------------

    deleteFingerprint(id);
    Serial.print("Hapus ID : ");
    Serial.println(id);

    delay(500);  
    Serial.println("Masukan Sidik Jari Baru");
    sidikbaru();

    //--------------------
    p = -1;
    while (p != FINGERPRINT_OK) {
    p = finger.getImage();
    currentState2 = digitalRead(Sw1); //Green
      if (currentState2 == HIGH && lastState2 == LOW) {
        Serial.println("Cancel");
        batal();
        display1();
        return p;
      }
    lastState2 = currentState2;  
    switch (p) {
      case FINGERPRINT_OK:
        Serial.println("Image taken");
        break;
      case FINGERPRINT_NOFINGER:
        break;
      default:
        Serial.println("Unknown error");
        error();
        delay(1000);
        display1();
        break;
      }
    }
    
    //--------------------
    
    p = finger.image2Tz(1);
    switch (p) {
      case FINGERPRINT_OK:
        Serial.println("Image converted");
        break;
      case FINGERPRINT_IMAGEMESS:
        Serial.println("Image too messy");
        return p;
      case FINGERPRINT_PACKETRECIEVEERR:
        Serial.println("Communication error");
        kurangjelas();
        delay(1000);
        display1();
        return p;
      default:
        Serial.println("Unknown error");
        error();
        delay(1000);
        display1();
        return p;
    }
    
    //--------------------
    
    Serial.println("Konfirmasi ulang Sidik Jari Baru");
  
    lcd.clear();
    lcd.setCursor(3, 1);
    lcd.printstr("Konfimasi");
    lcd.setCursor(4, 2);
    lcd.printstr("Ulang");

    delay(1000);
    
    p = 0;
    while (p != FINGERPRINT_NOFINGER) {
      p = finger.getImage();
    }
    
    //--------------------
    
    p = -1;
    Serial.println("Place same finger again");
    while (p != FINGERPRINT_OK) {
      p = finger.getImage();
      currentState2 = digitalRead(Sw1); //Green
      if (currentState2 == HIGH && lastState2 == LOW) {
        Serial.println("Cancel");
        batal();
        display1();
        return p;
      }
      
      lastState2 = currentState2;  
    
      switch (p) {
      case FINGERPRINT_OK:
        Serial.println("Image taken");
        break;
      case FINGERPRINT_NOFINGER:
        break;
      default:
        Serial.println("Unknown error");
        error();
        delay(1000);
        display1();
        break;
      }
    }

    p = finger.image2Tz(2);
    switch (p) {
      case FINGERPRINT_OK:
        Serial.println("Image converted");
        break;
      case FINGERPRINT_IMAGEMESS:
        Serial.println("Image too messy");
        kurangjelas();
        delay(1000);
        display1();
        return p;
      case FINGERPRINT_PACKETRECIEVEERR:
        Serial.println("Communication error");
        error();
        delay(1000);
        display1();
        return p;
      default:
        Serial.println("Unknown error");
        error();
        delay(1000);
        display1();  
        return p;
      }

    p = finger.createModel();
      if (p == FINGERPRINT_OK) {
        Serial.println("Prints matched!");
      } 
      else if (p == FINGERPRINT_PACKETRECIEVEERR) {
        Serial.println("Communication error");
        error();
        delay(1000);
        display1();  
        return p;
      } 
      else if (p == FINGERPRINT_ENROLLMISMATCH) {
        Serial.println("Fingerprints did not match");
        gagaldaftar();
        delay(1000);
        display1();
        return p;
      } 
      else {
        Serial.println("Unknown error");
        error();
        delay(1000);
        display1();  
        return p;
      }   
  
    p = finger.storeModel(id);
      if (p == FINGERPRINT_OK) {
        Serial.println("Stored!");
      } 
      else if (p == FINGERPRINT_PACKETRECIEVEERR) {
        Serial.println("Communication error");
        error();
        delay(1000);
        display1();  
        return p;
      } 
      else {
        Serial.println("Unknown error");
        error();
        delay(1000);
        display1();  
        return p;
    } 

    
    //--------------------

    Serial.println("Sidik Jari Baru Tersimpan");

    berhasil();
    delay(1000);
    display1();
  }

  currentState4 = digitalRead(Sw3); //Green
  lastState4 = currentState4;

}

//================================================== Scan Fingerprint untuk membuka Doorlock ==================================================

int getFingerprintIDez() {
  // Ambil Gambar
  uint8_t p = finger.getImage();
  switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Sidik Jari didapatkan");
      lcd.clear();
      lcd.setCursor(2, 1);
      lcd.printstr("Sidik Jari");
      lcd.setCursor(2, 2);
      lcd.printstr("Terbaca");
      delay(750);
      break;
    case FINGERPRINT_NOFINGER:
      return p;
    case FINGERPRINT_IMAGEFAIL:
      Serial.println("Gambar Tidak Jelas");
      kurangjelas();
      //myMp3.play(2);
      delay(1000);
      display1();
      return p;
  }

  // Converting Image
  p = finger.image2Tz();
  switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Screening Sidik Jari");
      break;
    case FINGERPRINT_IMAGEMESS:
      Serial.println("Gambar kurang jelas"); //indikator
      kurangjelas();
      //myMp3.play(2);
      delay(1000);
      display1();
      return p;
    default:
      Serial.println("Unknown error"); //indikator
      error();
      delay(1000);
      display1();
      return p;
  }

  // OK converted!
  p = finger.fingerFastSearch();
  if (p == FINGERPRINT_OK) {
    Serial.println("Sidik Jari Cocok!");
    Serial.println("Door Unlocked");
    Door();
    delay(100);
  }
  else if (p == FINGERPRINT_NOTFOUND) {
    Serial.println("Sidik Jari Tidak Terdaftar"); //indikator
    gagal();
    //myMp3.play(2);
    delay(1000);
    display1();
    return p;
  }
  else {
    Serial.println("Unknown error"); //indikator
    error();
    delay(1000);
    display1();
    return p;
  }

}

//================================================== Hapus Memory Finger Adress ==================================================

uint8_t deleteFingerprint(uint8_t id) {
  uint8_t p = -1;

  p = finger.deleteModel(akun);

  if (p == FINGERPRINT_OK) {
    Serial.println("Deleted!");
  }
  else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    Serial.println("Communication error");
    error();
    delay(1000);
    display1();
    return p;
  }
  else if (p == FINGERPRINT_BADLOCATION) {
    Serial.println("Could not delete in that location");
    error();
    delay(1000);
    display1();
    return p;
  }
  else {
    Serial.print("Unknown error: 0x"); Serial.println(p, HEX);
    error();
    delay(1000);
    display1();
    return p;
  }
}

//================================================== Selesai ==================================================
