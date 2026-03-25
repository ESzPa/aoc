#include <limits.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/resource.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

#define BUFFER_MAX 1024

#define BIN_NAME "main"

typedef enum Color : uint8_t {
    BLACK = 0,
    RED,
    GREEN,
    YELLOW,
    BLUE,
    PURPLE,
    CYAN,
    WHITE
} Color;

const char* ANSIColorTable[] = {"\e[0;30m", "\e[0;31m", "\e[0;32m",
                                "\e[0;33m", "\e[0;34m", "\e[0;35m",
                                "\e[0;36m", "\e[0;37m"};
const char* ANSIReset = "\033[0m";

char exe_path[PATH_MAX];

void printc(Color color, const char* format, ...) {
    va_list args;
    va_start(args, format);

    printf("%s", ANSIColorTable[(size_t)color]);
    vprintf(format, args);
    printf("%s", ANSIReset);

    va_end(args);
}

void make_full_path(char* buffer, size_t size, const char* dir1,
                    const char* dir2) {
    snprintf(buffer, size, "%s/%s", dir1, dir2);
}

bool run_part(const char* year, const char* day, int real_part) {
    char dir_buf[PATH_MAX];
    char src_buf[PATH_MAX];
    char bin_buf[PATH_MAX];
    char day_part[PATH_MAX];
    snprintf(day_part, sizeof(day_part), "%s/%s%s", year, day,
             (real_part == 1 ? ".0" : ".5"));
    make_full_path(dir_buf, sizeof(dir_buf), exe_path, day_part);
    make_full_path(src_buf, sizeof(src_buf), dir_buf, "main.cpp");
    make_full_path(bin_buf, sizeof(bin_buf), dir_buf, BIN_NAME);

    if (access(src_buf, F_OK) != 0) { return 0; }

    printc(GREEN, "%s/%s:%d ", year, day, real_part);

    struct timespec start, end;
    char buffer[BUFFER_MAX];

    snprintf(buffer, sizeof(buffer),
             "g++ -std=c++23 -O2 -pipe -I. -o %s %s", bin_buf,
             src_buf);
    clock_gettime(CLOCK_MONOTONIC, &start);
    if (system(buffer) != 0) { return 0; }
    clock_gettime(CLOCK_MONOTONIC, &end);

    double elapsed_ms = (end.tv_sec - start.tv_sec) * 1000.0 +
                        (end.tv_nsec - start.tv_nsec) / 1000000.0;

    printc(BLUE, "(gcc %dms)", (int)elapsed_ms);
    printf("\n");

    snprintf(buffer, sizeof(buffer), "cd %s && ./main", dir_buf);

    int status;
    struct rusage usage;

    clock_gettime(CLOCK_MONOTONIC, &start);

    pid_t pid = fork();
    if (pid == 0) {
        execl("/bin/sh", "sh", "-c", buffer, (char*)NULL);
        _exit(1);
    }
    wait4(pid, &status, 0, &usage);

    clock_gettime(CLOCK_MONOTONIC, &end);

    elapsed_ms = (end.tv_sec - start.tv_sec) * 1000.0 +
                 (end.tv_nsec - start.tv_nsec) / 1000000.0;

    printc(CYAN, "[%d]", WEXITSTATUS(status));
    printc(BLUE, "[%dms | %ldkb]", (int)elapsed_ms, usage.ru_maxrss);
    printf("\n");

    return WEXITSTATUS(status) == 0;
}

int main(int argc, char** argv) {
    const char* exe = argv[0];

    if (argc < 3) {
        printf("Usage: %s <year> <day> [part]\n", exe);
        printf("\tExample: %s 2024 5 1\n", exe);
        return 1;
    }

    const char* year = argv[1];
    const char* day = argv[2];
    bool part_spec = argc > 3;
    const char* part = part_spec ? argv[3] : "\0";

    ssize_t len =
        readlink("/proc/self/exe", exe_path, sizeof(exe_path) - 1);
    if (len != -1) {
        exe_path[len] = '\0';
        char* last_slash = strrchr(exe_path, '/');
        if (last_slash) *last_slash = '\0';
    } else {
        return 1;
    }

    if (!part_spec) {
        if (!run_part(year, day, 1)) {
            printc(RED, "Failed");
            printf("\n");
        }
        if (!run_part(year, day, 2)) {
            printc(RED, "Failed");
            printf("\n");
        }
    } else {
        if (!run_part(year, day, (!strcmp(part, "1") ? 1 : 2))) {
            printc(RED, "Failed");
            printf("\n");
        }
    }

    return 0;
}
