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
    note p1[D_NOTE], p2[D_NOTE], p3[D_NOTE], p4[D_NOTE];
    note tmp1[D_NOTE], tmp2[D_NOTE], tmp3[D_NOTE], tmp4[D_NOTE];

    int prev_state1 = 0, next_state1 = 0, sx1 = 0;
    int prev_state2 = 0, next_state2 = 0, sx2 = 0;
    int prev_state3 = 0, next_state3 = 0, sx3 = 0;
    int prev_state4 = 0, next_state4 = 0, sx4 = 0;
    int press_state[14];
    int cursor_pos = 0, active = 8;

    enum state state;

    char filepath[50] = "save.stickit";

    CLEAR_ARRAY(press_state, 14)

    note_init(p1);
    note_init(p2);
    note_init(p3);
    note_init(p4);
    file_load(filepath, p1, p2, p3, p4);

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
        if (state == LOAD) {
            file_load("save.stickit", p1, p2, p3, p4);
            state = MAIN;
        } else if (state == SAVE) {
            file_save("save.stickit", p1, p2, p3, p4);
            state = MAIN;
        } else if (state == SAVE_AS) {
            load_save_return = save_menu(screen, filepath, &cursor_pos);
            if (load_save_return == 1) {
                file_save(filepath, p1, p2, p3, p4);
                cursor_pos = 0;
                state = MAIN;
            } else if (load_save_return == 2) {
                cursor_pos = 0;
                state = MAIN;
            }
        } else if (state == LOAD_FROM) {
            load_save_return = load_menu(screen, filepath, &cursor_pos);
            if (load_save_return == 1) {
                file_load(filepath, p1, p2, p3, p4);
                cursor_pos = 0;
                state = MAIN;
            } else if (load_save_return == 2) {
                cursor_pos = 0;
                state = MAIN;
            }
        } else if (state == EDIT1)
            edit_notes(screen, p1, sx1, press_state, &cursor_pos, &active) == 1 ? (state = MAIN) : (state = EDIT1);
        else if (state == EDIT2)
            edit_notes(screen, p2, sx2, press_state, &cursor_pos, &active) == 1 ? (state = MAIN) : (state = EDIT2);
        else if (state == EDIT3)
            edit_notes(screen, p3, sx3, press_state, &cursor_pos, &active) == 1 ? (state = MAIN) : (state = EDIT3);
        else if (state == EDIT4)
            edit_notes(screen, p4, sx4, press_state, &cursor_pos, &active) == 1 ? (state = MAIN) : (state = EDIT4);
        else {
            draw_note_panel(screen, p1, &prev_state1, &next_state1, &sx1, 10, 40);
            draw_note_panel(screen, p2, &prev_state2, &next_state2, &sx2, 140, 40);
            draw_note_panel(screen, p3, &prev_state3, &next_state3, &sx3, 10, 180);
            draw_note_panel(screen, p4, &prev_state4, &next_state4, &sx4, 140, 180);
        }


        edit1 = detect_click(screen, 20+sx1*O_NOTE, 100+sx1*O_NOTE, 65-sx1*O_NOTE, 145-sx1*O_NOTE);
        edit2 = detect_click(screen, 150+sx2*O_NOTE, 230+sx2*O_NOTE, 65-sx2*O_NOTE, 145-sx2*O_NOTE);
        edit3 = detect_click(screen, 20+sx3*O_NOTE, 100+sx3*O_NOTE, 205-sx3*O_NOTE, 285-sx3*O_NOTE);
        edit4 = detect_click(screen, 150+sx4*O_NOTE, 230+sx4*O_NOTE, 205-sx4*O_NOTE, 285-sx4*O_NOTE);

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
    file_save(filepath, p1, p2, p3, p4);
    return 0;
}
