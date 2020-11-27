#include "Arduino.h"
#include "LedStripDemo.h"
#include "MqttTeun.h"

#define BUTTONPIN 4

bool volatile buttonPressed = false;

void IRAM_ATTR buttonISR();

void setup()
{
  LedStripDemo::setup();
  MqttTeun::setup();
  pinMode(BUTTONPIN, INPUT_PULLUP);
  attachInterrupt(BUTTONPIN, buttonISR, FALLING);
}

void loop()
{
  String message = MqttTeun::getNextSubscriptionMessage();
  // *** debug
  if (message != "")
    Serial.println(message);
  // *** end debug
  if (message == "LED_Control_ON")
    LedStripDemo::setLedState(HIGH);
  else if (message == "LED_Control_OFF")
    LedStripDemo::setLedState(LOW);

  LedStripDemo::Fire2012();

  if (buttonPressed)
  {
    MqttTeun::publishLedSwitch(LedStripDemo::getLedState());
    buttonPressed = false;
  }
}

void IRAM_ATTR buttonISR()
{
  buttonPressed = true;
}