//
// Created by Richard Chien on 6/19/16.
//

#include <stdio.h>
#include "basic.h"

int objectCompare(const void *p1, const void *p2) {
    return (int) (p1 - p2);
}

bool fileExists(const char *path) {
    FILE *fp = fopen(path, "r");
    if (fp != NULL) {
        fclose(fp);
        return true;
    } else {
        return false;
    }
}
