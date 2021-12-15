#include <Arduino.h>
#include <arInterface.h> // my Interface Library (for TFT)

// for MFRC522
#include <MFRC522.h>
#define SS_PIN 53
#define RST_PIN 49
MFRC522 rfid(SS_PIN, RST_PIN);
MFRC522::MIFARE_Key key;

byte nuidPICC[4]; // Init array that will store new NUID
void printHexDec(byte *buffer, byte bufferSize, bool flHex);

AR_Interface interface;

void setup()
{
  Serial.begin(115200);
  Serial.println("_____________SETUP________________");

  SPI.begin();     // Init SPI bus
  rfid.PCD_Init(); // Init MFRC522

  for (byte i = 0; i < 6; i++)
  {
    key.keyByte[i] = 0xFF;
  }

  Serial.println(F("This code scan the MIFARE Classsic NUID."));
  Serial.print(F("Using the following key:"));
  printHexDec(key.keyByte, MFRC522::MF_KEY_SIZE, true);

  interface.tftInit(0);    //ініціалізація TFT екрану
  interface.drawStatic(2); //сторінка із статичними елементами
}

void loop()
{
  // Verify if new card preset or the NUID has been readed
  if ( ! rfid.PICC_IsNewCardPresent() || ! rfid.PICC_ReadCardSerial())
    return;

  Serial.print(F("PICC type: "));
  MFRC522::PICC_Type piccType = rfid.PICC_GetType(rfid.uid.sak);
  Serial.println(rfid.PICC_GetTypeName(piccType));

  // Check is the PICC of Classic MIFARE type
  if (piccType != MFRC522::PICC_TYPE_MIFARE_MINI &&
      piccType != MFRC522::PICC_TYPE_MIFARE_1K &&
      piccType != MFRC522::PICC_TYPE_MIFARE_4K) {
    Serial.println(F("Your tag is not of type MIFARE Classic."));
    return;
  }
  if (rfid.uid.uidByte[0] != nuidPICC[0] || 
    rfid.uid.uidByte[1] != nuidPICC[1] || 
    rfid.uid.uidByte[2] != nuidPICC[2] || 
    rfid.uid.uidByte[3] != nuidPICC[3] ) {
    Serial.println(F("A new card has been detected."));

    // Store NUID into nuidPICC array
    for (byte i = 0; i < 4; i++) {
      nuidPICC[i] = rfid.uid.uidByte[i];
    }
   
    Serial.println(F("The NUID tag is:"));
    Serial.print(F("In hex: "));
    printHexDec(rfid.uid.uidByte, rfid.uid.size, true);
    Serial.println();
    Serial.print(F("In dec: "));
    printHexDec(rfid.uid.uidByte, rfid.uid.size, false);
    Serial.println();
  }
  else Serial.println(F("Card read previously."));

  // Halt PICC
  rfid.PICC_HaltA();

  // Stop encryption on PCD
  rfid.PCD_StopCrypto1();
}

/**
   Helper routine to dump a byte array as hex values to Serial.
*/
void printHexDec(byte *buffer, byte bufferSize, bool flHex) {
  for (byte i = 0; i < bufferSize; i++) {
    Serial.print(buffer[i] < 0x10 ? " 0" : " ");
    if (flHex) Serial.print(buffer[i], HEX); else Serial.print(buffer[i], DEC);
  }
}