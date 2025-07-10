#include <string.h>
#include <stdbool.h>

#include <gint/display.h>
#include <gint/keyboard.h>

#include "ivk/instruction.h"
#include "ivk/manual.h"
#include "ivk/utils.h"

//---
// Internals
//---

/* GUI scrollable menu info */
struct menu {
    i8 len;
    i8 offset;
    i8 pos;
    i8 top;
    i8 bottom;
};

/* initialize scrollable list */
static void menu_init(struct menu *menu)
{
    menu->len = -1;
    while (true) {
        if (instruction_table[++menu->len].trampoline == NULL)
            break;
    }
    menu->offset = 0;
    menu->pos = 0;
    menu->top = 1;
    menu->bottom = ROW_COUNT + 1;
}

/* move the internal cursor */
static void menu_move(struct menu *menu, int key, int quick, int wrap)
{
    int visible = menu->bottom - menu->top;
    int max_offset = max(menu->len - visible, 0);

    /* Quick moves */
    if(key == KEY_UP && quick)
    {
        menu->pos = 0;
        menu->offset = 0;
    }
    else if(key == KEY_DOWN && quick)
    {
        menu->pos = menu->len - 1;
        menu->offset = max_offset;
    }
    /* Normal move up and wrapping move up */
    else if(key == KEY_UP && menu->pos > 0)
    {
        menu->pos--;
        menu->offset = min(menu->offset, menu->pos);
    }
    else if(key == KEY_UP && !menu->pos && wrap)
    {
        menu->pos = menu->len - 1;
        menu->offset = max_offset;
    }
    /* Normal move down and wrapping move down */
    else if(key == KEY_DOWN && menu->pos + 1 < menu->len)
    {
        menu->pos++;
        if(menu->pos > menu->offset + visible - 1
                && menu->offset + 1 <= max_offset)
        {
            menu->offset++;
        }
    }
    else if(key == KEY_DOWN && menu->pos + 1 == menu->len && wrap)
    {
        menu->pos = 0;
        menu->offset = 0;
    }
}

/* show menu */
static void menu_show(struct menu const *menu)
{
    int offset = menu->offset;
    int pos = menu->pos;
    int top = menu->top;
    int bottom = menu->bottom;
    int i = 0;
    int j = top;

    while(j < bottom && instruction_table[offset+i].trampoline != NULL)
    {
        _row(
            2, j,
            instruction_table[offset+i].opname,
            instruction_table[offset+i].desc
        );
        i++;
        j++;
    }
    if(menu->len > bottom - top)
    {
        int area_x      = 391;
        int area_width  = 2;
        int area_top    = ROW_Y + ROW_H * (top - 1);
        int area_height = ROW_H * (bottom - top);

        int bar_top = (offset * area_height) / menu->len;
        int bar_height = ((bottom - top) * area_height) / menu->len;

        drect(area_x, area_top + bar_top + 3, area_x + area_width - 1,
                area_top + bar_top + bar_height + 4, C_BLACK);
    }

    int selected = top + (pos - offset);
    if(selected >= top && selected < bottom)
        _highlight(selected);
}

/* execute the selected entry */
static void menu_exec(struct menu *menu)
{
    manual_gui_entry(&instruction_table[menu->pos]);
}

//---
// Public
//---

int main(void)
{
    struct menu menu;
    int key;

    menu_init(&menu);
    while (true)
    {
        dclear(C_WHITE);
        _title("Instruction selection");
        menu_show(&menu);
        dupdate();

        key = getkey().key;
        switch (key)
        {
            case KEY_EXE:
                menu_exec(&menu);
                break;
            default:
                menu_move(&menu, key, 0, 0);
                break;
        }
    }
    return 1;
}
