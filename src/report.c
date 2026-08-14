#include "report.h"
#include "country.h"
#include "game.h"

static u16 pad0;
static u16 checkIndex;

static bool CheckToNextCountry()
{
    while (checkIndex < COUNTRY_COUNT)
    {
        if (Country_HaveConflictPending(&countries[checkIndex]))
        {
            Game_UpdateCurrentCountry(checkIndex);
            return true;
        }

        ++checkIndex;
    }
    return false;
}

static void Init(void)
{
    checkIndex = 0;
    if (!CheckToNextCountry())
    {
        Game_SwitchToModeFreeRoam();
    }
}

static void Cleanup(void)
{
}

static void ReachCountry(void)
{
}

static void Update(bool isTurnStarted)
{
    pad0 = padsDown(0);

    switch (pad0)
    {
    }
}

static Phase phase = {
    &Init,
    &Cleanup,
    &ReachCountry,
    &Update
};

Phase* Report_GetPhase()
{
    return &phase;
}