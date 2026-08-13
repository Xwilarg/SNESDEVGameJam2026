#ifndef MENU_H
#define MENU_H

#include <snes.h>

#define MENU_MAX_CHOICE_COUNT 6

typedef struct
{
    s16 index;
    u16 (*getNumItems)(void);

    char* (*getTitle)(void);
    char* (*getLabel)(u16 index);
    bool (*select)(u16 index);
    void (*reset)(void);
} Menu;

void Menu_Draw(Menu* menu);
void Menu_Clear(void);
void Menu_Input(Menu* menu, u16 pad0);

#endif