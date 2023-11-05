#ifndef _CP_OLED_DISPLAY_H_
#define _CP_OLED_DISPLAY_H_

#include <Wire.h>               // Only needed for Arduino 1.6.5 and earlier
#include "SSD1306Wire.h"        // legacy: #include "SSD1306.h"

//OLED引脚定义
#define SDA   21
#define SCL   22


class CPOledDisplay {
public:
  CPOledDisplay();
  void init();
  void setStatus(String text);
  void updatePagerMessage(String sender, String receiver, String message, String textPixelBase64, int textCount);
  void drawTextPixel(int x, int y, int width, int height, const uint16_t* textPixels);
  void drawTest(const unsigned char* data, int dataLen, int textCount);
private:
  SSD1306Wire display;  
};
#endif