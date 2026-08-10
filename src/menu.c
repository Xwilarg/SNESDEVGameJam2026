#include "menu.h"
#include <snes.h>

s16 Wrap(s16 x, s16 min, s16 max);

void Menu_Draw(Menu* menu)
{
    u16 numItems = menu->getNumItems();

    u16 i;
    consoleDrawText(0, 4, "%s                    ", menu->getTitle());
    for (i = 0; i < numItems; i++)
    {
        consoleDrawText(0, 5 + 2 * (i + 1), "%s%s                    ", menu->index == i ? "x" : " ", menu->getLabel(i));
        // DEBUG
        // consoleDrawText(0, 5 + 2 * (i + 1), "%p                    ", menu->getLabel(i));
    }
    for (i = numItems; i < 5; i++)
    {
        consoleDrawText(0, 5 + 2 * (i + 1), "                              ");
    }
}

void Menu_Input(Menu* menu, u16 pad0)
{
    switch (pad0)
    {
    case KEY_UP:
        menu->index = Wrap(menu->index - 1, 0, menu->getNumItems() - 1);
        Menu_Draw(menu);
        break;
    case KEY_DOWN:
        menu->index = Wrap(menu->index + 1, 0, menu->getNumItems() - 1);
        Menu_Draw(menu);
        break;
    case KEY_X:
        if (menu->select(menu->index))
        {
            menu->index = 0;
            Menu_Draw(menu);
        }
        break;
    }
}