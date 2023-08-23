#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

#define MAX_NODES 10000

// Structure to represent a node in the graph
struct Node {
    int data;
    struct Node* next;
};

// Structure to represent the graph
struct Graph {
    int numNodes;
    struct Node* adjList[MAX_NODES];
};

// Function to initialize a graph
void initializeGraph(struct Graph* graph, int numNodes) {
    graph->numNodes = numNodes;
    for (int i = 0; i < numNodes; i++) {
        graph->adjList[i] = NULL;
    }
}

// Function to add an edge to the graph
void addEdge(struct Graph* graph, int src, int dest) {
    struct Node* newNode = (struct Node*)malloc(sizeof(struct Node));
    newNode->data = dest;
    newNode->next = graph->adjList[src];
    graph->adjList[src] = newNode;
}

// Helper function for finding all cycles
void findCycles(struct Graph* graph, int v, bool visited[], int path[], int pathLength) {
    visited[v] = true;
    path[pathLength] = v;
    pathLength++;

    struct Node* neighbor = graph->adjList[v];
    while (neighbor != NULL) {
        int neighborNode = neighbor->data;
        if (!visited[neighborNode]) {
            findCycles(graph, neighborNode, visited, path, pathLength);
        } else {
            // Check if the neighbor is part of the current path (cycle)
            for (int i = 0; i < pathLength; i++) {
                if (path[i] == neighborNode) {
                    printf("Cycle Detected: ");
                    for (int j = i; j < pathLength; j++) {
                        printf("%d -> ", path[j]);
                    }
                    printf("%d\n", neighborNode);
                }
            }
        }
        neighbor = neighbor->next;
    }

    visited[v] = false; // Reset visited status for backtracking
}

// Function to find and print all cycles in the graph
void findAllCycles(struct Graph* graph) {
    bool visited[MAX_NODES] = {false};
    int path[MAX_NODES];
    for (int i = 0; i < graph->numNodes; i++) {
        findCycles(graph, i, visited, path, 0);
    }
}

int main(int argc, char *argv[]) {
    struct Graph graph;
    int numNodes, numEdges;
    
    FILE *cf;	// input configuration file

    if (argc == 4){
        // require argc be equal to number of command line entries
        numNodes = atol(argv[1]);
        numEdges = atol(argv[2]);
    } 
    else {
        // error input value of argc 
        fprintf(stderr,"\n Initialization error:\n");
        fprintf(stderr,"Usage: dcycle.x N configfile\n");
        return 1;
    }

    initializeGraph(&graph, numNodes);
    
    cf = fopen(argv[3],"r");
    for (int i = 0; i < numEdges; i++) {
        int src, dest;
        
        fscanf(cf, "%d %d", &src, &dest);
        addEdge(&graph, src, dest);
    }
    
    findAllCycles(&graph);
    
    return 0;
}
