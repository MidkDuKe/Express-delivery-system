#include "ExpressStruct.h"
#include <stdio.h>

#include "Graph.h"

int main() {
    int choice;
    int choiceManage;
    int chiceUse;
    // ���ļ����ؿ����Ϣ
    loadFromFile("express.csv");
     menuC();
    // �������Ϣ���浽�ļ�
    saveToFile("express.csv");

    // �ͷ�����
    freeList();

    return 0;
}
