#include "ai.h"

static Country* GetRandomTarget(Country* source)
{
    u16 length = 0;
    u16* ptr = source->nearbyCountries;
    while (*ptr != -1)
    {
        ptr++;
        length++;
    }

    return &countries[source->nearbyCountries[rand() % length]];
}

static void MoveTroop(Country* country, Troop* t)
{
    Country_RemoveExisting(country, NULL, t);
    Country_AddExisting(GetRandomTarget(country), t);
}

void AI_ResolveTurn(Country* country)
{
    u16 spawnCount = 0;
    while (country->population > 0)
    {
        if (country->team == 1) // Taureki
        {
            Troop* t = Country_NewTroop(country, MILITIA, country->team);
            if (rand() % 3 == 1) MoveTroop(country, t);
        }
        else if (country->team == 2) // Amareki
        {
            Troop* t = Country_NewTroop(country, rand() % 4 == 1 ? GUARDIAN : SWORDMAN, country->team);
            if (rand() % 5 == 1) MoveTroop(country, t);
        }
        else if (country->team == 3) // Eno
        {
            Troop* t = Country_NewTroop(country, SPEARMAN, country->team);
            if (rand() % 3 == 1) MoveTroop(country, t);
        }
        else if (country->team == 4) // Bretna
        {
            Troop* t = Country_NewTroop(country, spawnCount == 0 ? SWORDMAN : BOWMAN, country->team);
            if (rand() % 5 == 1) MoveTroop(country, t);
        }
        else if (country->team == 5) // Strena
        {
            Troop* t = Country_NewTroop(country, spawnCount == 0 ? BOWMAN : rand() % 2 == 1 ? MILITIA : BOWMAN, country->team);
            if (rand() % 3 == 1) MoveTroop(country, t);
        }
        else if (country->team == 6 ) // East Brina
        {
            u16 r = rand();
            int troop;
            if (r == 0) troop = SWORDMAN;
            else if (r == 1) troop = SPEARMAN;
            else troop = CROSSBOWMAN;

            Troop* t = Country_NewTroop(country, troop, country->team);
            if (rand() % 3 == 1) MoveTroop(country, t);
        }
        else if (country->team == 7) // Republic of Brina
        {
            u16 r = rand();
            int troop;
            if (r == 0) troop = SWORDMAN;
            else if (r == 1) troop = HORSERIDER;
            else troop = BOWMAN;

            Troop* t = Country_NewTroop(country, troop, country->team);
            if (rand() % 3 == 1) MoveTroop(country, t);
        }
        else if (country->team == 8) // Larti
        {
            Troop* t = Country_NewTroop(country, spawnCount == 0 ? CHAMPION : rand() % 2 == 1 ? MILITIA : SWORDMAN, country->team);
            if (rand() % 3 == 1) MoveTroop(country, t);
        }
        else if (country->team == 9) // Boet
        {
            Troop* t = Country_NewTroop(country, rand() % 3 == 1 ? SPEARMAN : BOWMAN, country->team);
            if (rand() % 3 == 1) MoveTroop(country, t);
        }
        else if (country->team == 10) // Seranegao
        {
            Troop* t = Country_NewTroop(country, HORSERIDER, country->team);
            if (rand() % 2 == 1) MoveTroop(country, t);
        }

        ++spawnCount;
        --country->population;
    }
}