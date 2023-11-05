#include "CPOledDisplay.h"


CPOledDisplay::CPOledDisplay(): display(0x3c, SDA, SCL, GEOMETRY_128_32) {

}
void CPOledDisplay::init() {
  display.init();//初始化UI
  display.flipScreenVertically();//垂直翻转屏幕设置
  display.setFont(ArialMT_Plain_16);//设置字体大小
  display.drawString(0, 0, "Hello World");//显示
  display.display();//将缓存数据写入到显示器
}
void CPOledDisplay::setStatus(String text) {
  display.clear();
  display.drawString(0, 0, text);//显示
  display.display();//将缓存数据写入到显示器

}

void CPOledDisplay::updatePagerMessage(String sender, String receiver, String message, String textPixelBase64, int textCount) {

}

void CPOledDisplay::drawTextPixel(int x, int y, int width, int height, const uint16_t* textPixels) {

}

void CPOledDisplay::drawTest(const unsigned char* data, int dataLen, int textCount) {

}