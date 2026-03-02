#include <stdio.h>
#include <dirent.h>
#include "elements.h"

#ifndef FILE_H
#define FILE_H

static inline void
helper_file_save(FILE *fp, note *p)
{
    for (int i = 0; i < D_NOTE; i++)
        for (int j = 0; j < H_NOTE; j++)
            fprintf(fp, "%s,%d\n", p[i].text[j], p[i].bar[j] != 0);
}

void
file_save(note *p1, note *p2, note *p3, note *p4)
{
    FILE *fp;
    fp = fopen("save.stickit", "w");

    helper_file_save(fp, p1);
    helper_file_save(fp, p2);
    helper_file_save(fp, p3);
    helper_file_save(fp, p4);

    fclose(fp);
}

static inline void
helper_file_load(FILE *fp, note *p)
{
    for (int i = 0; i < D_NOTE; i++)
        for (int j = 0; j < H_NOTE; j++) {
            char first = fgetc(fp);
            if (first == ',') {
                fscanf(fp, "%d\n", &p[i].bar[j]);
                strcpy(p[i].text[j], "");
            } else {
                fseek(fp, -1, SEEK_CUR);
                fscanf(fp, "%[^,],%d\n", p[i].text[j], &p[i].bar[j]);
            }
        }
}

void
file_load(note *p1, note *p2, note *p3, note *p4)
{
    FILE *fp;
    fp = fopen("save.stickit", "r");

    if (fp == 0)
        return;

    helper_file_load(fp, p1);
    helper_file_load(fp, p2);
    helper_file_load(fp, p3);
    helper_file_load(fp, p4);

    fclose(fp);
}


#endif
