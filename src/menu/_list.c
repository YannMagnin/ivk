#include <string.h>

#include "ivk/menu.h"
#include "ivk/utils.h"

//---
// List interface
//---

void menu_list_init(
    IvkList *list,
    void *data,
    int (*disp)(void*,int,int),
    void (*exec)(void*,int),
    int nb_entry
) {
    memset(list, 0x00, sizeof(IvkList));
    list->user_data   = data;
    list->user_disp   = disp;
    list->user_exec   = exec;
    list->nb_entry    = nb_entry;
    list->offset      = 0;
    list->pos         = 0;
    list->top         = 1;
    list->bottom      = ROW_COUNT + 1;
}

void menu_list_display(IvkList *list)
{
    int i;
    int j;

    i = 0;
    j = list->top;
    while(j < list->bottom)
    {
        if (list->user_disp(list->user_data, j, list->offset+i) != 0)
            break;
        i++;
        j++;
    }
    if(list->nb_entry > (list->bottom - list->top))
    {
        int area_x      = 391;
        int area_width  = 2;
        int area_top    = ROW_Y + ROW_H * (list->top - 1);
        int area_height = ROW_H * (list->bottom - list->top);
        int bar_top     = (list->offset * area_height) / list->nb_entry;
        int bar_height  = \
            ((list->bottom - list->top) * area_height) / list->nb_entry;
        drect(
            area_x,
            area_top + bar_top + 3,
            area_x + area_width - 1,
            area_top + bar_top + bar_height + 4,
            C_BLACK
        );
    }
    int selected = list->top + (list->pos - list->offset);
    if(selected >= list->top && selected < list->bottom) {
        drect(
            0,
            10 + (selected * 14) - 1,
            DWIDTH,
            10 + ((selected + 1) * 14 ) - 1,
            C_INVERT
        );
    }
}

void menu_list_key(IvkList *list, key_event_t keyev)
{
    int max_offset;
    int visible;

    visible = list->bottom - list->top;
    max_offset = max(list->nb_entry - visible, 0);
    switch(keyev.key)
    {
        case KEY_EXE:
            if (keyev.type == KEYEV_DOWN)
                list->user_exec(list->user_data, list->pos);
            break;
        case KEY_UP:
            if (list->pos > 0) {
                list->pos--;
                list->offset = min(list->offset, list->pos);
            } else {
                list->pos = list->nb_entry - 1;
                list->offset = max_offset;
            }
            break;
        case KEY_DOWN:
            if (++list->pos < list->nb_entry) {
                if(list->pos > list->offset + visible - 1 &&
                        list->offset + 1 <= max_offset) {
                    list->offset++;
                }
            } else {
                list->pos = 0;
                list->offset = 0;
            }
            break;
    }
}
