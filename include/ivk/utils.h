#ifndef _UTILS_H
#define _UTILS_H 1

#include <stddef.h>
#include <stdint.h>

#include <gint/display.h>
#include <gint/keyboard.h>
#include <gint/defs/util.h>

/* internal short-hand for print */
#define _(str, ...) \
    dprint(4, 6+((y++)*14), C_BLACK, str __VA_OPT__(,) __VA_ARGS__)

/* internal short-hand for printXY */
#define _row(x, y, name, desc)                  \
    do {                                        \
        dtext(4, 10+(y*14)+2, C_BLACK, name);   \
        dtext_opt(                              \
            DWIDTH - 9, 10+(y*14)+2,            \
            C_BLACK, C_NONE,                    \
            DTEXT_RIGHT, DTEXT_TOP,             \
            desc                                \
        );                                      \
    } while (0)

/* reverse color area */
#define _highlight(y) \
    drect(0, 10+(y*14)-1, DWIDTH, 10+((y+1)*14)-1, C_INVERT);

/* title */
#define _title(str)                                                     \
    do {                                                                \
        dtext_opt(                                                      \
            DWIDTH/2, 4, C_BLACK, C_NONE, DTEXT_CENTER, DTEXT_TOP, str  \
        );                                                              \
        drect(0, 0, DWIDTH, 4+14-1, C_INVERT);                          \
    } while (0)

/* FXCG50 screen config */
#define ROW_X      6
#define ROW_W      8
#define ROW_Y      20
#define ROW_YPAD   2
#define ROW_H      14
#define ROW_COUNT  14

#endif /* _UTILS_H */
