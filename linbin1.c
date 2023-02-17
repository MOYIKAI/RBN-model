// Simple C example program that performs historgram binning
// by Yi-Kai Mo, originally written 8/2022
//
// Program uses Numerical Recipies (NR) arrays, random numbers, 
// and uses simple graphics to show the evolution of the lattice.
//
// Parameters:
// W - bin size
// inputfile - data need to be binning
// outfile   - output histogram

#include <stdio.h>	  // standard io library
#include <string.h>   // standard string library
#include <stdlib.h>	  // standard library with lots of functions
#include <math.h>	    // standard math library
#define NRANSI		    // needed for NR
#include "my_nrutil.h"    // include NR header files

int main(int argc, char *argv[]){ 
  int *nums; 
  int i,j,lnum;
  int max, min;
  int bin, nbins, W;
  double normalized, error, nSIG;

  int *histo;

  FILE *infile;   // pointer to inputfile
  FILE *outfile;	// pointer to outputfile

  if (argc == 5){
    fprintf(stderr,"\n Good Initializtion:\n");
    W = atol(argv[1]);      // read long variable from command line spot 1
    nSIG = atof(argv[2]);
  }
  else {			// error in value of argc 
    fprintf(stderr,"\n Initialization error:\n");
    fprintf(stderr,"Usage: linhist.x W nSIG infile outfile\n");  // correct input syntax 
    return 1;
  }
  

  if (infile = fopen(argv[3],"r"))
  {
    min = 0;  // min num in file
    max = 0;  // max num in file
    i = 0;
    j = 0;
    lnum = 0; // numbers of element in num array

    nums = ivector(0,100000000);

    // Read all lines in the file and save into num array
    while (fscanf(infile, "%d", &nums[i]) != EOF ){
      if (nums[i] < min) min = nums[i];
      if (nums[i] > max) max = nums[i];
      i++;
      lnum++;
    }
    fclose(infile);
    nbins = (max-min+1)/W;
    printf("%d, %d \n", min, max);

    histo = ivector(0,nbins);                    // use NR subroutine to allocate memory for the array histo[bin], where bin in range [-nbins, nbins]
    for(bin=0; bin<=nbins; ++bin){histo[bin]=0;} // initialize histo[] array

    // Get historgram
    for (j=0; j<=lnum; j++){bin = nums[j]/W;++histo[bin];}

    // Print the histogram value out
    outfile = fopen(argv[4],"w");
    for (bin=0; bin<=nbins; ++bin){
      normalized = (double) histo[bin]/lnum;
      error = nSIG*sqrt((double) histo[bin])/((double) lnum); 
      fprintf(outfile, "%d, %f, %f, %d, %d, %f\n", bin*W, normalized/W, error, bin*W, histo[bin],sqrt(histo[bin]));
    }
    fclose(outfile);

    free_ivector(histo,0,nbins);
    free_ivector(nums,0,100000000);
  }
  else{ fprintf(stderr,"No such input file \n"); return 1;}

  return 0;
}