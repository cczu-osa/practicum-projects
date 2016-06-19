//
// Created by Richard Chien on 16/5/5.
//

#ifndef TEST_LOG_UTILS_H
#define TEST_LOG_UTILS_H

#define VERBOSE 0
#define DEBUG 1
#define INFO 2
#define WARNING 3
#define ERROR 4
#define NOTHING 5

void log_v(const char *tag, const char *msg, ...);

void log_d(const char *tag, const char *msg, ...);

void log_i(const char *tag, const char *msg, ...);

void log_w(const char *tag, const char *msg, ...);

void log_e(const char *tag, const char *msg, ...);

#endif //TEST_LOG_UTILS_H
