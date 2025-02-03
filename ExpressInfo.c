#include "ExpressStruct.h"
#include "Graph.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

Express *head = NULL;  // ���� head ����

typedef struct addressRack {
    char address[MAX_LEN];
    int rack_id;
    int max_find_id; // ���һ���ֶ�������ÿ����ַ����� find_id
    struct addressRack *next;
} AddressRack;

AddressRack *addressHead = NULL;  // ���� addressHead ����

// ��CSV�ļ���ȡ�����Ϣ
void loadFromFile(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        printf("�޷����ļ� %s\n", filename);
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

// �������Ϣ���浽CSV�ļ�
void saveToFile(const char *filename) {
    FILE *file = fopen(filename, "w");
    if (!file) {
        printf("�޷����ļ� %s\n", filename);
        return;
    }

    Express *current = head;
    while (current) {
        fprintf(file, "%s,%s,%s,%s\n", current->id, current->address, current->name, current->phone);
        current = current->next;
    }

    fclose(file);
}

// �ж��ַ����Ƿ�Ϊ����
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

// ��������Ϣ
void insertExp(const char *id, const char *address, const char *name, const char *phone) {
    if (!isNumeric(phone)) {
        printf("\033[0;32m-------------------\n\033[0m");
        printf("\033[0;31m*¼��ʧ�ܣ��绰����Ϊ����\n\033[0m");
        return;
    }
    if (checkDuplicateID(id)) {
        printf("\033[0;32m-------------------\n\033[0m");
        printf("\033[0;31m*�����ظ�¼��\n\033[0m");
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
    printf("*¼��ɹ�\n\033[0m");
}

// ��ʾ���п����Ϣ
void showExp() {
    Express *current = head;
    while (current) {
        printf("\033[0;32m����: %s, ��ַ: %s, ����: %s, �绰: %s\n\033[0m", current->id, current->address, current->name, current->phone);
        current = current->next;
    }
}

// �ͷ�����
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

// ��ȡ��ַ��Ӧ�� rack_id�������ַ������������µ� rack_id
int getRackId(const char *address) {
    AddressRack *current = addressHead;
    while (current) {
        if (strcmp(current->address, address) == 0) {
            return current->rack_id;
        }
        current = current->next;
    }

    // ��ַ�����ڣ������µ� rack_id
    AddressRack *newAddressRack = (AddressRack *)malloc(sizeof(AddressRack));
    static int next_rack_id = 1; // ��̬������������Ψһ�� rack_id
    newAddressRack->rack_id = next_rack_id++;
    newAddressRack->max_find_id = 1; // ��ʼ�� max_find_id Ϊ 1
    strncpy(newAddressRack->address, address, MAX_LEN);
    newAddressRack->next = addressHead;
    addressHead = newAddressRack;

    return newAddressRack->rack_id;
}

// ��ݷ���
void expressClassify(const char *filename) {
    FILE *file = fopen(filename, "w");
    if (!file) {
        printf("\033[0;31m�޷����ļ� %s\n\033[0m", filename);
        return;
    }

    Express *current = head;
    while (current) {
        int rack_id = getRackId(current->address);
        int find_id = addressHead->max_find_id++; // ʹ�õ�ַ����� find_id�����������

        fprintf(file, "%d,%d,%s,%s,%s,%s\n", rack_id, find_id, current->id, current->address, current->name, current->phone);
        current = current->next;
    }

    fclose(file);
}

void removeFromRackFileByPhone(const char *phone) {
    FILE *file = fopen("expressRack.csv", "r");
    if (!file) {
        printf("\033[0;31m\n*��������ʱ�ر��ļ�\n\033[0m");
        return;
    }

    // ����һ����ʱ�ļ���������º������
    FILE *tempFile = fopen("temp.csv", "w");
    if (!tempFile) {
        printf("\033[0;32m�޷�������ʱ�ļ� temp.csv\n\033[0m");
        fclose(file);
        return;
    }

    char line[MAX_LEN];
    while (fgets(line, sizeof(line), file)) {
        char tempPhone[MAX_LEN];
        sscanf(line, "%*d,%*d,%*[^,],%*[^,],%*[^,],%s", tempPhone);
        if (strcmp(tempPhone, phone) != 0) {
            // ������Ҫɾ���Ŀ����Ϣд����ʱ�ļ�
            fputs(line, tempFile);
        }
    }

    // �ر��ļ�
    fclose(file);
    fclose(tempFile);

    // ɾ��ԭ�ļ�������ʱ�ļ�������Ϊԭ�ļ���
    remove("expressRack.csv");
    rename("temp.csv", "expressRack.csv");
}

// �޸� extractExpress ������ɾ�������Ϣ��Ҳ�� expressRack.csv �ļ���ɾ����Ӧ�Ŀ����Ϣ
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

// ͨ���ֻ��������������Ϣ
void searchID(const char *phone) {
    Express *current = head;
    int found = 0;
    printf("\033[0;32m-------------------\n\033[0m");
    while (current) {
        if (strcmp(current->phone, phone) == 0) {
            int rack_id = getRackId(current->address);
            static int find_id = 1; // ��̬������������Ψһ�� find_id

            printf("\033[0;32m���%d-%d, ����: %s\n\033[0m", rack_id, find_id++, current->id);
            found = 1;
        }

        current = current->next;
    }
    if (!found) {
        printf("\033[0;31m*û���ҵ����û��Ŀ����Ϣ\n\033[0m");
        return;
    }

    // ��ʾ�û��Ƿ�Ҫ��ȡ���з��������Ŀ����Ϣ
    printf("\033[0;32m-------------------\n");
    printf("*�Ƿ�(1/0)��ȡ���п�ݣ�: \033[0m");
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

        // �����ֻ�����ɾ�� expressRack.csv �еĿ����Ϣ
        removeFromRackFileByPhone(phone);
        printf("\033[0;32m-------------------\n");
        printf("*���п������ȡ\n\033[0m");
    }
}
// ������뻺����
void clearInputBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}
// �˵�����
void menu() {
    printf("\033[0;32m*******************\n");
    printf("    ��ӭʹ��ϵͳ\n");
    printf("-------------------\n");
    printf("    1. ��̨ϵͳ\n");
    printf("    2. �û�ϵͳ\n");
    printf("    3. ��ȫ����\n");
    printf("*******************\n\033[0m");
}

