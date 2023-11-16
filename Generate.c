#include <string.h>   // standard string library
#include <stdio.h>	  // standard io library
#include <stdlib.h>	  // standard library with lots of functions
#include <math.h>	  // standard math library
#include "random.h"

// Reset info for arrays
void Zeros1d_int(int *vector, int row){
  int i;
  for (i=0; i<row; i++){vector[i]= 0;}
}
void Zeros2d_int(int **matrix, int row, int col){
  int i,j;
  for (i=0; i<row; i++){for (j=0; j < col; j++){matrix[i][j] = 0;}}
}
void Zeros1d_dobule(double *vector, int row){
  int i;
  for (i=0; i<row; i++){vector[i]= 0;}
}

// Generate network structure
void Gindeg(int **connect, int N, int K, long seed){
  int i,j;
  long rollint;
  // Randomly connected the in-degree
  for (i=0; i<N; ++i){
    for (j=0; j<K; ++j){
  // while loop to avoid self-connect
      rollint = N*ran2(&seed); // unifrom distrubution between 0 to N-1 integer numbers
      while ( i == rollint ){rollint = N*ran2(&seed);}
      connect[i][j] = rollint;
    }
  }
}

// Generate one specific Initial condition for all population
void GIniS1(int *node, int N, long seed){ 
  int i;
  // Generate Initial configuration
  for (i=0; i<N; ++i){
    if(ran1(&seed) > 0.5){node[i] = 1;}
    else{node[i]=0;}
  }
}

// Generate Uniform Initial condition population 
void GIniS2(int **IniS, int N, int S, long seed){ 
  int i,j;
  // Generate Initial Population uniformally
  for (i=0; i<S; ++i){
    for (j=0; j<N; ++j){
      if(ran1(&seed) > 0.5){IniS[i][j] = 1;}
      else{IniS[i][j]=0;}
    }
  }
}
// Generate Uniform boolean function population
void GBoolfS(int **BoolfS, int N, int K, int S, float p, long seed){
  int i,j,k;
  int F=pow(2,K);
  double dice;
  for (i=0; i<S; ++i){
    for (j=0; j<N; ++j){
      dice = ran1(&seed);
      //printf("%lf ",dice);
      if (dice < 0.5){
        for (k=0; k<F; ++k){
          dice = ran1(&seed);
          if (dice < p) {BoolfS[i][j]+= (int) pow(2,k);} // bias output
        }
      }
      else{
        for (k=0; k<F; ++k){
          dice = ran1(&seed);
          if (dice < (1-p)) {BoolfS[i][j]+= (int) pow(2,k) ;} // bias output

        }
      }
    }
  }
}

// Duplicate or Mutation
void GenerateNextS(int **BoolfS, int **BoolfSN, int *rep, float eps, int N, int K, int S, long seed){
  int i,j,k,rollint;
  double dM;   // For mutation
  for (i=0; i<S; ++i){
    dM = ran1(&seed);
    // Duplicate
    if(eps < dM){
      //printf("eps %f dM %f Duplicate\n", eps, dM);
      for (j=0; j<N; ++j){BoolfSN[i][j] = BoolfS[rep[i]][j];}
    } // rep[i] = population index which need to be replicate or Mutate
    
    // Mutate one node's Boolean function set
    else{
      rollint = (int) N*ran1(&seed); // Node which should be mutate
      //printf("eps %f dM %f Mutate node rollint %d\n", eps, dM, rollint);
      for (j=0; j<N; ++j){
        if (j == rollint){BoolfSN[i][j] = (int) pow(2,pow(2,K))*ran1(&seed);} // Randomly assiged boolean function set (Uniform distribution)
        else{BoolfSN[i][j] = BoolfS[rep[i]][j];}
      }
    }
  }
}