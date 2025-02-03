#include "ExpressStruct.h"
#include <stdio.h>

#include "Graph.h"

int main() {
    int choice;
    int choiceManage;
    int chiceUse;
    // 从文件加载快递信息
    loadFromFile("express.csv");
     menuC();
    // 将快递信息保存到文件
    saveToFile("express.csv");

    // 释放链表
    freeList();

    return 0;
}
