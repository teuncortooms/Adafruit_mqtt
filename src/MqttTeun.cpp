#include "MqttTeun.h"

namespace MqttTeun
{
    WiFiClient client;
    Adafruit_MQTT_Client mqtt(&client, AIO_SERVER, AIO_SERVERPORT, AIO_USERNAME, AIO_KEY);
    Adafruit_MQTT_Subscription LED_Control_sub = Adafruit_MQTT_Subscription(&mqtt, AIO_USERNAME "/feeds/LED_Control");
    Adafruit_MQTT_Publish LED_Control_pub = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/LED_Control");

    void setup()
    {
        Serial.begin(115200);
        connectToWifi();
        mqtt.subscribe(&LED_Control_sub);
    }

    void connectToWifi()
    {
        Serial.printf("Connecting to %s... ", WLAN_SSID);
        WiFi.begin(WLAN_SSID, WLAN_PASS);
        while (WiFi.status() != WL_CONNECTED)
        {
            delay(50);
            Serial.print(".");
        }
        Serial.println("WiFi connected");
        Serial.println(WiFi.localIP());
    }

    void connectToMqtt()
    {
        Serial.print("Connecting to MQTT... ");
        int returnCode;
        while ((returnCode = mqtt.connect()) != 0)
        {
            Serial.println(mqtt.connectErrorString(returnCode));
            mqtt.disconnect();
            Serial.print("Retrying...");
            delay(5000);
        }
        Serial.println("MQTT Connected!");
    }

    String getNextSubscriptionMessage()
    {
        if (!mqtt.connected())
        {
            connectToMqtt();
        }
        Adafruit_MQTT_Subscription *subscription;
        if ((subscription = mqtt.readSubscription(20)))
        {
            if (subscription == &LED_Control_sub)
            {
                String message = String((char *)LED_Control_sub.lastread);
                return processNextLedControlMessage(message);
            }
        }
        return "";
    }

    String processNextLedControlMessage(String message)
    {
        Serial.println("Got: " + message);
        if (message == "ON")
            return "LED_Control_ON";
        else if (message == "OFF")
            return "LED_Control_OFF";
        return "";
    }

    void publishLedSwitch(bool ledState)
    {
        if (!mqtt.connected())
        {
            connectToMqtt();
        }
        String message;
        if (ledState == LOW)
            message = "ON";
        else if (ledState == HIGH)
            message = "OFF";
        if (!LED_Control_pub.publish(message.c_str()))
        {
            Serial.println("Failed");
        }
        else
        {
            Serial.println("OK!");
        }
    }
} // namespace MqttTeun