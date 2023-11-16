#include <string.h>   // standard string library
#include <stdio.h>	  // standard io library
#include <stdlib.h>	  // standard library with lots of functions
#include <math.h>	  // standard math library

void saveAttractor(char *name, int *Att, int S, int G);
void saveconnect(char *name, int **connect, int N, int K);
void saveBoolfS(char *name, int **BoolfS, int N, int K, int S, int G);
void saveIniS(char *name, int **IniS, int N, int S, int G);
void savenode(char *name, int *node, int N, int G);
void saveMI(char *name, double *MI, int S, int G);