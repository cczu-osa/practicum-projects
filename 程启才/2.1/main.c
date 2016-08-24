#include <stdio.h>
#include "LinkedList.h"
#include "student.h"
#include "management.h"

const char *FILE_PATH = "data.out";

LinkedList *gStuList;

void save();

void add();

void del();

void find();

void findById();

void findByName();

void deleteById();

void deleteByName();

void displayTable();

void printTable();

void clearInput() {
    // 清除缓存区的其他字符
    while (getchar() != '\n');
}

void printStudent(Student stu) {
    float totalScore = 0.0f;
    for (int i = 0; i < COURSE_TOTAL; i++) {
        totalScore += stu.score[i];
    }
    printf("学号: %d\n"
                   "姓名: %s\n"
                   "数学成绩: %.1f\n"
                   "英语成绩: %.1f\n"
                   "C语言成绩: %.1f\n"
                   "体育成绩: %.1f\n"
                   "总成绩: %.1f\n",
           stu.id, stu.name, stu.score[COURSE_MATH], stu.score[COURSE_ENGLISH], stu.score[COURSE_C],
           stu.score[COURSE_PE], totalScore);
}

BOOL fileExists(const char *path) {
    FILE *fp = fopen(path, "r");
    if (fp != NULL) {
        fclose(fp);
        return TRUE;
    } else {
        return FALSE;
    }
}

int main() {
    printf("\n\n---------成绩统计管理系统---------\n\n");

    if (fileExists(FILE_PATH)) {
        gStuList = restoreStudentListFromDisk(FILE_PATH);
    } else {
        gStuList = LinkedListInit(sizeof(Student));
    }

    int choice;
    // 主循环
    for (; ;) {
        printf("0. 退出并保存\n"
                       "1. 添加学生\n"
                       "2. 删除学生\n"
                       "3. 查询学生成绩\n"
                       "4. 显示成绩表\n");
        printf("请输入选项: ");
        scanf("%d", &choice);
        switch (choice) {
            case 0:
                save();
                LinkedListDestroy(gStuList);
                return 0;
            case 1:
                add();
                break;
            case 2:
                del();
                break;
            case 3:
                find();
                break;
            case 4:
                displayTable();
                break;
            default:
                printf("输入的选项有误.\n\n");
                break;
        }
    }
}

void save() {
    saveStudentListToDisk(gStuList, FILE_PATH);
}

void add() {
    printf("\n");

    Student stu;
    printf("请输入学生信息:\n");

    printf("学号: ");
    scanf("%d", &stu.id);

    printf("姓名: ");
    clearInput();
    gets(stu.name);

    printf("请输入学生成绩:\n");

    printf("数学: ");
    scanf("%f", &stu.score[COURSE_MATH]);

    printf("英语: ");
    scanf("%f", &stu.score[COURSE_ENGLISH]);

    printf("C语言: ");
    scanf("%f", &stu.score[COURSE_C]);

    printf("体育: ");
    scanf("%f", &stu.score[COURSE_PE]);

    printf("\n即将添加该学生:\n");
    printStudent(stu);
    printf("\n");

    printf("回车继续, 输入其它任意字符取消...");
    clearInput();
    if (getchar() != '\n') {
        clearInput();
        printf("已取消.\n\n");
        return;
    }

    if (addStudent(gStuList, &stu)) {
        printf("添加成功!\n\n");
    } else {
        printf("添加失败, 请重试.\n\n");
    }
}

void del() {
    printf("\n");

    int choice;
    printf("0. 返回上一级\n"
                   "1. 根据学号删除\n"
                   "2. 根据姓名删除\n");

    printf("请输入选项: ");
    scanf("%d", &choice);
    switch (choice) {
        case 0:
            break;
        case 1:
            deleteById();
            break;
        case 2:
            deleteByName();
            break;
        default:
            break;
    }
}

