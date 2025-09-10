#include <string.h>

#include <gint/display.h>
#include <gint/keyboard.h>
#include <gint/gint.h>

#include "ivk/menu.h"
#include "ivk/utils.h"

//---
// disco information
//---

struct {
    char const *opcode;
    u16 x;
    u16 y;
    int invalid;
    int valid;
} _disco_table[] = {
    // 8xxx:
    //   83xx libre
    //   86xx libre
    //   87xx libre
    {"1000.0011.xxxx.xxxx", 0xffff, 0x0000, -1, -1},
    {"1000.0110.xxxx.xxxx", 0xffff, 0x0000, -1, -1},
    {"1000.0111.xxxx.xxxx", 0xffff, 0x0000, -1, -1},
    // 4xxx
    //   4xy0 libre (y = 3..15)
    //   4xy1 libre (y = 3..15)
    //   4xy4 libre (y = 4..15)
    //   4xy5 libre (y = 3..15)
    //   4xy8 libre (y = 3..15)
    //   4xy9 libre (y = 3..9, 11, 12, 13, 15)
    //   4xyb libre (y = 3..15)
    {"1000.xxxx.yyyy.0000", 0xffff, 0x1fff, -1, -1},
    {"1000.xxxx.yyyy.0001", 0xffff, 0x1fff, -1, -1},
    {"1000.xxxx.yyyy.0100", 0xffff, 0x0fff, -1, -1},
    {"1000.xxxx.yyyy.0101", 0xffff, 0x1fff, -1, -1},
    {"1000.xxxx.yyyy.1000", 0xffff, 0x1fff, -1, -1},
    {"1000.xxxx.yyyy.1001", 0xffff, 0x1fdd, -1, -1},
    {"1000.xxxx.yyyy.1011", 0xffff, 0x1fff, -1, -1},
    // 3xxx
    //  3xx1 libre
    //  3xx9 libre
    {"0011.xxxx.xxxx.0001", 0xffff, 0x0000, -1, -1},
    {"0011.xxxx.xxxx.1001", 0xffff, 0x0000, -1, -1},
    // 2xxx
    //  2xx3 libre
    {"0010.xxxx.xxxx.0011", 0xffff, 0x0000, -1, -1},
    // 0xxx
    //   0xx0 libre
    //   0xx1 libre
    //   0xy3 libre (y = 1, 3, 4, 5, 15)
    //   0zy8 libre (y = 6, 7, 10, 11, 13, 14, 15 || z != 0)
    //   0zy9 libre (y != 2 && (y = 3..15 || z != 0))
    //   0zyb libre (y = 3..9, 11..15 || z != 0)
    {"0000.xxxx.xxxx.0000", 0xffff, 0x0000, -1, -1},
    {"0000.xxxx.xxxx.0001", 0xffff, 0x0000, -1, -1},
    {"0000.xxxx.yyyy.0011", 0xffff, 0x5c01, -1, -1},
    {"0000.0000.xxxx.1000", 0xffff, 0x0337, -1, -1},
    {"0000.xxxx.yyyy.1000", 0x7fff, 0xffff, -1, -1},

    // end
    {NULL, 0xffff, 0xffff, -1, -1},
};


void menu_disco_exec(IvkMenuInst *menu, int idx)
{
    (void)menu;
    (void)idx;
}

int menu_disco_display_row(IvkMenuDisco *menu, int y, int idx)
{
    if (idx >= menu->nb_entry)
        return -1;
    __auto_type opcode = &_disco_table[idx];
    dtext(4, 10 + (y * 14) + 2, C_BLACK, opcode->opcode);
    if(opcode->invalid < 0 && opcode->valid < 0) {
        dtext_opt(
            DWIDTH - 9,
            10 + (y * 14) + 2,
            C_BLACK,
            C_NONE,
            DTEXT_RIGHT,
            DTEXT_TOP,
            "TEST"
        );
    }
    return 0;
}

//---
// menu interface
//---

void menu_disco_init(IvkMenuDisco *menu)
{
    int nb_entry;

    nb_entry = -1;
    while (true) {
        if(_disco_table[++nb_entry].opcode == NULL)
            break;
    }

    memset(menu, 0x00, sizeof(IvkMenuDisco));
    menu->nb_entry = nb_entry;
    menu_list_init(
        &menu->list,
        menu,
        (void*)&menu_disco_display_row,
        (void*)&menu_disco_exec,
        menu->nb_entry
    );
}

void menu_disco_display(IvkMenuDisco *menu)
{
    dclear(C_WHITE);
    _title("< Discovery");
    menu_list_display(&menu->list);
    dupdate();
}

void menu_disco_key(IvkMenuDisco *menu, key_event_t keyev)
{
    menu_list_key(&menu->list, keyev);
}
