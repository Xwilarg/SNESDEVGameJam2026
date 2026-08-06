#include "country.h"

Country countries[COUNTRY_COUNT] = {
    {
        "Elanoe              ",
        0,
        286,
        NULL
    },
    {
        "Taureki             ",
        76,
        286,
        NULL
    },
    {
        "Amareki             ",
        256,
        286,
        NULL
    },
    {
        "Eno                 ",
        0,
        184,
        NULL
    },
    {
        "Bretna              ",
        198,
        188,
        NULL
    },
    {
        "Strena              ",
        256,
        226,
        NULL
    },
    {
        "East Brina          ",
        256,
        50,
        NULL
    },
    {
        "Republic of Brina   ",
        186,
        36,
        NULL
    },
    {
        "Larti               ",
        34,
        30,
        NULL
    },
    {
        "Boet                ",
        0,
        90,
        NULL
    },
    {
        "Seranegao           ",
        0,
        0,
        NULL
    }
};

Troop* Country_NewTroop(Country* country)
{
    Troop* troop = Troop_New();

    if (country->troops != NULL)
    {
        troop->next = country->troops;
    }

    country->troops = troop;

    return troop;
}