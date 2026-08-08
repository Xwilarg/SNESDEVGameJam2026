#include "troop.h"

#include <stdlib.h>

Troop* Troop_New(TroopType type, int team)
{
    Troop* troop = (Troop*)malloc(sizeof(Troop));

    troop->next = NULL;
    troop->size = 1;
    troop->type = type;
    troop->team = team;

    return troop;
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