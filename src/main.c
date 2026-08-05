#include <snes.h>

#include "world.h"
#include "country.h"

#define DEBUG 1

#define SCROLL_SPEED 5

int lerp_toward(int x, int dest)
{
    if (x < dest)
    {
        if (x + SCROLL_SPEED > dest) return dest;
        return x + SCROLL_SPEED;
    }
    if (x > dest)
    {
        if (x - SCROLL_SPEED < dest) return dest;
        return x - SCROLL_SPEED;
    }
    return x;
}

int main(void)
{
    u16 scrX = 256, scrY = 0;
    u16 targetX = 0, targetY = 0;

    u16 pad0, move;

    u16 countrySelectionIndex = 0;

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

    Country curr = countries[countrySelectionIndex];
    targetX = curr.xPos;
    targetY = curr.yPos;

    // Wait for nothing :P
    while (1)
    {
        // no move currently
        move = 0;

        if (scrX != targetX) {
            scrX = lerp_toward(scrX, targetX);
            move = 1;
        }
        if (scrY != targetY) {
            scrY = lerp_toward(scrY, targetY);
            move = 1;
        }

        // Get current #0 pad
        pad0 = padsDown(0);

        // Country selection
        switch (pad0)
        {
        case KEY_DOWN:
            if (countrySelectionIndex == 0) countrySelectionIndex = COUNTRY_COUNT - 1;
            else --countrySelectionIndex;

            curr = countries[countrySelectionIndex];
            targetX = curr.xPos;
            targetY = curr.yPos;
            move = 1;
            break;
        case KEY_UP:
            if (countrySelectionIndex == COUNTRY_COUNT - 1) countrySelectionIndex = 0;
            else ++countrySelectionIndex;

            curr = countries[countrySelectionIndex];
            targetX = curr.xPos;
            targetY = curr.yPos;
            move = 1;
            break;
        }

        if (move)
            World_SetCamera(scrX, scrY);

#if DEBUG == 1
        consoleDrawText(0, 0, "X=%d Y=%d    ", scrX, scrY);
#endif

        WaitForVBlank();
    }
    return 0;
}