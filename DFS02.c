// Adjascency List representation in C
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

struct node {
  int vertex;
  struct node* next;
};

struct Graph {
  int numVertices;
  struct node** adjLists;
};

// Create a node
struct node* createNode(int v) {
  struct node* newNode = malloc(sizeof(struct node));
  newNode->vertex = v;
  newNode->next = NULL;
  return newNode;
}

// Create a graph
struct Graph* createAGraph(int vertices) {
  struct Graph* graph = malloc(sizeof(struct Graph));
  graph->numVertices = vertices;
  graph->adjLists = malloc(vertices * sizeof(struct node*));

  int i;
  for (i = 0; i < vertices; i++)
    graph->adjLists[i] = NULL;

  return graph;
}

// Add edge
void addEdge(struct Graph* graph, int s, int d) {
  // Add edge from s to d
  struct node* newNode = createNode(d);
  newNode->next = graph->adjLists[s];
  graph->adjLists[s] = newNode;
}

// Print the graph
void printGraph(struct Graph* graph) {
  int v;
  for (v = 0; v < graph->numVertices; v++) {
    struct node* temp = graph->adjLists[v];
    printf("%d ", v);
    while (temp) {
      printf("%d", temp->vertex);
      temp = temp->next;
    }
    printf("\n");
  }
}

// Helper function for finding all cycles
void findCycles(struct Graph* graph, int v, bool visited[], int path[], int pathLength) {
    int cyclel=0;
    visited[v] = true;
    path[pathLength] = v;
    pathLength++;

    struct node* neighbor = graph->adjLists[v];
    while (neighbor != NULL) {
        int neighborNode = neighbor->vertex;
        if (!visited[neighborNode]) {
            findCycles(graph, neighborNode, visited, path, pathLength);
        } else {
            // Check if the neighbor is part of the current path (cycle)
            for (int i = 0; i < pathLength; i++) {
                if (path[i] == neighborNode) {
                    for (int j = i; j < pathLength; j++) {
                        printf("%d ", path[j]);
                        cyclel++;
                    }
                    printf("%d\n",cyclel);
                }
            }
        }
        neighbor = neighbor->next;
    }

}

// Function to find and print all cycles in the graph
void findAllCycles(struct Graph* graph) {
    bool *visited = malloc(graph->numVertices * sizeof(struct Graph));
    int *path = malloc(graph->numVertices * sizeof(struct Graph));
    for (int i = 0; i < graph->numVertices; i++) {
        if (visited[i] == false){
            findCycles(graph, i, visited, path, 0);
        }
    }
}

int main(int argc, char *argv[]) {

  int numNodes, numEdges;
  FILE *cf;   // input configuration file

  if (argc == 4){
  // require argc be equal to number of command line entries
    numNodes = atol(argv[1]);
    numEdges = atol(argv[2]);
  }
  else {
    // error input value of argc
    fprintf(stderr,"\n Initialization error:\n");
    fprintf(stderr,"Usage: DFS.x N K Functiongraph\n");
    return 1;
  }

  struct Graph* graph = createAGraph(numNodes);


  cf = fopen(argv[3],"r");
  for (int i = 0; i < numEdges; i++) {
    int src, dest;
    fscanf(cf, "%d %d", &src, &dest);
    addEdge(graph, src, dest);
  }
  fclose(cf);

  //printGraph(graph);
  findAllCycles(graph);
  free(graph);

  return 0;
}
