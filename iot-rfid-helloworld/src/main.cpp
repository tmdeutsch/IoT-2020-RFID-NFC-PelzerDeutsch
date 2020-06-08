#include <MFRC522.h>
#include <SPI.h>

//#define RST_PIN 9 // Configurable, see typical pin layout above
//#define SS_PIN 10 // Configurable, see typical pin layout above
const int RST_PIN = 22;  // Reset pin
const int SS_PIN = 21;   // Slave select pin

#define PIN_OUT1 33
#define PIN_OUT2 26

MFRC522 mfrc522(SS_PIN, RST_PIN);  // Create MFRC522 instance

void setup() {
    pinMode(PIN_OUT1, OUTPUT);
    pinMode(PIN_OUT2, OUTPUT);
    Serial.begin(9600);  // Initialize serial communications with the PC
    while (!Serial)
        ;  // Do nothing if no serial port is opened (added for Arduinos based
           // on ATMEGA32U4)
    SPI.begin();                        // Init SPI bus
    mfrc522.PCD_Init();                 // Init MFRC522
    mfrc522.PCD_DumpVersionToSerial();  // Show details of PCD - MFRC522 Card
    // Reader details
    Serial.println(F("Scan PICC to see UID, SAK, type, and data blocks..."));
}

/**
 * mfrc522.PICC_IsNewCardPresent() should be checked before
 * @return the card UID
 */
unsigned long getID() {
    if (!mfrc522.PICC_ReadCardSerial()) {  // Since a PICC placed get Serial and
                                           // continue
        return -1;
    }
    unsigned long hex_num;
    hex_num = mfrc522.uid.uidByte[0] << 24;
    hex_num += mfrc522.uid.uidByte[1] << 16;
    hex_num += mfrc522.uid.uidByte[2] << 8;
    hex_num += mfrc522.uid.uidByte[3];
    mfrc522.PICC_HaltA();  // Stop reading
    return hex_num;
}

void loop() {
    digitalWrite(PIN_OUT1, LOW);
    digitalWrite(PIN_OUT2, LOW);
    // Serial.println("Look for new cards");
    if (mfrc522.PICC_IsNewCardPresent()) {
        unsigned long uid = getID();
        if (uid != -1) {
            Serial.print("Card detected, UID: ");
            Serial.println(uid);
        }
        if (uid == 2589037589) {
            digitalWrite(PIN_OUT1, HIGH);
        }
        if (uid == 3111100811) {
            digitalWrite(PIN_OUT2, HIGH);
        }
    }

    // Select one of the cards
    if (!mfrc522.PICC_ReadCardSerial()) {
        return;
    }

    // Dump debug info about the card; PICC_HaltA() is automatically called
    // mfrc522.PICC_DumpToSerial(&(mfrc522.uid));
}
