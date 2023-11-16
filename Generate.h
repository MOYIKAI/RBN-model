#include <string.h>   // standard string library
#include <stdio.h>	  // standard io library
#include <stdlib.h>	  // standard library with lots of functions
#include <math.h>	  // standard math library

// Reset info for arrays
void Zeros1d_int(int *vector, int row);
void Zeros2d_int(int **matrix, int row, int col);
void Zeros1d_dobule(double *vector, int row);

// Generate network structure
void Gindeg(int **connect, int N, int K, int seed);
// Generate one specific Initial condition for all population
void GIniS1(int *node, int N, long seed);
// Generate Uniform Initial condition population
void GIniS2(int **IniS, int N, int S, int seed);
// Generate Uniform boolean function population
void GBoolfS(int **BoolfS, int N, int K, int S, float p, long seed);

// Duplicate or Mutation
void GenerateNextS(int **BoolfS, int **BoolfSN, int *rep, float eps, int N, int K, int S, long seed);