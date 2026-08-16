#include "report.h"
#include "country.h"
#include "game.h"

static u16 pad0;
static u16 checkIndex;

#define REPORT_PHASE_PENDING 0
#define REPORT_PHASE_INTRO 1
#define REPORT_PHASE_VICTORY 2
#define REPORT_PHASE_BATTLE 3

static u16 subPhase;
static u16 currBattleRound;

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

static void AdvanceCountryCheck()
{
    if (!CheckToNextCountry())
    {
        Game_SwitchToModeFreeRoam();
    }
    else
    {
        subPhase = REPORT_PHASE_PENDING;
    }

}

static void Init(void)
{
    checkIndex = 0;
    AdvanceCountryCheck();
}

static void Cleanup(void)
{
    consoleDrawText(0, 4, "                                                   ");
    ClearScreen();
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

static void ClearScreen()
{
    u16 y;
    for (y = 6; y < 15; y++)
    {
        consoleDrawText(0, y, "                                                   ");
    }
}

static void StartBattle(void)
{
    ClearScreen();

    Country* country = Game_GetCurrentCountry();
    consoleDrawText(0, 4, "Battle for %s              ", country->name);

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
    subPhase = REPORT_PHASE_INTRO;
    StartBattle();
}

static void ShowVictoryScreen(u16 winningTeam)
{
    subPhase = REPORT_PHASE_VICTORY;
    ClearScreen();

    if (winningTeam)
    {
        consoleDrawText(0, 6, "You captured the city");
    }
    else
    {
        consoleDrawText(0, 6, "Another team captured the city");
    }
}

static void ShowBattleRound()
{
    ClearScreen();
    consoleDrawText(0, 6, "Round %d", currBattleRound);
}

static void Update(bool isTurnStarted)
{
    pad0 = padsDown(0);

    switch (pad0)
    {
    case KEY_A:
    case KEY_B:
        if (subPhase == REPORT_PHASE_INTRO)
        {
            currBattleRound = 0;
            s16 winningTeam = GetWinningTeam();
            if (winningTeam == -1)
            {
                currBattleRound = 0;
                subPhase = REPORT_PHASE_BATTLE;
                ShowBattleRound();
            }
            else
            {
                ShowVictoryScreen((u16)winningTeam);
            }
        }
        else if (subPhase == REPORT_PHASE_VICTORY)
        {
            AdvanceCountryCheck();
        }
        else if (subPhase == REPORT_PHASE_BATTLE)
        {
            ++currBattleRound;
            s16 winningTeam = GetWinningTeam();
            if (winningTeam == -1)
            {
                ShowBattleRound();
            }
            else
            {
                ShowVictoryScreen(winningTeam);
            }
        }

        break;
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