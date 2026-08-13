#include <snes.h>

#include "game.h"
#include "world.h"
#include "country.h"

int main(void)
{
    // Initialize text console with our font
    // Default Map is 0x6800, Gfx is 0x3000 and offset is 0
    consoleInitDefaultText(0);

    World_Init();

    // Init background
    bgSetGfxPtr(0, 0x3000);
    bgSetMapPtr(0, 0x6800, SC_32x32);

    // Now Put in 16 color mode and disable Bgs except current
    setMode(BG_MODE1, 0);
    bgSetDisable(2);

    // Wait for nothing :P
    setScreenOn();

    Game_Init();

    while (1)
    {
        Game_Update();
    }

    return 0;
}