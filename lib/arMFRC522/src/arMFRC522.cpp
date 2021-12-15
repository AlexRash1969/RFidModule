#include "arMFRC522.h"

AR_MFRC522::AR_MFRC522(uint8_t pSS, uint8_t pRst)
{
  _rfid = new MFRC522(pSS, pRst);
}

void AR_MFRC522::rfidInit()
{
  SPI.begin();       // Init SPI bus
  _rfid->PCD_Init(); // Init MFRC522

  //очищаємо масив для ключа key
  for (byte i = 0; i < 6; i++)
  {
    key.keyByte[i] = 0xFF;
  }

  Serial.println(F("This code scan the MIFARE Classsic NUID."));
  Serial.print(F("Using the following key:"));
  printHexDec(key.keyByte, MFRC522::MF_KEY_SIZE, true);
}

bool AR_MFRC522::readNewNUID(byte *nuidPICC, String *whiwh)
{
  // Verify if new card preset or the NUID has been readed
  if (!_rfid->PICC_IsNewCardPresent() || !_rfid->PICC_ReadCardSerial())
    return false;

  Serial.print(F("PICC type: "));
  MFRC522::PICC_Type piccType = _rfid->PICC_GetType(_rfid->uid.sak);
  Serial.println(_rfid->PICC_GetTypeName(piccType));

  // Check is the PICC of Classic MIFARE type
  if (piccType != MFRC522::PICC_TYPE_MIFARE_MINI &&
      piccType != MFRC522::PICC_TYPE_MIFARE_1K &&
      piccType != MFRC522::PICC_TYPE_MIFARE_4K)
  {
    Serial.println(F("Your tag is not of type MIFARE Classic."));
    return false;
  }
  //перевіряємо чи мітка не та, яку підносили останнього разу
  if (_rfid->uid.uidByte[0] != nuidPICC[0] ||
      _rfid->uid.uidByte[1] != nuidPICC[1] ||
      _rfid->uid.uidByte[2] != nuidPICC[2] ||
      _rfid->uid.uidByte[3] != nuidPICC[3])
  {
    Serial.println(F("A new card has been detected."));

    // Store NUID into nuidPICC array
    for (byte i = 0; i < 4; i++)
    {
      nuidPICC[i] = _rfid->uid.uidByte[i];
    }

    Serial.println(F("The NUID tag is:"));
    Serial.print(F("In hex: "));
    printHexDec(_rfid->uid.uidByte, _rfid->uid.size, true);
    Serial.println();
    Serial.print(F("In dec: "));
    printHexDec(_rfid->uid.uidByte, _rfid->uid.size, false);
    Serial.println();
    uint16_t tagId = seekId(nuidPICC);
    if(tagId){
      //якщо знайшли заповнюємо дані про власника
      for (int i = 0; i < 4; i++){
        whiwh[i] = whoISwho[tagId][i];
      }
    }
  }
  else
  {
    Serial.println(F("Card read previously."));
    return false;
  }

  // Halt PICC
  _rfid->PICC_HaltA();

  // Stop encryption on PCD
  _rfid->PCD_StopCrypto1();
  return true;
}

uint16_t AR_MFRC522::seekId(const byte *tagNUID)
{
  for (int i = 0; i < 8; i++)
  {
    if (aNUIDb[i][0] == tagNUID[0] &&
        aNUIDb[i][1] == tagNUID[1] &&
        aNUIDb[i][2] == tagNUID[2] &&
        aNUIDb[i][3] == tagNUID[3])
    {
      return i;
    }
  }
  return 0;
}

void AR_MFRC522::printHexDec(byte *buffer, byte bufferSize, bool flHex)
{
  for (byte i = 0; i < bufferSize; i++)
  {
    Serial.print(buffer[i] < 0x10 ? " 0" : " ");
    if (flHex)
      Serial.print(buffer[i], HEX);
    else
      Serial.print(buffer[i], DEC);
  }
}