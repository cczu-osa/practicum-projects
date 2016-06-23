//
// Created by Richard Chien on 6/19/16.
//

#include "LinkedList.h"
#include "student.h"

#ifndef __MANAGEMENT__
#define __MANAGEMENT__

BOOL addStudent(LinkedList *list, Student *stu);

BOOL findStudentById(LinkedList *list, int id, Student *out);

BOOL findStudentByName(LinkedList *list, const char *name, Student *out);

BOOL removeStudentById(LinkedList *list, int id);

BOOL removeStudentByName(LinkedList *list, const char *name);

void sortById(LinkedList *list);

void sortByScore(LinkedList *list, int course);

void saveStudentListToDisk(LinkedList *list, const char *path);

LinkedList *restoreStudentListFromDisk(const char *path);

#endif //__MANAGEMENT__
