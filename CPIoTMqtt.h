#ifndef CPIOT_MQTT_H
#define CPIOT_MQTT_H
#include <Arduino.h>
#pragma once

// WIFI
#include <WiFi.h>
#include <WiFiMulti.h>
// MQTT
#include <PubSubClient.h>
#define MQTT_MAX_PACKET_SIZE 10240
// JSON
#include <ArduinoJson.h>


#include "wifi_const.h"
#include "mqtt_const.h"


class CPIoTMqtt {
    using m_cb = void (*)(char*); //alias function pointer
    using m_cb_s = void (*)(String);
    using m_cb_p = void (*)(String, String, String, String, int);
  public:
    CPIoTMqtt();
  public:
    m_cb action;
    m_cb_s displayCallback;
    m_cb_p pagerCallback;    
    
  public:
    void connect(char* broker, int port);
    void addDisplayCallback(m_cb_s act);
    void addPagerCallback(m_cb_p act);
    void add_callback(m_cb act);
    void process();
    void wifi_connect(void);
    char* getDeviceUDID();

    void mqtt_pong();
    void mqtt_subscribe();
    static void mqtt_callback(char *topic, byte *payload, unsigned int length);
    void mqtt_connect(char* broker, int port);
    void mqtt_publish_received(float temperature);
    void loop();

  private:
    WiFiMulti wifiMulti;

    // Use WiFiClient class to create TCP connections
    WiFiClient client;
    PubSubClient mqttClient;
    char DEVICE_UDID[18];
};
#endif // CPIOT_MQTT_H
