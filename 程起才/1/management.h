//
// Created by Richard Chien on 6/19/16.
//

#include <stdbool.h>
#include <string.h>
#include "basic.h"
#include "DataStructure/DynamicArray.h"
#include "student.h"

#ifndef STUDENTMANAGEMENTSYSTEM_MANAGEMENT_SYSTEM_H
#define STUDENTMANAGEMENTSYSTEM_MANAGEMENT_SYSTEM_H

// Add a new student to the student array,
// return true if added successfully, or the student is already exists.
bool addStudent(Array *studentArr, const Student *stu);

// Remove a student from the student array,
// return true if removed successfully, or the student is not exists.
bool removeStudent(Array *studentArr, const Student *stu);

bool removeStudentById(Array *studentArr, int id);

bool removeStudentByName(Array *studentArr, const char *name);

bool findStudentById(Array *studentArr, int id, Student *out);

bool findStudentByName(Array *studentArr, const char *name, Student *out);

bool updateStudent(Array *studentArr, const Student *stu);

void saveStudentArrayToDisk(Array *stuArr, const char *path);

Array *restoreStudentArrayFromDisk(const char *path);

#endif //STUDENTMANAGEMENTSYSTEM_MANAGEMENT_SYSTEM_H
