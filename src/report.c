#include "report.h"

#include <stdlib.h>

#include "country.h"
#include "game.h"

static u16 pad0;
static u16 checkIndex;

// Current troop attacking
static s16 troopIndex;

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

static u16 GetTroopRoll(Troop* t, s16 fightingForce)
{
    u16 nb = 0;
    u16 i = 0;
    while (i < t->level + fightingForce)
    {
        nb += 1 + (rand() % 4);
        ++i;
    }
    return nb;
}

static s16 GetForceMultiplier(TroopType me, TroopType target)
{
    if (me == SWORDMAN)
    {
        if (target == HORSERIDER) return -1;
        if (target == SPEARMAN) return 1;
        if (target == BOWMAN) return 1;
    }
    else if (me == SPEARMAN)
    {
        if (target == SWORDMAN) return -1;
        if (target == HORSERIDER) return 1;
        if (target == BOWMAN) return 1;
    }
    else if (me == HORSERIDER)
    {
        if (target == SPEARMAN) return -1;
        if (target == SWORDMAN) return 1;
        if (target == BOWMAN) return 1;
    }
    return 0;
}

static bool LookForTargetAndFight(Country* country, Troop* me)
{
    Troop* backlineCandidate = NULL;
    Troop* frontlineCandidate = NULL;
    s16 backlineForce = 0;
    s16 frontlineForce = 0;

    Troop* it = country->troops;
    Troop* lastIt = NULL;
    while (it != NULL)
    {
        if (it->team != me->team) // it is of different team
        {
            if (backlineCandidate == NULL && it->type == BOWMAN)
            {
                backlineCandidate = it;
                backlineForce = GetForceMultiplier(me->type, it->type);
            }
            else if (it->type != BOWMAN)
            {
                s16 currForce = GetForceMultiplier(me->type, it->type);
                if (frontlineCandidate == NULL || currForce > frontlineForce)
                {
                    frontlineCandidate = it;
                    frontlineForce = currForce;
                }
            }
        }

        lastIt = it;
        it = it->next;
    }

    Troop* fightingCandidate = frontlineCandidate == NULL ? backlineCandidate : frontlineCandidate;

    if (fightingCandidate == NULL) return false;

    s16 fightingForce = frontlineCandidate == NULL ? backlineForce : frontlineForce;
    
    u16 attack = GetTroopRoll(me, fightingForce);
    u16 defense = GetTroopRoll(fightingCandidate, -fightingForce);

    char* attackStr = Troop_ToShortString(me);
    char* defenseStr = Troop_ToShortString(fightingCandidate);
    consoleDrawText(0, yWriteIndex, "[%s] %s (%d) vs [%s] %s (%d)", me->team == MY_TEAM ? "YOU" : "ENN", attackStr, attack, fightingCandidate->team == MY_TEAM ? "YOU" : "ENN", defenseStr, defense);
    ++yWriteIndex;

    if (attack >= defense)
    {
        it = country->troops;
        while (it != NULL)
        {
            if (it->next == fightingCandidate) break;

            it = it->next;
        }

        Country_RemoveExisting(country, it, fightingCandidate);
        consoleDrawText(0, yWriteIndex, "[%s] %s is dead", fightingCandidate->team == MY_TEAM ? "YOU" : "ENN", defenseStr);
        free(fightingCandidate);
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
                    troopIndex = -1;
                    it = NULL; // Nobody else to fight
                }
                else
                {
                    ++troopIndex;
                }
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
            troopIndex = -1;
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
            s16 winningTeam = GetWinningTeam();
            if (winningTeam == -1)
            {
                currBattleRound = 1;
                subPhase = REPORT_PHASE_BATTLE;
                troopIndex = 0;
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
            if (troopIndex != -1)
            {
                ShowBattleRound();
            }
            else
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