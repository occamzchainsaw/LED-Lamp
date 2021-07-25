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
#include <list>
#include <iterator>

class ColourRotationEffect
{
    protected:
        std::list<CRGB> ColoursList;                                //  List of colours to cycle through (see colour_lists.h)
        CRGB CurrentColour;                                         //  Currently displayed colour (changes while fading)
        CRGB NextColour;                                            //  Next colour on the list (crucial for blending one into the other)
        uint32_t LastFade;
        std::list<CRGB>::iterator Index;                            //  list iterator
        float StaticTime;                                           //  How long each colour should be displayed [ms]

    public:
        ColourRotationEffect(std::list<CRGB> colours, float staticTime = 1.0f)
        {
            ColoursList = colours;
            CurrentColour = ColoursList.front();
            Index = std::next(ColoursList.begin());
            NextColour = *Index;
            LastFade = millis();
            StaticTime = staticTime * 1000.0f;
        }

        ~ColourRotationEffect() {  }

        void draw(uint8_t setBrightness)
        {
            uint8_t scaledBrightness = calculate_max_brightness_for_power_mW(FastLED.leds(), FastLED.size(), setBrightness, MAX_POWER);

            if (millis() - LastFade > (uint32_t)StaticTime)                         //  Only change the colour if the current one has been displayed for a set amount of time
            {
                CurrentColour = fadeTowardColour(CurrentColour, NextColour, 2);     //  Fade the current colour towards the next by a small amount each cycle

                if (CurrentColour == NextColour)
                {
                    LastFade = millis();
                    Index = std::next(Index);
                    if (Index == ColoursList.end())                         //  list.end() is actually next(list.back()), so one past the last element, so...
                    {                                                       //  ...this is equivalent to saying index > list.size()
                        Index = ColoursList.begin();                        //  Wrap the index around back to 0
                    }
                    NextColour = *Index;                                    //  Assign next colour according to the new iterator value
                }
            }
            
            fill_solid(FastLED.leds(), FastLED.size(), CurrentColour);
            FastLED.setBrightness(scaledBrightness);
        }

};