// Simple C program that generates The structre of RBN. Originally written by Yi-Kai Mo 06/2023
// Program uses Numerical Recipies (NR) arrays, random numbers.
//
// Parameters:
// N - number of Nodes for the Network 
// k - number of in-degree links for each node
/* Randomly assigned the in-degree links for each node. It does not allow self-connected. */

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
long seedt, tpseedt; // seedt(Random number for the topology)
clock_t TIME; // time for program


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

    // Connection list for in-degree. Row(node number); Columns(number of links); elements(connected nodes)
    connect = imatrix(0,N-1,0,K-1);
    
    
    // initialized
    init();

    // Print in-degree info
    indegf = fopen(argv[4],"w");
    for (i=0; i<N; ++i){
        for (j=0; j<K; ++j){ fprintf(indegf,"%d ",connect[i][j]);}
        fprintf(indegf, "\n");
    }
    fclose(indegf);


    free_all();

    return 0;
}

void free_all(){
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