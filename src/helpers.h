/*
 * helpers.h
 *
 * Created on Sun Jan 10 2021
 * 
 * This header file contains some helper functions for stuff like precision drawing, etc.
 *
 * Copyright (c) 2021 occamzchainsaw
 */
#include <Arduino.h>                                                //  Arduino Framework
#define FASTLED_INTERNAL
#include <FastLED.h>                                                //  Great lib for the LEDs

extern CRGB         g_LEDs[];
extern uint8_t      g_brightness;
#define NUM_LEDS    144                                             //  Number of LEDs on the whole strip
#define MAX_POWER   17500                                           //  Power limit for FastLED


float uscaled_power, actual_power;
uint8_t actual_brightness;
//  Caclulate the actual power consumption in Watts
void calculate_actual_power()
{
    actual_brightness = calculate_max_brightness_for_power_mW(g_LEDs, NUM_LEDS, g_brightness, MAX_POWER);
    uscaled_power = (float)calculate_unscaled_power_mW(g_LEDs, NUM_LEDS) / 1000.f;
    actual_power = (float)actual_brightness / 255.0f * (float)uscaled_power;
}

//  Fractional pixel draw
//
//  Dims each fractional pixel by the proper amount, to create the illusion of very smoooth motion
void DrawPixels(double fPos, double count, CRGB color)
{
  double availFirstPixel = 1.0 - (fPos - (long)(fPos));               //  (long)(fPos) trucates the float to a long, which has the effect of leaving you
                                                                      //  with just the decimal part (after the .) - pretty neat little trick
  double amtFirstPixel = min(availFirstPixel, count);
  count = min(count, FastLED.size() - fPos);
  
  if (fPos >= 0 && fPos < FastLED.size())
  {
    CRGB frontColor = color;
    frontColor.fadeToBlackBy(255 * (1.0 - amtFirstPixel));
    FastLED.leds()[(uint)fPos] += frontColor;
  }

  fPos += amtFirstPixel;
  count -= amtFirstPixel;

  while (count >= 1.0)
  {
    if (fPos >= 0 && fPos < FastLED.size())
    {
      FastLED.leds()[(uint)fPos] += color;
      count -= 1.0;
    }
    fPos += 1.0;
  }

  if (count > 0.0)
  {
    if (fPos >= 0 && fPos < FastLED.size())
    {
      CRGB backColor = color;
      backColor.fadeToBlackBy(255 * (1.0 - count));
      FastLED.leds()[(uint)fPos] += backColor;
    }
  }
}