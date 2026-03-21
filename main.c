#include "config.h"
#include "tigr/tigr.h"
#include "font.h"
#include "elements.h"
#include "file.h"
#include "util.h"

enum state {
    LOAD,
    SAVE,
    LOAD_FROM,
    SAVE_AS,
    MAIN,
    EDIT1,
    EDIT2,
    EDIT3,
    EDIT4
};

int
main(int argc, char* argv[])
{
    /* state */
    note p[4][D_NOTE];
    note tmp[4][D_NOTE];

    int sx[4], prev_state[4], next_state[4], press_state[14];
    int cursor_pos = 0, active = 8;

    enum state state;

    char filepath[50] = "save.stickit";

    CLEAR_ARRAY(sx, 4)
    CLEAR_ARRAY(prev_state, 4)
    CLEAR_ARRAY(next_state, 4)
    CLEAR_ARRAY(press_state, 14)

    note_init(p);
    file_load(filepath, p);

    Tigr* screen = tigrWindow(270, 320, "Hello", TIGR_FIXED);
    while (!tigrClosed(screen) && !tigrKeyDown(screen, TK_ESCAPE)) {

        /* variables */
        int save = 0, load = 0;
        int save_as = 0, load_from = 0;
        int edit1 = 0, edit2 = 0, edit3 = 0, edit4 = 0;
        int load_save_return = 0;

        tigrClear(screen, COL_BG);

        /* top bar */
        draw_panel(screen, 0, 0, 269, 30);
        save = draw_button(screen, "SAVE", 2, 2, 40, 26);
        load = draw_button(screen, "LOAD", 227, 2, 40, 26);
        save_as = draw_button(screen, "SAVE AS", 44, 2, 55, 26);
        load_from = draw_button(screen, "LOAD FROM", 165, 2, 60, 26);
        draw_double_text(screen, "STICKIT", 113, 10, 1);

        /* click logic */
        if ((save & 1) && state == MAIN)
            state = SAVE;
        if ((load & 1) && state == MAIN)
            state = LOAD;
        if ((save_as & 1) && state != LOAD_FROM)
            state = SAVE_AS;
        if ((load_from & 1) && state != SAVE_AS)
            state = LOAD_FROM;

        /* lower panel */
        draw_panel(screen, 0, 31, 269, 288);

        /* main loop */
        switch (state) {
            case LOAD:
                file_load(filepath, p);
                state = MAIN;
                break;
            case SAVE:
                file_save(filepath, p);
                state = MAIN;
                break;
            case SAVE_AS:
            case LOAD_FROM:
                load_save_return = state == SAVE_AS ? save_menu(screen, filepath, &cursor_pos) : load_menu(screen, filepath, &cursor_pos);
                if (load_save_return == 1) {
                    state == SAVE_AS ? file_save(filepath, p) : file_load(filepath, p);
                    cursor_pos = 0;
                    state = MAIN;
                } else if (load_save_return == 2) {
                    cursor_pos = 0;
                    state = MAIN;
                }
                break;
            case EDIT1:
            case EDIT2:
            case EDIT3:
            case EDIT4:
                state = edit_notes(screen, p[state-EDIT1], sx[state-EDIT1], press_state, &cursor_pos, &active) == 1 ? MAIN : state;
                break;
            case MAIN:
            default:
                draw_note_panel(screen, p[0], &prev_state[0], &next_state[0], &sx[0], 10, 40);
                draw_note_panel(screen, p[1], &prev_state[1], &next_state[1], &sx[1], 140, 40);
                draw_note_panel(screen, p[2], &prev_state[2], &next_state[2], &sx[2], 10, 180);
                draw_note_panel(screen, p[3], &prev_state[3], &next_state[3], &sx[3], 140, 180);
                break;
        }

        edit1 = detect_click(screen, 20+sx[0]*O_NOTE, 100+sx[0]*O_NOTE, 65-sx[0]*O_NOTE, 145-sx[0]*O_NOTE);
        edit2 = detect_click(screen, 150+sx[1]*O_NOTE, 230+sx[1]*O_NOTE, 65-sx[1]*O_NOTE, 145-sx[1]*O_NOTE);
        edit3 = detect_click(screen, 20+sx[2]*O_NOTE, 100+sx[2]*O_NOTE, 205-sx[2]*O_NOTE, 285-sx[2]*O_NOTE);
        edit4 = detect_click(screen, 150+sx[3]*O_NOTE, 230+sx[3]*O_NOTE, 205-sx[3]*O_NOTE, 285-sx[3]*O_NOTE);

        if ((edit1 & 1) && state == MAIN)
            state = EDIT1;
        if ((edit2 & 1) && state == MAIN)
            state = EDIT2;
        if ((edit3 & 1) && state == MAIN)
            state = EDIT3;
        if ((edit4 & 1) && state == MAIN)
            state = EDIT4;

        tigrUpdate(screen);
    }

    tigrFree(screen);
    file_save(filepath, p);
    return 0;
}
