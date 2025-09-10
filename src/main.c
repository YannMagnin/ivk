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
    key_event_t keyev;

    menu_instruction_init(&menu_inst);
    while (true)
    {
        menu_instruction_display(&menu_inst);

        while((keyev = pollevent()).type != KEYEV_NONE)
        {
            if (keyev.type == KEYEV_UP)
                continue;
            if (keyev.key == KEY_MENU) {
                if (keyev.type == KEYEV_DOWN)
                    gint_osmenu();
                continue;
            }
            menu_instruction_key(&menu_inst, keyev);
        }
    }
    return 1;
}
