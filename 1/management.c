//
// Created by Richard Chien on 6/19/16.
//

#include "management.h"

int studentCompareById(const void *s1, const void *s2);

int studentCompareByName(const void *s1, const void *s2);

FILE *stream;

void writeStudentToStream(void *s);

bool addStudent(Array *studentArr, const Student *stu) {
    int pos = ArrayFind(studentArr, stu, &studentCompareById);
    if (pos < 0) {
        return ArrayAppendItem(studentArr, stu);
    } else {
        return true;
    }
}

bool removeStudent(Array *studentArr, const Student *stu) {
    int pos = ArrayFind(studentArr, stu, &objectCompare);
    if (pos < 0) {
        return true;
    } else {
        return ArrayDeleteItem(studentArr, pos);
    }
}

bool removeStudentById(Array *studentArr, int id) {
    Student tempStu;
    tempStu.id = id;
    int pos = ArrayFind(studentArr, &tempStu, &studentCompareById);
    if (pos < 0) {
        return true;
    } else {
        return ArrayDeleteItem(studentArr, pos);
    }
}

bool removeStudentByName(Array *studentArr, const char *name) {
    Student tempStu;
    strcpy(tempStu.name, name);
    int pos = ArrayFind(studentArr, &tempStu, &studentCompareByName);
    if (pos < 0) {
        return true;
    } else {
        return ArrayDeleteItem(studentArr, pos);
    }
}

bool findStudentById(Array *studentArr, int id, Student *out) {
    Student tempStu;
    tempStu.id = id;
    int pos = ArrayFind(studentArr, &tempStu, &studentCompareById);
    if (pos < 0) {
        return false;
    }
    return ArrayGetItem(studentArr, pos, out);
}

bool findStudentByName(Array *studentArr, const char *name, Student *out) {
    Student tempStu;
    strcpy(tempStu.name, name);
    int pos = ArrayFind(studentArr, &tempStu, &studentCompareByName);
    if (pos < 0) {
        return false;
    }
    return ArrayGetItem(studentArr, pos, out);
}

bool updateStudent(Array *studentArr, const Student *stu) {
    int pos = ArrayFind(studentArr, stu, &studentCompareById);
    if (pos < 0) {
        return false;
    }
    return ArraySetItem(studentArr, pos, stu);
}

void saveStudentArrayToDisk(Array *stuArr, const char *path) {
    stream = fopen(path, "w");
    ArrayTraverse(stuArr, &writeStudentToStream);
    fclose(stream);
}

Array *restoreStudentArrayFromDisk(const char *path) {
    stream = fopen(path, "r");
    Array *stuArr = ArrayInit(sizeof(Student));
    Student stu;
    while (fread(&stu, sizeof(Student), 1, stream) > 0) {
        ArrayAppendItem(stuArr, &stu);
    }
    fclose(stream);
    return stuArr;
}

#pragma mark - Inner Functions

int studentCompareById(const void *s1, const void *s2) {
    Student *stu1 = (Student *) s1;
    Student *stu2 = (Student *) s2;
    return stu1->id - stu2->id;
}

int studentCompareByName(const void *s1, const void *s2) {
    Student *stu1 = (Student *) s1;
    Student *stu2 = (Student *) s2;
    return strcmp(stu1->name, stu2->name);
}

void writeStudentToStream(void *s) {
    fwrite(s, sizeof(Student), 1, stream);
}
