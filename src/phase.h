#ifndef PHASE_H
#define PHASE_H

#include "snes.h"

typedef struct
{
    void (*update)(bool isTurnStarted);
    void (*reachCountry)(void);
} Phase;

#endif