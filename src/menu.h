#pragma once

#include <snes.h>

typedef struct Menu
{
    s16 index;
    u16 (*getNumItems)(void);

    char* (*getTitle)(void);
    char* (*getLabel)(u16 index);
    void (*select)(u16 index);
} Menu;

void Menu_Draw(Menu* menu);
void Menu_Input(Menu* menu, u16 pad0);