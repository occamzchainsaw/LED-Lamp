/*
 * ripple.h
 *
 * Created on Wed Aug 04 2021
 * 
 * Ripple effect (sort of). Lights up a random pixel with a colour chosen from a given palette, and fades it out nicely.
 *
 * Copyright (c) 2021 occamzchainsaw
 */

//  External libraries
#include <Arduino.h>                                                //  Arduino Framework
#define FASTLED_INTERNAL
#include <FastLED.h>                                                //  Great lib for the LEDs

#include <helpers.h>

class RippleEffect
{
    protected:
        std::vector<CRGB> ColourPalette;
        CRGB *Leds;
        int Size;
        uint32_t LastDraw;

    public:
        RippleEffect(std::vector<CRGB> colours, int size)
            :   ColourPalette(colours),
                Size(size)
        {
            Leds = new CRGB[Size] { CRGB(0,0,0) };
            LastDraw = millis();
        }

        void draw(uint8_t setBrightness)
        {
            uint8_t scaledBrightness = calculate_max_brightness_for_power_mW(FastLED.leds(), FastLED.size(), setBrightness, MAX_POWER);

            static uint8_t pixel = 0;

            //  Choose a random pixel, and colour
            //  then light that pixel up with that colour
            //  do this every few secs
            if (millis() - LastDraw > 1000)
            {
                pixel = random8(Size);
                int index = random8(ColourPalette.size());

                Leds[pixel] = ColourPalette[index];

                LastDraw = millis();
            }            

            FastLED.clear();

            fadeToBlackBy(Leds, Size, 2);

            for (int i = 0; i < Size; i++)
            {
                DrawPixels(i, 10.0f, Leds[i]);
            }

            FastLED.setBrightness(scaledBrightness);
        }
};