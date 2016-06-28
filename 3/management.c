#include "management.h"

int compareById(const void *s1, const void *s2) {
    Staff *staff1 = (Staff *) s1;
    Staff *staff2 = (Staff *) s2;
    return staff1->id - staff2->id;
}

int compareByName(const void *s1, const void *s2) {
    Staff *staff1 = (Staff *) s1;
    Staff *staff2 = (Staff *) s2;
    return strcmp(staff1->name, staff2->name);
}

int compareByAge(const void *s1, const void *s2) {
    Staff *staff1 = (Staff *) s1;
    Staff *staff2 = (Staff *) s2;
    return staff1->age - staff2->age;
}

int compareBySalary(const void *s1, const void *s2) {
    Staff *staff1 = (Staff *) s1;
    Staff *staff2 = (Staff *) s2;
    if (staff1->salary > staff2->salary) {
        return 1;
    } else if (staff1->salary < staff2->salary) {
        return -1;
    }
    return 0;
}

FILE *stream;

void writeStudentToStream(void *s) {
    fwrite(s, sizeof(Staff), 1, stream);
}

BOOL addStaff(LinkedList *list, Staff *staff) {
    int pos = LinkedListFind(list, staff, &compareById);
    if (pos < 0) {
        return LinkedListAppendItem(list, staff);
    } else {
        return TRUE;
    }
}

BOOL findStaffById(LinkedList *list, int id, Staff *out) {
    Staff tempStu;
    tempStu.id = id;
    int pos = LinkedListFind(list, &tempStu, &compareById);
    if (pos < 0) {
        return FALSE;
    }
    return LinkedListGetItem(list, pos, out);
}

BOOL findStaffByName(LinkedList *list, const char *name, Staff *out) {
    Staff tempStu;
    strcpy(tempStu.name, name);
    int pos = LinkedListFind(list, &tempStu, &compareByName);
    if (pos < 0) {
        return FALSE;
    }
    return LinkedListGetItem(list, pos, out);
}

BOOL removeStaffById(LinkedList *list, int id) {
    Staff tempStu;
    tempStu.id = id;
    int pos = LinkedListFind(list, &tempStu, &compareById);
    if (pos < 0) {
        return TRUE;
    } else {
        return LinkedListDeleteItem(list, pos);
    }
}

BOOL removeStaffByName(LinkedList *list, const char *name) {
    Staff tempStu;
    strcpy(tempStu.name, name);
    int pos = LinkedListFind(list, &tempStu, &compareByName);
    if (pos < 0) {
        return TRUE;
    } else {
        return LinkedListDeleteItem(list, pos);
    }
}

BOOL updateStaffById(LinkedList *list, const Staff *staff) {
    int pos = LinkedListFind(list, &staff, &compareById);
    if (pos < 0) {
        return FALSE;
    }
    return LinkedListSetItem(list, pos, staff);
}

void sortById(LinkedList *list) {
    LinkedListSort(list, &compareById, TRUE);
}

void sortByName(LinkedList *list) {
    LinkedListSort(list, &compareByName, TRUE);
}

void sortByAge(LinkedList *list) {
    LinkedListSort(list, &compareByAge, TRUE);
}

void sortBySalary(LinkedList *list) {
    LinkedListSort(list, &compareBySalary, FALSE);
}

void saveStaffListToDisk(LinkedList *list, const char *path) {
    stream = fopen(path, "w");
    LinkedListTraverse(list, &writeStudentToStream);
    fclose(stream);
}

LinkedList *restoreStaffListFromDisk(const char *path) {
    stream = fopen(path, "r");
    LinkedList *list = LinkedListInit(sizeof(Staff));
    Staff staff;
    while (fread(&staff, sizeof(Staff), 1, stream) > 0) {
        LinkedListAppendItem(list, &staff);
    }
    fclose(stream);
    return list;
}
