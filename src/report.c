#include "report.h"
#include "country.h"
#include "game.h"

static u16 pad0;
static u16 checkIndex;

#define REPORT_PHASE_INTRO 0

static u16 subPhase;

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
    else
    {
        subPhase = REPORT_PHASE_INTRO;
    }
}

static void Cleanup(void)
{
}

static u16 GetPlayerTroopCount(Country* country)
{
    u16 count = 0;

    Troop* it = country->troops;
    while (it != NULL)
    {
        if (it->team == MY_TEAM)
        {
            ++count;
        }
        it = it->next;
    }
    return count;
}

static u16 GetNonPlayerTroopCount(Country* country)
{
    u16 count = 0;

    Troop* it = country->troops;
    while (it != NULL)
    {
        if (it->team != MY_TEAM)
        {
            ++count;
        }
        it = it->next;
    }
    return count;
}

static s16 GetWinningTeam()
{
    Country* country = Game_GetCurrentCountry();

    if (country->troops == NULL) return country->team;

    Troop* it = country->troops;
    u16 ref = it->team;

    while (it != NULL)
    {
        if (it->team != ref) return -1;

        it = it->next;
    }

    return (s16)ref;
}

static void StartBattle(void)
{
    Country* country = Game_GetCurrentCountry();
    consoleDrawText(0, 4, "Battle in %s", country->name);

    u16 playerTroopCount = GetPlayerTroopCount(country);
    u16 otherTroopCount = GetNonPlayerTroopCount(country);
    if (playerTroopCount == 0)
    {
        consoleDrawText(0, 6, "Your are not in this battle");
        consoleDrawText(0, 7, "Others troops: %d", otherTroopCount);
    }
    else
    {
        consoleDrawText(0, 6, "Your troops: %d", playerTroopCount);
        consoleDrawText(0, 7, "Enemies troops: %d", otherTroopCount);
    } 
    
    consoleDrawText(0, 20, "Press A to continue");
}

static void ReachCountry(void)
{
    StartBattle();
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