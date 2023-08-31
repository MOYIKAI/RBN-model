// Adjascency List representation in C
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include "my_nrutil.h"// include NR header files

void findCycles(int v, int pathlength, int **carrays, int* clength, int* graph, int* path, bool* carray, bool* visited, int node) {
  int i;
  // Recursive 
  if (visited[v] == false) {
    visited[v] = true;
    path[pathlength] = v;
    pathlength++;
    findCycles(graph[v], pathlength, carrays, clength, graph, path, carray, visited, node);
  }
  // Exit if visted is ture
  else{
    // From the path array and check if the cycles exits
    for (i=0; i<pathlength; i++){if (graph[v] == path[i]){carray[node] = 1;break;}}
    
    if (carray[node] == 1){
      int tmp = graph[v]; // save the starting points of the cycle
      for (i=0;i<pathlength;i++){
        if (tmp != v){clength[node]++; carrays[node][i] = tmp;tmp = graph[tmp];}
        else{clength[node]++; carrays[node][i] = tmp; break;}
      }
    }
  }
}
// Function to find and print Detected cycles in the graph
void findAllCycles(int *graph, int numVertices) {
  bool *visited = malloc(numVertices * sizeof(bool));
  bool *carray = malloc(numVertices* sizeof(bool));
  int **carrays = imatrix(0,numVertices,0,numVertices); 
  int *clength = malloc(numVertices * sizeof(int));
  int *path = malloc(numVertices * sizeof(int));

  int i,j;
  for (i = 0; i < numVertices; i++) {
    visited[i] = 0;
    carray[i] =0;
    clength[i] =0;
    path[i] = 0;
  }

  for (i = 0; i < numVertices; i++) {
    findCycles(i, 0, carrays, clength, graph, path, carray, visited, i); 
    if(carray[i]){
      for (j=0;j<clength[i]; j++){printf("%d ",carrays[i][j]);}
      printf("find cycle %d\n",clength[i]);
    }
  }

  free(carray);
  free_imatrix(carrays,0,numVertices,0,numVertices);
  free(clength);
  free(visited);
  free(path);

  return;

}


int main(int argc, char *argv[]) {

  int numNodes;
  int *confignet;    // Configuration network
  FILE *cf;   // input configuration file

  if (argc == 3){
  // require argc be equal to number of command line entries
    numNodes = atol(argv[1]);
  }
  else {
    // error input value of argc
    fprintf(stderr,"\n Initialization error:\n");
    fprintf(stderr,"Usage: DFS.x N Functiongraph\n");
    return 1;
  }

  // information Configuration network
  confignet = ivector(0,numNodes);
  
  cf = fopen(argv[2],"r");
  for (int i = 0; i < numNodes; i++) {
    fscanf(cf, "%d", &confignet[i]);
    //printf("%d\n", confignet[i]);
  }
  fclose(cf);
  findAllCycles(confignet, numNodes);
  free_ivector(confignet,0,numNodes);
  return 0;

}