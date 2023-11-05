#include "CPOledDisplay.h"


CPOledDisplay::CPOledDisplay(): 
#if USE_U8G2
display(U8G2_R0, /*clock=*/SCL, /*data=*/SDA, /*reset=*/U8X8_PIN_NONE)
#else
display(0x3c, SDA, SCL, GEOMETRY_128_32) 
#endif
{

}
void CPOledDisplay::init() {
#if USE_U8G2
  display.begin();
  display.enableUTF8Print();    // enable UTF8 support for the Arduino print() function
  
  display.setFont(u8g2_font_wqy12_t_gb2312);  // use wqy gb2312

  //u8g2.setFont(u8g2_font_wqy12_t_chinese3);  // use wqy chinese2 for all the glyphs of "你好世界"
  display.setFontDirection(0);
#else
  display.init();//初始化UI
  display.flipScreenVertically();//垂直翻转屏幕设置
  display.setFont(ArialMT_Plain_16);//设置字体大小
  display.drawString(0, 0, "Hello World");//显示
  display.display();//将缓存数据写入到显示器
#endif
}
void CPOledDisplay::setStatus(String text) {
#if USE_U8G2
  display.clearBuffer();
  display.setCursor(0, 10);
  display.print(text);    // Chinese "Hello World" 
  display.sendBuffer();
#else
  display.clear();
  display.drawString(0, 0, text);//显示
  display.display();//将缓存数据写入到显示器

#endif
}

void CPOledDisplay::updatePagerMessage(String sender, String receiver, String message, String textPixelBase64, int textCount) {
#if USE_U8G2
  display.clearBuffer();
  display.setCursor(0, 10);
  display.print(message);    // Chinese "Hello World" 
  display.sendBuffer();
#else
  display.clear();
  display.drawString(0, 0, text);//显示
  display.display();//将缓存数据写入到显示器

#endif
}

void CPOledDisplay::drawTextPixel(int x, int y, int width, int height, const uint16_t* textPixels) {

}

void CPOledDisplay::drawTest(const unsigned char* data, int dataLen, int textCount) {

}