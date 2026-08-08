#include "troop.h"

#include <stdlib.h>

#define MAX_UPGRADE_LEVEL 3

Troop* Troop_New(TroopType type, int team)
{
    Troop* troop = (Troop*)malloc(sizeof(Troop));

    troop->next = NULL;
    troop->level = 1;
    troop->type = type;
    troop->team = team;

    return troop;
}

bool CanBeUpgrade(Troop* t)
{
    return t->level < MAX_UPGRADE_LEVEL;
}

bool HaveAnyAllies(Troop* list)
{
    Troop* it = list;
    while (it != NULL)
    {
        if (it->team == MY_TEAM)
        {
            return true;
        }
        it = it->next;
    }

    return false;
}

bool HaveAnyUpgradableAllies(Troop* list)
{
    Troop* it = list;
    while (it != NULL)
    {
        if (it->team == MY_TEAM && it->level < MAX_UPGRADE_LEVEL)
        {
            return true;
        }
        it = it->next;
    }

    return false;
}

char* TroopTypeToString(TroopType type)
{
    if (type == BOWMAN) return "Bowman";
    if (type == SWORDMAN) return "Swordman";
    if (type == SPEARMAN) return "Spearman";
}