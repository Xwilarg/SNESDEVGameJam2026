#include "freeroam.h"

#include <snes.h>

#include <stdio.h> // snprintf

#include "game.h"
#include "country.h"
#include "menu.h"

#define MENU_UNASSIGNED 0
#define MENU_CREATE_TROOP 1
#define MENU_UPGRADE_TROOP 2
#define MENU_MOVE_TROOP 3

static u16 pad0;

static u16 menuIndex = 0;

// Moving troops
static s16 moveDestination = -1;

u16 GetMenuItemCount(void);
char* OnMenuTitle(void);
char* OnMenuLabel(u16 index);
bool OnMenuSelect(u16 index);
void OnMenuReset(void);

static char buffer[30];

static Menu menu = {
    0,
    &GetMenuItemCount,
    &OnMenuTitle,
    &OnMenuLabel,
    &OnMenuSelect,
    &OnMenuReset
};

s16 Wrap(s16 x, s16 min, s16 max)
{
    if (x < min) x = max;
    if (x > max) x = min;
    return x;
}

static void Init(void)
{
    menu.index = 0;

    u16 i;
    for (i = 0; i < COUNTRY_COUNT; i++)
    {
        Country* country = &countries[i];
        country->population = country->maxPopulation;
    }

    Game_UpdateCurrentCountry(Game_GetCountryIndex());
}

static void Cleanup(void)
{
    u16 i;
    for (i = 0; i < COUNTRY_COUNT; i++)
    {
        Country* country = &countries[i];
        if (country->team != MY_TEAM)
        {
            Country_ResolveAITurn(country);
        }
    }

    Menu_Clear();
}

static void ReachCountry(void)
{
    Menu_Draw(&menu);
}

static void Update(bool isTurnStarted)
{
    pad0 = padsDown(0);

    switch (pad0)
    {
    case KEY_LEFT:
        if (!isTurnStarted)
        {
            menuIndex = MENU_UNASSIGNED;
            Game_UpdateCurrentCountry(Wrap(Game_GetCountryIndex() - 1, 0, COUNTRY_COUNT - 1));
        }
        break;
    case KEY_RIGHT:
        if (!isTurnStarted)
        {
            menuIndex = MENU_UNASSIGNED;
            Game_UpdateCurrentCountry(Wrap(Game_GetCountryIndex() + 1, 0, COUNTRY_COUNT - 1));
        }
        break;

    default:
        Menu_Input(&menu, pad0);
        break;
    }
}

static Phase phase = {
    &Init,
    &Cleanup,
    &ReachCountry,
    &Update
};

u16 GetMenuItemCount()
{
    Country* country = Game_GetCurrentCountry();

    if (country->team == MY_TEAM)
    {
        if (menuIndex == MENU_UNASSIGNED) return 4;
        if (menuIndex == MENU_CREATE_TROOP) return 4; // Amount of troop types + back
        if (menuIndex == MENU_UPGRADE_TROOP)
        {
            Troop* it = country->troops;

            // Number of choices is amount of troops that are ours and not level max
            int i = 0;
            while (it != NULL)
            {
                if (it->team == MY_TEAM && CanBeUpgrade(it))
                {
                    i++;
                }
                it = it->next;
            }
            return i + 1;
        }
        else if (menuIndex == MENU_MOVE_TROOP)
        {
            if (moveDestination == -1)
            {
                // Choose where to send the troop
                u16* it = country->nearbyCountries;

                // Amount of adjacent countries + back
                int i = 0;
                while (*it != -1)
                {
                    it++;
                    i++;
                }
                return i + 1;
            }
            else
            {
                Troop* it = country->troops;

                // Troops + back
                int i = 0;
                while (it != NULL)
                {
                    if (it->team == MY_TEAM)
                    {
                        i++;
                    }
                    it = it->next;
                }
                return i + 1;
            }
        }
        return 0;
    }
    return 1;
}

char* OnMenuTitle()
{
    Country* country = Game_GetCurrentCountry();

    if (country->team == MY_TEAM)
    {
        if (menuIndex == MENU_UNASSIGNED) return "Troop Management";
        if (menuIndex == MENU_CREATE_TROOP) return "New troop";
        if (menuIndex == MENU_UPGRADE_TROOP) return "Train troop";
        if (menuIndex == MENU_MOVE_TROOP)
        {
            if (moveDestination == -1)
            {
                return "Destination";
            }
            else
            {
                Country* c = &countries[moveDestination];
                snprintf(buffer, 30, "Send troops to %s", c->name);
                return buffer;
            }
        }

        return "UNKNOWN";
    }

    return "Diplomacy Options";
}

void OnMenuReset()
{
    Country* country = Game_GetCurrentCountry();

    if (country->team == MY_TEAM)
    {
        if (menuIndex == MENU_MOVE_TROOP && moveDestination != -1) moveDestination = -1;
        else
        {
            menuIndex = MENU_UNASSIGNED;
        }
    }
}

