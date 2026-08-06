#include "troop.h"

#include <stdlib.h>

Troop* Troop_New(void)
{
    Troop* troop = (Troop*)malloc(sizeof(Troop));

    troop->next = NULL;
    troop->health = 0;
    troop->strength = 0;

    return troop;
}