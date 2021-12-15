/* Клас інтерфейсу
 * Займається всім, що пов'язано з виведенням на екран
 * + робота з SD Card
       The circuit:
       SD card attached to SPI bus as follows:
     ** MOSI - pin 51
     ** MISO - pin 50
     ** CLK  - pin 52
     ** CS   - pin 53
*/

#pragma once

#define LCD_CS A3 // Chip Select goes to Analog 3
#define LCD_CD A2 // Command/Data goes to Analog 2
#define LCD_WR A1 // LCD Write goes to Analog 1
#define LCD_RD A0 // LCD Read goes to Analog 0
#define LCD_RESET A4 // Can alternately just connect to Arduino's reset pin

#include <Arduino.h>
#include <SPI.h>
#include <MCUFRIEND_kbv.h>
#include <Fonts/FreeMonoBold9pt7b.h>
#include <Fonts/FreeMonoBold12pt7b.h>
#include <Fonts/FreeSansBold12pt7b.h>
#include <Fonts/FreeSansBold24pt7b.h>
#include <Fonts/FreeSerifBold12pt7b.h>
#include <Fonts/FreeSerifBold18pt7b.h>
#include <Fonts/FreeSansBoldOblique24pt7b.h>
#include <Fonts/FreeMonoBoldOblique18pt7b.h>
#include <FreeDefaultFonts.h>
#include <FontsRus/TimesNRCyr18.h>
#include "arBitmap.h"

// Assign human-readable names to some common 16-bit color values:
#define BLACK   0x0000
#define BLUE    0x001F
#define DSBLUE  0x06FF
#define RED     0xF800
#define GREEN   0x07E0
#define CYAN    0x07FF
#define MAGENTA 0xF81F
#define YELLOW  0xFFE0
#define WHITE   0xFFFF
#define GRAY    0xBDF7
#define PURPLE  0xA89F
#define DGREEN  0x1364

class AR_Interface
{
  private:
    MCUFRIEND_kbv* _tft;
    String utf8cyr(String source);        //для виведення українських букв
    //виведення англійського тексту
    void smartPrint(uint16_t, uint16_t, uint8_t, uint16_t, const char*);
    void smartPrint(uint16_t, uint16_t, uint8_t, uint16_t, const String);
    //виведення українського тексту
    void smartPrintCyr(uint16_t, uint16_t, uint8_t, uint16_t, const String);
    //виведення цифр
    void smartPrint(uint16_t, uint16_t, uint8_t, uint16_t, uint32_t);
    void procMyRect(uint16_t, uint16_t, uint16_t, uint16_t, uint8_t, uint16_t, uint8_t); // wLine - ширина лінії в пікселях
    uint16_t procNdigit(uint16_t, uint8_t, uint16_t); //метод розрахунку позиції тексту в залежності від кількості розрядів
    uint16_t procNdigit(uint16_t, uint8_t, float);    //метод розрахунку позиції тексту в залежності від кількості розрядів
    float prevValue = 0;                              //попереднє виведене значення (для затирання чорним)

  public:
    AR_Interface();
    //ініціалізація TFT
    void tftInit(uint8_t);
    //очищення екрану
    void clearTFT(uint16_t);
    //виведення статичної інформації
    void drawStatic(uint8_t);
};