void menuManage() {
    printf("\033[0;32m*******************\n");
    printf("    ��̨����ϵͳ\n");
    printf("-------------------\n");
    printf("    1. ���¼��\n");
    printf("    2. �����Ϣ\n");
    printf("    3. ��ݷ���\n");
    printf("    4. ���·��\n");
    printf("    5. ���ز˵�\n");
    printf("*******************\n\033[0m");
}

void menuUse() {
    printf("\033[0;32m*******************\n");
    printf("    �û�����ϵͳ\n");
    printf("-------------------\n");
    printf("    1. ��ѯ���\n");
    printf("    2. ���ز˵�\n");
    printf("*******************\n\033[0m");
}

void menuPath() {
    printf("\033[0;32m*******************\n");
    printf("      ���·��\n");
    printf("-------------------\n");
    printf("    1. ��;����\n");
    printf("    2. ��;����\n");
    printf("    3. ���ز˵�\n");
    printf("*******************\n\033[0m");
}

int choice;
int choiceManage;
int choiceUse;

void menuC() {
    do {
        menu();
        printf("\033[0;32m*��ѡ��: \033[0m");
        if (scanf("%d", &choice) != 1) {
            clearInputBuffer();
            printf("\033[0;32m-------------------\n\033[0m");
            printf("\033[0;31m*��Ч��ѡ��\n\033[0m");
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
                printf("*ϵͳ�ѽ�������\n");
                printf("-------------------\033[0m");
                break;
            default:
                printf("\033[0;32m-------------------\n\033[0m");
                printf("\033[0;31m*��Ч��ѡ��\n\033[0m");
                break;
        }
    } while (choice != 3);

}
void menuManageC() {
    do {
        menuManage();
        printf("\033[0;32m*��ѡ��: \033[0m");
        if (scanf("%d", &choiceManage) != 1) {
            clearInputBuffer();
            printf("\033[0;32m-------------------\n\033[0m");
            printf("\033[0;31m*��Ч��ѡ��\n\033[0m");
            menuManageC();
            break;
        }

        switch (choiceManage) {
            case 1:
                printf("\033[0;32m-------------------\n");
                printf("*����������Ϣ\n");
                printf("-------------------\n");
                char id[MAX_LEN], address[MAX_LEN], name[MAX_LEN], phone[MAX_LEN];
                printf("*����: \033[0m");
                scanf("%s", id);
                printf("\033[0;32m*��ַ: \033[0m");
                scanf("%s", address);
                printf("\033[0;32m*����: \033[0m");
                scanf("%s", name);
                printf("\033[0;32m*�绰: \033[0m");
                scanf("%s", phone);
                insertExp(id, address, name, phone);
                break;
            case 2:
                printf("\033[0;32m-------------------\n");
                printf("*�����Ϣ����\n");
                printf("-------------------\n");
                showExp();
                break;
            case 3:
                printf("\033[0;32m-------------------\n");
                printf("*�Ѹ��¿�ݷ���\n\033[0m");
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
                printf("\033[0;31m*��Ч��ѡ��\n\033[0m");
                break;
        }
    } while (choiceManage != 5);
}

void menuUseC() {
    do {
        menuUse();
        printf("\033[0;32m*��ѡ��: \033[0m");
        if (scanf("%d", &choiceUse) != 1) {
            clearInputBuffer();
            printf("\033[0;32m-------------------\n\033[0m");
            printf("\033[0;31m*��Ч��ѡ��\n\033[0m");
            menuUseC();
            break;
        }

        switch (choiceUse) {
            case 1:
                printf("\033[0;32m-------------------\n");
                printf("*�������ֻ�����: \033[0m");
                char search_phone[MAX_LEN];
                scanf("%s", search_phone);
                searchID(search_phone);
                break;
            case 2:
                menuC();
                break;
            default:
                printf("\033[0;32m-------------------\n\033[0m");
                printf("\033[0;31m*��Ч��ѡ��\n\033[0m");
                break;
        }
    } while (choiceUse != 2);
}

int choicePath;

void menuPathC() {
    do {
        menuPath();
        printf("\033[0;32m*��ѡ��: \033[0m");
        if (scanf("%d", &choicePath) != 1) {
            clearInputBuffer();
            printf("\033[0;32m-------------------\n\033[0m");
            printf("\033[0;31m*��Ч��ѡ��\n\033[0m");
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
                printf("\033[0;31m*��Ч��ѡ��\n\033[0m");
                break;
        }
    } while (choicePath != 3);
}