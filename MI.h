#include <string.h>   // standard string library
#include <stdio.h>	  // standard io library
#include <stdlib.h>	  // standard library with lots of functions
#include <math.h>	  // standard math library

// For MI
void SumProbability(int **Pxy, int **Px, int *node, int N);
void Entropy(int **Pxy, int **Px, double *Sxy, double *Sx, int N, int len);
double Mutual(double *Sxy, double *Sx, int N);
void showMI(double *MI, int S);