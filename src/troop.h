#pragma once

#include <snes.h>

#define MY_TEAM 0

typedef enum
{
    BOWMAN,
    SWORDMAN,
    SPEARMAN
} TroopType;

typedef struct Troop
{
    struct Troop* next;

    int team;
    u16 level;
    TroopType type;
} Troop;

Troop* Troop_New(TroopType type, int team);
bool HaveAnyAllies(Troop* list);
bool HaveAnyUpgradableAllies(Troop* list);
bool CanBeUpgrade(Troop* t);