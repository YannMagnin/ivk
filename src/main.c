#include <string.h>
#include <stdbool.h>

#include <gint/display.h>
#include <gint/keyboard.h>
#include <gint/gint.h>

#include "ivk/menu.h"

//---
// Public
//---

int main(void)
{
    IvkMenuInst menu_inst;
    IvkMenuDisco menu_disco;
    key_event_t keyev;
    void (*menu_display)(void*);
    void (*menu_keyboard)(void*,key_event_t);
    void *menu_data;

    menu_display  = (void*)&menu_instruction_display;
    menu_keyboard = (void*)&menu_instruction_key;
    menu_data     = (void*)&menu_inst;

    menu_disco_init(&menu_disco);
    menu_instruction_init(&menu_inst);
    while (true)
    {
        menu_display(menu_data);

        while((keyev = pollevent()).type != KEYEV_NONE)
        {
            if (keyev.type == KEYEV_UP)
                continue;
            if (keyev.type == KEYEV_DOWN) {
                switch(keyev.key) {
                    case KEY_MENU:
                        gint_osmenu();
                        continue;
                    case KEY_F1:
                        menu_display  = (void*)&menu_instruction_display;
                        menu_keyboard = (void*)&menu_instruction_key;
                        menu_data     = (void*)&menu_inst;
                        continue;
                    case KEY_F6:
                        menu_display  = (void*)&menu_disco_display;
                        menu_keyboard = (void*)&menu_disco_key;
                        menu_data     = (void*)&menu_disco;
                        continue;
                }
            }
            menu_keyboard(menu_data, keyev);
        }
    }
    return 1;
}
