#include <stdio.h>
#include <limits.h>
#include <stdbool.h>
#include "Graph.h"

// 地点名称数组
const char *locationNames[MAX_NODES] = {
    "校医院", "北门", "北侧门", "大活", "一食堂", "二食堂", "信息楼", "图书馆",
    "体育馆", "实验2楼", "实验6楼", "实验7楼", "教师公寓", "五围合", "四围合",
    "三围合", "二围合", "一围合", "六围合", "七围合", "E教", "A教", "B教",
    "C教", "D教", "南门"
};

// 图的初始化
void initGraph(Graph *graph, int numNodes) {
    graph->numNodes = numNodes;
    for (int i = 0; i < numNodes; i++) {
        for (int j = 0; j < numNodes; j++) {
            if (i == j)
                graph->adjMatrix[i][j] = 0;
            else
                graph->adjMatrix[i][j] = INF;
        }
    }
}

// 添加边
void addEdge(Graph *graph, int src, int dest, int weight) {
    graph->adjMatrix[src][dest] = weight;
    graph->adjMatrix[dest][src] = weight; // 无向图
}

// 找到距离最近的未访问节点
int minDistance(int dist[], bool sptSet[], int numNodes) {
    int min = INF, minIndex;

    for (int v = 0; v < numNodes; v++)
        if (!sptSet[v] && dist[v] <= min)
            min = dist[v], minIndex = v;

    return minIndex;
}

// 打印路径
void printPath(int parent[], int j) {
    if (parent[j] == -1)
        return;

    printPath(parent, parent[j]);
    printf("\033[0;32m->%s\033[0m", locationNames[j]);
}
// 打印解决方案
void printSolution(int dist[], int parent[], int startNode, int endNode) {
    printf("\033[0;32m-------------------\n");
    printf("*最短距离：%d单位\n", dist[endNode]);
    printf("*最短路径: %s\033[0m", locationNames[startNode]);
    printPath(parent, endNode);
    printf("\n");
}

// Dijkstra算法
void dijkstra(Graph *graph, int startNode, int endNode) {
    int dist[MAX_NODES];
    bool sptSet[MAX_NODES];
    int parent[MAX_NODES];

    for (int i = 0; i < graph->numNodes; i++) {
        parent[startNode] = -1;
        dist[i] = INF;
        sptSet[i] = false;
    }

    dist[startNode] = 0;

    for (int count = 0; count < graph->numNodes - 1; count++) {
        int u = minDistance(dist, sptSet, graph->numNodes);
        sptSet[u] = true;

        for (int v = 0; v < graph->numNodes; v++)
            if (!sptSet[v] && graph->adjMatrix[u][v] && dist[u] + graph->adjMatrix[u][v] < dist[v]) {
                parent[v] = u;
                dist[v] = dist[u] + graph->adjMatrix[u][v];
            }
    }

    printSolution(dist, parent, startNode, endNode);
}
// Dijkstra算法
void dijkstraPoints(Graph *graph, int startNode, int dist[], int parent[]) {
    bool sptSet[MAX_NODES];

    for (int i = 0; i < graph->numNodes; i++) {
        parent[startNode] = -1;
        dist[i] = INF;
        sptSet[i] = false;
    }

    dist[startNode] = 0;

    for (int count = 0; count < graph->numNodes - 1; count++) {
        int u = minDistance(dist, sptSet, graph->numNodes);
        sptSet[u] = true;

        for (int v = 0; v < graph->numNodes; v++)
            if (!sptSet[v] && graph->adjMatrix[u][v] && dist[u] + graph->adjMatrix[u][v] < dist[v]) {
                parent[v] = u;
                dist[v] = dist[u] + graph->adjMatrix[u][v];
            }
    }
}
void shortPathWithPoints(Graph *graph, int startNode, int pickup1, int pickup2) {
    int distStartToPickup1[MAX_NODES], parentStartToPickup1[MAX_NODES];
    int distStartToPickup2[MAX_NODES], parentStartToPickup2[MAX_NODES];

    // 从起点到两个收件处的最短路径
    dijkstraPoints(graph, startNode, distStartToPickup1, parentStartToPickup1);
    dijkstraPoints(graph, startNode, distStartToPickup2, parentStartToPickup2);

    int closerPickup, furtherPickup;

    // 判断哪个收件处离起点更近
    if (distStartToPickup1[pickup1] <= distStartToPickup2[pickup2]) {
        closerPickup = pickup1;
        furtherPickup = pickup2;
    } else {
        closerPickup = pickup2;
        furtherPickup = pickup1;
    }

    int distCloserToFurther[MAX_NODES], parentCloserToFurther[MAX_NODES];

    // 从较近的收件处到较远的收件处的最短路径
    dijkstraPoints(graph, closerPickup, distCloserToFurther, parentCloserToFurther);

    printf("\033[0;32m-------------------\n");
    printf("*最短距离：%d单位\n", distStartToPickup1[closerPickup] + distCloserToFurther[furtherPickup]);
    printf("*最短路径: %s\033[0m", locationNames[startNode]);
    printPath(parentStartToPickup1, closerPickup);
    printPath(parentCloserToFurther, furtherPickup);
    printf("\n");
}
void shortPath() {
    Graph graph;
    initGraph(&graph, 26); // 初始化图，节点数为26

    // 添加所有边
    addEdges(&graph);
    int start, pickup1, pickup2;
    printf("\033[0;32m-------------------\n");
    printf("*以下为地点编号\n");
    printf("-------------------\n\033[0m");
    for (int i = 0; i < MAX_NODES; i++) {
        printf("\033[0;32m%d: %s\n\033[0m", i, locationNames[i]);
    }
    printf("\033[0;32m-------------------\n");
    printf("*起点: \033[0m");
    scanf("%d", &start);
    printf("\033[0;32m*途径点1: \033[0m");
    scanf("%d", &pickup1);
    printf("\033[0;32m*途径点2: \033[0m");
    scanf("%d", &pickup2);

    // 调用shortPathWithPoints函数
    shortPathWithPoints(&graph, start, pickup1, pickup2);
}

