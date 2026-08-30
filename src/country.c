#include "country.h"

Country countries[COUNTRY_COUNT] = {
    {
        "Elanoe",

        // X,Y
        0,
        286,

        NULL, // Troops

        3, // Population
        3,

        MY_TEAM, // Team

        (u16[]){ 1, 3, -1 }
    },
    {
        "Taureki",
        76,
        286,

        NULL, // Troops

        4, // Population
        4,

        1, // Team

        (u16[]){ 0, 2, 3, -1 }
    },
    {
        "Amareki",
        256,
        286,

        NULL, // Troops

        4, // Population
        4,

        2, // Team

        (u16[]){ 1, 5, -1 }
    },
    {
        "Eno",
        0,
        184,

        NULL, // Troops

        4, // Population
        4,

        3, // Team

        (u16[]){ 0, 1, 9, -1 }
    },
    {
        "Bretna",
        198,
        188,

        NULL, // Troops

        2, // Population
        2,

        4, // Team

        (u16[]){ 5, 6, -1 }
    },
    {
        "Strena",
        256,
        226,

        NULL, // Troops

        2, // Population
        2,

        5, // Team

        (u16[]){ 2, 4, -1 }
    },
    {
        "East Brina",
        256,
        50,

        NULL, // Troops

        3, // Population
        3,

        6, // Team

        (u16[]){ 4, 7, -1 }
    },
    {
        "Republic of Brina",
        186,
        36,

        NULL, // Troops

        3, // Population
        3,

        7, // Team

        (u16[]){ 6, 8, -1 }
    },
    {
        "Larti",
        39,
        30,

        NULL, // Troops

        7, // Population
        7,

        8, // Team

        (u16[]){ 7, 9, 10, -1 }
    },
    {
        "Boet",
        5,
        90,

        NULL, // Troops

        1, // Population
        1,

        9, // Team

        (u16[]){ 3, 8, -1 }
    },
    {
        "Seranegao",
        0,
        0,

        NULL, // Troops

        3, // Population
        3,

        10, // Team

        (u16[]){ 8, -1 }
    }
};

bool Country_HaveConflictPending(Country* country)
{
    if (country->troops == NULL) return false;

    Troop* it = country->troops;
    u16 teamRef = country->team;

    while (it != NULL)
    {
        if (teamRef != it->team) return true;
        it = it->next;
    }

    return false;
}

void Country_ResolveAITurn(Country* country)
{
    u16 spawnCount = 0;
    while (country->population > 0)
    {
        Troop* t = Country_NewTroop(country, SWORDMAN, country->team);

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

Troop* Country_NewTroop(Country* country, TroopType troopType, int team)
{
    Troop* troop = Troop_New(troopType, team);

    troop->next = country->troops;
    country->troops = troop;

    return troop;
}

void Country_RemoveExisting(Country* country, Troop* last, Troop* t)
{
    if (last == NULL)
    {
        country->troops = t->next;
    }
    else
    {
        last->next = t->next;
    }
}

void Country_AddExisting(Country* country, Troop *t)
{
    t->next = country->troops;
    country->troops = t;
}