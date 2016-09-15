//
// Created by Richard Chien on 6/19/16.
//

#include "management.h"

int compareById(const void *s1, const void *s2) {
    Student *stu1 = (Student *) s1;
    Student *stu2 = (Student *) s2;
    return stu1->id - stu2->id;
}

int compareByName(const void *s1, const void *s2) {
    Student *stu1 = (Student *) s1;
    Student *stu2 = (Student *) s2;
    return strcmp(stu1->name, stu2->name);
}

int compareByCourse(const void *s1, const void *s2, int course) {
    Student *stu1 = (Student *) s1;
    Student *stu2 = (Student *) s2;
    if (stu1->score[course] > stu2->score[course]) {
        return 1;
    } else if (stu1->score[course] < stu2->score[course]) {
        return -1;
    } else {
        return 0;
    }
}

int compareByMath(const void *s1, const void *s2) {
    return compareByCourse(s1, s2, COURSE_MATH);
}

int compareByEnglish(const void *s1, const void *s2) {
    return compareByCourse(s1, s2, COURSE_ENGLISH);
}

int compareByC(const void *s1, const void *s2) {
    return compareByCourse(s1, s2, COURSE_C);
}

int compareByPE(const void *s1, const void *s2) {
    return compareByCourse(s1, s2, COURSE_PE);
}

int compareByTotal(const void *s1, const void *s2) {
    Student *stu1 = (Student *) s1;
    Student *stu2 = (Student *) s2;

    float t1 = 0.0f, t2 = 0.0f;
    for (int i = 0; i < COURSE_TOTAL; i++) {
        t1 += stu1->score[i];
        t2 += stu2->score[i];
    }

    if (t1 > t2) {
        return 1;
    } else if (t1 < t2) {
        return -1;
    } else {
        return 0;
    }
}

FILE *stream;

void writeStudentToStream(void *s) {
    fwrite(s, sizeof(Student), 1, stream);
}

BOOL addStudent(LinkedList *list, Student *stu) {
    int pos = LinkedListFind(list, stu, &compareById);
    if (pos < 0) {
        return LinkedListAppendItem(list, stu);
    } else {
        return TRUE;
    }
}

BOOL findStudentById(LinkedList *list, int id, Student *out) {
    Student tempStu;
    tempStu.id = id;
    int pos = LinkedListFind(list, &tempStu, &compareById);
    if (pos < 0) {
        return FALSE;
    }
    return LinkedListGetItem(list, pos, out);
}

BOOL findStudentByName(LinkedList *list, const char *name, Student *out) {
    Student tempStu;
    strcpy(tempStu.name, name);
    int pos = LinkedListFind(list, &tempStu, &compareByName);
    if (pos < 0) {
        return FALSE;
    }
    return LinkedListGetItem(list, pos, out);
}

BOOL removeStudentById(LinkedList *list, int id) {
    Student tempStu;
    tempStu.id = id;
    int pos = LinkedListFind(list, &tempStu, &compareById);
    if (pos < 0) {
        return TRUE;
    } else {
        return LinkedListDeleteItem(list, pos);
    }
}

BOOL removeStudentByName(LinkedList *list, const char *name) {
    Student tempStu;
    strcpy(tempStu.name, name);
    int pos = LinkedListFind(list, &tempStu, &compareByName);
    if (pos < 0) {
        return TRUE;
    } else {
        return LinkedListDeleteItem(list, pos);
    }
}

void sortById(LinkedList *list) {
    LinkedListSort(list, &compareById, TRUE);
}

void sortByScore(LinkedList *list, int course) {
    switch (course) {
        case COURSE_MATH:
            LinkedListSort(list, &compareByMath, FALSE);
            break;
        case COURSE_ENGLISH:
            LinkedListSort(list, &compareByEnglish, FALSE);
            break;
        case COURSE_C:
            LinkedListSort(list, &compareByC, FALSE);
            break;
        case COURSE_PE:
            LinkedListSort(list, &compareByPE, FALSE);
            break;
        case COURSE_TOTAL:
            LinkedListSort(list, &compareByTotal, FALSE);
            break;
        default:
            break;
    }
}

void saveStudentListToDisk(LinkedList *list, const char *path) {
    stream = fopen(path, "w");
    LinkedListTraverse(list, &writeStudentToStream);
    fclose(stream);
}

LinkedList *restoreStudentListFromDisk(const char *path) {
    stream = fopen(path, "r");
    LinkedList *list = LinkedListInit(sizeof(Student));
    Student stu;
    while (fread(&stu, sizeof(Student), 1, stream) > 0) {
        LinkedListAppendItem(list, &stu);
    }
    fclose(stream);
    return list;
}
