#pragma once

#include <snes.h>

typedef enum
{
    BOWMAN,
    SWORDMAN,
    SPEARMAN
} TroopType;

typedef struct Troop
{
    struct Troop* next;

    u16 size;
    TroopType type;
} Troop;

Troop* Troop_New(TroopType type);