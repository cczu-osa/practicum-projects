#include "LinkedList.h"
#include "staff.h"

#ifndef __MANAGEMENT__
#define __MANAGEMENT__

BOOL addStaff(LinkedList *list, Staff *staff);

BOOL findStaffById(LinkedList *list, int id, Staff *out);

BOOL findStaffByName(LinkedList *list, const char *name, Staff *out);

BOOL removeStaffById(LinkedList *list, int id);

BOOL removeStaffByName(LinkedList *list, const char *name);

BOOL updateStaffById(LinkedList *list, const Staff *staff);

void sortById(LinkedList *list);

void sortByName(LinkedList *list);

void sortByAge(LinkedList *list);

void sortBySalary(LinkedList *list);

void saveStaffListToDisk(LinkedList *list, const char *path);

LinkedList *restoreStaffListFromDisk(const char *path);

#endif //__MANAGEMENT__
