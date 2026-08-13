#ifndef PHASE_H
#define PHASE_H

#include "snes.h"

typedef struct
{
    void (*init)(void);
    void (*cleanup)(void);
    void (*reachCountry)(void);
    void (*update)(bool isTurnStarted);
} Phase;

#endif