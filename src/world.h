#ifndef WORLD_H
#define WORLD_H

#include <snes.h>

extern u16 worldX, worldY;

void World_Init(void);
void World_SetCamera(u16 tileX, u16 tileY);

#endif