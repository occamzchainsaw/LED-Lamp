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

class FireEffect
{
    protected:
        byte *Heat;
        int Size;
        std::vector<CRGB> ColourMap;
        uint8_t CoolingRate;
        uint8_t SparkingRate;
        uint8_t SparkingRegionSize;
        bool IsReversed;
        bool IsMirrored;

        static const byte w0 = 1;
        static const byte w1 = 2;
        static const byte w2 = 2;
        static const byte w3 = 3;
        static const byte w4 = 3;
        static const byte w5 = 5;
        static const byte w6 = 5;
        static const byte w7 = 3;
        static const byte wTotal = w0 + w1 + w2 + w3 + w4 + w5 + w6 + w7;

    public:
        //  FireEffect Constructor
        //
        //  Size - number of pixels, 
        //  Colours - colour map (vector<CRGB>),
        //  Cooling - up to how much to cool by every iteration,
        //  Sparking - chance of getting new sparks,
        //  SparkingHeight - how many pixels can spark,
        //  Reversed - upside down fire,
        //  Mirrored - split it in half and have a double fire
        FireEffect(int size, std::vector<CRGB> colours, uint8_t cooling, uint8_t sparking, uint8_t sparkingHeight, bool reversed, bool mirrored)
            :   ColourMap(colours),
                CoolingRate(cooling),
                SparkingRate(sparking),
                SparkingRegionSize(sparkingHeight),
                IsReversed(reversed),
                IsMirrored(mirrored)
        {
            Size = IsMirrored ? (size / 2) : size;
            Heat = new byte[Size] { 0 };
        }

        ~FireEffect() {   }

        void draw(uint8_t setBrightness)
        {
            uint8_t scaledBrightness = calculate_max_brightness_for_power_mW(FastLED.leds(), FastLED.size(), setBrightness, MAX_POWER);

            //  cool every pixel by a random amount
            for (int i = 0; i < Size; i++)
            {
                Heat[i] = qsub8(Heat[i], random8(0, ((CoolingRate * 10) / Size) + 2));
            }

            //  drift up, and blend with neighbours to diffuse the fire
            //  this is a mess, and won't scale at all, but I'm done giving fucks at this point.
            for (int i = Size; i >= 7; i--)
            {
                Heat[i] = ( Heat[i]   * w0 + 
                            Heat[i-1] * w1 + 
                            Heat[i-2] * w2 + 
                            Heat[i-3] * w3 + 
                            Heat[i-4] * w4 + 
                            Heat[i-5] * w5 +
                            Heat[i-6] * w6 +
                            Heat[i-7] * w7   ) / wTotal;
            }

            //  randomly spark in the sparking region
            if (random8(255) < SparkingRate)                            //  if true, we rolled a number below the selected rate, i.e. we got lucky
            {
                for (int i = 0; i < 4; i++)
                {
                    int x = random8(SparkingRegionSize);                    //  we gonna spark, so select a random pixel in the sparking region
                    Heat[x] = qadd8(Heat[x], random8(150, 255));            //  heat the selected pixel a bunch
                    //Heat[x] = Heat[x] + random8(160, 255);
                }
            }

            FastLED.clear();
            //  map the heat to a colour vector
            for (int i = 0; i < Size; i++)
            {
                Heat[i] = scale8(Heat[i], 240);                         //  cap the heat at 240 (FastLED recommends that for best results lol)
                CRGB colour = mapU8toCRGB(Heat[i], ColourMap);
                int j = IsReversed ? (Size - 1 - i) : i;
                DrawPixels(j, 1.5, colour);
                if (IsMirrored)
                {
                    int k = IsReversed ? (Size + i) : (2 * Size - 1 - i);
                    DrawPixels(k, 1.5, colour);
                }
            }

            FastLED.setBrightness(scaledBrightness);
        }
};