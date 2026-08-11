#include "country.h"

Country countries[COUNTRY_COUNT] = {
    {
        "Elanoe",

        // X,Y
        0,
        286,

        NULL, // Troops
        3, // Population
        MY_TEAM // Team
    },
    {
        "Taureki",
        76,
        286,

        NULL, // Troops
        4, // Population
        1 // Team
    },
    {
        "Amareki",
        256,
        286,

        NULL, // Troops
        4, // Population
        2 // Team
    },
    {
        "Eno",
        0,
        184,

        NULL, // Troops
        4, // Population
        3 // Team
    },
    {
        "Bretna",
        198,
        188,

        NULL, // Troops
        2, // Population
        4 // Team
    },
    {
        "Strena",
        256,
        226,

        NULL, // Troops
        2, // Population
        5 // Team
    },
    {
        "East Brina",
        256,
        50,

        NULL, // Troops
        3, // Population
        6 // Team
    },
    {
        "Republic of Brina",
        186,
        36,

        NULL, // Troops
        3, // Population
        7 // Team
    },
    {
        "Larti",
        39,
        30,

        NULL, // Troops
        7, // Population
        8 // Team
    },
    {
        "Boet",
        5,
        90,

        NULL, // Troops
        1, // Population
        9 // Team
    },
    {
        "Seranegao",
        0,
        0,

        NULL, // Troops
        3, // Population
        10 // Team
    }
};

Troop* Country_NewTroop(Country* country, TroopType troopType, int team)
{
    Troop* troop = Troop_New(troopType, team);

    if (country->troops != NULL)
    {
        troop->next = country->troops;
    }

    country->troops = troop;

    return troop;
}