#include <string.h>   // standard string library
#include <stdio.h>	  // standard io library
#include <stdlib.h>	  // standard library with lots of functions
#include <math.h>	  // standard math library

void shownode(int *node, int N);
void showftable(int **ftable, int N, int K);
void showconnect(int **connect, int N, int K);
void showBoolfS(int **BoolfS, int N, int K, int S);
void showIniS(int **IniS, int N, int S);
void showPx(int **Px, int N);
void showPxy(int **Pxy, int C);
void showSx(double *Sx, int N);
void showSxy(double *Sxy, int C);
void showrep(int *rep, int S);
void showfitness(double *fitness, int S);
void showMI(double *MI, int S);