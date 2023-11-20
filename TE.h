#include <stdio.h>	  // standard io library
#include <stdlib.h>	  // standard library with lots of functions
#include <math.h>     // standard math library
// For TE (Transfer Entropy)
void TE_P(int **Pxbt, int **Pxnt, int **Pxntybt, int **Pyntybt, int **Pxntybtxbt,int *bnode, int *nnode, int N);
double Transfer_Entropy(double *Sxbt, double *Sxntybt, double *Syntybt, double *Sxntybtxbt, int N);
void TE_E(int **Pxbt, int **Pxntybt, int **Pyntybt, int **Pxntybtxbt, double *Sxbt, double *Sxntybt, double *Syntybt, double *Sxntybtxbt, int N, int len);