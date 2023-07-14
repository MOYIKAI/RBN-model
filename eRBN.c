// Simple C program that explores the RBN(Random Boolean Network) configuration space network . Originally written by Yi-Kai Mo 06/2023
// Program uses Numerical Recipies (NR) arrays, random numbers.
//
// Parameters:
// 
/* 
Mututation of RBN

*/

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


/* Global vector */
int *node;         // node's Configuration
int *confignet;    // Configuration network

/* Global matrix */
int **connect;   // in-degree for connection list
int **functions; // Boolean functions for all nodes

/* Functions */
void free_all();   // free the memory 
void init();       // initialized the RBN
void evolution();  // evolution base on the states and Boolean functions
void changebool(); // change a node's boolean functions

int main(int argc, char *argv[]){
    TIME = clock();

    int i,j,k,num;
    
    FILE *stf;  // pointer to in-degree file (structure file)
    FILE *ibf;	// input boolean functions file
    FILE *obf;  // output boolean functions file
    FILE *conf;   // pointer to the configuration network  file

    if (argc == 8){
        // require argc be equal to number of command line entries
        N = atol(argv[1]);
        K = atol(argv[2]);
        seed = atol(argv[3]);
        tpseed = seed;
    } 
    else {
        // error input value of argc 
        fprintf(stderr,"\n Initialization error:\n");
        fprintf(stderr,"Usage: eRBN.x N k seed stucturef intialboolf finalboolf configef\n");
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
    stf = fopen(argv[4],"r");
    for (i=0; i<N; ++i){
        for (j=0; j<K; ++j){
            fscanf(stf, "%d", &connect[i][j]);
            //printf("%d ", connect[i][j]);
        }
        //printf("\n");
    }
    fclose(stf);

    // Read file and fill in the Boolean functions
    ibf = fopen(argv[5],"r");
    for (i=0; i<N; ++i){
        for (j=0; j<F; ++j){
            fscanf(ibf, "%d", &functions[i][j]);
            //printf("%d ", functions[i][j]);
        }
        //printf("\n");
    }
    fclose(ibf);
    
    changebool();

    // Print boolean functions info
    obf = fopen(argv[6],"w");
    for (i=0; i<N; ++i){
        for (j=0; j<F; ++j){ fprintf(obf,"%d ",functions[i][j]);} // function output
        fprintf(obf, "\n");
    }
    fclose(obf);

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
    conf = fopen(argv[7],"w");
    for (i=0; i<pow(2,N); ++i){
        fprintf(conf,"%d %d\n", i, confignet[i]);
    }
    fclose(conf);

    free_all();
    
    return 0;
}

void free_all(){
    free_ivector(node, 0, N-1);
    free_ivector(confignet, 0, (int) pow(2,N) - 1);
    free_imatrix(connect, 0, N-1, 0, K-1);
    free_imatrix(functions, 0, N-1, 0, F-1);
    return;
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

void changebool(){
    int j,k;
    int cnode;    // node number need to be change
    long boolnum; // labels of the boolean functions set 
    cnode = N*ran2(&seed);
    boolnum = ran2(&seed)*pow(2,pow(2,K));
    //printf("%ld %d\n", boolnum, cnode);

    for (j=0; j<F; ++j){
        functions[cnode][j] = boolnum % 2;
        //printf("%d ", functions[cnode][j]);
        boolnum = boolnum / 2;
    }
    //printf("\n");

}