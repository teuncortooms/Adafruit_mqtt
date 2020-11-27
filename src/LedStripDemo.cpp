#include "LedStripDemo.h"
namespace LedStripDemo
{
    CRGB leds[NUM_LEDS];

    // List of patterns to cycle through.  Each is defined as a separate function.
    typedef void (*SimplePatternList[])();
    SimplePatternList gPatterns = {Fire2012, police, rainbow, rainbowWithGlitter, confetti, sinelon, juggle, bpm};
    uint8_t gCurrentPatternNumber = 0;
    uint8_t gHue = 0; // rotating "base color" used by many of the patterns

    // Variables for Fire2012
    const int COOLING = 55;
    const int SPARKING = 120;
    bool gReverseDirection = false;
    bool ledState = LOW;

    bool getLedState()
    {
        return ledState;
    }

    void setLedState(bool highOrLow)
    {
        ledState = highOrLow;
    }

    void setup()
    {
        delay(3000);
        FastLED.addLeds<LED_TYPE, DATA_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
        FastLED.setBrightness(BRIGHTNESS);
    }

    void loop()
    {
        if (ledState)
        {
            // Call the current pattern function once, updating the 'leds' array
            gPatterns[gCurrentPatternNumber]();

            FastLED.show();

            FastLED.delay(1000 / FRAMES_PER_SECOND);

            EVERY_N_MILLISECONDS(20) { gHue++; }
            EVERY_N_SECONDS(20) { nextPattern(); }
        }
        else
        {
            FastLED.clear(true);
        }
    }

    void nextPattern()
    {
        // add one to the current pattern number, and wrap around at the end
        gCurrentPatternNumber = (gCurrentPatternNumber + 1) % ARRAY_SIZE(gPatterns);
    }

    void Fire2012()
    {
        int customBrightness = 200;
        int extraDelay = 1000 / 60 - 1000 / FRAMES_PER_SECOND;
        FastLED.setBrightness(customBrightness);

        // Array of temperature readings at each simulation cell
        static byte heat[NUM_LEDS];

        // Step 1.  Cool down every cell a little
        for (int i = 0; i < NUM_LEDS; i++)
        {
            heat[i] = qsub8(heat[i], random8(0, ((COOLING * 10) / NUM_LEDS) + 2));
        }

        // Step 2.  Heat from each cell drifts 'up' and diffuses a little
        for (int k = NUM_LEDS - 1; k >= 2; k--)
        {
            heat[k] = (heat[k - 1] + heat[k - 2] + heat[k - 2]) / 3;
        }

        // Step 3.  Randomly ignite new 'sparks' of heat near the bottom
        if (random8() < SPARKING)
        {
            int y = random8(7);
            heat[y] = qadd8(heat[y], random8(160, 255));
        }

        // Step 4.  Map from heat cells to LED colors
        for (int j = 0; j < NUM_LEDS; j++)
        {
            CRGB color = HeatColor(heat[j]);
            int pixelnumber;
            if (gReverseDirection)
            {
                pixelnumber = (NUM_LEDS - 1) - j;
            }
            else
            {
                pixelnumber = j;
            }
            leds[pixelnumber] = color;
        }

        FastLED.delay(extraDelay);
        FastLED.setBrightness(BRIGHTNESS);
    }

    void police()
    {
        const int interval = 500;
        fiftyFifty(CRGB::Blue, CRGB::Red);
        FastLED.show();
        FastLED.delay(interval); // TODO:

        fiftyFifty(CRGB::Red, CRGB::Blue);
        FastLED.show();
        FastLED.delay(interval); // TODO:
    }

    void fiftyFifty(CRGB::HTMLColorCode color1, CRGB::HTMLColorCode color2)
    {
        for (int i = 0; i < NUM_LEDS; i++)
        {
            leds[i] = i < NUM_LEDS / 2 ? color1 : color2;
        }
    }

    void rainbow()
    {
        fill_rainbow(leds, NUM_LEDS, gHue, 7);
    }

    void rainbowWithGlitter()
    {
        rainbow();
        addGlitter(80);
    }

    void addGlitter(fract8 chanceOfGlitter)
    {
        if (random8() < chanceOfGlitter)
        {
            leds[random16(NUM_LEDS)] += CRGB::White;
        }
    }

    void confetti()
    {
        fadeToBlackBy(leds, NUM_LEDS, 10);
        int pos = random16(NUM_LEDS);
        leds[pos] += CHSV(gHue + random8(64), 200, 255);
    }

    void sinelon()
    {
        fadeToBlackBy(leds, NUM_LEDS, 20);
        int pos = beatsin16(13, 0, NUM_LEDS - 1);
        leds[pos] += CHSV(gHue, 255, 192);
    }

    void bpm()
    {
        uint8_t BeatsPerMinute = 62;
        CRGBPalette16 palette = PartyColors_p;
        uint8_t beat = beatsin8(BeatsPerMinute, 64, 255);
        for (int i = 0; i < NUM_LEDS; i++)
        { //9948
            leds[i] = ColorFromPalette(palette, gHue + (i * 2), beat - gHue + (i * 10));
        }
    }

    void juggle()
    {
        fadeToBlackBy(leds, NUM_LEDS, 20);
        byte dothue = 0;
        for (int i = 0; i < 8; i++)
        {
            leds[beatsin16(i + 7, 0, NUM_LEDS - 1)] |= CHSV(dothue, 200, 255);
            dothue += 32;
        }
    }
} // namespace LedStripDemo
