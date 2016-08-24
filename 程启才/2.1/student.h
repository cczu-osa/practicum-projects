//
// Created by Richard Chien on 6/23/16.
//

#ifndef __STUDENT__
#define __STUDENT__

#define MAX_NAME_LEN 20
#define COURSE_TOTAL 4
#define COURSE_MATH 0
#define COURSE_ENGLISH 1
#define COURSE_C 2
#define COURSE_PE 3

typedef struct _Student {
    int id;
    char name[MAX_NAME_LEN];
    float score[COURSE_TOTAL];
} Student;

#endif //__STUDENT__
