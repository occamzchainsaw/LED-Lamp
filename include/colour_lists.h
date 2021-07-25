/*
 * colour_lists.h
 *
 * Created on Thu Jul 22 2021
 * 
 * Header file containing definitions of availabe colour lists for the rotating palette effect.
 * Using actual std::list, cause shit's going crazy if you try to do it with arrays.
 *
 * Copyright (c) 2021 occamzchainsaw
 */

#include <FastLED.h>
#include <list>
#include <vector>

//  pretty standard rainbow
std::list<CRGB> CLRainbow = {
    CRGB(255,   0,     0),      //  red
    CRGB(171,   85,    0),      //  orange
    CRGB(171,   171,   0),      //  yellow
    CRGB(0,     255,   0),      //  green
    CRGB(0,     171,   85),     //  aqua
    CRGB(0,     0,     255),    //  blue
    CRGB(85,    0,     171),    //  purple
    CRGB(171,   0,     85)      //  pink
};

//  some ocean vibes, stolen from FastLED, ofc
std::list<CRGB> CLOcean = {
    CRGB::MidnightBlue,
    CRGB::DarkBlue,
    CRGB::Navy,
    CRGB::MediumBlue,
    CRGB::SeaGreen,
    CRGB::Teal,
    CRGB::CadetBlue,
    CRGB::Blue,
    CRGB::DarkCyan,
    CRGB::CornflowerBlue,
    CRGB::Aquamarine,
    CRGB::Aqua,
    CRGB::LightSkyBlue
};

//  stolen forest stuff
std::list<CRGB> CLForest = {
    CRGB::DarkGreen,
    CRGB::DarkOliveGreen,
    CRGB::Green,
    CRGB::ForestGreen,
    CRGB::OliveDrab,
    CRGB::SeaGreen,
    CRGB::MediumAquamarine,
    CRGB::LimeGreen,
    CRGB::YellowGreen,
    CRGB::LightGreen,
    CRGB::LawnGreen,
};

//  heat map more or less like the black body radiator
std::list<CRGB> CLHeatMapBlackBody = {
    CRGB(0,   0,   0),          //  black
    CRGB(51,  0,   0),
    CRGB(102, 0,   0),
    CRGB(153, 0,   0),
    CRGB(204, 0,   0),
    CRGB(255, 0,   0),          //  red
    CRGB(255, 51,  0),
    CRGB(255, 102, 0),
    CRGB(255, 153, 0),
    CRGB(255, 204, 0),
    CRGB(255, 255, 0),          //  yellow
    CRGB(255, 255, 51),
    CRGB(255, 255, 102),
    CRGB(255, 255, 153),
    CRGB(255, 255, 204),
    CRGB(255, 255, 255)         //  white
};

std::vector<CRGB> CLHeatMap = {
    CRGB(0,   0,   0),          //  black
    CRGB(64,  0,   0),
    CRGB(128, 0,   0),
    CRGB(192, 0,   0),
    CRGB(255, 0,   0),
    CRGB(255, 64,  0),
    CRGB(255, 128, 0),
    CRGB(255, 192, 0),
    CRGB(255, 255, 0),
    CRGB(255, 255, 64),
    CRGB(255, 255, 128)
    // CRGB(255, 255, 153),
    // CRGB(255, 255, 204),
    // CRGB(255, 255, 204)         //  white
};

std::vector<CRGB> CLIceFire = {
    CRGB(0,   0,   0),
    CRGB(0,   0,   64),
    CRGB(0,   0,   128),
    CRGB(0,   0,   192),
    CRGB(0,   0,   255),
    CRGB(0,   64,  255),
    CRGB(0,   128, 255),
    CRGB(0,   192, 255),
    CRGB(0,   255, 255),
    CRGB(64,  255, 255),
    CRGB(128, 255, 255)
};

std::vector<CRGB> CLGreenFire = {
    CRGB(0,   0,   0),
    CRGB(0,   64,  0),
    CRGB(0,   128, 0),
    CRGB(0,   192, 0),
    CRGB(0,   255, 0),
    CRGB(64,  255, 0),
    CRGB(128, 255, 0),
    CRGB(192, 255, 0),
    CRGB(255, 255, 0),
    CRGB(255, 255, 64),
    CRGB(255, 255, 128)
};

//  for testing only
std::list<CRGB> CLTest = {
    CRGB(255, 0, 0),
    CRGB(0, 255, 0)
};