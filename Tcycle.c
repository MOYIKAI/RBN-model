#include <stdio.h>
#include <stdlib.h>


#define MAX_NODES 10000

// Define data structures for the graph
struct Node {
    int id;
    struct Node* next;
};

struct Graph {
    int numNodes;
    struct Node* adjList[MAX_NODES];
    int discovered[MAX_NODES];
    int stack[MAX_NODES];
    int stackSize;
};

void initializeGraph(struct Graph* G, int numNodes) {
    G->numNodes = numNodes;
    for (int i = 0; i < numNodes; i++) {
        G->adjList[i] = NULL;
        G->discovered[i] = 0;
    }
    G->stackSize = 0;
}

void addEdge(struct Graph* G, int from, int to) {
    struct Node* newNode = (struct Node*)malloc(sizeof(struct Node));
    newNode->id = to;
    newNode->next = G->adjList[from];
    G->adjList[from] = newNode;
}

// Tarjan's algorithm for finding strongly connected components
void tarjan(struct Graph* G, int u, int* time, int* low, int* disc) {
    disc[u] = *time;
    low[u] = *time;
    (*time)++;

    G->stack[G->stackSize++] = u;
    G->discovered[u] = 1;

    struct Node* v;
    for (v = G->adjList[u]; v != NULL; v = v->next) {
        int w = v->id;
        if (disc[w] == -1) {
            tarjan(G, w, time, low, disc);
            if (low[w] < low[u])
                low[u] = low[w];
        }
        else if (G->discovered[w]) {
            if (disc[w] < low[u])
                low[u] = disc[w];
        }
    }

    if (low[u] == disc[u]) {
        int poppedNode;
        do {
            poppedNode = G->stack[--G->stackSize];
            G->discovered[poppedNode] = 0;
            printf("%d ", poppedNode);
        } while (poppedNode != u);
        printf("\n");
    }
}

void findCycles(struct Graph* G, int u, int* time, int* low, int* disc) {
    disc[u] = *time;
    low[u] = *time;
    (*time)++;

    G->stack[G->stackSize++] = u;
    G->discovered[u] = 1;
    G->inStack[u] = 1;

    struct Node* v;
    for (v = G->adjList[u]; v != NULL; v = v->next) {
        int w = v->id;
        if (disc[w] == -1) {
            findCycles(G, w, time, low, disc);
            if (low[w] < low[u])
                low[u] = low[w];
        }
        else if (G->inStack[w]) {
            if (disc[w] < low[u])
                low[u] = disc[w];
        }
    }

    if (low[u] == disc[u]) {
        int poppedNode;
        printf("Cycle: ");
        do {
            poppedNode = G->stack[--G->stackSize];
            G->inStack[poppedNode] = 0;
            printf("%d ", poppedNode);
        } while (poppedNode != u);
        printf("\n");
    }
}

int main(int argc, char *argv[]) {

    int numNodes, numEdges;
    struct Graph G;

    if (argc == 4){
        // require argc be equal to number of command line entries
        numNodes = atol(argv[1]);
        numEdges = atol(argv[2]);
    }
    else {
        // error input value of argc 
        fprintf(stderr,"\n Initialization error:\n");
        fprintf(stderr,"Usage: dcycle.x N edges configfile\n");
        return 1;
    }

    // Open the file for reading
    FILE* file = fopen(argv[3], "r");
    if (file == NULL) {
        perror("Unable to open the file");
        return 1;
    }

    // Read the number of nodes and edges from the file
    initializeGraph(&G, numNodes);

    // Read edges from the file and add them to the graph
    for (int i = 0; i < numEdges; i++) {
        int from, to;
        fscanf(file, "%d %d", &from, &to);
        addEdge(&G, from, to);
    }

    fclose(file);


    printf("Strongly Connected Components (Cycles):\n");
    int disc[MAX_NODES];
    int low[MAX_NODES];
    int time = 0;

    for (int i = 0; i < G.numNodes; i++) {
        disc[i] = -1;
        low[i] = -1;
    }

    for (int i = 0; i < G.numNodes; i++) {
        if (disc[i] == -1) {
            findCycles(&G, i, &time, low, disc);
        }
    }

    return 0;
}

