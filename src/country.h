#pragma once

#include <snes.h>

#include "troop.h"

#define COUNTRY_COUNT 11

typedef struct
{
    char* name;

    u16 xPos;
    u16 yPos;
    Troop* troops;
} Country;

extern Country countries[COUNTRY_COUNT];

Troop* Country_NewTroop(Country* country);