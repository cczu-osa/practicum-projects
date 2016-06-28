#include <stdio.h>
#include "LinkedList.h"
#include "staff.h"
#include "management.h"

// 保存文件的文件名
const char *FILE_PATH = "data.out";

// 用来存职工信息的链表
LinkedList *gStaffList;

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

void update();

void retire();

void clearInput() {
    // 清除缓存区的其他字符
    while (getchar() != '\n');
}

// 打印职工信息
void printStaff(Staff staff) {
    printf("工号: %08d\n"
                   "姓名: %s\n"
                   "年龄: %d\n"
                   "性别: %s\n"
                   "学历: %d\n"
                   "工资: %.1f\n"
                   "邮箱: %s\n"
                   "电话: %s\n",
           staff.id, staff.name, staff.age, staff.sex ? "男" : "女", staff.education, staff.salary,
           staff.email, staff.phone);
}

// 判断指定的文件是否已经存在
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
    printf("\n\n---------职工信息管理系统---------\n\n");

    if (fileExists(FILE_PATH)) {
        gStaffList = restoreStaffListFromDisk(FILE_PATH);
    } else {
        gStaffList = LinkedListInit(sizeof(Staff));
    }

    int choice;
    // 主循环, 不断接受用户的选择, 直到输入0退出
    for (; ;) {
        printf("0. 退出并保存\n"
                       "1. 添加职工\n"
                       "2. 删除职工\n"
                       "3. 查询职工信息\n"
                       "4. 修改职工信息\n"
                       "5. 显示职工信息表\n"
                       "6. 查询即将退休的职工\n");
        printf("请输入选项: ");
        scanf("%d", &choice);
        switch (choice) {
            case 0:
                save();
                LinkedListDestroy(gStaffList);
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
                update();
                break;
            case 5:
                displayTable();
                break;
            case 6:
                retire();
                break;
            default:
                printf("输入的选项有误.\n\n");
                break;
        }
    }
}

void save() {
    // 把所有职工信息保存到文件
    saveStaffListToDisk(gStaffList, FILE_PATH);
}

void add() {
    // 添加职工
    printf("\n");

    Staff staff;
    printf("请输入职工信息:\n");

    printf("工号: ");
    scanf("%d", &staff.id);

    printf("姓名: ");
    clearInput();
    gets(staff.name);

    printf("年龄: ");
    scanf("%d", &staff.age);

    printf("性别(1表示男, 0表示女): ");
    scanf("%d", &staff.sex);

    printf("学历:\n");
    printf("1. 本科\n2. 硕士\n3. 博士\n4. 其它\n");
    printf(": ");
    int edu;
    scanf("%d", &edu);
    switch (edu) {
        case 1:
            staff.education = EDU_UNDERGRADUATE;
            break;
        case 2:
            staff.education = EDU_MASTER;
            break;
        case 3:
            staff.education = EDU_DOCTOR;
            break;
        case 4:
            staff.education = EDU_OTHER;
            break;
        default:
            printf("输入的信息有误!");
            return;
    }

    printf("工资: ");
    scanf("%f", &staff.salary);

    printf("邮箱: ");
    scanf("%s", staff.email);

    printf("手机号: ");
    scanf("%s", staff.phone);

    printf("\n即将添加该职工:\n");
    printStaff(staff);
    printf("\n");

    printf("回车继续, 输入其它任意字符取消...");
    clearInput();
    if (getchar() != '\n') {
        clearInput();
        printf("已取消.\n\n");
        return;
    }

    if (addStaff(gStaffList, &staff)) {
        printf("添加成功!\n\n");
    } else {
        printf("添加失败, 请重试.\n\n");
    }
}

