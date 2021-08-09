/*
 * colour_lists.h
 *
 * Created on Thu Jul 22 2021
 * 
 * Header file containing definitions of availabe colour lists for the rotating palette effect.
 * Using std::vector, cause multidimenstional arrays will drive me insane.
 *
 * Copyright (c) 2021 occamzchainsaw
 */

#include <FastLED.h>
#include <vector>
#include <list>

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

//  In vector form as well
std::vector<CRGB> CVOcean = {
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

std::vector<CRGB> CVForest = {
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

CRGBPalette16 RedFire = {
    0x000000, 0x550000, 0xAA0000, 0xFF0000,
    0xFF1C00, 0xFF3800, 0xFF5400, 0xFF7000, 0xFF8C00, 0xFFA800, 0xFFC400, 0xFFE000,
    0xFFFF00, 0xFFFF55, 0xFFFFAA, 0xFFFFFF
};

CRGBPalette16 BlueFire = {
    0x000000, 0x000055, 0x0000AA, 0x0000FF,
    0x001CFF, 0x0038FF, 0x0054FF, 0x0070FF, 0x008CFF, 0x00A8FF, 0x00C4FF, 0x00FEFF,
    0x00FFFF, 0x55FFFF, 0xAAFFFF, 0xFFFFFF
};

CRGBPalette16 GreenFire = {
    0x000000, 0x005500, 0x00AA00, 0x00FF00,
    0x1CFF00, 0x38FF00, 0x54FF00, 0x70FF00, 0x8CFF00, 0xA8FF00, 0xC4FF00, 0xFEFF00,
    0xFFFF00, 0xFFFF55, 0xFFFFAA, 0xFFFFFF
};

CRGBPalette16 pacifica_palette_1 = {
    0x000507, 0x000409, 0x00030B, 0x00030D,
    0x000210, 0x000212, 0x000114, 0x000117, 
    0x000019, 0x00001C, 0x000026, 0x000031,
    0x00003B, 0x000046, 0x14554B, 0x28AA50
};