char* OnMenuLabel(u16 index)
{
    Country* country = Game_GetCurrentCountry();

    if (country->team == MY_TEAM)
    {
        if (menuIndex == MENU_UNASSIGNED)
        {
            if (index == 0) return country->population > 0 ? "New Troop" : "No population to make troops";
            if (index == 1) return HaveAnyUpgradableAllies(country->troops) ? "Train Troop" : "No troop to train";
            if (index == 2) return HaveAnyAllies(country->troops) ? "Move Troop" : "No troop to move";
            if (index == 3) return "Pass turn";
        }
        else if (menuIndex == MENU_CREATE_TROOP)
        {
            if (index == 0) return TroopTypeToString(BOWMAN);
            if (index == 1) return TroopTypeToString(SWORDMAN);
            if (index == 2) return TroopTypeToString(SPEARMAN);
            if (index == 3) return "Go back";
        }
        else if (menuIndex == MENU_UPGRADE_TROOP)
        {
            Troop* it = country->troops;

            int i = 0;
            while (it != NULL)
            {
                if (it->team == MY_TEAM && CanBeUpgrade(it))
                {
                    if (i == index)
                    {
                        return Troop_ToString(it);
                    }
                }
                it = it->next;
                i++;
            }
            return "Back";
        }
        else if (menuIndex == MENU_MOVE_TROOP)
        {
            if (moveDestination == -1)
            {
                u16* it = country->nearbyCountries;

                int i = 0;
                while (*it != -1)
                {
                    if (i == index)
                    {
                        Country* dest = &countries[*it];
                        return dest->name;
                    }
                    i++;
                    it++;
                }
                return "Back";
            }
            else
            {
                Troop* it = country->troops;

                int i = 0;
                while (it != NULL)
                {
                    if (it->team == MY_TEAM)
                    {
                        if (i == index)
                        {
                            return Troop_ToString(it);
                        }
                    }
                    it = it->next;
                    i++;

                    if (i == MENU_MAX_CHOICE_COUNT) return "Back";
                }
                return "Back";
            }
        }
        return NULL;
    }

    if (index == 0) return "Make alliance";
    return NULL;
}

bool OnMenuSelect(u16 index)
{
    Country* country = Game_GetCurrentCountry();

    if (menuIndex == MENU_UNASSIGNED)
    {
        if (index == 0) // Create troop
        {
            if (country->population == 0) return false;
            menuIndex = MENU_CREATE_TROOP;
        }
        else if (index == 1) // Upgrade troop
        {
            bool haveAnyTroops = HaveAnyUpgradableAllies(country->troops);
            if (!haveAnyTroops) return false;

            menuIndex = MENU_UPGRADE_TROOP;
        }
        else if (index == 2) // Move troop
        {
            bool haveAnyTroops = HaveAnyAllies(country->troops);
            if (!haveAnyTroops) return false;

            menuIndex = MENU_MOVE_TROOP;
            moveDestination = -1;
        }
        else if (index == 3) // Pass turn
        {
            Game_PassTurn();
            return false;
        }
    }
    else if (menuIndex == MENU_CREATE_TROOP)
    {
        if (index != 3)
        {
            TroopType type;
            if (index == 0) type = BOWMAN;
            else if (index == 1) type = SWORDMAN;
            else if (index == 2) type = SPEARMAN;

            Country_NewTroop(country, type, MY_TEAM);

            --country->population;
            Game_UpdateCountryLabel();

            Game_StartTurn();
        }

        menuIndex = MENU_UNASSIGNED;
    }
    else if (menuIndex == MENU_UPGRADE_TROOP)
    {
        Troop* it = country->troops;

        int i = 0;
        while (it != NULL)
        {
            if (it->team == MY_TEAM && CanBeUpgrade(it))
            {
                if (i == index)
                {
                    ++it->level;
                    --country->population;
                    Game_UpdateCountryLabel();
                    Game_StartTurn();
                    break;
                }
            }
            it = it->next;
            i++;
        }

        menuIndex = MENU_UNASSIGNED;
    }
    else if (menuIndex == MENU_MOVE_TROOP)
    {
        if (moveDestination == -1)
        {
            u16* it = country->nearbyCountries;

            int i = 0;
            while (*it != -1)
            {
                if (i == index)
                {
                    moveDestination = *it;
                    break;
                }
                i++;
                it++;
            }
        }
        else
        {
            Troop* lastIt = NULL;
            Troop* it = country->troops;

            int i = 0;
            while (it != NULL && i < MENU_MAX_CHOICE_COUNT)
            {
                if (it->team == MY_TEAM && CanBeUpgrade(it))
                {
                    if (i == index)
                    {
                        Country* dest = &countries[moveDestination];

                        // Remove troop from current country...
                        Country_RemoveExisting(country, lastIt, it);

                        // ...and add it to the destination one
                        Country_AddExisting(dest, it);
                        Game_StartTurn();

                        return true;
                    }
                }
                lastIt = it;
                it = it->next;
                i++;
            }

            menuIndex = MENU_UNASSIGNED;
        }
    }

    return true;
}

Phase* FreeRoam_GetPhase()
{
    return &phase;
}