#include "ai.h"
#include "troop.h"

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
    t->wasMoved = true;
}

static u16 GetTroopCount(Country* country, u16 team)
{
    u16 count = 0;

    Troop* it = country->troops;
    while (it != NULL)
    {
        if (it->team == team)
        {
            ++count;
        }
        it = it->next;
    }
    return count;
}

u16 GetUnitCount(Country* country)
{
    u16 i;
    for (i = 0; country->units[i] != -1; i++)
    { }
    return i;
}

void AI_ResolveTurn(Country* country)
{
    Troop* tr = country->troops;
    while (tr != NULL)
    {
        if (tr->wasMoved == false && tr->team != MY_TEAM) // Enemy troop
        {
            if (rand() % 2 == 0) // Move troop (1/2 chance)
            {
                u16* it = country->nearbyCountries;

                // Amount of adjacent countries
                u16 i = 0;
                while (*it != -1)
                {
                    it++;
                    i++;
                }

                u16 chosen = rand() % i; // Pick a random target

                // Move troop
                Troop* tmp = tr->next;
                MoveTroop(countries->nearbyCountries + chosen, tr);
                tr->wasMoved = true;

                tr = tmp;
            }
            else
            {
                tr->wasMoved = true;
                tr = tr->next;
            }
        }
        else
        {
            tr = tr->next;
        }
    }

    u16 spawnCount = 0;
    while (country->population > 0)
    {
        // Try upgrading
        if (Country_GetArmyCount(country, country->team) >= MAX_TROOP_COUNTRY)
        {
            Troop* t = country->troops;
            while (t != NULL)
            {
                if (t->level < MAX_UPGRADE_LEVEL)
                {
                    ++t->level;
                    break;
                }

                t = t->next;
            }
        }
        else if (country->team != country->originalTeam)
        {
            // Not our country, let's just build random things
            u16 buildIndex = rand() % GetUnitCount(country);
            Troop* t = Country_NewTroop(country, country->units[buildIndex], country->team);
            if (rand() % 2 == 1) MoveTroop(country, t);
        }
        else
        {
            if (country->team == 1) // Taureki
            {
                Troop* t = Country_NewTroop(country, MILITIA, country->team);
                if (rand() % 3 == 1 || Country_GetArmyCount(country, country->team) >= MAX_TROOP_COUNTRY) MoveTroop(country, t);
            }
            else if (country->team == 2) // Amareki
            {
                Troop* t = Country_NewTroop(country, rand() % 4 == 1 ? GUARDIAN : SWORDMAN, country->team);
                if (rand() % 5 == 1 || Country_GetArmyCount(country, country->team) >= MAX_TROOP_COUNTRY) MoveTroop(country, t);
            }
            else if (country->team == 3) // Eno
            {
                Troop* t = Country_NewTroop(country, SPEARMAN, country->team);
                if (rand() % 3 == 1 || Country_GetArmyCount(country, country->team) >= MAX_TROOP_COUNTRY) MoveTroop(country, t);
            }
            else if (country->team == 4) // Bretna
            {
                Troop* t = Country_NewTroop(country, spawnCount == 0 ? SWORDMAN : BOWMAN, country->team);
                if (rand() % 5 == 1 || Country_GetArmyCount(country, country->team) >= MAX_TROOP_COUNTRY) MoveTroop(country, t);
            }
            else if (country->team == 5) // Strena
            {
                Troop* t = Country_NewTroop(country, spawnCount == 0 ? BOWMAN : rand() % 2 == 1 ? MILITIA : BOWMAN, country->team);
                if (rand() % 3 == 1 || Country_GetArmyCount(country, country->team) >= MAX_TROOP_COUNTRY) MoveTroop(country, t);
            }
            else if (country->team == 6 ) // East Brina
            {
                u16 r = rand();
                int troop;
                if (r == 0) troop = SWORDMAN;
                else if (r == 1) troop = SPEARMAN;
                else troop = CROSSBOWMAN;

                Troop* t = Country_NewTroop(country, troop, country->team);
                if (rand() % 3 == 1 || Country_GetArmyCount(country, country->team) >= MAX_TROOP_COUNTRY) MoveTroop(country, t);
            }
            else if (country->team == 7) // Republic of Brina
            {
                u16 r = rand();
                int troop;
                if (r == 0) troop = SWORDMAN;
                else if (r == 1) troop = HORSERIDER;
                else troop = BOWMAN;

                Troop* t = Country_NewTroop(country, troop, country->team);
                if (rand() % 3 == 1 || Country_GetArmyCount(country, country->team) >= MAX_TROOP_COUNTRY) MoveTroop(country, t);
            }
            else if (country->team == 8) // Larti
            {
                Troop* t = Country_NewTroop(country, spawnCount == 0 ? CHAMPION : rand() % 2 == 1 ? MILITIA : SWORDMAN, country->team);
                if (rand() % 3 == 1 || Country_GetArmyCount(country, country->team) >= MAX_TROOP_COUNTRY) MoveTroop(country, t);
            }
            else if (country->team == 9) // Boet
            {
                Troop* t = Country_NewTroop(country, rand() % 3 == 1 ? SPEARMAN : BOWMAN, country->team);
                if (rand() % 3 == 1 || Country_GetArmyCount(country, country->team) >= MAX_TROOP_COUNTRY) MoveTroop(country, t);
            }
            else if (country->team == 10) // Seranegao
            {
                Troop* t = Country_NewTroop(country, HORSERIDER, country->team);
                if (rand() % 2 == 1 || Country_GetArmyCount(country, country->team) >= MAX_TROOP_COUNTRY) MoveTroop(country, t);
            }
        }

        ++spawnCount;
        --country->population;
    }
}