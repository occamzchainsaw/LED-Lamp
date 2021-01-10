/*
 * light_control.h
 *
 * Created on Sun Jan 10 2021
 * 
 * This header file contains functions for controlling the lighting
 * Solid color, White with warmth control, Rainbow, etc.
 *
 * Copyright (c) 2021 occamzchainsaw
 */
#include <Arduino.h>                                                //  Arduino Framework
#define FASTLED_INTERNAL
#include <FastLED.h>                                                //  Great lib for the LEDs

extern CRGB g_LEDs[];