#include "CPIoTMqtt.h"

void* staticMqtt;

CPIoTMqtt::CPIoTMqtt():mqttClient(client),action(NULL) {
  staticMqtt = this;
}

void CPIoTMqtt::connect(char* broker, int port) {
  
  displayCallback("Wifi connecting...");
  wifi_connect();

  
  displayCallback("MQTT connecting...");
  mqtt_connect(broker, port);

  // Publish and subscribe
  mqtt_pong();
  
  displayCallback("MQTT subscribe...");
  mqtt_subscribe();

  delay(1000);  
  displayCallback("Pager ready...");
}

void CPIoTMqtt::add_callback(m_cb act) {
  action = act;
}

void CPIoTMqtt::addDisplayCallback(m_cb_s act) {
  displayCallback = act;
}

void CPIoTMqtt::addPagerCallback(m_cb_p act) {
  pagerCallback = act;
}
void CPIoTMqtt::process() {
  //do stuffs
  char message[] = "Hello World";
  if (action != NULL) {
    action(message); //invoke the callback
  }
}

char* CPIoTMqtt::getDeviceUDID() {
  return DEVICE_UDID;
}

//WIFI连接路由器
void CPIoTMqtt::wifi_connect(void)
{
  Serial.print("Connecting to ");
  delay(10);
  
  // We start by connecting to a WiFi network
  wifiMulti.addAP(WIFI_SSID, WIFI_PASSWD);

  Serial.println();
  Serial.println();
  Serial.print("\nDefault ESP32 MAC Address: ");
  Serial.println(WiFi.macAddress());
  
  Serial.print("\nDevice UDID: ");
  strncpy(DEVICE_UDID, WiFi.macAddress().c_str(), 17);
  Serial.println(DEVICE_UDID);
  
  Serial.print("\n\nWaiting for WiFi... ");

  while(wifiMulti.run() != WL_CONNECTED) {
      Serial.print(".");
      delay(500);
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  Serial.println("Connected to the Wi-Fi network");
  delay(500);
}

void CPIoTMqtt::mqtt_pong() {
  String mqttPong = String(MQTT_TOPIC_PREFIX) + String(MQTT_TOPIC_PONG);
  Serial.print("publish topic: " + mqttPong + "...\n");
  StaticJsonDocument<200> doc;
  doc["udid"] = String(DEVICE_UDID);
  doc["state"] = 1;
  String json;
  serializeJson(doc, json);
  
  Serial.println("pong json: ");
  Serial.print(json);
  Serial.println("");
  boolean ret = mqttClient.publish(mqttPong.c_str(), json.c_str());
  if (ret) {
    Serial.print("publish success!\n");
  } else {
    Serial.print("publish error!\n");    
  }
}

void CPIoTMqtt::mqtt_subscribe() {
  // udid ping
  String mqttPing = String(MQTT_TOPIC_PREFIX) + String(DEVICE_UDID) + "/" + String(MQTT_TOPIC_PING);  
  Serial.print("\nsubscribe udid ping topic: " + mqttPing + "...\n");
  boolean ret = mqttClient.subscribe(mqttPing.c_str());
  if (ret) {
    Serial.print("subscribe udid ping success!\n");
  } else {
    Serial.print("subscribe udid ping error!\n");    
  }
  // broadcasting ping
  mqttPing = String(MQTT_TOPIC_PREFIX) + String(MQTT_TOPIC_PING);  
  Serial.print("\nsubscribe broadcasting ping topic: " + mqttPing + "...\n");
  ret = mqttClient.subscribe(mqttPing.c_str());
  if (ret) {
    Serial.print("subscribe broadcasting ping success!\n");
  } else {
    Serial.print("subscribe broadcasting ping error!\n");    
  }
  // LED
  mqttPing = String(MQTT_TOPIC_PREFIX) + String(DEVICE_UDID) + "/" + String(MQTT_TOPIC_LED);  
  Serial.print("\nsubscribe led topic: " + mqttPing + "...\n");
  ret = mqttClient.subscribe(mqttPing.c_str());
  if (ret) {
    Serial.print("subscribe led success!\n");
  } else {
    Serial.print("subscribe led error!\n");    
  }

  // MESSAGE_BOARD
  mqttPing = String(MQTT_TOPIC_PREFIX) + String(DEVICE_UDID) + "/" + String(MQTT_TOPIC_MESSAGE_BOARD);  
  Serial.print("\nsubscribe message board topic: " + mqttPing + "...\n");
  ret = mqttClient.subscribe(mqttPing.c_str());
  if (ret) {
    Serial.print("subscribe message board success!\n");
  } else {
    Serial.print("subscribe message board error!\n");    
  }

  // MQTT_TOPIC_PAGER_MESSAGE
  mqttPing = String(MQTT_TOPIC_PREFIX) + String(DEVICE_UDID) + "/" + String(MQTT_TOPIC_PAGER_MESSAGE);  
  Serial.print("\nsubscribe pager message topic: " + mqttPing + "...\n");
  ret = mqttClient.subscribe(mqttPing.c_str());
  if (ret) {
    Serial.print("subscribe pager success!\n");
  } else {
    Serial.print("subscribe pager error!\n");    
  }
}


void CPIoTMqtt::mqtt_callback(char *topic, byte *payload, unsigned int length) {
    Serial.println("-----------------------mqtt arrived in ");
    Serial.println("Message len:");
    Serial.printf("%d\n", length);
    Serial.println("topic: " + String(topic));
    
    String data;// = String((const char*)payload);
    
    for (int i = 0; i < length; i++) {
        Serial.print((char) payload[i]);
        data += (char) payload[i];
    }
    
    Serial.println("data: " + data);

    if (strstr(topic, "PING")) {
      ((CPIoTMqtt*)staticMqtt)->mqtt_pong();
      
    } else if (strstr(topic, "LED")) {
      StaticJsonDocument<200> doc;
      DeserializationError error = deserializeJson(doc, data);
    
      // Test if parsing succeeds.
      if (error) {
        Serial.print(F("deserializeJson() failed: "));
        Serial.println(error.f_str());
      } else {
        long state = doc["state"];
        //gpio_led_toggle(state);
      }
    } else if (strstr(topic, "MESSAGE_BOARD")) {
      StaticJsonDocument<200> doc;
      DeserializationError error = deserializeJson(doc, data);
    
      // Test if parsing succeeds.
      if (error) {
        Serial.print(F("deserializeJson() failed: "));
        Serial.println(error.f_str());
      } else {
        String message = doc["message"];
        Serial.println(message);
        //oled_message_board(message);
      }
    } else if (strstr(topic, MQTT_TOPIC_PAGER_MESSAGE)) {
      StaticJsonDocument<2000> doc;
      //DynamicJsonDocument doc(ESP.getMaxAllocHeap());
      DeserializationError error = deserializeJson(doc, data);
    
      // Test if parsing succeeds.
      if (error) {
        Serial.print(F("deserializeJson() failed: "));
        Serial.println(error.f_str());
      } else {
        String message = doc["message"];
        String sender = doc["sender"];
        String receiver = doc["receiver"];
        String textPixelBase64 = doc["textPixelBase64"];
        int textCount = doc["textCount"];
        Serial.println(message);
        
        ((CPIoTMqtt*)staticMqtt)->pagerCallback(sender, receiver, message, textPixelBase64, textCount);
      }
    }
    Serial.println();
    Serial.println("-----------------------mqtt-end");
}


void CPIoTMqtt::mqtt_connect(char* broker, int port) {
  //connecting to a mqtt broker
  mqttClient.setServer(broker, port);
  mqttClient.setCallback(this->mqtt_callback);
  
  mqttClient.setBufferSize(51200);
  mqttClient.setKeepAlive(60*60);
  while (!mqttClient.connected()) {
      String client_id = "cp-esp32-client-";
      client_id += String(WiFi.macAddress());
      Serial.printf("The client %s connects to the public MQTT broker\n", client_id.c_str());
      if (mqttClient.connect(client_id.c_str(), MQTT_USERNAME, MQTT_PASSWORD)) {
          Serial.println("Public EMQX MQTT broker connected");
          //gpio_led_mqtt_toggle(1);
      } else {
          Serial.print("failed with state ");
          Serial.print(mqttClient.state());  
          Serial.println("\n");
          //gpio_led_mqtt_toggle(0);
          delay(2000);
      }
  }    
}

void CPIoTMqtt::mqtt_publish_received(float temperature) {
  String mqttTopic = String(MQTT_TOPIC_PREFIX) + String(MQTT_TOPIC_TEMPERATURE);
  Serial.print("publish topic: " + mqttTopic + "...\n");
  String data = "{\"udid\": \"" + String(DEVICE_UDID) + "\", \"state\": 1, \"value\": " + temperature + "}";
  boolean ret = mqttClient.publish(mqttTopic.c_str(), data.c_str());
  if (ret) {
    Serial.print("publish temperature success!\n");
  } else {
    Serial.print("publish temperature error!\n");    
  }
}

void CPIoTMqtt::loop() {
  if (mqttClient.connected()) {
    mqttClient.loop();
  } else {
    //Serial.println("mqtt disconnected");    
  }

}
