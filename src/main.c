#include <string.h>
#include <stdbool.h>

#include <gint/display.h>
#include <gint/keyboard.h>

#include "ivk/instruction.h"
#include "ivk/manual.h"
#include "ivk/utils.h"

//---
// Public
//---

int main(void)
{
    int idx_max;
    int idx;
    int y;

    idx_max = -1;
    while (table[++idx_max].trampoline != NULL) {
        ;
    }

    idx = 0;
    while (true)
    {
        y = 0;
        dclear(C_WHITE);
        _("i=%d", idx);
        for (int i = idx ; i < 15 ; i++) {
            if (table[i].trampoline == NULL) break;
            _("%s", table[i].opname);
        }
        dupdate();
        switch (getkey().key)
        {
            case KEY_LEFT:
                if (idx < idx_max) idx += 1;
                break;
            case KEY_RIGHT:
                if (idx > 0) idx -= 1;
                break;
            case KEY_EXE:
                manual_gui_entry(&table[idx]);
                break;
        }
    }
    return 1;
}
