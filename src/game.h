#ifndef GAME_H
#define GAME_H

#include "country.h"

void Game_Init(void);
void Game_Update(void);
Country *Game_GetCurrentCountry(void);
void Game_PassTurn(void);
void Game_StartTurn(void);
void Game_UpdateCurrentCountry(u16 index);
u16 Game_GetCountryIndex(void);

#endif