void deleteById() {
    printf("\n");

    int id;
    printf("请输入要删除的学生学号: ");
    scanf("%d", &id);
    Student stu;
    if (findStudentById(gStuList, id, &stu)) {
        printf("\n即将删除该学生:\n");
        printStudent(stu);
        printf("\n");

        printf("回车继续, 输入其它任意字符取消...");
        clearInput();
        if (getchar() != '\n') {
            clearInput();
            printf("已取消.\n\n");
            return;
        }

        if (removeStudentById(gStuList, id)) {
            printf("已删除成功.\n\n");
        } else {
            printf("删除失败, 请重试.\n\n");
        }
    } else {
        printf("很抱歉没找到.\n\n");
    }
}

void deleteByName() {
    printf("\n");

    char name[MAX_NAME_LEN];
    printf("请输入你要删除的学生姓名: ");
    scanf("%s", name);
    Student stu;
    if (findStudentByName(gStuList, name, &stu)) {
        printf("\n即将删除该学生:\n");
        printStudent(stu);
        printf("\n");

        printf("回车继续, 输入其它任意字符取消...");
        clearInput();
        if (getchar() != '\n') {
            clearInput();
            printf("已取消.\n\n");
            return;
        }

        if (removeStudentByName(gStuList, name)) {
            printf("已删除成功.\n\n");
        } else {
            printf("删除失败, 请重试.\n\n");
        }
    } else {
        printf("很抱歉没找到.\n\n");
    }
}

void find() {
    printf("\n");

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
    printf("\n");

    int id;
    printf("请输入要查找的学生学号: ");
    scanf("%d", &id);
    Student stu;
    if (findStudentById(gStuList, id, &stu)) {
        printf("\n你要查找的学生信息如下:\n");
        printStudent(stu);
        printf("\n");
    } else {
        printf("很抱歉没找到.\n\n");
    }
}

void findByName() {
    printf("\n");

    char name[MAX_NAME_LEN];
    printf("请输入你要查找的学生姓名: ");
    scanf("%s", name);
    Student stu;
    if (findStudentByName(gStuList, name, &stu)) {
        printf("\n你要查找的学生信息如下:\n");
        printStudent(stu);
        printf("\n");
    } else {
        printf("很抱歉没找到该学生.\n\n");
    }
}

void displayTable() {
    printf("\n");

    int choice;
    printf("0. 返回上一级\n"
                   "1. 按学号排序\n"
                   "2. 按数学成绩排序\n"
                   "3. 按英语成绩排序\n"
                   "4. 按C语言成绩排序\n"
                   "5. 按体育成绩排序\n"
                   "6. 按总成绩排序\n");

    printf("请输入选项: ");
    scanf("%d", &choice);
    switch (choice) {
        case 0:
            break;
        case 1:
            sortById(gStuList);
            printTable();
            break;
        case 2:
            sortByScore(gStuList, COURSE_MATH);
            printTable();
            break;
        case 3:
            sortByScore(gStuList, COURSE_ENGLISH);
            printTable();
            break;
        case 4:
            sortByScore(gStuList, COURSE_C);
            printTable();
            break;
        case 5:
            sortByScore(gStuList, COURSE_PE);
            printTable();
            break;
        case 6:
            sortByScore(gStuList, COURSE_TOTAL);
            printTable();
            break;
        default:
            break;
    }
}

void printTableItem(void *item) {
    Student *stu = (Student *) item;
    float totalScore = 0.0f;
    for (int i = 0; i < COURSE_TOTAL; i++) {
        totalScore += stu->score[i];
    }
    printf("%-10d%-12s%-6.1f%-6.1f%-6.1f%-6.1f%.1f\n", stu->id, stu->name, stu->score[COURSE_MATH],
           stu->score[COURSE_ENGLISH],
           stu->score[COURSE_C], stu->score[COURSE_PE], totalScore);
}

void printTable() {
    printf("\n学号       姓名      数学   英语   C语言   体育   总分\n");
    for (int i = 0; i < 52; i++) {
        printf("-");
    }
    printf("\n");
    LinkedListTraverse(gStuList, &printTableItem);
    printf("\n");
};
