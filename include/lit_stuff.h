/*
 * lit_stuff.h
 *
 * Created on Sat Jul 24 2021
 * 
 * Fire Effect. Hopefully.
 *
 * Copyright (c) 2021 occamzchainsaw
 */

//  External libraries
#include <Arduino.h>                                                //  Arduino Framework
#define FASTLED_INTERNAL
#include <FastLED.h>                                                //  Great lib for the LEDs
#include <helpers.h>

#define FIRE_COOLING            2250
#define FIRE_SPARKING           230
#define FIRE_SPARKS             4
#define FIRE_SPARKING_REGION    15
#define HEAT_SIZE               NUM_LEDS+20                         //  adding 20 elements, so that the sparking bottom of the flame is not actually drawn to the strip

const byte w0 = 1;
const byte w1 = 2;
const byte w2 = 2;
const byte w3 = 3;
const byte w4 = 3;
const byte w5 = 5;
const byte w6 = 5;
const byte w7 = 3;
const byte wTotal = w0 + w1 + w2 + w3 + w4 + w5 + w6 + w7;

void FireEffect(CRGBPalette16 colorPalette)
{
    //  array of temperatures for each pixel on the strip
    static uint16_t heat[HEAT_SIZE];

    //  cool every pixel by a random amount no larger than the cooling rate
    for (uint8_t i = 0; i < HEAT_SIZE; i++)
    {
        heat[i] = max(0, heat[i] - random16(0, FIRE_COOLING));
    }

    //  drift up, and blend with neighbours to diffuse the fire
    //  this is a mess, and won't scale at all, but I'm done giving fucks at this point.
    for (uint8_t i = HEAT_SIZE; i >= 7; i--)
    {
        heat[i] = ( heat[i]   * w0 + 
                    heat[i-1] * w1 + 
                    heat[i-2] * w2 + 
                    heat[i-3] * w3 + 
                    heat[i-4] * w4 + 
                    heat[i-5] * w5 +
                    heat[i-6] * w6 +
                    heat[i-7] * w7  ) / wTotal;
    }

     //  randomly spark in the sparking region
    if (random8(255) < FIRE_SPARKING)                           //  if true, we rolled a number below the selected rate, i.e. we got lucky
    {
        for (uint8_t i = 0; i < FIRE_SPARKS; i++)
        {
            uint8_t x = random8(FIRE_SPARKING_REGION);          //  we gonna spark, so select a random pixel in the sparking region
            heat[x] += random16(25000, 32767);                  //  heat the selected pixel a bunch
        }
    }

    //  quick clear of the strip before drawing
    FastLED.clear();

    //  assign a color value to each pixel based on its heat value
    for (uint8_t i = 0; i < NUM_LEDS; i++)
    {
        uint8_t scaledHeat = scale16(heat[i + 20], 240);
        CRGB colour = ColorFromPalette(colorPalette, scaledHeat, 255, LINEARBLEND);
        FastLED.leds()[i] = colour;
    }
}