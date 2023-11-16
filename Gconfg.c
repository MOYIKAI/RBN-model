// Simple C program that explores the RBN(Random Boolean Network) configuration space network . Originally written by Yi-Kai Mo 06/2023
// Program uses Numerical Recipies (NR) arrays, random numbers.
//
// Parameters:
// N - number of Nodes for the Network 
// k - number of in-degree links for each node
// 
/* Find the configuration map from Boolean function and Boolean map structure */

#include <string.h>   // standard string library
#include <stdio.h>	  // standard io library
#include <stdlib.h>	  // standard library with lots of functions
#include <math.h>	  // standard math library
#include "my_nrutil.h"// include NR header files
#include "random.h"
#define NRANSI		  // needed for NR

/* Global variable*/
long N; // Node 
long K; // in-degree number for each node
long F; // number of possible input states which is 2^K 

/* Global vector */
int *node;         // node's Configuration
int *confignet;    // Configuration network

/* Global matrix */
int **connect;   // in-degree for connection list
int **functions; // Boolean functions for all nodes

/* Functions */
void evolution();  // evolution base on the states and Boolean functions

int main(int argc, char *argv[]){

    int i,j,k,num;
    
    FILE *stf;  // pointer to in-degree file (structure file)
    FILE *ibf;	// input boolean functions file
    FILE *conf; // pointer to the configuration network file

    if (argc == 6){
        // require argc be equal to number of command line entries
        N = atol(argv[1]);
        K = atol(argv[2]);
    } 
    else {
        // error input value of argc 
        fprintf(stderr,"\n Initialization error:\n");
        fprintf(stderr,"Usage: Gconfg.x N k stucturef intialboolf configef\n");
        return 1;
    }

    // Number of possible input states
    F = pow(2,K);
    // State of each node will be 1 or 0. Index means node number; elements means the state
    node = ivector(0,N-1);
    // information Configuration network
    confignet = ivector(0,(int) pow(2,N) - 1);   
    // Boolean functions for each node. Row(node number); Columns(Truth table number); elements(output of truth table for specific node)
    functions = imatrix(0,N-1,0,F-1);
    // Connection list for in-degree. Row(node number); Columns(number of links); elements(connected nodes)
    connect = imatrix(0,N-1,0,K-1);
    
    // initialized
    // Read file and connected the in-degree
    stf = fopen(argv[3],"r");
    for (i=0; i<N; ++i){
        for (j=0; j<K; ++j){
            fscanf(stf, "%d", &connect[i][j]);
        }
    }
    fclose(stf);

    // Read file and fill in the Boolean functions
    ibf = fopen(argv[4],"r");
    for (i=0; i<N; ++i){
        for (j=0; j<F; ++j){
            fscanf(ibf, "%d", &functions[i][j]);
        }
    }
    fclose(ibf);
    
    // Find the Configuration space network
    for (j=0; j<pow(2,N); ++j){
        num = 0;
        k = j;
        for (i = 0; i<N; ++i){
            node[i] = k % 2;
            k = k/2;
        }
        evolution();
        for (i = 0; i<N; ++i){
            num = num + node[i]*pow(2,i);
        }
        confignet[j] = num;
    }

    // Print Configuration space network
    conf = fopen(argv[5],"w");
    for (i=0; i<pow(2,N); ++i){
        fprintf(conf,"%d\n", confignet[i]);
    }
    fclose(conf);

    free_ivector(node, 0, N-1);
    free_ivector(confignet, 0, (int) pow(2,N) - 1);
    free_imatrix(connect, 0, N-1, 0, K-1);
    free_imatrix(functions, 0, N-1, 0, F-1);
    
    return 0;
}

void evolution(){
    int i,j,k,n;
    int mnode[N];
    for (i=0; i<N; ++i){
        n=0;
        k=K-1;
        for (j=0; j<K; ++j){
            n=n+node[connect[i][j]]*pow(2,k);
            k = k-1;
        }
        mnode[i] = functions[i][n];
    }
    // Refreshing old configuration into new one
    for (i=0;i<N;++i){ node[i] = mnode[i];}

    return ;
}