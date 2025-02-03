#ifndef EXPRESSSTRUCT_H
#define EXPRESSSTRUCT_H

#define MAX_LEN 100

typedef struct express {
    char id[MAX_LEN];
    char address[MAX_LEN];
    char name[MAX_LEN];
    char phone[MAX_LEN];
    struct express *next;
} Express;

typedef struct expressRack {
    Express express;  // 继承 express 结构体
    int find_id;      // 在快递架下的编号
} ExpressRack;

typedef struct rack {
    int rack_id;       // 快递架编号
    ExpressRack expressRack; // 包含 expressRack 结构体
} Rack;

extern Express *head;  // 声明 head 变量为外部变量

// 函数声明
void loadFromFile(const char *filename);
void saveToFile(const char *filename);
int isNumeric(const char *str);
int checkDuplicateID(const char *id);
void insertExp(const char *id, const char *address, const char *name, const char *phone);
void showExp();
void freeList();
void menu();
void menuManage();
void menuUse();
void menuC();
void menuManageC();
void menuUseC();
void expressClassify(const char *filename);
void searchID(const char *phone);
void extractExpress(const char *id);
void removeFromRackFile(const char *id);
#endif // EXPRESSSTRUCT_H
