#pragma once

#include <MFRC522.h>

class AR_MFRC522
{
private:
  MFRC522 *_rfid;
  MFRC522::MIFARE_Key key;
  //метод для друку масивів з Hex i Dec значеннями
  void printHexDec(byte *, byte, bool);
  //метод для пошуку мітки в базі (повертає id)
  uint16_t seekId(const byte*);

  byte aNUIDb[8][4] =
      {
          {0x1A, 0x46, 0x6B, 0x15},
          {0xBA, 0x75, 0x1F, 0xA3},
          {0x1A, 0xBA, 0x3B, 0x16}, //gOld
          {0xCA, 0x80, 0x69, 0x15},
          {0x27, 0xA3, 0x6D, 0x4B},
          {0x8A, 0x90, 0x3D, 0x16},
          {0x0A, 0x3C, 0x8A, 0x16}, //7
          {0x37, 0x8D, 0x6E, 0x4B}};
  String whoISwho[8][4]{
      {"First", "Humanoid", "(095)000-00-01", "customer"},
      {"Second", "Humanoid","(095)000-00-02", "customer"},
      {"Ilon", "Mask","+1(000)000001", "Super Boss"},
      {"Fourth", "Humanoid","(095)000-00-04", "customer"},
      {"Fifth", "Humanoid","(095)000-00-05", "customer"},
      {"Sixth", "Humanoid","(095)000-00-06", "customer"},
      {"Bill", "Gates","0-800-308-80-0", "Big Boss"},
      {"Eighth", "Humanoid","(095)000-00-08", "customer"},
      };

public:
  //конструктор з параметрами SS_PIN, RST_PIN
  AR_MFRC522(uint8_t, uint8_t);
  void rfidInit();
  bool readNewNUID(byte *, String *);
};
