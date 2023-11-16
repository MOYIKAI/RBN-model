#include <string.h>   // standard string library
#include <stdio.h>	  // standard io library
#include <stdlib.h>	  // standard library with lots of functions
#include <math.h>	    // standard math library
void shownode(int *node, int N){
  int i;
  // Print a configuration
  printf("Nodes' config \n");
  for (i=0; i<N; ++i){
    printf("%d ", node[i]);
  }
  printf("\n");
}
void showftable(int **ftable, int N, int K){
  int i, j;
  // Print Initial Population
  printf("Nodes' Boolean table \n");
  for (i=0; i<N; ++i){
    printf("Node %d function table\n",i);
    for (j=0; j< (int) pow(2,K); ++j){printf("%d ", ftable[i][j]);}
    printf("\n");
  }
}
void showconnect(int **connect, int N, int K){
  int i, j;
  // Print in-degree info
  printf("Structure \n");
  for (i=0; i<N; ++i){
    for (j=0; j<K; ++j){ printf("%d ",connect[i][j]);}
    printf("\n");
  }
}
void showBoolfS(int **BoolfS, int N, int K, int S){
  int i, j, k, tmp, b;
  // Print Boolean Population
  printf("Boolean function population \n");
  for (i=0; i<S; ++i){
    printf("Population %d \n",i);
    for (j=0; j<N; ++j){
      printf("%d ",BoolfS[i][j]);
      printf("\n");
      tmp = BoolfS[i][j];
      for (k=0; k<(int) pow(2,K); k++){b=tmp%2; tmp/=2; printf("%d ",b);}
      printf("\n");
    }
  }
}
void showIniS(int **IniS, int N, int S){
  int i, j;
  // Print Initial Population
  printf("Initial config population \n");
  for (i=0; i<S; ++i){
    printf("Population %d\n",i);
    for (j=0; j<N; ++j){printf("%d ", IniS[i][j]);}
    printf("\n");
  }
}
void showPx(int **Px, int N){
  int i,k;
  for (i=0; i<N; i++){
    for (k=0; k < 2; k++){printf("Px[%d][%d]:%d ", i, k, Px[i][k]);}
    printf("\n");
  }
}
void showPxy(int **Pxy, int N){
  int i,k;
  for (i=0; i<N; i++){
    for (k=0; k < 4; k++){printf("Pxy[%d][%d]:%d ", i, k, Pxy[i][k]);}
    printf("\n");
  }
}
void showSx(double *Sx, int N){
  int i;
  for (i=0; i<N; i++){
    printf("Sx[%d]:%lf ", i, Sx[i]);
    printf("\n");
  }
}
void showSxy(double *Sxy, int C){
  int i;
  for (i=0; i<C; i++){
    printf("Sxy[%d]:%lf ", i, Sxy[i]);
    printf("\n");
  }
}
void showrep(int *rep, int S){
  int i;
  for (i=0; i<S; i++){
    printf("rep[%d]:%d ", i, rep[i]);
    printf("\n");
  }

}
void showfitness(double *fitness, int S){
  int i;
  for (i=0; i<S; i++){
    printf("fitness[%d]:%lf ", i, fitness[i]);
    printf("\n");
  }
}
void showMI(double *MI, int S){
  int i;
  for (i=0; i<S; i++){
    printf("MI[%d]:%lf ", i, MI[i]);
    printf("\n");
  }
}