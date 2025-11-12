/*
 * Sketch to test ESP32 with RC522 RFID module
 * 
 * Typical pin layout used for RC522:
 * ---------------------------------
 *             MFRC522      ESP32
 *             Reader/PCD   NodeMCU
 * Signal      Pin          Pin
 * ---------------------------------
 * RST/Reset   RST          G21
 * SPI SS      SDA(SS)      G5
 * SPI MOSI    MOSI         G23
 * SPI MISO    MISO         G19
 * SPI SCK     SCK          G18
 * 3V3         3V3          3V3
 * GND         GND          GND
 *
 * Typical pin layout used for LED: G12
 */

#include <MFRC522v2.h>
#include <MFRC522DriverSPI.h>
#include <MFRC522DriverPinSimple.h>
#include <MFRC522Debug.h>

#define RST 21
#define SDA 5
#define MOSI 23
#define MISO 19
#define SCK 18
#define LED 12

unsigned char ALLOWED_UID[] = { 0xB1, 0xB6, 0x72, 0x3C };

MFRC522DriverPinSimple ss_pin(SDA);  // Configurable, see typical pin layout above.
MFRC522DriverSPI driver{ ss_pin };   // Create SPI driver.
MFRC522 mfrc522{ driver };           // Create MFRC522 instance.

void setup() {
  Serial.begin(115200);  // Initialize serial communications with the PC for debugging.
  while (!Serial)
    ;                                                      // Do nothing if no serial port is opened (added for Arduinos based on ATMEGA32U4).
  pinMode(LED, OUTPUT);                                    // Initialize the LED (simulating door switch)
  mfrc522.PCD_Init();                                      // Init MFRC522 board.
  MFRC522Debug::PCD_DumpVersionToSerial(mfrc522, Serial);  // Show details of PCD - MFRC522 Card Reader details.
  Serial.println(F("Scan PICC to see UID, SAK, type, and data blocks..."));
}

void loop() {
  // Reset the loop if no new card present on the sensor/reader. This saves the entire process when idle.
  if (!mfrc522.PICC_IsNewCardPresent()) {
    return;
  }

  // Select one of the cards
  if (!mfrc522.PICC_ReadCardSerial()) {
    return;
  }

  // Dump debug info about the card; PICC_HaltA() is automatically called
  MFRC522Debug::PICC_DumpToSerial(mfrc522, Serial, &(mfrc522.uid));

  // Check if allowed UID is the same as the entered card
  if (memcmp(mfrc522.uid.uidByte, ALLOWED_UID, sizeof(ALLOWED_UID)) == 0) {
    Serial.println(F("Card present ..."));
    digitalWrite(LED, HIGH);  // turn the LED on (HIGH is the voltage level)
    delay(10000);             // wait for 10 second
    digitalWrite(LED, LOW);   // turn the LED off by making the voltage LOW
  }
}
