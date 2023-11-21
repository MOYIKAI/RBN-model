#include <string.h>   // standard string library
#include <stdio.h>	  // standard io library
#include <stdlib.h>	  // standard library with lots of functions
#include <math.h>	  // standard math library

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

void showMI(double *MI, int S){
  int i;
  for (i=0; i<S; i++){
    printf("MI[%d]:%lf ", i, MI[i]);
    printf("\n");
  }
}