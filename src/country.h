#ifndef COUNTRY_H
#define COUNTRY_H

#include <snes.h>

#include "troop.h"

#define COUNTRY_COUNT 11

typedef struct
{
    char* name;

    u16 xPos;
    u16 yPos;
    Troop* troops;

    u16 maxPopulation;
    u16 population;

    u16 team;
    u16* nearbyCountries;
} Country;

extern Country countries[COUNTRY_COUNT];

Troop* Country_NewTroop(Country* country, TroopType troopType, int team);
void Country_AddExisting(Country* country, Troop *t);
bool Country_HaveConflictPending(Country* country);
void Country_ResolveBattle(Country* country);
void Country_ResolveAITurn(Country* country);

#endif