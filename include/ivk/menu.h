#ifndef IVK_MENU_H
#define IVK_MENU_H 1

#include <gint/keyboard.h>
#include <gint/display.h>

#include "ivk/utils.h"

//---
// Internal list "widget"
//---

struct IvkList {
    int nb_entry;
    int offset;
    int pos;
    int top;
    int bottom;
    int (*user_disp)(void*,int,int);
    void (*user_exec)(void*,int);
    void *user_data;
};
typedef struct IvkList IvkList;

/* constructor */
extern void menu_list_init(
    IvkList *list,
    void *data,
    int (*disp)(void*,int,int),
    void (*exec)(void*,int),
    int nb_entry
);

/* display list */
extern void menu_list_display(IvkList *list);

/* handle key event */
extern void menu_list_key(IvkList *list, key_event_t keyev);

//---
// Instruction menu
//---

/* internal menu information */
struct IvkMenuInst {
    IvkList list;
    int nb_entry;
};
typedef struct IvkMenuInst IvkMenuInst;

/* menu init */
extern void menu_instruction_init(IvkMenuInst *menu);

/* menu display */
extern void menu_instruction_display(IvkMenuInst *menu);

/* menu key event handling */
extern void menu_instruction_key(IvkMenuInst *menu, key_event_t keyev);

//---
// discovery menu
//---

/* internal menu information */
struct IvkMenuDisco {
    IvkList list;
    int nb_entry;
};
typedef struct IvkMenuDisco IvkMenuDisco;

/* menu init */
extern void menu_disco_init(IvkMenuDisco *menu);

/* menu display */
extern void menu_disco_display(IvkMenuDisco *menu);

/* menu key event handling */
extern void menu_disco_key(IvkMenuDisco *menu, key_event_t keyev);

#endif /* IVK_MENU_H */
