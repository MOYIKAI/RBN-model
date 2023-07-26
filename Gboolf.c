// Simple C program that generates Boolean functions of a RBN. Originally written by Yi-Kai Mo 06/2023
// Program uses Numerical Recipies (NR) arrays, random numbers.
//
// Parameters:
// N - number of Nodes for the Network 
// k - number of in-degree links for each node
// p - probability of the output
/* Given the Probability p, p is the probability that output of Boolean functions is 1 */

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
long seedb, tpseedb;// seedt(Random number for the topology), seedb(Random number for assigned boolean function)
float p;            // Probability that outputs of Boolean functions is 1. Ex: p=0.5 is unbias
clock_t TIME; // time for program


/* Global matrix */
int **functions; // Boolean functions for all nodes

/* Functions */
void free_all();   // free the memory 
void init();       // initialized the RBN

int main(int argc, char *argv[]){
    TIME = clock();

    int i,j,k,num;
    
    FILE *boolff;	// pointer to boolean functions file
    if (argc == 6){
        // require argc be equal to number of command line entries
        N = atol(argv[1]);
        K = atol(argv[2]);
        p = atof(argv[3]);
        seedb = atol(argv[4]);

        tpseedb = seedb;
    } 
    else {
        // error input value of argc 
        fprintf(stderr,"\n Initialization error:\n");
        fprintf(stderr,"Usage: Gboolf.x N k p seedb booleanfile\n");
        return 1;
    }

    // Number of possible input states
    F = pow(2,K);
    // Boolean functions for each node. Row(node number); Columns(Truth table number); elements(output of truth table for specific node)
    functions = imatrix(0,N-1,0,F-1);

    // initialized
    init();

    // Print boolean functions info
    boolff = fopen(argv[5],"w");
    for (i=0; i<N; ++i){
        for (j=0; j<F; ++j){ fprintf(boolff,"%d ",functions[i][j]);} // function output
        fprintf(boolff, "\n");
    }
    fclose(boolff);

    free_all();

    return 0;
}

void free_all(){
    free_imatrix(functions, 0, N-1, 0, F-1);
    return;
}

void init(){
    int i,j,fl;
    float dice;
    float roll;
    int rollint;

    // Randomly assigned the truth table output
    for (i=0; i<N; ++i){
        dice = ran1(&seedb);
        if (dice < 0.5){
            for (j=0; j<F; ++j){
                dice = ran1(&seedb);
                if (dice < p) {functions[i][j] = 1;} // bias output
                else {functions[i][j]=0;}
            }
        }
        else{
            for (j=0; j<F; ++j){
                dice = ran1(&seedb);
                if (dice < (1-p)) {functions[i][j] = 1;} // bias output
                else {functions[i][j]=0;}
            }
        }
    }
}