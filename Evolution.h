#include <string.h>   // standard string library
#include <stdio.h>	  // standard io library
#include <stdlib.h>	  // standard library with lots of functions
#include <math.h>	  // standard math library

// Binary to Decimal
int Bin2Dec(int *node, int N);

// For Evolution
void Replicate(int *rep, double *fit, int S, long seed);

// For attractor
int compare(int *node, int *tnode, int N);
void Nextconfig(int **connect, int **ftable, int *node, int *mnode, int N, int K);

// Coping 
void ABP2ftable(int *BoolfS, int **ftable, int N, int K);
void AIP2node(int *IniS, int *node, int N);
void Anode2tnode(int *node, int *tnode, int N);
void cpNextG_BoolfS(int **BoolfS, int **BoolfSN, int S, int N);