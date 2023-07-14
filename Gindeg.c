// Simple C program that explores the RBN(Random Boolean Network) configuration space network . Originally written by Yi-Kai Mo 06/2023
// Program uses Numerical Recipies (NR) arrays, random numbers.
//
// Parameters:
// N - number of Nodes for the Network 
// k - number of in-degree links for each node
// p - probability of the output
/*
This RBN model is setup by 3 steps:
    1. First randomly choose the state(0 or 1) for each node
    2. Randomly assigned the in-degree links for each node. It does not allow self-connected.
    3. Given the Probability p, p is the probability that output of Boolean function is 1*/

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
long seedt, tpseedt; // seedt(Random number for the topology)
clock_t TIME; // time for program


/* Global vector */
int *node;         // node's Configuration

/* Global matrix */
int **connect;   // in-degree for connection list

/* Functions */
void free_all();   // free the memory 
void init();       // initialized the Strudture of RBN

int main(int argc, char *argv[]){
    TIME = clock();

    int i,j,k,num;
    
    FILE *indegf;   // pointer to in-degree file
    if (argc == 5){
        // require argc be equal to number of command line entries
        N = atol(argv[1]);
        K = atol(argv[2]);
        seedt = atol(argv[3]);

        tpseedt = seedt;
    } 
    else {
        // error input value of argc 
        fprintf(stderr,"\n Initialization error:\n");
        fprintf(stderr,"Usage: Gindeg.x N k seedt in-degreefile\n");
        return 1;
    }

    // Number of possible input states
    F = pow(2,K);
    // State of each node will be 1 or 0. Index means node number; elements means the state
    node = ivector(0,N-1);
    // Connection list for in-degree. Row(node number); Columns(number of links); elements(connected nodes)
    connect = imatrix(0,N-1,0,K-1);
    
    
    // initialized
    init();

    // Print in-degree info
    indegf = fopen(argv[6],"w");
    for (i=0; i<N; ++i){
        for (j=0; j<K; ++j){ fprintf(indegf,"%d ",connect[i][j]);}
        fprintf(indegf, "\n");
    }
    fclose(indegf);


    free_all();

    return 0;
}

void free_all(){
    free_ivector(node, 0, N-1);
    free_imatrix(connect, 0, N-1, 0, K-1);
    return;
}


void init(){
    int i,j;
    int rollint;

    // Randomly connected the in-degree
    for (i=0; i<N; ++i){
        for (j=0; j<K; ++j){
            // while loop to avoid self-connect
            rollint = N*ran2(&seedt);// unifrom distrubution between 0 to N-1 integer numbers
            while ( i == rollint ){
                rollint = N*ran2(&seedt);
            }
            connect[i][j] = rollint;
        }
    }
}