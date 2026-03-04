#ifndef ELEMENTS
#define ELEMENTS

#include <stdio.h>
#include <string.h>
#include "config.h"
#include "tigr/tigr.h"
#include "font.h"
#include "util.h"

#define HANDLE_BUTTON_PRESS(button_index, p, press_state, c) \
    if ((c[button_index] & 1) && !press_state[button_index]) { \
        press_state[button_index] = 1; \
        p->bar[button_index] = !p->bar[button_index]; \
        return 2; \
    }

#define HANDLE_TEXTBOX_PRESS(button_index, press_state, c, active) \
    if ((c[button_index] & 1) && !press_state[button_index]) { \
        press_state[button_index] = 1; \
        *active = button_index; \
        *cursor_pos = strlen(p->text[button_index]); \
        return 3; \
    }

static void
draw_basic_panel(Tigr *bmp, int x, int y, int w, int h, TPixel bg_col, TPixel border_col, TPixel shadow_col)
{
    /* fill */
    tigrFillRect(bmp, x, y, w+1, h+1, bg_col);
    /* left + top border */
    tigrLine(bmp, x, y, x+w, y, border_col);
    tigrLine(bmp, x, y, x, y+h, border_col);
    /* right + bottom border */
    tigrLine(bmp, x+w, y, x+w, y+h, shadow_col);
    tigrLine(bmp, x, y+h, x+w+1, y+h, shadow_col);
}

static void
draw_panel(Tigr *bmp, int x, int y, int w, int h)
{
    draw_basic_panel(bmp, x, y, w, h, COL_BG, COL_BORDER, COL_SHADOW);
}

typedef struct note_s {
    char text[H_NOTE][W_NOTE];
    int bar[H_NOTE];
} note;

void
note_init(note p[])
{
    for (int i = 0; i < D_NOTE; i++)
        for (int j = 0; j < H_NOTE; j++) {
            strcpy(p[i].text[j], "");
            p[i].bar[j] = 0;
        }
}

static void
draw_note(Tigr *bmp, int x, int y, note p)
{
    draw_basic_panel(bmp, x, y, 80, 80, COL_NOTE_BG, COL_NOTE_BORDER, COL_NOTE_SHADOW);
    for (int i = 0; i < H_NOTE; i++)
    {
        draw_text(bmp, p.text[i], x+4, y+4+i*(CELL_H+3), 1, COL_SHADOW);
        if (p.bar[i]) {
            tigrLine(bmp, x+2, y+H_NOTE+i*(CELL_H+3), x+7+6*strlen(p.text[i]), y+H_NOTE+i*(CELL_H+3), COL_SHADOW);
        }
    }
}

static void
draw_notes(Tigr *bmp, int x, int y, int number, note *notes, int first, int last)
{
    for (int i = last; i >= first ; i--) {
        draw_note(bmp, x+i*O_NOTE, y-i*O_NOTE, notes[i]);
    }
}

static int
draw_button(Tigr *bmp, const char *label, int x, int y, int w, int h)
{
    draw_panel(bmp, x, y, w, h);
    const int label_x = x + (w - TEXT_LEN(label)) / 2;
    const int label_y = y + (h - CELL_H)/2;
    draw_text(bmp, label, label_x, label_y, FONT_SCALE, COL_BORDER);
    return detect_click(bmp, x, x+w, y, y+h);
}

static int
draw_textbox(Tigr *bmp, const char *content, int active, int cursor_pos, int x, int y, int w, int h)
{
    draw_basic_panel(bmp, x, y, w, h, COL_BG_DARK, COL_SHADOW, COL_BORDER);
    const int label_x = x + (w - TEXT_LEN(content)) / 2;
    const int label_y = y + (h - CELL_H)/2;
    draw_text(bmp, content, label_x, label_y, FONT_SCALE, COL_SHADOW);
    if (active)
        tigrLine(bmp, label_x+1+6*cursor_pos, y+2, label_x+1+6*cursor_pos, y+h-2, COL_SHADOW);
    return detect_click(bmp, x, x+w, y, y+h);
}

void
draw_note_panel(Tigr *bmp, note *notes, int *prev_state, int *next_state, int *sx, int x_offset, int y_offset)
{
    draw_panel(bmp, 0+x_offset, 0+y_offset, 120, 130);

    int prev_pressed = draw_button(bmp, "PREV", 10+x_offset, 110+y_offset, 30, 15);
    int next_pressed = draw_button(bmp, "NEXT", 45+x_offset, 110+y_offset, 30, 15);

    if ((prev_pressed & 1) && !(*prev_state & 1))
        *sx = *sx > 0 ? *sx-1 : *sx;

    if ((next_pressed & 1) && !(*next_state & 1))
        *sx = *sx < 9 ? *sx+1 : *sx;

    /* state resets */
    *prev_state = prev_pressed;
    *next_state = next_pressed;

    /* dependent elements */
    draw_notes(bmp, 10+x_offset, 25+y_offset, 10, notes, *sx, 9);
    char pagenum[6];
    sprintf(pagenum, "%d/10", *sx+1);
    draw_button(bmp, pagenum, 80+x_offset, 110+y_offset, 31, 15);
}

int
edit_menu(Tigr *bmp, note *p, int x_offset, int y_offset, int *press_state, int *cursor_pos, int *active)
{
    int back = 0, c[7], t[7];
    for (int i = 0; i < 7; i++) {
        c[i] = 0;
        t[i] = 0;
    }

    int *text_press_state = &press_state[7];

    char number[3];
    number[1] = '.';
    number[2] = '\0';
    char cross[2] = "X";
    int y_off;

    back = draw_button(bmp, "<- BACK", 2, y_offset, 60, 20);

    for (int i = 0; i < H_NOTE; i++) {
        number[0] = i+49;
        cross[0] = p->bar[i] == 0 ? ' ' : 'X';
        y_off = y_offset+30+i*30;

        draw_text(bmp, number, x_offset - 30, y_off+4, 1, COL_BORDER);
        t[i] = draw_textbox(bmp, p->text[i], i==*active, *cursor_pos, x_offset, y_off, 100, 20);
        c[i] = draw_button(bmp, cross, x_offset+120, y_off, 20, 20);
        if (p->bar[i] == 1)
            tigrLine(bmp, x_offset+5, y_off+10, x_offset+90, y_off+10, COL_SHADOW);
    }

    if (back & 1)
        return 1;

    for (int i = 0; i < 7; i++) {
        HANDLE_BUTTON_PRESS(i, p, press_state, c);
        HANDLE_TEXTBOX_PRESS(i, text_press_state, t, active);
    }

    for (int i = 0; i < 7; i++) {
        press_state[i] = c[i];
        text_press_state[i] = t[i];
    }

    return 0;
}

#endif
