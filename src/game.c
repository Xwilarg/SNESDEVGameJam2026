#include "game.h"

#include <snes.h>

#include "world.h"
#include "country.h"
#include "menu.h"

#define SCROLL_SPEED 5

#define MENU_UNASSIGNED 0
#define MENU_CREATE_TROOP 1

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

    consoleDrawText(0, 0, "%s (%d)                   ", country->name, country->population);
    
    Menu_Draw(&menu);
}

u16 GetMenuItemCount()
{
    Country* country = &countries[countryIndex];

    if (country->team == MY_TEAM)
    {
        if (menuIndex == MENU_UNASSIGNED) return 3;
        if (menuIndex == MENU_CREATE_TROOP) return 4;
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
            bool haveAnyTroops = HaveAnyAllies(country->troops);

            if (index == 0) return country->population > 0 ? "New Troop" : "No population to make troops";
            if (index == 1) return haveAnyTroops ? "Train Troop" : "No troop to train";
            if (index == 2) return haveAnyTroops ? "Move Troop" : "No troop to move";
        }
        else if (menuIndex == MENU_CREATE_TROOP)
        {
            if (index == 0) return "Bowman";
            if (index == 1) return "Swordman";
            if (index == 2) return "Spearman";
            if (index == 3) return "Go back";
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
    }
    else if (menuIndex == MENU_CREATE_TROOP)
    {
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