// 添加所有边
void addEdges(Graph *graph) {
    addEdge(graph, 0, 1, 6);
    addEdge(graph, 0, 9, 6);
    addEdge(graph, 1, 12, 3);
    addEdge(graph, 1, 5, 5);
    addEdge(graph, 1, 12, 5);
    addEdge(graph, 2, 3, 1);
    addEdge(graph, 3, 4, 3);
    addEdge(graph, 3, 13, 4);
    addEdge(graph, 3, 14, 3);
    addEdge(graph, 4, 14, 1);
    addEdge(graph, 5, 6, 1);
    addEdge(graph, 5, 19, 4);
    addEdge(graph, 6, 9, 4);
    addEdge(graph, 6, 19, 4);
    addEdge(graph, 6, 20, 5);
    addEdge(graph, 7, 20, 2);
    addEdge(graph, 8, 24, 2);
    addEdge(graph, 8, 25, 6);
    addEdge(graph, 9, 10, 5);
    addEdge(graph, 10, 11, 1);
    addEdge(graph, 11, 25, 4);
    addEdge(graph, 12, 13, 3);
    addEdge(graph, 12, 19, 2);
    addEdge(graph, 13, 14, 2);
    addEdge(graph, 13, 18, 2);
    addEdge(graph, 14, 15, 2);
    addEdge(graph, 15, 17, 2);
    addEdge(graph, 16, 18, 2);
    addEdge(graph, 16, 19, 3);
    addEdge(graph, 16, 20, 3);
    addEdge(graph, 17, 21, 3);
    addEdge(graph, 18, 19, 4);
    addEdge(graph, 20, 21, 2);
    addEdge(graph, 21, 22, 2);
    addEdge(graph, 21, 23, 2);
    addEdge(graph, 22, 24, 2);
    addEdge(graph, 23, 24, 2);
}

void GraphMenu() {
    Graph graph;
    initGraph(&graph, 26); // 初始化图，节点数为26

    // 添加所有边
    addEdges(&graph);

    int start, end;
    printf("\033[0;32m*以下为地点编号\n");
    printf("-------------------\n\033[0m");
    for (int i = 0; i < MAX_NODES; i++) {
        printf("\033[0;32m%d: %s\n\033[0m", i, locationNames[i]);
    }
    printf("\033[0;32m-------------------\n");
    printf("*起点: \033[0m");
    scanf("%d", &start);
    printf("\033[0;32m*终点: \033[0m");
    scanf("%d", &end);
    dijkstra(&graph, start, end);
}
