#include <snes.h>

#include "world.h"

int main(void)
{
    u16 scrX = 0, scrY = 0;
    u16 pad0, move;

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

    // Wait for nothing :P
    while (1)
    {
        // no move currently
        move = 0;

        // Get current #0 pad
        pad0 = padsCurrent(0);

        // Update scrolling with current pad
        switch (pad0)
        {
        case KEY_RIGHT:
            scrX += 1;
            move = 1;
            break;
        case KEY_LEFT:
            scrX -= 1;
            move = 1;
            break;
        case KEY_DOWN:
            scrY += 1;
            move = 1;
            break;
        case KEY_UP:
            scrY -= 1;
            move = 1;
            break;
        }

        if (scrX > 3) scrX = 0;
        if (scrY > 3) scrY = 0;
        if (scrX < 0) scrY = 2;
        if (scrY < 0) scrY = 2;

        if (move)
            World_SetCamera(scrX, scrY);

        consoleDrawText(0, 0, "X=%d Y=%d", scrX, scrY);

        WaitForVBlank();
    }
    return 0;
}