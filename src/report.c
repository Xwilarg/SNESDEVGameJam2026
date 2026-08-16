#include "report.h"

#include <stdlib.h>

#include "country.h"
#include "game.h"

static u16 pad0;
static u16 checkIndex;

// Used for battle report, to know what line to write on
static u16 yWriteIndex;

#define REPORT_PHASE_INTRO 1
#define REPORT_PHASE_VICTORY 2
#define REPORT_PHASE_BATTLE 3

#define MAX_BATTLE_IN_ROUND 18

static u16 subPhase;
static u16 currBattleRound;

static void ClearScreen()
{
    u16 y;
    for (y = 6; y < 15; y++)
    {
        consoleDrawText(0, y, "                                                   ");
    }
}

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
    
    consoleDrawText(0, 26, "Press A to continue");
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

    if (winningTeam == MY_TEAM)
    {
        consoleDrawText(0, 6, "You captured the city");
    }
    else
    {
        consoleDrawText(0, 6, "Another team captured the city");
    }
}

static u16 GetTroopRoll(Troop* t)
{
    return rand() % (2 + (t->level * 2));
}

static bool LookForTargetAndFight(Country* country, Troop* me)
{
    Troop* fightingCandidate = NULL;

    Troop* it = country->troops;
    Troop* lastIt = NULL;
    while (it != NULL)
    {
        if (it->team != me->team) // it is of different team
        {
            if (fightingCandidate == NULL)
            {
                fightingCandidate = it;
                break;
            }
            // TODO: Determine target
        }

        lastIt = it;
        it = it->next;
    }

    if (fightingCandidate == NULL) return false;
    
    u16 attack = GetTroopRoll(me);
    u16 defense = GetTroopRoll(fightingCandidate);

    char* attackStr = Troop_ToShortString(me);
    char* defenseStr = Troop_ToShortString(fightingCandidate);
    consoleDrawText(0, yWriteIndex, "[%s] %s (%d) vs [%s] %s (%d)", me->team == MY_TEAM ? "YOU" : "ENN", attackStr, attack, fightingCandidate->team == MY_TEAM ? "YOU" : "ENN", defenseStr, defense);
    ++yWriteIndex;

    if (attack >= defense)
    {
        Country_RemoveExisting(country, lastIt, fightingCandidate);
        free(fightingCandidate);
        consoleDrawText(0, yWriteIndex, "[%s] %s is dead", fightingCandidate->team == MY_TEAM ? "YOU" : "ENN", defenseStr);
        ++yWriteIndex;
    }

    return true;
}

static void ShowBattleRound()
{
    ClearScreen();
    consoleDrawText(0, 6, "Round %d", currBattleRound);
    yWriteIndex = 8;

    Country* country = Game_GetCurrentCountry();
    
    int troopIndex = 0;

    while (true)
    {
        Troop* it = country->troops;
        int i = 0;
        while (it != NULL)
        {
            if (troopIndex == i)
            {
                if (!LookForTargetAndFight(country, it))
                {
                    it = NULL; // Nobody else to fight
                }
                ++troopIndex;
                break;
            }

            it = it->next;
            ++i;
        }
        

        if (yWriteIndex >= MAX_BATTLE_IN_ROUND + 6) // Can't have too many people fighting in a single round
        {
            break;
        }

        if (it == NULL) // Everyone attacked or there is nobody else that can fight (everyone is dead)
        {
            break;
        }
    }
}

static void Update(bool isTurnStarted)
{
    rand(); // Shake randomness

    pad0 = padsDown(0);

    switch (pad0)
    {
    case KEY_A:
    case KEY_B:
        if (subPhase == REPORT_PHASE_INTRO)
        {
            currBattleRound = 1;
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