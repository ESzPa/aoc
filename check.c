#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <libgen.h>
#include <unistd.h>
#include <sys/types.h>

typedef enum {
    C_RED,
    C_BLUE,
    C_GREEN,
    C_DEFAULT
} Color;

void print(const char* text, Color color);
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

    check_year(2024);

    return 0;
}

void print(const char* text, Color color) {
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
    double year_template[31*2];
    double found_days[31*2] = {0};

    double _d = 1.0;
    for(int i = 0; i < sizeof(year_template)/sizeof(year_template[0]); ++i) {
        year_template[i] = _d;
        _d += 0.5;
    }

    char yeardir[1024];
    char year_str[16];
    snprintf(year_str, sizeof(year_str), "%d", year);
    snprintf(yeardir, sizeof(yeardir), "%s%s", path, year_str);

    DIR *dir = opendir(yeardir);
    if(dir == NULL) {
        perror("opendir failed");
        exit(1);
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


    char dbuf[64];
    char ybuf[64];
    for(int i = 0; i < sizeof(found_days)/sizeof(found_days[0]); ++i) {
        snprintf(dbuf, sizeof(dbuf), "%.1f", found_days[i]);
        snprintf(ybuf, sizeof(ybuf), "%.1f", year_template[i]);
        if(strcmp(dbuf, ybuf) == 0) {
            print(ybuf, C_BLUE);
        }
        else {
            print(ybuf, C_RED);
        }
    }

    closedir(dir);
}
