#pragma once

#include <snes.h>

#define COUNTRY_COUNT 11

typedef struct
{
    char* name;
    u16 nameLength;

    u16 xPos;
    u16 yPos; 
} Country;

extern Country countries[COUNTRY_COUNT];