#include "ExpressStruct.h"
#include "Graph.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

Express *head = NULL;  // 定义 head 变量

typedef struct addressRack {
    char address[MAX_LEN];
    int rack_id;
    int max_find_id; // 添加一个字段来跟踪每个地址的最大 find_id
    struct addressRack *next;
} AddressRack;

AddressRack *addressHead = NULL;  // 定义 addressHead 变量

// 从CSV文件读取快递信息
void loadFromFile(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        printf("无法打开文件 %s\n", filename);
        return;
    }

    char line[MAX_LEN];
    while (fgets(line, sizeof(line), file)) {
        Express *newExpress = (Express *)malloc(sizeof(Express));
        sscanf(line, "%[^,],%[^,],%[^,],%s", newExpress->id, newExpress->address, newExpress->name, newExpress->phone);
        newExpress->next = head;
        head = newExpress;
    }

    fclose(file);
}

// 将快递信息保存到CSV文件
void saveToFile(const char *filename) {
    FILE *file = fopen(filename, "w");
    if (!file) {
        printf("无法打开文件 %s\n", filename);
        return;
    }

    Express *current = head;
    while (current) {
        fprintf(file, "%s,%s,%s,%s\n", current->id, current->address, current->name, current->phone);
        current = current->next;
    }

    fclose(file);
}

// 判断字符串是否为数字
int isNumeric(const char *str) {
    while (*str) {
        if (!isdigit(*str)) {
            return 0;
        }
        str++;
    }
    return 1;
}

int checkDuplicateID(const char *id) {
    Express *current = head;
    while (current) {
        if (strcmp(current->id, id) == 0) {
            return 1;
        }
        current = current->next;
    }
    return 0;
}

// 插入快递信息
void insertExp(const char *id, const char *address, const char *name, const char *phone) {
    if (!isNumeric(phone)) {
        printf("\033[0;32m-------------------\n\033[0m");
        printf("\033[0;31m*录入失败，电话必须为数字\n\033[0m");
        return;
    }
    if (checkDuplicateID(id)) {
        printf("\033[0;32m-------------------\n\033[0m");
        printf("\033[0;31m*不能重复录入\n\033[0m");
        return;
    }
    Express *newExpress = (Express *)malloc(sizeof(Express));
    strncpy(newExpress->id, id, MAX_LEN);
    strncpy(newExpress->address, address, MAX_LEN);
    strncpy(newExpress->name, name, MAX_LEN);
    strncpy(newExpress->phone, phone, MAX_LEN);
    newExpress->next = head;
    head = newExpress;
    printf("\033[0;32m-------------------\n");
    printf("*录入成功\n\033[0m");
}

// 显示所有快递信息
void showExp() {
    Express *current = head;
    while (current) {
        printf("\033[0;32m单号: %s, 地址: %s, 姓名: %s, 电话: %s\n\033[0m", current->id, current->address, current->name, current->phone);
        current = current->next;
    }
}

// 释放链表
void freeList() {
    Express *current = head;
    while (current) {
        Express *temp = current;
        current = current->next;
        free(temp);
    }

    AddressRack *addrCurrent = addressHead;
    while (addrCurrent) {
        AddressRack *temp = addrCurrent;
        addrCurrent = addrCurrent->next;
        free(temp);
    }
}

// 获取地址对应的 rack_id，如果地址不存在则分配新的 rack_id
int getRackId(const char *address) {
    AddressRack *current = addressHead;
    while (current) {
        if (strcmp(current->address, address) == 0) {
            return current->rack_id;
        }
        current = current->next;
    }

    // 地址不存在，分配新的 rack_id
    AddressRack *newAddressRack = (AddressRack *)malloc(sizeof(AddressRack));
    static int next_rack_id = 1; // 静态变量用于生成唯一的 rack_id
    newAddressRack->rack_id = next_rack_id++;
    newAddressRack->max_find_id = 1; // 初始化 max_find_id 为 1
    strncpy(newAddressRack->address, address, MAX_LEN);
    newAddressRack->next = addressHead;
    addressHead = newAddressRack;

    return newAddressRack->rack_id;
}

