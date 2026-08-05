#include "world.h"

extern char patterns, patterns_end;
extern char palette;
extern char map, map_end;

void World_Init(void)
{
    // Copy tiles to VRAM
    bgInitTileSet(1, &patterns, &palette, 0, (&patterns_end - &patterns), 16 * 2, BG_16COLORS, 0x4000);

    // Copy Map to VRAM
    bgInitMapSet(1, &map, (&map_end - &map), SC_64x64, 0x1000);
}

void World_SetCamera(u16 tileX, u16 tileY)
{
    bgSetScroll(1, tileX * 160, tileY * 160);
}