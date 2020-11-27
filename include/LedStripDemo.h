#ifndef LedStripDemo_h
#define LedStripDemo_h

#include <Arduino.h>
#include <FastLED.h>

#define DATA_PIN 13
#define LED_TYPE WS2812B
#define COLOR_ORDER GRB
#define NUM_LEDS 60
#define BRIGHTNESS 96
#define FRAMES_PER_SECOND 120
#define ARRAY_SIZE(A) (sizeof(A) / sizeof((A)[0]))

namespace LedStripDemo
{
    bool getLedState();
    void setLedState(bool highOrLow);
    void setup();
    void loop();

    void nextPattern();
    void Fire2012();
    void fiftyFifty(CRGB::HTMLColorCode color1, CRGB::HTMLColorCode color2);
    void police();
    void rainbow();
    void rainbowWithGlitter();
    void addGlitter(fract8 chanceOfGlitter);
    void confetti();
    void sinelon();
    void bpm();
    void juggle();

} // namespace LedStripDemo

#endif