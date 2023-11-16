#include <string.h>   // standard string library
#include <stdio.h>	  // standard io library
#include <stdlib.h>	  // standard library with lots of functions
#include <math.h>	    // standard math library
void saveAttractor(char *name, int *Att, int S, int G){
  int i;
  FILE *file = fopen(name, "a");
  fprintf(file, "G %d\n", G);
  for (i=0; i<S; i++){
    fprintf(file, "%-3d ", Att[i]);
  }
  fprintf(file, "\n");
  fclose(file);
}

void saveconnect(char *name, int **connect, int N, int K){
  int i, j;
  FILE *file = fopen(name, "w");
  for (i=0; i<N; ++i){
    for (j=0; j<K; ++j){ fprintf(file, "%-3d ",connect[i][j]);}
    fprintf(file, "\n");
  }
  fclose(file);
}
void saveBoolfS(char *name, int **BoolfS, int N, int K, int S, int G){
  int i, j;
  FILE *file = fopen(name, "a");
  fprintf(file, "G %d\n", G);
  for (i=0; i<S; ++i){
    for (j=0; j<N; ++j){
      fprintf(file, "%-4d ",BoolfS[i][j]);
    }
    fprintf(file, "\n");
  }
  fclose(file);
}
void saveIniS(char *name, int **IniS, int N, int S, int G){
  int i, j;
  FILE *file = fopen(name, "a");
  fprintf(file, "G %d\n", G);
  for (i=0; i<S; ++i){
    for (j=0; j<N; ++j){fprintf(file, "%d ", IniS[i][j]);}
    fprintf(file, "\n");
  }
  fclose(file);
}
void savenode(char *name, int *node, int N, int G){
  int i;
  FILE *file = fopen(name, "a");
  fprintf(file, "G %d\n", G);
  for (i=0; i<N; i++){
    fprintf(file, "%d ", node[i]);
    fprintf(file, "\n");
  }
  fclose(file);
}
void saveMI(char *name, double *MI, int S, int G){
  int i;
  FILE *file = fopen(name, "a");
  fprintf(file, "G %d\n", G);
  for (i=0; i<S; i++){
    fprintf(file, "%lf ", MI[i]);
  }
  fprintf(file, "\n");
  fclose(file);
}