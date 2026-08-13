#include "game.h"

#include <snes.h>

#include <stdio.h> // snprintf

#include "world.h"
#include "menu.h"
#include "freeroam.h"
#include "phase.h"


#define MODE_FREEROAM 0
#define MODE_COMBAT_REPORT 1

u16 currentMode = MODE_FREEROAM;

#define SCROLL_SPEED 5

s16 countryIndex = 0;

u16 turn = 0;
bool isTurnStarted = false;

Phase* phase;

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
    if (countryIndex != 0 && country->team == MY_TEAM)
    {
        consoleDrawText(0, 0, "%s (%d) [CONQUIERED]           ", country->name, country->population);
    }
    else
    {
        consoleDrawText(0, 0, "%s (%d)                       ", country->name, country->population);
    }
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

    phase->reachCountry();
}

Country *Game_GetCurrentCountry(void)
{
    return &countries[countryIndex];
}

void Game_Init()
{
    phase = FreeRoam_GetPhase();
    LerpTowardsCountry(countryIndex);
}

void Game_Update(void)
{
    phase->update(isTurnStarted);

    WaitForVBlank();
}

void Game_PassTurn(void)
{
    ++turn;
    isTurnStarted = false;
    UpdateCountryLabel(Game_GetCurrentCountry());
}

void Game_StartTurn(void)
{
    isTurnStarted = true;
}

void Game_UpdateCurrentCountry(u16 index)
{
    countryIndex = index;
    LerpTowardsCountry(countryIndex);
}

u16 Game_GetCountryIndex(void)
{
    return countryIndex;
}