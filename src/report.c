#include "report.h"

static u16 pad0;

static void ReachCountry(void)
{
}

static void Update(bool isTurnStarted)
{
    pad0 = padsDown(0);

    switch (pad0)
    {
    }
}

static Phase phase = {
    &ReachCountry,
    &Update
};

Phase* Report_GetPhase()
{
    return &phase;
}