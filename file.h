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
file_save(char *filename, note p[4][10])
{
    FILE *fp;
    fp = fopen(filename, "w");

    helper_file_save(fp, p[0]);
    helper_file_save(fp, p[1]);
    helper_file_save(fp, p[2]);
    helper_file_save(fp, p[3]);

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
file_load(char *filename, note p[4][10])
{
    FILE *fp;
    fp = fopen(filename, "r");

    if (fp == 0)
        return;

    helper_file_load(fp, p[0]);
    helper_file_load(fp, p[1]);
    helper_file_load(fp, p[2]);
    helper_file_load(fp, p[3]);

    fclose(fp);
}


#endif
