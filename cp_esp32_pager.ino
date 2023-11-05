
//#include <SD.h>

#include "CPOledDisplay.h"
#include "CPIoTMqtt.h"

#include "mqtt_const.h"

#include <SD.h>
#define SDCARA_CS           0

CPOledDisplay display;
CPIoTMqtt mqtt;

void mqtt_callback_display(String text) {
  display.setStatus(text);
}

void mqtt_callback_pager_message(String sender, String receiver, String message, String textPixelBase64, int textCount) {
  Serial.println("mqtt_callback_pager_message");
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
void setup(){
  delay(2000);
  
  Serial.begin(115200);
  Serial.setDebugOutput(true);
  Serial.println("init display...");

  display.init();

  //sdcard_init();
  delay(1000);

  mqtt.addDisplayCallback(&mqtt_callback_display);
  mqtt.addPagerCallback(&mqtt_callback_pager_message);
  mqtt.connect(MQTT_BROKER, MQTT_PORT);
  
}

void loop(){
  // clear the display
  mqtt.loop();
}
