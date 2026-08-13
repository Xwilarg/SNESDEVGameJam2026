#pragma once

#include "snes.h"

typedef struct
{
    void (*update)(bool isTurnStarted);
    void (*reachCountry)(void);
} Phase;