/*
 * rainbows.h
 *
 * Created on Wed Aug 04 2021
 * 
 * Object with all kinds of rainbow effects. Static, running, oscillating rainbows.
 *
 * Copyright (c) 2021 occamzchainsaw
 */

//  External libraries
#include <Arduino.h>                                                //  Arduino Framework
#define FASTLED_INTERNAL
#include <FastLED.h>                                                //  Great lib for the LEDs

#include <helpers.h>

class RainbowEffect
{
    protected:
        uint8_t Delta;

        const int MIN_CYCLE_TIME = 1;
        const byte RUNNING_SPEED = 2;

        //  myFillRainbow
        //
        //  Figures out the hue delta for FastLED to fill the strip with the given number of rainbows.
        void myFillRainbow(uint8_t initialHue)
        {
            CHSV hsv;
            hsv.hue = initialHue;
            hsv.val = 255;
            hsv.sat = 240;

            int hueTotal = NumRainbows * 256;
            for (int i = 0; i < FastLED.size(); i++)
            {
                hsv.hue = (hueTotal * i) / FastLED.size() + Delta;
                FastLED.leds()[i] = hsv;
            }
        }


    public:
        uint8_t NumRainbows = 1;
        bool RunUpwards = true;
        int OscillationCycleTime = 6;

        //  Empty Constructor
        //
        //  Uses the default values. Should look nice.
        RainbowEffect() 
        {
            Delta = (256 * NumRainbows) / FastLED.size();
        }

        //  Below, are the member functions invoking one of three effects.
        //  1. Static Rainbow
        void drawStatic(uint8_t setBrightness)
        {
            uint8_t scaledBrightness = calculate_max_brightness_for_power_mW(FastLED.leds(), FastLED.size(), setBrightness, MAX_POWER);

            myFillRainbow(0);
            //fill_rainbow(FastLED.leds(), FastLED.size(), 0, Delta);

            FastLED.setBrightness(scaledBrightness);
        }

        //  2. Running Rainbow
        void drawRunning(uint8_t setBrightness)
        {
            uint8_t scaledBrightness = calculate_max_brightness_for_power_mW(FastLED.leds(), FastLED.size(), setBrightness, MAX_POWER);

            if (RunUpwards)
            {
                static byte hue = 255;
                hue = (hue <= 0) ? 255 : (hue - RUNNING_SPEED);
                fill_rainbow(FastLED.leds(), FastLED.size(), hue, Delta);
            }
            else
            {
                static byte hue = 0;
                hue = (hue >= 255) ? 0 : (hue + RUNNING_SPEED);
                fill_rainbow(FastLED.leds(), FastLED.size(), hue, Delta);
            }

            FastLED.setBrightness(scaledBrightness);
        }

        //  3. Oscillating Rainbow
        void drawOscillating(uint8_t setBrightness)
        {
            uint8_t scaledBrightness = calculate_max_brightness_for_power_mW(FastLED.leds(), FastLED.size(), setBrightness, MAX_POWER);

            int bpm = 60 / max(OscillationCycleTime, MIN_CYCLE_TIME);
            uint8_t hue = beatsin8(bpm);
            fill_rainbow(FastLED.leds(), FastLED.size(), hue, Delta);

            FastLED.setBrightness(scaledBrightness);
        }
};