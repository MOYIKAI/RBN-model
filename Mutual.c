// Adjascency List representation in C
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include "my_nrutil.h"// include NR header files
float Mutual(int** cycles, int* clength, int cnum){
  float MI=0;
  int C=cnum*(cnum-1)/2;
  int i,j,k,l,data;
  bool ***binconfig = (bool***) malloc(cnum * sizeof(bool**));
  float **Px = (float**) malloc(cnum * sizeof(float*));
  float ***Pxy = (float ***) malloc(cnum * sizeof(float**));
  float **Sx = (float**) malloc(cnum* sizeof(float*));
  float ***Sxy = (float ***) malloc(cnum * sizeof(float**));

  // Assign configuration into boolean configuration
  for (i=0; i<cnum; i++){
    binconfig[i] = (bool**) malloc(clength[i] * sizeof(bool*));
    printf("cycle length %d\n", clength[i]);
    for (j=0;j<clength[i]; j++){
      binconfig[i][j] = (bool*) malloc(10 * sizeof(bool));
      data = cycles[i][j];
      for (k=0; k < 10; k++){
        binconfig[i][j][k] = data % 2;
        data = data/2;
        printf("%d ", binconfig[i][j][k]);
      }
      printf("%d\n", cycles[i][j]);
    }
  }


  // From binary configuration to Probability
  for (i=0; i<cnum; i++){
    Px[i] = (float*) malloc(clength[i] * sizeof(float));
    printf("cycle length %d\n", clength[i]);
    for (j=0; j<clength[i];j++){   
      for (k=0; k <10; k++){
        if (binconfig[i][j][k] == 1){ Px[i][j]++;}
      }
      Px[i][j] = Px[i][j]/10;
      printf("Px:%f\n", Px[i][j]);
    }
  }

  for (i=0;i<cnum;i++){
    for (j=0;j<clength[i]; j++){free(binconfig[i][j]);}
    free(binconfig[i]);
    free(Px[i]);
  }
  free(binconfig);
  free(Pxy);
  free(Px);

  free(Sx);
  free(Sxy);
  return MI;
}

int main(int argc, char *argv[]) {

  int data,i,j;
  int cnum; // cycle length
  float MI;
  FILE *cf;   // input configuration file

  if (argc == 2){
  // require argc be equal to number of command line entries
  }
  else {
    // error input value of argc
    fprintf(stderr,"\n Initialization error:\n");
    fprintf(stderr,"Usage: Mutual.x Cycledata\n");
    return 1;
  }
  // Fisrt time to read the data info
  cnum=0;
  cf = fopen(argv[1],"r");
  while(fscanf(cf,"%d", &j) != EOF){
    for (i=0; i<j; i++){fscanf(cf,"%d", &data);}
    cnum++;
  }
  fclose(cf);

  int **cycles = (int**) malloc(cnum * sizeof(int*));
  int *clength = (int *) malloc(cnum * sizeof(int));
  cf = fopen(argv[1],"r");
  j=0;
  while(fscanf(cf,"%d",&clength[j]) != EOF){
    cycles[j] = (int*) malloc(clength[j] * sizeof(int));
    for (i=0; i<clength[j]; i++){fscanf(cf,"%d", &cycles[j][i]);}
    j++;
  }
  fclose(cf);


  MI = Mutual(cycles, clength, cnum);
  printf("%f\n", MI);

  for (j=0;j<cnum;j++){ free(cycles[j]);}
  free(cycles);
  return 0;

}