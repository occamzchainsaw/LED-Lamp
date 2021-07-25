/*
 * solid_colour.h
 *
 * Created on Tue Jul 20 2021
 * 
 * An object for a solid colour lighting effect with possible breathing
 *
 * Copyright (c) 2021 occamzchainsaw
 */

//  External libraries
#include <Arduino.h>                                                //  Arduino Framework
#define FASTLED_INTERNAL
#include <FastLED.h>                                                //  Great lib for the LEDs

class SolidColourEffect
{
    protected:
        CRGB Colour;                            //  obvious.
        bool IsBreathing;                       //  turns on thr breathing effect
        uint32_t LastDraw;                      //  for handling the breathing smoothly
        uint8_t BrightnessDelta;                //  same here

        const uint8_t MinBrightness = 15;       //  if we go lower, it looks way too dim

    public:
        //  SolidColour
        //
        //  Costructor - chosen colour, and a bool to set the breathing on or off
        SolidColourEffect(CRGB colour = CRGB(255, 0, 188), bool isBreathing = false)
            :   Colour(colour),
                IsBreathing(isBreathing)
        {
            LastDraw = millis();
            BrightnessDelta = 0;
        }

        ~SolidColourEffect() {    }

        void draw(uint8_t setBrightness)
        {
            uint8_t brightness = setBrightness;                             //  If we don't breathe, just use the set point
            if (IsBreathing)
            {
                if (millis() - LastDraw > 10)
                {
                    LastDraw = millis();
                    uint8_t maxBrightness = calculate_max_brightness_for_power_mW(FastLED.leds(), FastLED.size(), 255, MAX_POWER);
                    brightness = scale8(triwave8(BrightnessDelta), maxBrightness-MinBrightness) + MinBrightness;              //  sawtooth wave scaled to fit inside the power limits
                    BrightnessDelta++;                                                                          //  this magically overflows and returns to 0
                }
            }
            fill_solid(FastLED.leds(), FastLED.size(), Colour);
            FastLED.setBrightness(brightness);
        }
};