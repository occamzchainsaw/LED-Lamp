/*
 * rainbows.h
 *
 * Created on Wed Aug 04 2021
 * 
 * Static, running, oscillating rainbows.
 *
 * Copyright (c) 2021 occamzchainsaw
 */

//  External libraries
#include <Arduino.h>                                                //  Arduino Framework
#define FASTLED_INTERNAL
#include <FastLED.h>                                                //  Great lib for the LEDs

#include <helpers.h>

#define RUN_SPEED 2
#define CYCLE_TIME 6

void StaticRainbow()
{
    for (int i = 0; i < NUM_LEDS; i++)
    {
        float percentile = (float)i / (float)NUM_LEDS;
        uint8_t hue = (uint8_t)(percentile * 256);
        CHSV color;
        color.h = hue;
        color.s = 240;
        color.v = 255;
        FastLED.leds()[i] = color;
    }
}

void RunningRainbow(bool inverted)
{
    static uint8_t hue = inverted ? 0 : 255;

    if (inverted) {   hue = (hue >= 255) ? 0 : (hue + RUN_SPEED); }
    else {  hue = (hue <= 0) ? 255 : (hue - RUN_SPEED); }

    fill_rainbow(FastLED.leds(), NUM_LEDS, hue, 1);
}

void OscillatingRainbow()
{
    int bpm = 60 / CYCLE_TIME;
    uint8_t hue = beatsin8(bpm);
    fill_rainbow(FastLED.leds(), NUM_LEDS, hue, 1);
}