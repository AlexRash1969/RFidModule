#include <Arduino.h>
#include <arInterface.h> // my Interface Library (for TFT)

AR_Interface interface;

void setup()
{
  Serial.begin(115200);
  Serial.println("_____________SETUP________________");
  interface.tftInit(0);     //ініціалізація TFT екрану
  interface.drawStatic(2);    //стартова сторінка
}

void loop()
{
  // put your main code here, to run repeatedly:
}