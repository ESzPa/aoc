#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <libgen.h>
#include <unistd.h>
#include <sys/types.h>
#include <math.h>

#define DAYWRAP (4)
#define TOTAL_SLOTS (31*2)

typedef enum {
    C_RED,
    C_BLUE,
    C_GREEN,
    C_DEFAULT
} Color;

void print(const char* text);
void printc(const char* text, Color color);
void check_year(int year);

const int start_year  = 2015;
const int latest_year = 2024;

char path[1024];

int main(int argc, const char** argv) {
    ssize_t count = readlink("/proc/self/exe", path, sizeof(path) - 1);
    if (count == -1) {
        perror("readlink failed");
        return 1;
    }
    path[count] = '\0';

    char *last_slash = strrchr(path, '/');
    if (last_slash != NULL) {
        *(last_slash + 1) = '\0';
    }

    for(int i = start_year; i <= latest_year; ++i){
        check_year(i);
    }

    return 0;
}

void print(const char* text) {
    printc(text, C_DEFAULT);
}

void printc(const char* text, Color color) {
    char code[32];
    switch(color) {
        case C_RED:
            strcpy(code,"\x1b[31m");
            break;
        case C_BLUE:
            strcpy(code,"\x1b[34m");
            break;
        case C_GREEN:
            strcpy(code,"\x1b[32m");
            break;
        case C_DEFAULT:
            strcpy(code,"\x1b[0m");
            break;

        default:
            perror("How did you get here?");
            exit(1);
    }

    printf("%s%s\x1b[0m\n", code, text);
}

int daypart_compare(const void *a, const void *b) {
    double arg1 = *(const double *)a;
    double arg2 = *(const double *)b;
    return (arg1 > arg2) - (arg1 < arg2);    
}

void check_year(int year) {
    double year_template[TOTAL_SLOTS];
    double found_days[TOTAL_SLOTS] = {0};

    for(int i = 0; i < TOTAL_SLOTS; ++i) {
        year_template[i] = 1.0 + 0.5 * i;
    }

    char yeardir[1024];
    char year_str[16];
    snprintf(year_str, sizeof(year_str), "%d", year);
    snprintf(yeardir, sizeof(yeardir), "%s%s", path, year_str);

    DIR *dir = opendir(yeardir);
    if(dir == NULL) {
        if(year >= start_year && year <= latest_year) {
            printc(year_str, C_RED);
            return;
        }
        else {
            perror("opendir failed");
            exit(1);
        }
    }

    struct dirent *entry;
    while((entry = readdir(dir)) != NULL) {
        if(strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
            continue;
        }

        char* endptr;
        double day = strtod(entry->d_name, &endptr);
        if(endptr == entry->d_name) continue;

        found_days[(size_t)((day-1)*2)] = day;
    }

    char freebuf[128];

    snprintf(freebuf, sizeof(freebuf), "%d", year);
    printc(freebuf, C_GREEN);

    int idx = 0;
    while (idx < TOTAL_SLOTS) {
        bool done = fabs(found_days[idx] - year_template[idx]) < 0.01;

        int wrap_until = idx;
        while (wrap_until + 1 < TOTAL_SLOTS) {
            bool next_done = fabs(found_days[wrap_until + 1] - year_template[wrap_until + 1]) < 0.01;
            if (next_done != done) break;
            ++wrap_until;
        }

        char mark[8];
        if(wrap_until == TOTAL_SLOTS-1) {
            snprintf(mark, sizeof(mark), "└─");
        }
        else {
            snprintf(mark, sizeof(mark), "├─");
        }

        if (wrap_until == idx) {
            snprintf(freebuf, sizeof(freebuf), "%s%4.1f", mark, year_template[idx]);
            printc(freebuf, done ? C_BLUE : C_RED);
        } 
        else {
            snprintf(freebuf, sizeof(freebuf), "%s%4.1f –> %-5.1f", mark, year_template[idx], year_template[wrap_until]);
            printc(freebuf, done ? C_BLUE : C_RED);
        }

        idx = wrap_until + 1;
    }

    closedir(dir);
}
