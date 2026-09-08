#include "ai.h"

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
            Country_AddExisting(&countries[country->nearbyCountries[0]], t);
        }

        --country->population;
    }
}