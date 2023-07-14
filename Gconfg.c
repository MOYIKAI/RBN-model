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
long seedt, tpseedt, seedb, tpseedb;// seedt(Random number for the topology), seedb(Random number for assigned boolean function)
float p;            // Probability that outputs of Boolean functions is 1. Ex: p=0.5 is unbias
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

int main(int argc, char *argv[]){
    TIME = clock();

    int i,j,k,num;
    
    FILE *indegf;   // pointer to in-degree file
    FILE *boolff;	// pointer to boolean functions file
    FILE *confgf;   // pointer to the configuration network  file
    if (argc == 9){
        // require argc be equal to number of command line entries
        N = atol(argv[1]);
        K = atol(argv[2]);
        p = atof(argv[3]);
        seedt = atol(argv[4]);
        seedb = atol(argv[5]);

        tpseedt = seedt;
        tpseedb = seedb;
    } 
    else {
        // error input value of argc 
        fprintf(stderr,"\n Initialization error:\n");
        fprintf(stderr,"Usage: cRBN.x N k p seedt seedb in-degreefile booleanfile confignetfile\n");
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
    init();

    // Print in-degree info
    indegf = fopen(argv[6],"w");
    for (i=0; i<N; ++i){
        for (j=0; j<K; ++j){ fprintf(indegf,"%d ",connect[i][j]);}
        fprintf(indegf, "\n");
    }
    fclose(indegf);

    // Print boolean functions info
    boolff = fopen(argv[7],"w");
    for (i=0; i<N; ++i){
        for (j=0; j<F; ++j){ fprintf(boolff,"%d ",functions[i][j]);} // function output
        fprintf(boolff, "\n");
    }
    fclose(boolff);

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
    confgf = fopen(argv[8],"w");
    for (i=0; i<pow(2,N); ++i){
        fprintf(confgf,"%d %d\n", i, confignet[i]);
    }
    fclose(confgf);

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