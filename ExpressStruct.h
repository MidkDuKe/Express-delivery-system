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
    Express express;  // �̳� express �ṹ��
    int find_id;      // �ڿ�ݼ��µı��
} ExpressRack;

typedef struct rack {
    int rack_id;       // ��ݼܱ��
    ExpressRack expressRack; // ���� expressRack �ṹ��
} Rack;

extern Express *head;  // ���� head ����Ϊ�ⲿ����

// ��������
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
