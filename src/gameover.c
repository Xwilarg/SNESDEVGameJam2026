#include "gameover.h"

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
    consoleDrawText(0, 6, "Your capital fell to the hand of your enemies");
    consoleDrawText(0, 9, "You lost");
}

static void Cleanup(void)
{
}

static void ReachCountry(void)
{
}

static void Update(bool isTurnStarted)
{
}

static Phase phase = {
    &Init,
    &Cleanup,
    &ReachCountry,
    &Update
};

Phase* GameOver_GetPhase()
{
    return &phase;
}