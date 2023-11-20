#include <stdio.h>	  // standard io library
#include <stdlib.h>	  // standard library with lots of functions
#include <math.h>     // standard math library
// TE x->y (Transfer Entropy X -> Y)
void TE_P(int **Pxbt, int **Pxnt, int **Pxntybt, int **Pyntybt, int **Pxntybtxbt, int *bnode, int *nnode, int N){
  int i,j,k;
  int w=0; 
  int c1=0; int c2=0;
  int h=0;
  for (i=0; i<N; i++){
    Pxbt[i][bnode[i]]++;
    Pxnt[i][nnode[i]]++;
    for (j=0; j<N; j++){
      w=2*nnode[i]+bnode[j];
      Pyntybt[c1][w]++;
      c1++;
      for (k=0; k<N; k++){
        w=4*nnode[i]+2*bnode[j]+bnode[k];
        Pxntybtxbt[h][w]++;
        h++;
      }
    }
    for (j=N-1; j>-1; j--){
      w=2*nnode[i]+bnode[j];
      Pxntybt[c2][w]++;
      c2++;
    }
  }
}

double Transfer_Entropy(double *Sxbt, double *Sxntybt, double *Syntybt, double *Sxntybtxbt, int N){
  int i,j,k;
  int c=0; int h=0;
  double TE=0;
  for (i=0; i<N; i++){
    for (j=0; j<N; j++){
      for (k=0; k<N; k++){
        TE = TE + Syntybt[c] + Sxntybt[c] - Sxbt[i] - Sxntybtxbt[h];
        h++;
      }
      c++;
    }
  }
  return TE;
}

void TE_E(int **Pxbt, int **Pxntybt, int **Pyntybt, int **Pxntybtxbt, double *Sxbt, double *Sxntybt, double *Syntybt, double *Sxntybtxbt, int N, int len){
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
        Pab = (double) Pxntybt[c][xy]/len;
        if (Pab!=0) {Sxntybt[c] = Sxntybt[c] - Pab*log2(Pab);}

        Pab = (double) Pyntybt[c][xy]/len;
        if (Pab!=0) {Sxntybt[c] = Sxntybt[c] - Pab*log2(Pab);}
      }
      c++;
      for (k=0; k<N; k++){
        for (xyz=0; xy<8; xyz++){
          Pabc = (double) Pxntybtxbt[h][xyz]/len;
          if (Pabc!=0) {Sxntybt[h] = Sxntybt[h] - Pabc*log2(Pabc);}
        }
        h++;
      }
    }
  }
}