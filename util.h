#include "tigr/tigr.h"

#ifndef UTIL_H
#define UTIL_H

inline int
detect_click(Tigr* bmp, int sx, int dx, int tp, int bm)
{
    int mx, my, btn;
    tigrMouse(bmp, &mx, &my, &btn);

    if ((btn & 1) && mx >= sx && mx <= dx && my >= tp && my <= bm)
        return 1;
    return 0;
}

#endif
