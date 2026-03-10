#include "config.h"
#include "tigr/tigr.h"
#include "font.h"
#include "elements.h"
#include "file.h"
#include "util.h"

int
main(int argc, char* argv[])
{
    note p1[D_NOTE], p2[D_NOTE], p3[D_NOTE], p4[D_NOTE];
    note tmp1[D_NOTE], tmp2[D_NOTE], tmp3[D_NOTE], tmp4[D_NOTE];
    note_init(p1);
    note_init(p2);
    note_init(p3);
    note_init(p4);
    file_load("save.stickit", p1, p2, p3, p4);

    /* state */
    int prev_state1 = 0, next_state1 = 0, sx1 = 0;
    int prev_state2 = 0, next_state2 = 0, sx2 = 0;
    int prev_state3 = 0, next_state3 = 0, sx3 = 0;
    int prev_state4 = 0, next_state4 = 0, sx4 = 0;
    int save_state = 0, load_state = 0;
    int edit1_state = 0, edit2_state = 0, edit3_state = 0, edit4_state = 0;
    int press_state[14];
    for (int i = 0; i < 14; i++)
        press_state[i] = 0;
    int cursor_pos = 1;
    int active = 8;

    Tigr* screen = tigrWindow(270, 320, "Hello", TIGR_FIXED);
    while (!tigrClosed(screen) && !tigrKeyDown(screen, TK_ESCAPE)) {
        tigrClear(screen, COL_BG);

        /* variables */
        int save = 0, load = 0;
        int edit1 = 0, edit2 = 0, edit3 = 0, edit4 = 0;

        /* top bar */
        draw_panel(screen, 0, 0, 269, 30);
        save = draw_button(screen, "SAVE", 2, 2, 40, 26);
        load = draw_button(screen, "LOAD", 227, 2, 40, 26);

        draw_double_text(screen, "STICKY NOTES", 97, 10, 1);

        /* click logic */
        if ((save & 1) && !load_state)
            save_state = 1;
        if ((load & 1) && !save_state)
            load_state = 1;

        /* lower panel */
        draw_panel(screen, 0, 31, 269, 288);
        if (load_state) {
            file_load("save.stickit", p1, p2, p3, p4);
            load_state = 0;
        } else if (save_state) {
            file_save("save.stickit", p1, p2, p3, p4);
            save_state = 0;
        } else if (edit1_state)
            edit_notes(screen, p1, sx1, &edit1_state, press_state, &cursor_pos, &active);
        else if (edit2_state)
            edit_notes(screen, p2, sx2, &edit2_state, press_state, &cursor_pos, &active);
        else if (edit3_state)
            edit_notes(screen, p3, sx3, &edit3_state, press_state, &cursor_pos, &active);
        else if (edit4_state)
            edit_notes(screen, p4, sx4, &edit4_state, press_state, &cursor_pos, &active);
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

        int editable = (!(edit1_state || edit2_state || edit3_state || edit4_state));

        if ((edit1 & 1) && editable)
            edit1_state = 1;
        if ((edit2 & 1) && editable)
            edit2_state = 1;
        if ((edit3 & 1) && editable)
            edit3_state = 1;
        if ((edit4 & 1) && editable)
            edit4_state = 1;

        tigrUpdate(screen);
    }

    tigrFree(screen);
    file_save("save.stickit", p1, p2, p3, p4);
    return 0;
}
