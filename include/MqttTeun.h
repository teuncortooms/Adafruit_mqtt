#ifndef MqttTeun_h
#define MqttTeun_h

#include "Arduino.h"
#include <WiFi.h>
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"

#define WLAN_SSID "Teun"
#define WLAN_PASS "jezes666"
#define AIO_SERVER "io.adafruit.com"
#define AIO_SERVERPORT 1883
#define AIO_USERNAME "teuncortooms"
#define AIO_KEY "aio_wymm81aQSjXrOvHuJavWPSsrO5oQ"

namespace MqttTeun
{
    void setup();

    void connectToWifi();
    void connectToMqtt();
    String getNextSubscriptionMessage();
    String processNextLedControlMessage(String message);
    void publishLedSwitch(bool ledState);
} // namespace MqttTeun

#endif