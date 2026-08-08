#include "game.h"

#include <snes.h>

#include "world.h"
#include "country.h"

#define SCROLL_SPEED 5

u16 pad0;

s16 countryIndex = 0;

s16 Lerp(s16 x, s16 dest)
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

s16 Wrap(s16 x, s16 min, s16 max)
{
    if (x < min) x = max;
    if (x > max) x = min;
    return x;
}

void LerpTowards(u16 targetX, u16 targetY)
{
    while (targetX != worldX || targetY != worldY)
    {
        World_SetCamera(Lerp(worldX, targetX), Lerp(worldY, targetY));
        WaitForVBlank();
    }
}

void LerpTowardsCountry(u16 index)
{
    Country* country = &countries[index];
    LerpTowards(country->xPos, country->yPos);

    consoleDrawText(0, 0, "%s", country->name);
}

void Game_Update()
{
    pad0 = padsDown(0);

    switch (pad0)
    {
    case KEY_LEFT:
        countryIndex = Wrap(countryIndex - 1, 0, COUNTRY_COUNT - 1);
        LerpTowardsCountry(countryIndex);
        break;
    case KEY_RIGHT:
        countryIndex = Wrap(countryIndex + 1, 0, COUNTRY_COUNT - 1);
        LerpTowardsCountry(countryIndex);
        break;
    }

    WaitForVBlank();
}