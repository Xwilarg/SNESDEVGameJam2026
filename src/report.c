#include "report.h"

static u16 pad0;

static void Init(void)
{
}

static void Cleanup(void)
{
}

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
    &Init,
    &Cleanup,
    &ReachCountry,
    &Update
};

Phase* Report_GetPhase()
{
    return &phase;
}