/*
 * light_control.h
 *
 * Created on Sun Jan 10 2021
 * 
 * This header file contains functions for controlling the lighting
 * Solid color, White with warmth control, Rainbow, Gradient, etc. + Brightness control
 *
 * Copyright (c) 2021 occamzchainsaw
 */
#include <Arduino.h>                                                //  Arduino Framework
#define FASTLED_INTERNAL
#include <FastLED.h>                                                //  Great lib for the LEDs

extern CRGB g_LEDs[];
extern uint8_t g_brightness;
#define MAX_LEDS FastLED.size()
#define MIN_BRIGHTNESS 12
#define MIN_CYCLE_T 1

#pragma region: //  Declarations
//  Fill Solid
//
//  Converts chosen R, G, and B values to a color and fills the strip.
//  Utilises FastLED's fill_solid, because it's easy.
void solid_color(byte red, byte green, byte blue);

//  White with temperature setting
//
//  Fills the strip with white, and sets it's temperature, according to the
//  ColorTemperature enum from FastLED.
void solid_white(ColorTemperature temp);

//  Fill Solid Rainbow
//
//  Fills the strip with a static rainbow. Choose a start hue, and the step size.
//  The lower the step size the more spread out the rainbow will be.
void solid_rainbow(byte initialHue, byte deltaHue);

//  Oscillating Rainbow
//
//  Oscillates the hue, to give the effect of the rainbow oscillating on the strip.
//  Oscillation frequency is based on the cycle time (so keep it under 60s)
//  Delta defines how spread out the rainbow should be.
void oscillating_rainbow(int cycle_time, byte deltaHue);

//  Running Rainbow
//
//  Runs the rainbow through all 255 hues, and repeats.
//  Gives the effect of a rainbow running constantly trhough the strip.
//  Delta defines how spread out the rainbow should be. Speed is increment of the starting hue.
//  Down chooses whether to run it downwards, otherwise, run upwards.
void running_rainbow(byte speed, byte deltaHue, bool down);

//  Fill Gradient
//
//  Choose two RGB colors, and fill the strip with a gradient between them.
void solid_gradient(byte red1, byte green1, byte blue1, byte red2, byte green2, byte blue2);

//  Breathing effect (or static brightness)
//
//  Takes the cycle time, converts to bpm and oscillates based on a sine wave.
//  If breathing effect is off, just set the brightness to the desired level.
void brightness_control(byte brightness, bool breathe, int cycle_time);
#pragma endregion

void solid_color(byte red, byte green, byte blue)
{
    CRGB color(red, green, blue);
    fill_solid(g_LEDs, MAX_LEDS, color);
}

void solid_white(ColorTemperature temp)
{
    fill_solid(g_LEDs, MAX_LEDS, CRGB::White);
    FastLED.setTemperature(temp);
}

void solid_rainbow(byte initialHue, byte deltaHue)
{
    fill_rainbow(g_LEDs, MAX_LEDS, initialHue, deltaHue);
}

void oscillating_rainbow(int cycle_time, byte deltaHue)
{
    int bpm = 60 / max(cycle_time, MIN_CYCLE_T);                                        //  As much as I'd like to create a black hole, I can't.
    uint8_t hue = beatsin8(bpm, 0, 255);
    fill_rainbow(g_LEDs, MAX_LEDS, hue, deltaHue);
}

void running_rainbow(byte speed, byte deltaHue, bool down)
{
    if (down)
    {
        static byte hue = 0;

        if (hue >= 255)
            hue =0;
        else
            hue += speed;

        fill_rainbow(g_LEDs, MAX_LEDS, hue, deltaHue);
    }
    else
    {
        static byte hue = 255;
        if (hue <= 0)
            hue = 255;
        else
            hue -= speed;

        fill_rainbow(g_LEDs, MAX_LEDS, hue, deltaHue);
    }
}

void solid_gradient(byte red1, byte green1, byte blue1, byte red2, byte green2, byte blue2)
{
    CRGB c1(red1, green1, blue1);
    CRGB c2(red2, green2, blue2);

    fill_gradient_RGB(g_LEDs, MAX_LEDS, c1, c2);
}

void brightness_control(byte brightness, bool breathe, int cycle_time)
{
    if (breathe)
    {
        int bpm = 60 / max(cycle_time, MIN_CYCLE_T);                                //  As much as I'd like to create a black hole, I can't.
        uint8_t brite = beatsin8(bpm, MIN_BRIGHTNESS, brightness);
        g_brightness = brite;
    }
    else
    {
        g_brightness = brightness;
    }
    FastLED.setBrightness(g_brightness);
}