/*
 * colour_rotation
 *
 * Created on Tue Jul 20 2021
 *
 * An object for running through chosen colours.
 * 
 * Copyright (c) 2021 occamzchainsaw
 */

//  External libraries
#include <Arduino.h>                                                //  Arduino Framework
#define FASTLED_INTERNAL
#include <FastLED.h>                                                //  Great lib for the LEDs

#include <helpers.h>

#define STEP_SIZE 16
#define FADE_AMNT 2

void ColorRotationEffect(CRGBPalette16 colorPalette, uint32_t staticTime, uint8_t startIndex = 0U)
{
    static uint32_t lastFade = millis();
    static uint8_t index = startIndex;
    static uint8_t value = 0;
    static CRGB currentColor = ColorFromPalette(colorPalette, index);
    static CRGB nextColor = ColorFromPalette(colorPalette, index+STEP_SIZE);

    if (millis() - lastFade > staticTime)
    {
        currentColor = fadeTowardColour(currentColor, nextColor, FADE_AMNT);
        value += FADE_AMNT;

        if (value == 0)                                                                 //  only works with 256 % FADE_AMNT == 0
        {
            lastFade = millis();
            index += STEP_SIZE;                                                         //  only works with 256 % STEP_SIZE == 0
            nextColor = ColorFromPalette(colorPalette, index);
        }
    }

    fill_solid(FastLED.leds(), NUM_LEDS, currentColor);
}