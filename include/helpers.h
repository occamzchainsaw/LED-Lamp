/*
 * helpers.h
 *
 * Created on Sun Jan 10 2021
 * 
 * This header file contains some helper functions for stuff like precision drawing, etc.
 *
 * Copyright (c) 2021 occamzchainsaw
 */

#pragma once

#include <Arduino.h>                                                //  Arduino Framework
#define FASTLED_INTERNAL
#include <FastLED.h>                                                //  Great lib for the LEDs
#include <sys/time.h>
#include <list>
#include <iterator>

extern CRGB g_LEDs[];
extern uint8_t g_brightness;

#define NUM_LEDS    174                                             //  Number of LEDs on the whole strip
#define MAX_POWER   18500                                           //  Power limit for FastLED

#define ARRSIZE(x) (sizeof(x) / sizeof(*x))

float uscaled_power, actual_power;
uint8_t actual_brightness;

//  Caclulate the actual power consumption in Watts
void calculate_actual_power()
{
    actual_brightness = calculate_max_brightness_for_power_mW(FastLED.leds(), NUM_LEDS, FastLED.getBrightness(), MAX_POWER);
    uscaled_power = (float)calculate_unscaled_power_mW(FastLED.leds(), NUM_LEDS) / 1000.f;
    actual_power = (float)actual_brightness / 255.0f * (float)uscaled_power;
}

// FractionalColor
//
// Returns a fraction of a color; abstracts the fadeToBlack out to this function in case we
// want to improve the color math or do color correction all in one location at a later date.

CRGB ColorFraction(CRGB colorIn, float fraction)
{
  fraction = min(1.0f, fraction);
  return CRGB(colorIn).fadeToBlackBy(255 * (1.0f - fraction));
}

//  Fractional pixel draw
//
//  Dims each fractional pixel by the proper amount, to create the illusion of very smoooth motion
void DrawPixels(float fPos, float count, CRGB color)
{
   // Calculate how much the first pixel will hold
  float availFirstPixel = 1.0f - (fPos - (long)(fPos));
  float amtFirstPixel = min(availFirstPixel, count);
  float remaining = min(count, FastLED.size()-fPos);
  int iPos = fPos;

  // Blend (add) in the color of the first partial pixel

  if (remaining > 0.0f)
  {
    FastLED.leds()[iPos++] += ColorFraction(color, amtFirstPixel);
    remaining -= amtFirstPixel;
  }

  // Now draw any full pixels in the middle

  while (remaining > 1.0f)
  {
    FastLED.leds()[iPos++] += color;
    remaining--;
  }

  // Draw tail pixel, up to a single full pixel

  if (remaining > 0.0f)
  {
    FastLED.leds()[iPos] += ColorFraction(color, remaining);
  }
}

//  RandomFloat
//
//  Gives a random number in the float type from 0.0f to 1.0f
inline float RandomFloat()
{
  float r = random(1000000L) / 1000000.0f;
  return r;
}


inline double UnixTime()
{
  timeval tv = {0};
  gettimeofday(&tv, nullptr);
  return (double)(tv.tv_sec / 1000000.0 + (double)tv.tv_sec);
}

//  fpsToMilis
//
//  Calculates the cycle length for a desired FPS count. For example, 20 FPS means a frame every 50 ms; 40 FPS -> 25ms; 30FPS -> ~33ms
//  For the purposes of using it with the lib8tion header, returns an integer value.
int fpsToMillis(int fps)
{
  return 1000 / fps;
}


//  blendU8TowardU8
//
//  Blend one uint8 toward another uint8, by a given amount
void blendU8TowardU8(uint8_t &cur, const uint8_t target, uint8_t amount)
{
  if (cur == target) return;

  if (cur < target)
  {
    uint8_t delta = target - cur;
    delta = scale8_video(delta, amount);
    cur += delta;
  } 
  else
  {
    uint8_t delta = cur - target;
    delta = scale8_video(delta, amount);
    cur -= delta;
  }
}

//  fadeTowardColour
//
//  Fades one CRGB colour toward another CRGB colour by a given amount
CRGB fadeTowardColour(CRGB &cur, const CRGB &target, uint8_t amount)
{
  blendU8TowardU8(cur.red,    target.red,   amount);
  blendU8TowardU8(cur.green,  target.green, amount);
  blendU8TowardU8(cur.blue,   target.blue,  amount);
  return cur;
}

//  mapU8ToCRGB
//
//  Maps a U8 to a CRGB colour in a vector.
CRGB mapU8toCRGB(uint8_t input, std::vector<CRGB> colours)
{
  int size = colours.size();
  int segmentSize = 256 / size;
  int i = 1;
  int upperBound = i * segmentSize;

  while ((input >= upperBound) && (i < size))
  {
    i++;
    upperBound = i * segmentSize;
  }

  return colours[i - 1];
}

//  weightedAverage
//
//  Takes a vector of U8s and a vector of weights. Calcultes the weighted average of the U8s.
//  NO ERROR CHECKING IS DONE. MAKE SURE THE VECTORS ARE THE SAME SIZE, YOU DUMB FUCK.
uint8_t weightedAverage(std::vector<uint8_t> data, std::vector<uint8_t> weights)
{
  int size = data.size();
  uint8_t nominator = 0;
  uint8_t denominator = 0;
  for (int i = 0; i < size; i++)
  {
    nominator += data[i] * weights[i];
    denominator += weights[i];
  }

  return (nominator / denominator);
}

//  bunch of parsers for the web server

byte parseColor(String color)
{
  float fColor = color.toFloat();
  return (byte)fColor;
}

byte parseDelta(String deltaHue)
{
  float fDelta = deltaHue.toFloat();
  return (byte)fDelta;
}

byte parseBrightness(String b)
{
    float brite = b.toFloat() / 100.0f;
    return brite * 255;
}

bool parseBool(String sBool)
{
    if (sBool == "true")
        return true;
    else
        return false;
}