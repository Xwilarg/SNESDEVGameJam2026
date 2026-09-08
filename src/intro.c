#include "intro.h"
#include "game.h"

static u16 pad0;

static void ClearScreen()
{
    u16 y;
    for (y = 0; y < 27; y++)
    {
        consoleDrawText(0, y, "                                                   ");
    }
}

static void Init(void)
{
    ClearScreen();
    consoleDrawText(0, 6, "    Welcome to Uncivilized");
    consoleDrawText(0, 8, "    Press A to play");
}

static void Cleanup(void)
{
    ClearScreen();
}

static void ReachCountry(void)
{
}

static void Update(bool isTurnStarted)
{
    rand();
    pad0 = padsDown(0);

    switch (pad0)
    {
    case KEY_A:
    case KEY_B:
        Game_SwitchToModeFreeRoam();
        break;
    }
}

static Phase phase = {
    &Init,
    &Cleanup,
    &ReachCountry,
    &Update
};

Phase* Intro_GetPhase()
{
    return &phase;
}