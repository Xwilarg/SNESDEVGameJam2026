#include "game.h"

#include <snes.h>

#include <stdio.h> // snprintf

#include "world.h"
#include "country.h"
#include "menu.h"

#define SCROLL_SPEED 5

#define MENU_UNASSIGNED 0
#define MENU_CREATE_TROOP 1
#define MENU_UPGRADE_TROOP 2
#define MENU_MOVE_TROOP 3

u16 pad0;

s16 countryIndex = 0;
u16 menuIndex = 0;

// Moving troops
s16 moveDestination = -1;

u16 GetMenuItemCount(void);
char* OnMenuTitle(void);
char* OnMenuLabel(u16 index);
bool OnMenuSelect(u16 index);
void OnMenuReset(void);

char buffer[30];

u16 turn = 0;
bool is_turn_started = false;

Menu menu = {
    0,
    &GetMenuItemCount,
    &OnMenuTitle,
    &OnMenuLabel,
    &OnMenuSelect,
    &OnMenuReset
};

s16 Lerp(s16 x, s16 dest)
{
    if (x < dest)
    {
        if (x + SCROLL_SPEED > dest) return dest;
        return x + SCROLL_SPEED;
    }
    if (x > dest)
    {
        if (x - SCROLL_SPEED < dest) return dest;
        return x - SCROLL_SPEED;
    }
    return x;
}

s16 Wrap(s16 x, s16 min, s16 max)
{
    if (x < min) x = max;
    if (x > max) x = min;
    return x;
}

void UpdateCountryLabel(Country* country)
{
    consoleDrawText(0, 0, "%s (%d)                   ", country->name, country->population);
    consoleDrawText(22, 27, "Turn: %d   ", turn);

    // DEBUG
    // consoleDrawText(0, 0, "%s (%d) (ID %d)               ", country->name, country->population, countryIndex);
}

void LerpTowards(u16 targetX, u16 targetY)
{
    while (targetX != worldX || targetY != worldY)
    {
        World_SetCamera(Lerp(worldX, targetX), Lerp(worldY, targetY));
        WaitForVBlank();
    }
}

void LerpTowardsCountry(u16 index)
{
    Country* country = &countries[index];
    LerpTowards(country->xPos, country->yPos);

    UpdateCountryLabel(country);
    
    Menu_Draw(&menu);
}

u16 GetMenuItemCount()
{
    Country* country = &countries[countryIndex];

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
    Country* country = &countries[countryIndex];

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
    Country* country = &countries[countryIndex];

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
    Country* country = &countries[countryIndex];

    if (country->team == MY_TEAM)
    {
        if (menuIndex == MENU_UNASSIGNED)
        {
            if (index == 0) return country->population > 0 ? "New Troop" : "No population to make troops";
            if (index == 1) return HaveAnyUpgradableAllies(countries->troops) ? "Train Troop" : "No troop to train";
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
                        snprintf(buffer, sizeof(buffer), "%s (%d)", TroopTypeToString(it->type), it->level);
                        return buffer;
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
                            snprintf(buffer, sizeof(buffer), "%s (%d)", TroopTypeToString(it->type), it->level);
                            return buffer;
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
    Country* country = &countries[countryIndex];

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
            ++turn;
            is_turn_started = false;
            UpdateCountryLabel(country);
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
            UpdateCountryLabel(country);

            is_turn_started = true;
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
                    UpdateCountryLabel(country);
                    is_turn_started = true;
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
                        if (lastIt == NULL)
                        {
                            country->troops = it->next;
                        }
                        else
                        {
                            lastIt->next = it->next;
                        }

                        // ...and add it to the destination one
                        Country_AddExisting(dest, it);
                        is_turn_started = true;

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

void Game_Init(void)
{
    LerpTowardsCountry(countryIndex);
}

void Game_Update(void)
{
    pad0 = padsDown(0);

    switch (pad0)
    {
    case KEY_LEFT:
        if (!is_turn_started)
        {
            countryIndex = Wrap(countryIndex - 1, 0, COUNTRY_COUNT - 1);
            menuIndex = MENU_UNASSIGNED;
            LerpTowardsCountry(countryIndex);
        }
        break;
    case KEY_RIGHT:
        if (!is_turn_started)
        {
            countryIndex = Wrap(countryIndex + 1, 0, COUNTRY_COUNT - 1);
            menuIndex = MENU_UNASSIGNED;
            LerpTowardsCountry(countryIndex);
        }
        break;

    default:
        Menu_Input(&menu, pad0);
        break;
    }

    WaitForVBlank();
}