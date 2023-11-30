#include <stdio.h>	  // standard io library
#include <stdlib.h>	  // standard library with lots of functions
#include <math.h>     // standard math library
// For TE (Transfer Entropy)
void TE_P(int **Pxbt, int **Pxntxbt, int **Pybtxbt, int **Pxntybtxbt,int *bnode, int *nnode, int N);
void TE_E(int **Pxbt, int **Pxntxbt, int **Pybtxbt, int **Pxntybtxbt, double *Sxbt, double *Sxntxbt, double *Sybtxbt, double *Sxntybtxbt, int N, int len);
double Transfer_Entropy(double *Sxbt, double *Sxntxbt, double *Sybtxbt, double *Sxntybtxbt, int N);
void showTE(double *TE, int S);