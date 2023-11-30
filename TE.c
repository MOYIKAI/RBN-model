#include <stdio.h>	  // standard io library
#include <stdlib.h>	  // standard library with lots of functions
#include <math.h>     // standard math library
// TE x->y (Transfer Entropy X -> Y)
void TE_P(int **Pxbt, int **Pxntxbt, int **Pybtxbt, int **Pxntybtxbt, int *bnode, int *nnode, int N){
  int i,j,k;
  int w1=0; int w2=0;
  int c1=0; int c2=0;
  int w=0;  int h=0;
  for (i=0; i<N; i++){
    // P(X_t-1)
    Pxbt[i][bnode[i]]++;
    for (j=0; j<N; j++){
      // P(Y_t-1, X_t-1)
      w1=2*bnode[i]+bnode[j];
      Pybtxbt[c1][w1]++;
      c1++;

      // P(X_t, X_t-1)
      w2=2*nnode[i]+bnode[j];
      Pxntxbt[c2][w2]++;
      c2++;

      for (k=0; k<N; k++){
        // P(X_t, Y_t-1, X_t-1)
        w=4*nnode[i]+2*bnode[j]+bnode[k];
        Pxntybtxbt[h][w]++;
        h++;
      }
    }
  }
}

void TE_E(int **Pxbt, int **Pxntxbt, int **Pybtxbt, int **Pxntybtxbt, double *Sxbt, double *Sxntxbt, double *Sybtxbt, double *Sxntybtxbt, int N, int len){
  double Pabc, Pab, Pa;
  int i, j, k;
  int c, h;
  int x,xy, xyz;
  c=0; // Combination number 
  h=0;
  // Assigned Joint entropy and entropy
  for (i=0; i<N; i++){
    for (x=0; x<2; x++){
      Pa  = (double) Pxbt[i][x]/len;
      if (Pa!=0) {Sxbt[i] = Sxbt[i] - Pa*log2(Pa);}
    }
        
    for (j=0; j<N; j++){
      for (xy=0; xy<4; xy++){
        // P(X_t, X_t-1) to S(X_t, X_t-1)
        Pab = (double) Pxntxbt[c][xy]/len;
        if (Pab!=0) {Sxntxbt[c] = Sxntxbt[c] - Pab*log2(Pab);}

        // P(Y_t-1, X_t-1) to S(Y_t-1, X_t-1)
        Pab = (double) Pybtxbt[c][xy]/len;
        if (Pab!=0) {Sybtxbt[c] = Sybtxbt[c] - Pab*log2(Pab);}
      }
      c++;

      for (k=0; k<N; k++){
        //P(X_t, Y_t-1, X_t-1) to S(X_t, Y_t-1, X_t-1)
        for (xyz=0; xyz<8; xyz++){
          Pabc = (double) Pxntybtxbt[h][xyz]/len;
          if (Pabc!=0) {Sxntybtxbt[h] = Sxntybtxbt[h] - Pabc*log2(Pabc);}
        }
        h++;
      }
    }
  }
}

double Transfer_Entropy(double *Sxbt, double *Sxntxbt, double *Sybtxbt, double *Sxntybtxbt, int N){
  int i,j,k;
  int c=0; int h=0;
  double TE=0;
  for (i=0; i<N; i++){
    for (j=0; j<N; j++){
      for (k=0; k<N; k++){
        TE = TE + Sxntxbt[i*N+j] + Sybtxbt[j*N+k] - Sxbt[k] - Sxntybtxbt[h];
        //printf("%d: %lf \n", h, Sxntxbt[i*N+k] + Sybtxbt[j*N+k] - Sxbt[k] - Sxntybtxbt[h]);
        h++;
      }
      c++;
    }
  }
  //printf("TE %lf, TEnor %lf\n",TE, TE/(N*N*N));
  return TE;
}

void showTE(double *TE, int S){
  int i;
  for (i=0; i<S; i++){
    printf("TE[%d]:%lf ", i, TE[i]);
    printf("\n");
  }
}