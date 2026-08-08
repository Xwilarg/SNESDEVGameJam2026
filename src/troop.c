#include "troop.h"

#include <stdlib.h>

Troop* Troop_New(TroopType type)
{
    Troop* troop = (Troop*)malloc(sizeof(Troop));

    troop->next = NULL;
    troop->size = 1;
    troop->type = type;

    return troop;
}