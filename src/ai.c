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

void AI_ResolveTurn(Country* country)
{
    u16 spawnCount = 0;
    while (country->population > 0)
    {
        Troop* t = Country_NewTroop(country, MERCENARY, country->team);

        ++spawnCount;
        if (spawnCount % 2 == 1)
        {
            // Move troop
            Country_RemoveExisting(country, NULL, t);
            Country_AddExisting(GetRandomTarget(country), t);
        }

        --country->population;
    }
}