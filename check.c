#include <dirent.h>
#include <libgen.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>

#define DAYWRAP (4)
#define MAX_SLOTS (25 * 2)
#define TOTAL_SLOTS(DAYS) (DAYS ? 12 * 2 : 25 * 2)
#define MAX_PATH_BUFFER 4096

typedef enum { C_RED, C_BLUE, C_GREEN, C_DEFAULT } Color;

void print(const char* text);
void printc(const char* text, Color color);
void check_year(int year);
int target_year();

const int start_year = 2015;

char path[MAX_PATH_BUFFER];

int main(int argc, const char** argv) {
    ssize_t count = readlink("/proc/self/exe", path, sizeof(path) - 1);
    if(count == -1) {
        perror("readlink failed");
        return 1;
    }
    path[count] = '\0';

    char* last_slash = strrchr(path, '/');
    if(last_slash != NULL) {
        *(last_slash + 1) = '\0';
    }

    for(int i = start_year; i <= target_year(); ++i) {
        check_year(i);
    }

    return 0;
}

void print(const char* text) {
    printc(text, C_DEFAULT);
}

void printc(const char* text, Color color) {
    const char* code;

    switch(color) {
        case C_RED:
            code = "\x1b[31m";
            break;
        case C_BLUE:
            code = "\x1b[34m";
            break;
        case C_GREEN:
            code = "\x1b[32m";
            break;
        case C_DEFAULT:
            code = "\x1b[0m";
            break;

        default:
            fprintf(stderr, "How did you get here?\n");
            exit(1);
    }

    printf("%s%s\x1b[0m\n", code, text);
}

int daypart_compare(const void* a, const void* b) {
    double arg1 = *(const double*)a;
    double arg2 = *(const double*)b;
    return (arg1 > arg2) - (arg1 < arg2);
}

void check_year(int year) {
    const int total_slots = TOTAL_SLOTS(year >= 2025);

    double year_template[MAX_SLOTS];
    double found_days[MAX_SLOTS];

    memset(found_days, 0, sizeof(found_days));

    for(int i = 0; i < total_slots; ++i) {
        year_template[i] = 1.0 + 0.5 * i;
    }

    char yeardir[MAX_PATH_BUFFER];
    char year_str[16];
    snprintf(year_str, sizeof(year_str), "%d", year);
    snprintf(yeardir, sizeof(yeardir), "%s%s", path, year_str);

    DIR* dir = opendir(yeardir);
    if(dir == NULL) {
        if(year >= start_year && year <= target_year()) {
            printc(year_str, C_RED);
            return;
        }
        else {
            perror("opendir failed");
            exit(1);
        }
    }

    struct dirent* entry;
    while((entry = readdir(dir)) != NULL) {
        if(strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
            continue;
        }

        char* endptr;
        double day = strtod(entry->d_name, &endptr);
        if(endptr == entry->d_name)
            continue;

        size_t idx = (size_t)((day - 1.0) * 2.0);
        if(idx < (size_t)total_slots) {
            found_days[idx] = day;
        }
        else {
            fprintf(stderr, "ERROR: Unexpected directory name: %s\n", entry->d_name);
        }
    }

    closedir(dir);

    char freebuf[128];
    snprintf(freebuf, sizeof(freebuf), "%d", year);
    printc(freebuf, C_GREEN);

    int idx = 0;
    while(idx < total_slots) {
        bool done = fabs(found_days[idx] - year_template[idx]) < 0.01;

        int wrap_until = idx;
        while(wrap_until + 1 < total_slots) {
            bool next_done = fabs(found_days[wrap_until + 1] - year_template[wrap_until + 1]) < 0.01;
            if(next_done != done)
                break;
            ++wrap_until;
        }

        char mark[8];
        if(wrap_until == total_slots - 1) {
            snprintf(mark, sizeof(mark), "└─");
        }
        else {
            snprintf(mark, sizeof(mark), "├─");
        }

        if(wrap_until == idx) {
            snprintf(freebuf, sizeof(freebuf), "%s%4.1f", mark, year_template[idx]);
            printc(freebuf, done ? C_BLUE : C_RED);
        }
        else {
            snprintf(freebuf, sizeof(freebuf), "%s%4.1f –> %-5.1f", mark, year_template[idx],
                     year_template[wrap_until]);
            printc(freebuf, done ? C_BLUE : C_RED);
        }

        idx = wrap_until + 1;
    }
}

int target_year() {
    time_t t = time(NULL);
    struct tm* now = localtime(&t);

    int current_year = now->tm_year + 1900;

    if(now->tm_mon < 11) {
        return current_year - 1;
    }
    else {
        return current_year;
    }
}
