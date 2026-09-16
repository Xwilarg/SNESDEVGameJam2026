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

        ++spawnCount;
        --country->population;
    }
}