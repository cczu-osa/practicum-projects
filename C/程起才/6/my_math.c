//
// Created by Richard Chien on 6/18/16.
//

#include "my_math.h"

#include <stdarg.h>

int max_int(int count, ...) {
    va_list ap;
    va_start(ap, count);
    int max = va_arg(ap, int);
    for (int i = 0; i < count - 1; i++) {
        int cur = va_arg(ap, int);
        max = cur > max ? cur : max;
    }
    return max;
}

int min_int(int count, ...) {
    va_list ap;
    va_start(ap, count);
    int min = va_arg(ap, int);
    for (int i = 0; i < count - 1; i++) {
        int cur = va_arg(ap, int);
        min = cur < min ? cur : min;
    }
    return min;
}

double avg_int(int count, ...) {
    va_list ap;
    va_start(ap, count);
    int sum = 0;
    for (int i = 0; i < count; i++) {
        sum += va_arg(ap, int);
    }
    return sum / count;
}

double max_double(int count, ...) {
    va_list ap;
    va_start(ap, count);
    double max = va_arg(ap, double);
    for (int i = 0; i < count - 1; i++) {
        double cur = va_arg(ap, double);
        max = cur > max ? cur : max;
    }
    return max;
}

double min_double(int count, ...) {
    va_list ap;
    va_start(ap, count);
    double min = va_arg(ap, double);
    for (int i = 0; i < count - 1; i++) {
        double cur = va_arg(ap, double);
        min = cur < min ? cur : min;
    }
    return min;
}

double avg_double(int count, ...) {
    va_list ap;
    va_start(ap, count);
    double sum = 0;
    for (int i = 0; i < count; i++) {
        sum += va_arg(ap, double);
    }
    return sum / count;
}
