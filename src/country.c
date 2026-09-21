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

        MY_TEAM,
        MY_TEAM, // Team

        (u16[]){ 1, 3, -1 } // Taureki and Eno
    },
    {
        "Taureki",

        // X,Y
        76,
        286,

        NULL, // Troops

        4, // Population
        4,

        1,
        1, // Team

        (u16[]){ 0, 2, 3, -1 } // Elanoe, Amareki and Eno
    },
    {
        "Amareki",

        // X,Y
        256,
        286,

        NULL, // Troops

        4, // Population
        4,

        2,
        2, // Team

        (u16[]){ 1, 5, -1 } // Taureki and Strena
    },
    {
        "Eno",

        // X,Y
        0,
        184,

        NULL, // Troops

        4, // Population
        4,

        3,
        3, // Team

        (u16[]){ 0, 1, 9, -1 } // Taureki and Boet
    },
    {
        "Bretna",

        // X,Y
        198,
        188,

        NULL, // Troops

        2, // Population
        2,

        4,
        4, // Team

        (u16[]){ 5, 6, -1 } // Strena and East Brina
    },
    {
        "Strena",

        // X,Y
        256,
        226,

        NULL, // Troops

        2, // Population
        2,

        5,
        5, // Team

        (u16[]){ 2, 4, -1 } // Amareki and Bretna
    },
    {
        "East Brina",

        // X,Y
        256,
        50,

        NULL, // Troops

        3, // Population
        3,

        6,
        6, // Team

        (u16[]){ 4, 7, -1 } // Bretna and Republic of Brina
    },
    {
        "Republic of Brina",

        // X,Y
        186,
        36,

        NULL, // Troops

        3, // Population
        3,

        7,
        7, // Team

        (u16[]){ 6, 8, -1 } // East Brina and Larti
    },
    {
        "Larti",

        // X,Y
        39,
        30,

        NULL, // Troops

        7, // Population
        7,

        8,
        8, // Team

        (u16[]){ 7, 9, 10, -1 } // Republic of Brina, Boet and Seranegao 
    },
    {
        "Boet",

        // X,Y
        5,
        90,

        NULL, // Troops

        1, // Population
        1,

        9,
        9, // Team

        (u16[]){ 3, 8, 10, -1 } // Larti, Eno and Seranegao
    },
    {
        "Seranegao",

        // X,Y
        0,
        0,

        NULL, // Troops

        3, // Population
        3,

        10,
        10, // Team

        (u16[]){ 8, 9, -1 } // Larti and Boet
    }
};

void Country_ClearDescription(void)
{
    consoleDrawText(0, 4, "                                ");
    consoleDrawText(0, 5, "                                ");
    consoleDrawText(0, 6, "                                ");
    consoleDrawText(0, 7, "                                ");
    consoleDrawText(0, 8, "                                ");
    consoleDrawText(0, 9, "                                ");
    consoleDrawText(0, 10, "                                ");
    consoleDrawText(0, 11, "                                ");
    consoleDrawText(0, 12, "                                ");
    consoleDrawText(0, 13, "                                ");
}

void Country_WriteDescription(int countryIndex)
{
    if (countryIndex == 1) // Taureki
    {
        consoleDrawText(0, 4, "After the great war, Taureki");
        consoleDrawText(0, 5, "army was left in a very poor");
        consoleDrawText(0, 6, "state. The king urgently started");
        consoleDrawText(0, 7, "to recruit peasants to build");
        consoleDrawText(0, 8, "back a semblance of an army");
    }
    else if (countryIndex == 2) // Amareki
    {
        consoleDrawText(0, 4, "Amareki carved its way in");
        consoleDrawText(0, 5, "history thanks to its");
        consoleDrawText(0, 6, "mastership of the forge");
        consoleDrawText(0, 7, "and ability to make very");
        consoleDrawText(0, 8, "high quality swords.");
        consoleDrawText(0, 9, "They train elite units");
        consoleDrawText(0, 10, "named guardians that are");
        consoleDrawText(0, 11, "said to use the sword");
        consoleDrawText(0, 12, "expertly and have no");
        consoleDrawText(0, 13, "weakness");
    }
    else if (countryIndex == 3) // Eno
    {
        consoleDrawText(0, 4, "Eno always privileged");
        consoleDrawText(0, 5, "discipline, which reflect");
        consoleDrawText(0, 6, "in its army. The use of spear");
        consoleDrawText(0, 7, "is heavily dominant and");
        consoleDrawText(0, 8, "formations are driven by");
        consoleDrawText(0, 9, "infantry squares");
    }
    else if (countryIndex == 4) // Bretna
    {
        // Expertise of the bow but mostly defense
    }
    else if (countryIndex == 5) // Strena
    {
        consoleDrawText(0, 4, "Strena compensate its lack");
        consoleDrawText(0, 5, "of land by an expertise of");
        consoleDrawText(0, 6, "the bow");
    }
    else if (countryIndex == 9) // Boet
    {
        consoleDrawText(0, 4, "Boet is an old colony of Eno,");
        consoleDrawText(0, 5, "keeping their martiality they");
        consoleDrawText(0, 6, "also integrated the use of");
        consoleDrawText(0, 7, "the bow as support");
    }
    else
    {
        consoleDrawText(0, 4, "No information is known about");
        consoleDrawText(0, 5, "this country so far...");
    }
}

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