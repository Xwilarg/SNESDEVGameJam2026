#include <snes.h>

#include "sound.h"

void Sound_Init(void)
{
    // Initialize sound engine (take some time)
    spcBoot();

    // Set soundbanks in reverse order
    spcSetBank(&SOUNDBANK__);
}

void Sound_Play(u16 musIndex)
{
    spcLoad(musIndex);
    spcPlay(0);
}