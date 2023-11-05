#include "CPButton.h"
#include <Arduino.h>

//BUTTON 引脚定义
#define CP_GPIO_BUTTON0   32
#define CP_GPIO_BUTTON1   33
#define CP_GPIO_BUTTON2   12
#define CP_GPIO_BUTTON3   13

CPButton::CPButton() {

}
void CPButton::init() {
  pinMode(CP_GPIO_BUTTON0, INPUT_PULLUP);
  pinMode(CP_GPIO_BUTTON1, INPUT_PULLUP);
  pinMode(CP_GPIO_BUTTON2, INPUT_PULLUP);
  pinMode(CP_GPIO_BUTTON3, INPUT_PULLUP);
}

int CPButton::readButton() {
  int button = 0;
  int b0 = 1 - digitalRead(CP_GPIO_BUTTON0);
  int b1 = 1 - digitalRead(CP_GPIO_BUTTON1);
  int b2 = 1 - digitalRead(CP_GPIO_BUTTON2);
  int b3 = 1 - digitalRead(CP_GPIO_BUTTON3);
  button = b0 + (b1 << 1) + (b2 << 2) + (b3 << 3);
  //Serial.printf("digital read buttons: %d-%d-%d-%d\n", b0, b1, b2, b3);
  if (button != 0) {
    Serial.printf("digital read buttons: %d\n", button);  
  }
  return button;
}