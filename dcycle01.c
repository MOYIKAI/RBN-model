#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

#define MAX_NODES 2000

struct Node {
    int data;
    struct Node* next;
};

struct Graph {
    int numNodes;
    struct Node* adjList[MAX_NODES];
};

// Structure to represent a cycle
struct Cycle {
    int path[MAX_NODES];
    int length;
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
void findCycles(struct Graph* graph, int v, bool visited[], int path[], int pathLength, struct Cycle uniqueCycles[], int* numUniqueCycles) {
    visited[v] = true;
    path[pathLength] = v;
    pathLength++;

    struct Node* neighbor = graph->adjList[v];
    while (neighbor != NULL) {
        int neighborNode = neighbor->data;
        if (!visited[neighborNode]) {
            findCycles(graph, neighborNode, visited, path, pathLength, uniqueCycles, numUniqueCycles);
        } else {
            // Check if the neighbor is part of the current path (cycle)
            for (int i = 0; i < pathLength; i++) {
                if (path[i] == neighborNode) {
                    // Create a new cycle
                    struct Cycle cycle;
                    cycle.length = pathLength - i;
                    for (int j = i; j < pathLength; j++) {
                        cycle.path[j - i] = path[j];
                    }
                    
                    // Check if this cycle is unique
                    bool isUnique = true;
                    for (int k = 0; k < *numUniqueCycles; k++) {
                        if (cycle.length != uniqueCycles[k].length) {
                            continue;
                        }
                        bool isEqual = true;
                        for (int j = 0; j < cycle.length; j++) {
                            if (cycle.path[j] != uniqueCycles[k].path[j]) {
                                isEqual = false;
                                break;
                            }
                        }
                        if (isEqual) {
                            isUnique = false;
                            break;
                        }
                    }

                    // If the cycle is unique, add it to the list and print it
                    if (isUnique) {
                        uniqueCycles[*numUniqueCycles] = cycle;
                        (*numUniqueCycles)++;
                        
                        // Print the cycle
                        printf("Cycle Detected: ");
                        for (int j = i; j < pathLength; j++) {
                            printf("%d -> ", path[j]);
                        }
                        printf("%d\n", neighborNode);
                    }
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
    struct Cycle uniqueCycles[MAX_NODES];
    int numUniqueCycles = 0;
    
    for (int i = 0; i < graph->numNodes; i++) {
        findCycles(graph, i, visited, path, 0, uniqueCycles, &numUniqueCycles);
    }
}

int main(int argc, char* argv[]) {
    struct Graph graph;
    int numNodes, numEdges;

    FILE* cf; // input configuration file

    if (argc == 4) {
        // require argc be equal to number of command line entries
        numNodes = atol(argv[1]);
        numEdges = atol(argv[2]);
    } else {
        // error input value of argc
        fprintf(stderr, "\n Initialization error:\n");
        fprintf(stderr, "Usage: dcycle.x N edges configfile\n");
        return 1;
    }

    initializeGraph(&graph, numNodes);

    cf = fopen(argv[3], "r");
    for (int i = 0; i < numEdges; i++) {
        int src, dest;

        fscanf(cf, "%d %d", &src, &dest);
        addEdge(&graph, src, dest);
    }

    findAllCycles(&graph);

    return 0;
}

