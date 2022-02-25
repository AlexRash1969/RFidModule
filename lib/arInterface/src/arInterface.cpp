/*
  розмір екрану 240 х 320
*/
#include "arInterface.h"

AR_Interface::AR_Interface()
{
  _tft = new MCUFRIEND_kbv;
}

void AR_Interface::clearTFT(uint16_t color)
{
  _tft->fillScreen(color);
}

void AR_Interface::tftInit(uint8_t aspect)
{
  uint16_t ID = _tft->readID();
  _tft->begin(ID);
  _tft->setRotation(aspect); //встановлення орієнтації зображення на екрані (0-4)
  _tft->fillScreen(BLACK);   //заливка екрану чорним
  _tft->cp437(true);         //для кирилизації бібліотеки Adafruit-GFX
}

void AR_Interface::drawStatic(uint8_t countPlayer)
{
  _tft->setFont(&FreeSerifBold18pt7b);
  _tft->fillRect(0, 0, 240, 30, BLUE);
  smartPrint(8, 26, 1, YELLOW, "RFiD terminal");

  _tft->setFont(&FreeMonoBold9pt7b);
  _tft->fillRect(0, 300, 240, 20, BLUE);
  smartPrint(14, 314, 1, GREEN, "BMY v0.0 c2021-2022");
}

void AR_Interface::printNIUDinfo(byte *nuidPICC, String *info)
{
  //змінні кординат для синхронного пересування пов'язаних об'єктів (тексту і графіки)
  uint8_t  x = 4;
  uint16_t y = 85;
  _tft->setFont(&FreeSerifBold18pt7b);
  //точка відліку - ліва верхня
  _tft->fillRoundRect(2, y - 27, 236, 76, 5, WHITE);
  _tft->fillRoundRect(2, y - 27 + 95, 236, 36, 5, WHITE);
  _tft->fillRoundRect(2, y - 27 + 150, 236, 36, 5, WHITE);
  smartPrint(x, y, 1, PURPLE, info[0]);
  smartPrint(x, y + 40, 1, PURPLE, info[1]);
  smartPrint(x, y + 95, 1, PURPLE, info[2]);
  smartPrint(x, y + 150, 1, PURPLE, info[3]);

  String tmpNUID = "NUID: ";
  for (byte i = 0; i < 4; i++)
  {
    String tmp = String(nuidPICC[i], HEX);
    tmp = tmp.length() < 2 ? "0" + tmp : tmp;
    tmp += " ";
    tmpNUID += tmp;
  }
  tmpNUID.toUpperCase();
  _tft->setFont(&FreeSerifBold12pt7b);
  _tft->fillRect(0, 270, 240, 18, BLACK);
  smartPrint(16, 286, 1, CYAN, tmpNUID);
}

void AR_Interface::procMyRect(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint8_t r, uint16_t colour, uint8_t wLine)
{
  for (int i = 0; i < wLine; i++) //Щоб намалювати 3 прямокутники - збільшити товщину лінії
  {
    _tft->drawRoundRect(x + i, y + i, w - 2 * i, h - 2 * i, r - i, colour);
  }
}

void AR_Interface::smartPrint(uint16_t x, uint16_t y, uint8_t sz, uint16_t color, const char *msg)
{
  _tft->setTextColor(color);
  _tft->setCursor(x, y);
  _tft->setTextSize(sz);
  _tft->print(msg);
}

void AR_Interface::smartPrint(uint16_t x, uint16_t y, uint8_t sz, uint16_t color, const String msg)
{
  _tft->setTextColor(color);
  _tft->setCursor(x, y);
  _tft->setTextSize(sz);
  _tft->print(msg);
}

void AR_Interface::smartPrintCyr(uint16_t x, uint16_t y, uint8_t sz, uint16_t color, const String msg)
{
  _tft->setTextColor(color);
  _tft->setFont();
  _tft->setCursor(x, y);
  _tft->setTextSize(sz);
  _tft->print(utf8cyr(msg));
}

void AR_Interface::smartPrint(uint16_t x, uint16_t y, uint8_t sz, uint16_t color, uint32_t value)
{
  _tft->setTextColor(color);
  _tft->setCursor(x, y);
  _tft->setTextSize(sz);
  _tft->print(value);
}

String AR_Interface::utf8cyr(String source)
{
  int i, k;
  String target;
  unsigned char n;
  char m[2] = {'0', '\0'};

  k = source.length();
  i = 0;

  while (i < k)
  {
    n = source[i];
    i++;

    if (n >= 0xC0)
    {
      switch (n)
      {
      case 0xD0:
      {
        n = source[i];
        i++;
        if (n == 0x81)
        {
          n = 0xA8;
          break;
        }
        if (n >= 0x90 && n <= 0xBF)
          n = n + 0x30;
        break;
      }
      case 0xD1:
      {
        n = source[i];
        i++;
        if (n == 0x91)
        {
          n = 0xB8;
          break;
        }
        if (n >= 0x80 && n <= 0x8F)
          n = n + 0x70;
        break;
      }
      }
    }
    m[0] = n;
    target = target + String(m);
  }
  return target;
}

uint16_t AR_Interface::procNdigit(uint16_t start, uint8_t shift, uint16_t value)
{
  uint8_t n = 0;
  if (value > 999)
    n = 0;
  else if (value > 99)
    n = 1;
  else if (value > 9)
    n = 2;
  else
    n = 3;
  return start + shift * n;
}

uint16_t AR_Interface::procNdigit(uint16_t start, uint8_t shift, float value)
{
  uint8_t n = 0;
  if (value >= 1000)
    n = 3;
  else if (value >= 100)
    n = 2;
  else if (value >= 10)
    n = 1;
  else
    n = 0;
  return 349 - start - shift * (n + 1);
}