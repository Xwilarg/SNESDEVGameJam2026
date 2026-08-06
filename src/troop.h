#pragma once

#include <snes.h>

typedef struct Troop
{
    struct Troop* next;

    u16 health;
    u16 strength;
} Troop;

Troop* Troop_New(void);