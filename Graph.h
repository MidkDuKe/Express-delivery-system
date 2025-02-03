#ifndef GRAPH_H
#define GRAPH_H

#define MAX_NODES 26
#define INF 1000000

// 图结构定义
typedef struct {
    int adjMatrix[MAX_NODES][MAX_NODES];
    int numNodes;
} Graph;

// 地点名称数组
extern const char *locationNames[MAX_NODES];

// 图的初始化
void initGraph(Graph *graph, int numNodes);

// 添加边
void addEdge(Graph *graph, int src, int dest, int weight);

// 添加所有边
void addEdges(Graph *graph);

// 最短路径
void dijkstra(Graph *graph, int startNode, int endNode);

void GraphMenu();
void shortPath();
void menuPathC();
#endif // GRAPH_H
