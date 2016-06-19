//
// Created by Richard Chien on 16/5/5.
//

#include <stdarg.h>
#include <time.h>
#include <stdio.h>
#include "log_utils.h"

const int LEVEL = NOTHING;

void print_now_time();

void log(int level, const char *tag, const char *msg, va_list ap);

void log_v(const char *tag, const char *msg, ...) {
    va_list ap;
    va_start(ap, msg);
    log(VERBOSE, tag, msg, ap);
    va_end(ap);
}

void log_d(const char *tag, const char *msg, ...) {
    va_list ap;
    va_start(ap, msg);
    log(DEBUG, tag, msg, ap);
    va_end(ap);
}

void log_i(const char *tag, const char *msg, ...) {
    va_list ap;
    va_start(ap, msg);
    log(INFO, tag, msg, ap);
    va_end(ap);
}

void log_w(const char *tag, const char *msg, ...) {
    va_list ap;
    va_start(ap, msg);
    log(WARNING, tag, msg, ap);
    va_end(ap);
}

void log_e(const char *tag, const char *msg, ...) {
    va_list ap;
    va_start(ap, msg);
    log(ERROR, tag, msg, ap);
    va_end(ap);
}

void print_now_time() {
    time_t t = time(NULL);
    struct tm time = *localtime(&t);
    printf("%02d-%02d %02d:%02d:%02d", time.tm_mon, time.tm_mday, time.tm_hour, time.tm_min, time.tm_sec);
}

void log(int level, const char *tag, const char *msg, va_list ap) {
    char log_level_ch = 0;
    switch (level) {
        case VERBOSE:
            log_level_ch = 'v';
            break;
        case DEBUG:
            log_level_ch = 'd';
            break;
        case INFO:
            log_level_ch = 'i';
            break;
        case WARNING:
            log_level_ch = 'w';
            break;
        case ERROR:
            log_level_ch = 'e';
            break;
        default:
            break;
    }

    if (level >= LEVEL) {
        printf("LOG ");
        print_now_time();
        printf(" %c/%s: ", log_level_ch, tag);
        vprintf(msg, ap);
        putchar('\n');
    }
}
