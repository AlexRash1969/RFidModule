#include <Arduino.h>
#include "arInterface.h" // my Interface Library (for TFT)
#include "arMFRC522.h"   // my RFiD Library

AR_Interface tft;
AR_MFRC522   rfid(53, 49);
byte nuidPICC[4];      // Init array that will store new NUID
String whoISwho[4];    // Для інформації про людину (Ім'я, телефон, права)

void setup()
{
  Serial.begin(115200);
  Serial.println("_____________SETUP________________");
  rfid.rfidInit();         //ініціалізація RFiD модуля
  tft.tftInit(0);    //ініціалізація TFT екрану
  tft.drawStatic(2); //сторінка із статичними елементами
}

void loop()
{
  if(rfid.readNewNUID(nuidPICC, whoISwho)){
    Serial.println(F("-A new card has been detected.-"));
    tft.printNIUDinfo(nuidPICC, whoISwho);
  }
}
