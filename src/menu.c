#include "menu.h"

#include <snes.h>

s16 Wrap(s16 x, s16 min, s16 max);

void Menu_Draw(Menu* menu)
{
    u16 i;
    for (i = 0; i < menu->numItems; i++)
    {
        consoleDrawText(0, 4 * (i + 1), "%s%s", menu->index == i ? "x" : " ", menu->getLabel(i));
    }
}

void Menu_Input(Menu* menu, u16 pad0)
{
    switch (pad0)
    {
    case KEY_UP:
        menu->index = Wrap(menu->index - 1, 0, menu->numItems - 1);
        Menu_Draw(menu);
        break;
    case KEY_DOWN:
        menu->index = Wrap(menu->index + 1, 0, menu->numItems - 1);
        Menu_Draw(menu);
        break;
    case KEY_X:
        menu->select(menu->index);
        break;
    }
}