// 快递分类
void expressClassify(const char *filename) {
    FILE *file = fopen(filename, "w");
    if (!file) {
        printf("\033[0;31m无法打开文件 %s\n\033[0m", filename);
        return;
    }

    Express *current = head;
    while (current) {
        int rack_id = getRackId(current->address);
        int find_id = addressHead->max_find_id++; // 使用地址的最大 find_id，并将其递增

        fprintf(file, "%d,%d,%s,%s,%s,%s\n", rack_id, find_id, current->id, current->address, current->name, current->phone);
        current = current->next;
    }

    fclose(file);
}

void removeFromRackFileByPhone(const char *phone) {
    FILE *file = fopen("expressRack.csv", "r");
    if (!file) {
        printf("\033[0;31m\n*请在运行时关闭文件\n\033[0m");
        return;
    }

    // 创建一个临时文件来保存更新后的内容
    FILE *tempFile = fopen("temp.csv", "w");
    if (!tempFile) {
        printf("\033[0;32m无法创建临时文件 temp.csv\n\033[0m");
        fclose(file);
        return;
    }

    char line[MAX_LEN];
    while (fgets(line, sizeof(line), file)) {
        char tempPhone[MAX_LEN];
        sscanf(line, "%*d,%*d,%*[^,],%*[^,],%*[^,],%s", tempPhone);
        if (strcmp(tempPhone, phone) != 0) {
            // 将不是要删除的快递信息写入临时文件
            fputs(line, tempFile);
        }
    }

    // 关闭文件
    fclose(file);
    fclose(tempFile);

    // 删除原文件并将临时文件重命名为原文件名
    remove("expressRack.csv");
    rename("temp.csv", "expressRack.csv");
}

// 修改 extractExpress 函数以删除快递信息后，也从 expressRack.csv 文件中删除相应的快递信息
void extractExpress(const char *id) {
    Express *current = head;
    Express *prev = NULL;

    while (current) {
        if (strcmp(current->id, id) == 0) {
            if (prev) {
                prev->next = current->next;
            } else {
                head = current->next;
            }
            free(current);

            return;
        }
        prev = current;
        current = current->next;
    }
}

// 通过手机号码搜索快递信息
void searchID(const char *phone) {
    Express *current = head;
    int found = 0;
    printf("\033[0;32m-------------------\n\033[0m");
    while (current) {
        if (strcmp(current->phone, phone) == 0) {
            int rack_id = getRackId(current->address);
            static int find_id = 1; // 静态变量用于生成唯一的 find_id

            printf("\033[0;32m编号%d-%d, 单号: %s\n\033[0m", rack_id, find_id++, current->id);
            found = 1;
        }

        current = current->next;
    }
    if (!found) {
        printf("\033[0;31m*没有找到该用户的快递信息\n\033[0m");
        return;
    }

    // 提示用户是否要提取所有符合条件的快递信息
    printf("\033[0;32m-------------------\n");
    printf("*是否(1/0)提取所有快递？: \033[0m");
    int extractAll;
    scanf("%d", &extractAll);
    if (extractAll == 1) {
        current = head;
        while (current) {
            if (strcmp(current->phone, phone) == 0) {
                extractExpress(current->id);
            }
            current = current->next;
        }

        // 根据手机号码删除 expressRack.csv 中的快递信息
        removeFromRackFileByPhone(phone);
        printf("\033[0;32m-------------------\n");
        printf("*所有快递已提取\n\033[0m");
    }
}
// 清除输入缓冲区
void clearInputBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}
// 菜单功能
void menu() {
    printf("\033[0;32m*******************\n");
    printf("    欢迎使用系统\n");
    printf("-------------------\n");
    printf("    1. 后台系统\n");
    printf("    2. 用户系统\n");
    printf("    3. 安全结束\n");
    printf("*******************\n\033[0m");
}

void menuManage() {
    printf("\033[0;32m*******************\n");
    printf("    后台管理系统\n");
    printf("-------------------\n");
    printf("    1. 快递录入\n");
    printf("    2. 快递信息\n");
    printf("    3. 快递分类\n");
    printf("    4. 最短路径\n");
    printf("    5. 返回菜单\n");
    printf("*******************\n\033[0m");
}

void menuUse() {
    printf("\033[0;32m*******************\n");
    printf("    用户管理系统\n");
    printf("-------------------\n");
    printf("    1. 查询快递\n");
    printf("    2. 返回菜单\n");
    printf("*******************\n\033[0m");
}

