#include <string.h>   // standard string library
#include <stdio.h>	  // standard io library
#include <stdlib.h>	  // standard library with lots of functions
#include <math.h>	  // standard math library
#include "random.h"


int Bin2Dec(int *node, int N){
  int i;
  int dec=0;
  for (i=0; i<N; ++i) dec=dec+node[i]*pow(2,i);

  return dec;
}

// For Evolution
void Replicate(int *rep, double *fit, int S, long seed){
  double gate,dice;
  int i,j;

  for (i=0; i < S; i++){
    dice = ran1(&seed);
    j = 0;
    gate = 0;
    while(gate < dice){
      gate = gate + fit[j];
      if (gate > dice){rep[i]= j;}
      j++;
    }
    //printf("Dice %f gate %lf rep[%d]=%d\n", dice, gate, i, rep[i]);
  }
}

// MI
void SumProbability(int **Pxy, int **Px, int *node, int N){
  int i,j;
  int w=0;
  int c=0;
  for (i=0; i<N; i++){
    Px[i][node[i]]++;
    for (j=i+1; j<N; j++){
      w=2*node[i]+node[j];
      Pxy[c][w]++;
      c++;
    }
  }
}
double Mutual(double *Sxy, double *Sx, int N){
  int i,j;
  int c=0;
  double MI=0;
  for (i=0; i<N; i++){
    for (j=i+1; j<N; j++){
      MI = MI + Sx[i] + Sx[j] - Sxy[c];
      //printf("%lf %lf %lf %lf\n", Sx[i], Sx[j], Sxy[c], MI);
      c++;
    }
  }
  return MI;
}
void Entropy(int **Pxy, int **Px, double *Sxy, double *Sx, int N, int len){
  double Pab, Pa;
  int i,j;
  int c;
  int x,xy;
  c=0; // Combination number 
  // Assigned Joint entropy and entropy
  for (i=0; i<N; i++){
    for (x=0; x<2; x++){
      Pa  = (double) Px[i][x]/len;
      if (Pa!=0) {Sx[i] = Sx[i] - Pa*log2(Pa);}
    }
        
    for (j=i+1; j<N; j++){
      for (xy=0; xy<4; xy++){
        Pab = (double) Pxy[c][xy]/len;
        if (Pab!=0) {Sxy[c] = Sxy[c] - Pab*log2(Pab);}
      }
      c++;
    }
  }
}

// For attractor
int compare(int *node, int *tnode, int N) {
  int i;
  for(i = 0; i < N; ++i) {
    if (tnode[i] != node[i]) return 0;
  }
  return 1;
}
void Nextconfig(int **connect, int **ftable, int *node, int *mnode, int N, int K){
  int i,j,k,n;
  for (i=0; i<N; ++i){
    n=0;
    for (j=0; j<K; ++j){
      n=n+node[connect[i][j]]*pow(2,j);
      // printf("%d %d \n",connect[i][j], node[connect[i][j]]);
    }
    // printf("result %d %d", n, ftable[i][n]);
    mnode[i] = ftable[i][n];
  }
  for (i=0; i<N; ++i){node[i] = mnode[i];}
}

// Coping 
void ABP2ftable(int *BoolfS, int **ftable, int N, int K){
  int j, k, b, tmp;
  for (j=0; j<N; ++j){
    tmp = BoolfS[j];
    for (k=0; k<(int) pow(2,K); k++){
      b=tmp%2; tmp/=2;
      ftable[j][k] = b;
    }
  }
}
void AIP2node(int *IniS, int *node, int N){
  int i;
  for (i=0; i<N; ++i){node[i] = IniS[i];}
}
void Anode2tnode(int *node, int *tnode, int N){
  int i;
  for (i=0; i<N; ++i){tnode[i] = node[i];}
}
void cpNextG_BoolfS(int **BoolfS, int **BoolfSN, int S, int N){
  int i,j;
  for (i=0; i<S; i++){
    for (j=0; j<N; j++){
      BoolfS[i][j] = BoolfSN[i][j];
    }
  }
}