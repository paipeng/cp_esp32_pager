
//#include <SD.h>

#include "CPOledDisplay.h"
#include "CPIoTMqtt.h"
#include "mqtt_const.h"
#include "CPButton.h"

#include <SD.h>
#define SDCARA_CS           0

#define CP_GPIO_BEEP 25


CPOledDisplay display;
CPIoTMqtt mqtt;
CPButton button;

void mqtt_callback_display(String text) {
  display.setStatus(text);
}

void mqtt_callback_pager_message(String sender, String receiver, String message, String textPixelBase64, int textCount) {
  Serial.println("mqtt_callback_pager_message");
  beep();
  display.updatePagerMessage(sender, receiver, message, textPixelBase64, textCount);
}

void sdcard_init() {
  if (!SD.begin(SDCARA_CS)) {
    Serial.println("SD Init Fail");
  } else {
    Serial.printf("SD Init Pass Type:%d Size:%lu\n", SD.cardType(), SD.cardSize() / 1024 / 1024);
    Serial.printf("totalBytes:%lu usedBytes:%lu\n", SD.totalBytes(), SD.usedBytes());
    delay(2000);
  }
}

void beep() {
  digitalWrite(CP_GPIO_BEEP, HIGH); // turn on
  delay(60);//延时200ms
  digitalWrite(CP_GPIO_BEEP, LOW); // turn on
  delay(60);//延时200ms
}

void setup(){
  delay(2000);
  
  Serial.begin(115200);
  Serial.setDebugOutput(true);

  pinMode(CP_GPIO_BEEP, OUTPUT);       // set ESP32 pin to output mode
  beep();

  button.init();
  
  Serial.println("init display...");
  display.init();

  //sdcard_init();
  delay(1000);
  display.setStatus("你好 CP IoT==========");
  delay(2000);

  mqtt.addDisplayCallback(&mqtt_callback_display);
  mqtt.addPagerCallback(&mqtt_callback_pager_message);
  mqtt.connect(MQTT_BROKER, MQTT_PORT);
  beep();
  beep();
}

void loop(){
  // clear the display
  mqtt.loop();
  int b = button.readButton();
  if (b != 0) {
    beep();
  }
}
