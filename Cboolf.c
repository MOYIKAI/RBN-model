// Simple C program that read nodes Boolean functions set and chnage one nodes's Boolean set. Originally written by Yi-Kai Mo 06/2023
// Program uses Numerical Recipies (NR) arrays, random numbers.
//
// Parameters:
// 
/*  Mututation of Boolean functions. Choose one specific node and change its Boolean functions */

#include <string.h>   // standard string library
#include <stdio.h>	  // standard io library
#include <stdlib.h>	  // standard library with lots of functions
#include <time.h>     // standard library for measuring the time
#include <math.h>	  // standard math library
#include "my_nrutil.h"// include NR header files
#include "random.h"
#define NRANSI		  // needed for NR

/* Global variable*/
long N; // Node 
long K; // in-degree number for each node
long F; // number of possible input states which is 2^K 
long seed, tpseed;// seed(Random number for the changing one node's boolean functions)
clock_t TIME; // time for program



/* Global matrix */
int **functions; // Boolean functions for all nodes

/* Functions */
void free_all();   // free the memory 
void init();       // initialized the RBN
void changebool(); // change a node's boolean functions

int main(int argc, char *argv[]){
    TIME = clock();

    int i,j,k,num;
    
    FILE *ibf;	// input boolean functions file
    FILE *obf;  // output boolean functions file

    if (argc == 6){
        // require argc be equal to number of command line entries
        N = atol(argv[1]);
        K = atol(argv[2]);
        seed = atol(argv[3]);
        tpseed = seed;
    } 
    else {
        // error input value of argc 
        fprintf(stderr,"\n Initialization error:\n");
        fprintf(stderr,"Usage: Cboolf.x N k seed intialboolf finalboolf\n");
        return 1;
    }

    // Number of possible input states
    F = pow(2,K); 
    // Boolean functions for each node. Row(node number); Columns(Truth table number); elements(output of truth table for specific node)
    functions = imatrix(0,N-1,0,F-1);
    

    // Read file and fill in the Boolean functions
    ibf = fopen(argv[4],"r");
    for (i=0; i<N; ++i){
        for (j=0; j<F; ++j){
            fscanf(ibf, "%d", &functions[i][j]);
        }
    }
    fclose(ibf);
    
    changebool();

    // Print boolean functions info
    obf = fopen(argv[5],"w");
    for (i=0; i<N; ++i){
        for (j=0; j<F; ++j){ fprintf(obf,"%d ",functions[i][j]);} // function output
        fprintf(obf, "\n");
    }
    fclose(obf);

    free_all();
    
    return 0;
}

void free_all(){
    free_imatrix(functions, 0, N-1, 0, F-1);
    return;
}

void changebool(){
    int j,k;
    int cnode;    // node number need to be change
    long boolnum; // labels of the boolean functions set 
    cnode = N*ran2(&seed);
    boolnum = ran2(&seed)*pow(2,pow(2,K));

    for (j=0; j<F; ++j){
        functions[cnode][j] = boolnum % 2;
        boolnum = boolnum / 2;
    }

}