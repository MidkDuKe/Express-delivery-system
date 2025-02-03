#ifndef GRAPH_H
#define GRAPH_H

#define MAX_NODES 26
#define INF 1000000

// ͼ�ṹ����
typedef struct {
    int adjMatrix[MAX_NODES][MAX_NODES];
    int numNodes;
} Graph;

// �ص���������
extern const char *locationNames[MAX_NODES];

// ͼ�ĳ�ʼ��
void initGraph(Graph *graph, int numNodes);

// ��ӱ�
void addEdge(Graph *graph, int src, int dest, int weight);

// ������б�
void addEdges(Graph *graph);

// ���·��
void dijkstra(Graph *graph, int startNode, int endNode);

void GraphMenu();
void shortPath();
void menuPathC();
#endif // GRAPH_H
