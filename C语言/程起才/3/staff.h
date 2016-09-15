#ifndef __STAFF__
#define __STAFF__

#define MAX_NAME_LEN 20
#define MAX_EMAIL_LEN 100
#define PHONE_LEN 11

typedef enum _EDUCATION {
    EDU_OTHER,
    EDU_UNDERGRADUATE,
    EDU_MASTER,
    EDU_DOCTOR
} EDUCATION;

typedef struct _Staff {
    int id;
    char name[MAX_NAME_LEN];
    int age;
    int sex;
    EDUCATION education;
    float salary;
    char email[MAX_EMAIL_LEN];
    char phone[PHONE_LEN];
} Staff;

#endif //__STAFF__
