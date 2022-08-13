/*
 * colour_lists.h
 *
 * Created on Thu Jul 22 2021
 * 
 * Header file containing definitions of availabe colour lists for the rotating palette effect and other stuff.
 *
 * Copyright (c) 2021 occamzchainsaw
 */

#pragma once
#include <FastLED.h>
#include <vector>
#include <list>

CRGBPalette16 Rainbow = {
    0xFF0000, 0xFF5E00, 0xFFBB00, 0xE5FF00,
    0x88FF00, 0x2BFF00, 0x00FF33, 0x00FF91,
    0x00FFEE, 0x00B3FF, 0x0055FF, 0x0800FF,
    0x6600FF, 0xC300FF, 0xFF00DD, 0xFF0055
};

//  Fire palettes with different base colours
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

std::map<std::string, CRGBPalette16> PaletteMap = {
    {"rainbow", Rainbow},
    {"redFire", RedFire},
    {"blueFire", BlueFire},
    {"greenFire", GreenFire}
};