void del() {
    // 删除职工
    printf("\n");

    int choice;
    printf("0. 返回上一级\n"
                   "1. 根据工号删除\n"
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
    // 根据工号删除职工
    printf("\n");

    int id;
    printf("请输入要删除的职工工号: ");
    scanf("%d", &id);
    Staff stu;
    if (findStaffById(gStaffList, id, &stu)) {
        printf("\n即将删除该职工:\n");
        printStaff(stu);
        printf("\n");

        printf("回车继续, 输入其它任意字符取消...");
        clearInput();
        if (getchar() != '\n') {
            clearInput();
            printf("已取消.\n\n");
            return;
        }

        if (removeStaffById(gStaffList, id)) {
            printf("已删除成功.\n\n");
        } else {
            printf("删除失败, 请重试.\n\n");
        }
    } else {
        printf("很抱歉没找到.\n\n");
    }
}

void deleteByName() {
    // 根据姓名删除职工
    printf("\n");

    char name[MAX_NAME_LEN];
    printf("请输入你要删除的职工姓名: ");
    scanf("%s", name);
    Staff stu;
    if (findStaffByName(gStaffList, name, &stu)) {
        printf("\n即将删除该职工:\n");
        printStaff(stu);
        printf("\n");

        printf("回车继续, 输入其它任意字符取消...");
        clearInput();
        if (getchar() != '\n') {
            clearInput();
            printf("已取消.\n\n");
            return;
        }

        if (removeStaffByName(gStaffList, name)) {
            printf("已删除成功.\n\n");
        } else {
            printf("删除失败, 请重试.\n\n");
        }
    } else {
        printf("很抱歉没找到.\n\n");
    }
}

void find() {
    // 查找职工
    printf("\n");

    int choice;
    printf("0. 返回上一级\n"
                   "1. 根据工号查找\n"
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
    // 根据工号查找职工
    printf("\n");

    int id;
    printf("请输入要查找的职工工号: ");
    scanf("%d", &id);
    Staff stu;
    if (findStaffById(gStaffList, id, &stu)) {
        printf("\n你要查找的职工信息如下:\n");
        printStaff(stu);
        printf("\n");
    } else {
        printf("很抱歉没找到.\n\n");
    }
}

void findByName() {
    // 根据姓名查找职工
    printf("\n");

    char name[MAX_NAME_LEN];
    printf("请输入你要查找的职工姓名: ");
    scanf("%s", name);
    Staff stu;
    if (findStaffByName(gStaffList, name, &stu)) {
        printf("\n你要查找的职工信息如下:\n");
        printStaff(stu);
        printf("\n");
    } else {
        printf("很抱歉没找到.\n\n");
    }
}

void update() {
    // 更新职工信息
    printf("\n");

    int id;
    printf("请输入要修改的职工工号: ");
    scanf("%d", &id);
    Staff staff;
    // 先根据工号来找到职工
    if (findStaffById(gStaffList, id, &staff)) {
        printf("\n找到该学生目前信息为:\n");
        printStaff(staff);
        printf("\n");

        printf("请输入新的信息:\n");

        printf("姓名: ");
        clearInput();
        gets(staff.name);

        printf("年龄: ");
        scanf("%d", &staff.age);

        printf("性别(1表示男, 0表示女): ");
        scanf("%d", &staff.sex);

        printf("学历:\n");
        printf("1. 本科\n2. 硕士\n3. 博士\n4. 其它\n");
        printf(": ");
        int edu;
        scanf("%d", &edu);
        switch (edu) {
            case 1:
                staff.education = EDU_UNDERGRADUATE;
                break;
            case 2:
                staff.education = EDU_MASTER;
                break;
            case 3:
                staff.education = EDU_DOCTOR;
                break;
            case 4:
                staff.education = EDU_OTHER;
                break;
            default:
                printf("输入的信息有误!");
                return;
        }

        printf("工资: ");
        scanf("%f", &staff.salary);

        printf("邮箱: ");
        scanf("%s", staff.email);

        printf("手机号: ");
        scanf("%s", staff.phone);

        printf("即将修改该职工:\n");
        printStaff(staff);


        printf("回车继续, 输入其它任意字符取消...");
        clearInput();
        if (getchar() != '\n') {
            clearInput();
            printf("已取消.\n\n");
            return;
        }

        if (updateStaffById(gStaffList, &staff)) {
            printf("已修改成功.\n\n");
        } else {
            printf("修改失败, 请重试.\n\n");
        }
    } else {
        printf("很抱歉没找到.\n\n");
    }
}

void displayTable() {
    // 显示职工表
    printf("\n");

    // 选择排序方式
    int choice;
    printf("0. 返回上一级\n"
                   "1. 按工号排序\n"
                   "2. 按姓名排序\n"
                   "3. 按年龄排序\n"
                   "4. 按工资排序\n");

    printf("请输入选项: ");
    scanf("%d", &choice);
    switch (choice) {
        case 0:
            break;
        case 1:
            sortById(gStaffList);
            printTable();
            break;
        case 2:
            sortByName(gStaffList);
            printTable();
            break;
        case 3:
            sortByAge(gStaffList);
            printTable();
            break;
        case 4:
            sortBySalary(gStaffList);
            printTable();
            break;
        default:
            break;
    }
}

// 打印单个列表项
void printTableItem(void *item) {
    Staff *staff = (Staff *) item;
    printf("-----------------------------------------\n");
    printStaff(*staff);
}

// 遍历链表打印职工表
void printTable() {
    printf("\n");
    LinkedListTraverse(gStaffList, &printTableItem);
    printf("\n");
};

// 距退休的年数
int retireYear;

// 打印单个即将退休的职工
void printRetireStaff(void *s) {
    Staff *staff = (Staff *) s;
    if (1 == staff->sex) {
        // 男性
        if (60 - staff->age == retireYear) {
            printTableItem(s);
        }
    } else {
        // 女性
        if (55 - staff->age == retireYear) {
            printTableItem(s);
        }
    }
}

void retire() {
    // 查找即将退休的职工
    printf("\n");

    printf("请输入筛选条件, 即还剩多少年退休: ");
    scanf("%d", &retireYear);

    printf("找到以下符合条件的职工:\n");

    printf("\n");
    LinkedListTraverse(gStaffList, &printRetireStaff);
    printf("\n");
}
