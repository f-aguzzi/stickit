#include <string.h>
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

typedef struct {
    int left_pressed;
    int right_pressed;
    int backspace_pressed;
    int space_pressed;
    int enter_pressed;
    int text;
} pressed_buttons;

inline int
read_keys(int *cursor_pos, pressed_buttons pb, char *str, char *temp)
{
    if (pb.left_pressed != 0 && *cursor_pos > 0)
        *cursor_pos -= 1;
    else if (pb.right_pressed != 0 && *cursor_pos < strlen(str))
        *cursor_pos += 1;
    else if (pb.backspace_pressed) {
        if (*cursor_pos > 0) {
            int i=0, len = strlen(str);
            *cursor_pos -= 1;
            for (; i < *cursor_pos; i++)
                temp[i] = str[i];
            for (; i < len; i++)
                temp[i] = str[i+1];
            temp[i] = '\0';
            strcpy(str, temp);
        }
    } else if (pb.enter_pressed)
        return 1;
    else if ((pb.text != 0 || pb.space_pressed) && *cursor_pos < 12) {
        if (pb.space_pressed)
            pb.text = ' ';

        int i, split, len = strlen(str);
        split = *cursor_pos > len ? *cursor_pos-1 : *cursor_pos;

        for (i = 0; i <= split; i++)
            temp[i] = str[i];

        temp[split] = pb.text;
        split += 1;

        for (i = split; i <= len+1; i++)
            temp[i] = str[i-1];

        strcpy(str, temp);
        *cursor_pos +=1;
    }
    return 0;
}

#endif