void menuPath() {
    printf("\033[0;32m*******************\n");
    printf("      最短路径\n");
    printf("-------------------\n");
    printf("    1. 无途径点\n");
    printf("    2. 有途径点\n");
    printf("    3. 返回菜单\n");
    printf("*******************\n\033[0m");
}

int choice;
int choiceManage;
int choiceUse;

void menuC() {
    do {
        menu();
        printf("\033[0;32m*请选择: \033[0m");
        if (scanf("%d", &choice) != 1) {
            clearInputBuffer();
            printf("\033[0;32m-------------------\n\033[0m");
            printf("\033[0;31m*无效的选择\n\033[0m");
            menuC();
            break;
        }

        switch (choice) {
            case 1:
                menuManageC();
                break;
            case 2:
                menuUseC();
                break;
            case 3:
                printf("\033[0;32m-------------------\n");
                printf("*系统已结束运行\n");
                printf("-------------------\033[0m");
                break;
            default:
                printf("\033[0;32m-------------------\n\033[0m");
                printf("\033[0;31m*无效的选择\n\033[0m");
                break;
        }
    } while (choice != 3);

}
void menuManageC() {
    do {
        menuManage();
        printf("\033[0;32m*请选择: \033[0m");
        if (scanf("%d", &choiceManage) != 1) {
            clearInputBuffer();
            printf("\033[0;32m-------------------\n\033[0m");
            printf("\033[0;31m*无效的选择\n\033[0m");
            menuManageC();
            break;
        }

        switch (choiceManage) {
            case 1:
                printf("\033[0;32m-------------------\n");
                printf("*请输入快递信息\n");
                printf("-------------------\n");
                char id[MAX_LEN], address[MAX_LEN], name[MAX_LEN], phone[MAX_LEN];
                printf("*单号: \033[0m");
                scanf("%s", id);
                printf("\033[0;32m*地址: \033[0m");
                scanf("%s", address);
                printf("\033[0;32m*姓名: \033[0m");
                scanf("%s", name);
                printf("\033[0;32m*电话: \033[0m");
                scanf("%s", phone);
                insertExp(id, address, name, phone);
                break;
            case 2:
                printf("\033[0;32m-------------------\n");
                printf("*快递信息如下\n");
                printf("-------------------\n");
                showExp();
                break;
            case 3:
                printf("\033[0;32m-------------------\n");
                printf("*已更新快递分类\n\033[0m");
                expressClassify("expressRack.csv");
                break;
            case 4:
                menuPathC();
                break;
            case 5:
                menuC();
                break;
            default:
                printf("\033[0;32m-------------------\n\033[0m");
                printf("\033[0;31m*无效的选择\n\033[0m");
                break;
        }
    } while (choiceManage != 5);
}

void menuUseC() {
    do {
        menuUse();
        printf("\033[0;32m*请选择: \033[0m");
        if (scanf("%d", &choiceUse) != 1) {
            clearInputBuffer();
            printf("\033[0;32m-------------------\n\033[0m");
            printf("\033[0;31m*无效的选择\n\033[0m");
            menuUseC();
            break;
        }

        switch (choiceUse) {
            case 1:
                printf("\033[0;32m-------------------\n");
                printf("*请输入手机号码: \033[0m");
                char search_phone[MAX_LEN];
                scanf("%s", search_phone);
                searchID(search_phone);
                break;
            case 2:
                menuC();
                break;
            default:
                printf("\033[0;32m-------------------\n\033[0m");
                printf("\033[0;31m*无效的选择\n\033[0m");
                break;
        }
    } while (choiceUse != 2);
}

int choicePath;

void menuPathC() {
    do {
        menuPath();
        printf("\033[0;32m*请选择: \033[0m");
        if (scanf("%d", &choicePath) != 1) {
            clearInputBuffer();
            printf("\033[0;32m-------------------\n\033[0m");
            printf("\033[0;31m*无效的选择\n\033[0m");
            menuPathC();
            break;
        }

        switch (choicePath) {
            case 1:
                GraphMenu();
                break;
            case 2:
                shortPath();
                break;
            case 3:
                menuManageC();
            default:
                printf("\033[0;32m-------------------\n\033[0m");
                printf("\033[0;31m*无效的选择\n\033[0m");
                break;
        }
    } while (choicePath != 3);
}