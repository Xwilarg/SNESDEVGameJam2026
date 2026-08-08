#include "game.h"

#include <snes.h>

#include "world.h"
#include "country.h"
#include "menu.h"

#define SCROLL_SPEED 5

#define MENU_UNASSIGNED 0
#define MENU_CREATE_TROOP 1
#define MENU_UPGRADE_TROOP 2

u16 pad0;

s16 countryIndex = 0;
u16 menuIndex = 0;

u16 GetMenuItemCount(void);
char* OnMenuTitle(void);
char* OnMenuLabel(u16 index);
bool OnMenuSelect(u16 index);

Menu menu = {
    0,
    &GetMenuItemCount,
    &OnMenuTitle,
    &OnMenuLabel,
    &OnMenuSelect
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
        if (menuIndex == MENU_UNASSIGNED) return 3;
        if (menuIndex == MENU_CREATE_TROOP) return 4;
        if (menuIndex == MENU_UPGRADE_TROOP) 
        {
            Troop* it = countries->troops;

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

        return "UNKNOWN";
    }

    return "Diplomacy Options";
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
        }
        else if (menuIndex == MENU_CREATE_TROOP)
        {
            if (index == 0) return "Bowman";
            if (index == 1) return "Swordman";
            if (index == 2) return "Spearman";
            if (index == 3) return "Go back";
        }
        else if (menuIndex == MENU_UPGRADE_TROOP)
        {
            Troop* it = countries->troops;

            int i = 0;
            while (i < index)
            {
                if (it->team == MY_TEAM && CanBeUpgrade(it))
                {
                    if (i == index) return "TROOP";
                    it = it->next;
                }

                if (it == NULL) break;
            }
            return "Back";
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
    }
    else if (menuIndex == MENU_CREATE_TROOP)
    {
        if (index != 3)
        {
            TroopType type;
            if (index == 0) type = BOWMAN;
            else if (index == 1) type = SWORDMAN;
            else if (index == 2) type = SPEARMAN;

            Troop* new = Troop_New(type, MY_TEAM);

            Troop* it = country->troops;
            if (it == NULL)
            {
                country->troops = new;
            }
            else
            {
                Troop* next = it->next;
                while (it->next != NULL)
                {
                    it = it->next;
                }
                it->next = new;
            }

            --country->population;
            UpdateCountryLabel(country);
        }

        menuIndex = MENU_UNASSIGNED;
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
        countryIndex = Wrap(countryIndex - 1, 0, COUNTRY_COUNT - 1);
        menuIndex = MENU_UNASSIGNED;
        LerpTowardsCountry(countryIndex);
        break;
    case KEY_RIGHT:
        countryIndex = Wrap(countryIndex + 1, 0, COUNTRY_COUNT - 1);
        menuIndex = MENU_UNASSIGNED;
        LerpTowardsCountry(countryIndex);
        break;

    case KEY_UP:
    case KEY_DOWN:
    case KEY_X:
        Menu_Input(&menu, pad0);
        break;
    }

    WaitForVBlank();
}