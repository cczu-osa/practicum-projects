#include <stdio.h>
#include "DataStructure/DynamicArray.h"
#include "student.h"
#include "management.h"
#include "LogUtils/log_utils.h"

const char *FILE_PATH = "data.out";

Array *gStuArr;

void save();

void add();

void delete();

void find();

void findById();

void findByName();

void update();

void clearInput() {
    while (getchar() != '\n');
}

void Student_log(Student stu) {
    log_d("Student_log", "Name: %s, Id: %d, Major: %s, Class: %d",
          stu.name, stu.id, stu.major, stu.class_id);
}

int main() {
    printf("\n\n---------学籍管理系统 v1.0.0---------\n\n");

    if (fileExists(FILE_PATH)) {
        gStuArr = restoreStudentArrayFromDisk(FILE_PATH);
    } else {
        gStuArr = ArrayInit(sizeof(Student));
    }

    int choice;
    // Main loop
    for (; ;) {
        printf("0. 退出并保存\n"
                       "1. 添加学生\n"
                       "2. 删除学生\n"
                       "3. 查找学生\n"
                       "4. 修改学生信息\n");
        printf("请输入选项: ");
        scanf("%d", &choice);
        switch (choice) {
            case 0:
                save();
                ArrayDestroy(gStuArr);
                return 0;
            case 1:
                add();
                break;
            case 2:
                delete();
                break;
            case 3:
                find();
                break;
            case 4:
                update();
                break;
            default:
                break;
        }
    }
}

void save() {
    saveStudentArrayToDisk(gStuArr, FILE_PATH);
}

void add() {
    Student stu;
    printf("请输入学生信息:\n");

    printf("姓名: ");
    clearInput();
    gets(stu.name);
    log_d("add()", "Name: %s", stu.name);

    printf("学号: ");
    scanf("%d", &stu.id);
    log_d("add()", "Id: %d", stu.id);

    printf("专业: ");
    clearInput();
    gets(stu.major);
    log_d("add()", "Major: %s", stu.major);

    printf("班级: ");
    scanf("%d", &stu.class_id);
    log_d("add()", "Class Id: %d", stu.class_id);

    printf("即将添加该学生:\n");
    printf("姓名: %s\n"
                   "学号: %d\n"
                   "专业: %s\n"
                   "班级: %d\n",
           stu.name, stu.id, stu.major, stu.class_id);

    printf("回车继续, 输入其它任意字符取消...");
    clearInput();
    if (getchar() != '\n') {
        clearInput();
        printf("已取消.\n\n");
        return;
    }

    if (addStudent(gStuArr, &stu)) {
        printf("添加成功!\n\n");
    } else {
        printf("添加失败, 请重试.\n\n");
    }
}

void delete() {
    int id;
    printf("请输入要删除的学生学号: ");
    scanf("%d", &id);
    Student stu;
    if (findStudentById(gStuArr, id, &stu)) {
        Student_log(stu);
        printf("即将删除该学生:\n");
        printf("姓名: %s\n"
                       "学号: %d\n"
                       "专业: %s\n"
                       "班级: %d\n",
               stu.name, stu.id, stu.major, stu.class_id);

        printf("回车继续, 输入其它任意字符取消...");
        clearInput();
        if (getchar() != '\n') {
            clearInput();
            printf("已取消.\n\n");
            return;
        }

        if (removeStudentById(gStuArr, id)) {
            printf("已删除成功.\n\n");
        } else {
            printf("删除失败, 请重试.\n\n");
        }
    } else {
        printf("很抱歉没找到.\n\n");
    }
}

void find() {
    int choice;
    printf("0. 返回上一级\n"
                   "1. 根据学号查找\n"
                   "2. 根据姓名查找\n");

    printf("请输入选项: ");
    scanf("%d", &choice);
    switch (choice) {
        case 0:
            break;
        case 1:
            findById();
            break;
        case 2:
            findByName();
            break;
        default:
            break;
    }
}

void findById() {
    int id;
    printf("请输入要查找的学生学号: ");
    scanf("%d", &id);
    Student stu;
    if (findStudentById(gStuArr, id, &stu)) {
        Student_log(stu);
        printf("你要查找的学生信息如下:\n");
        printf("姓名: %s\n"
                       "学号: %d\n"
                       "专业: %s\n"
                       "班级: %d\n\n",
               stu.name, stu.id, stu.major, stu.class_id);
    } else {
        printf("很抱歉没找到.\n\n");
    }
}

void findByName() {
    char name[30];
    printf("请输入你要查找的学生姓名: ");
    scanf("%s", name);
    Student stu;
    if (findStudentByName(gStuArr, name, &stu)) {
        Student_log(stu);
        printf("你要查找的学生信息如下:\n");
        printf("姓名: %s\n"
                       "学号: %d\n"
                       "专业: %s\n"
                       "班级: %d\n\n",
               stu.name, stu.id, stu.major, stu.class_id);
    } else {
        printf("很抱歉没找到该学生.\n\n");
    }
}

void update() {
    int id;
    printf("请输入要修改的学生学号: ");
    scanf("%d", &id);
    Student stu;
    if (findStudentById(gStuArr, id, &stu)) {
        Student_log(stu);
        printf("找到该学生目前信息为:\n");
        printf("姓名: %s\n"
                       "学号: %d\n"
                       "专业: %s\n"
                       "班级: %d\n",
               stu.name, stu.id, stu.major, stu.class_id);

        printf("请输入新的信息:\n");

        printf("姓名: ");
        clearInput();
        gets(stu.name);
        log_d("add()", "Name: %s", stu.name);

        printf("专业: ");
        gets(stu.major);
        log_d("add()", "Major: %s", stu.major);

        printf("班级: ");
        scanf("%d", &stu.class_id);
        log_d("add()", "Class Id: %d", stu.class_id);

        printf("即将修改该学生:\n");
        printf("姓名: %s\n"
                       "学号: %d\n"
                       "专业: %s\n"
                       "班级: %d\n",
               stu.name, stu.id, stu.major, stu.class_id);


        printf("回车继续, 输入其它任意字符取消...");
        clearInput();
        if (getchar() != '\n') {
            clearInput();
            printf("已取消.\n\n");
            return;
        }

        if (updateStudent(gStuArr, &stu)) {
            printf("已修改成功.\n\n");
        } else {
            printf("修改失败, 请重试.\n\n");
        }
    } else {
        printf("很抱歉没找到.\n\n");
    }
}
