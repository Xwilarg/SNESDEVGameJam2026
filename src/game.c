#include "game.h"

#include <snes.h>

#include "world.h"
#include "menu.h"
#include "freeroam.h"
#include "report.h"

#define SCROLL_SPEED 5

static s16 countryIndex = 0;

static u16 turn = 0;
static bool isTurnStarted = false;

static Phase* currPhase;

static s16 Lerp(s16 x, s16 dest)
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

void Game_UpdateCountryLabel()
{
    Country* country = Game_GetCurrentCountry();

    if (countryIndex != 0 && country->team == MY_TEAM)
    {
        consoleDrawText(0, 0, "%s (%d) [CONQUERED]           ", country->name, country->population);
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

    Game_UpdateCountryLabel(country);

    currPhase->reachCountry();
}

Country *Game_GetCurrentCountry(void)
{
    return &countries[countryIndex];
}

void Game_SwitchToModeFreeRoam(void)
{
    currPhase = FreeRoam_GetPhase();
    currPhase->init();
}

void Game_SwitchToModeReport(void)
{
    currPhase = Report_GetPhase();
    currPhase->init();
}

void Game_Init()
{
    Game_SwitchToModeFreeRoam();
    Game_UpdateCurrentCountry(0);
}

void Game_Update(void)
{
    currPhase->update(isTurnStarted);

    WaitForVBlank();
}

void Game_PassTurn(void)
{
    ++turn;
    isTurnStarted = false;
    Game_UpdateCountryLabel(Game_GetCurrentCountry());

    Game_SwitchToModeReport();
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