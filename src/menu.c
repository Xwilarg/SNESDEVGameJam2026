#include "menu.h"
#include <snes.h>

s16 Wrap(s16 x, s16 min, s16 max);

void Menu_Draw(Menu* menu)
{
    u16 numItems = menu->getNumItems();
    if (numItems > MENU_MAX_CHOICE_COUNT) numItems = MENU_MAX_CHOICE_COUNT - 1;

    u16 i;
    consoleDrawText(0, 4, "%s                    ", menu->getTitle());
    for (i = 0; i < numItems; i++)
    {
        char* label = menu->getLabel(i);
        consoleDrawText(0, 5 + 2 * (i + 1), "%s%s                    ", menu->index == i ? "x" : " ", label);
    }
    for (i = numItems; i < MENU_MAX_CHOICE_COUNT; i++)
    {
        consoleDrawText(0, 5 + 2 * (i + 1), "                              ");
    }
}

void Menu_Clear(void)
{
    consoleDrawText(0, 4, "                              ");
    u16 i;
    for (i = 0; i < MENU_MAX_CHOICE_COUNT; i++)
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
    case KEY_B:
        if (menu->select(menu->index))
        {
            menu->index = 0;
            Menu_Draw(menu);
        }
        break;

    case KEY_A:
        menu->index = 0;
        menu->reset();
        Menu_Draw(menu);
        break;
    }
}