// Adjascency List representation in C
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include "my_nrutil.h"// include NR header files


int main(int argc, char *argv[]) {

  int data,i;
  int clength,cnum; // cycle length
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
  while(fscanf(cf,"%d", &clength) != EOF){
    printf("cycle length %d \n",clength);
    for (i=0; i<clength; i++){
      fscanf(cf,"%d", &data);
      printf("%d ",data);
    }
    printf("\n");
    cnum++;
  }
  fclose(cf);

  cf = fopen(argv[1],"r");
  while(fscanf(cf,"%d",&clength) != EOF){
    printf("%d \n",clength);
  }
  fclose(cf);
  return 0;

}
