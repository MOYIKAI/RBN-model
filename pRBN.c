// Simple C program that explores the RBN properties. Originally written by Yi-Kai Mo 6/2023
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
    3. Given the Probability p, p is the probability that output of Boolean function is 1
    4. After chosing the p, starts generating each node's Boolean functions table
    5. Finally, find the attractor length and print out its properties
After setting up, let system evolves. Do not do any pertubations on Nodes, Connections and Boolean functions.
All nodes are updated simultaneously */

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
long len;    // attractor length 
long tmax;   // evolution timestep (simulation steps)
long seedt,seeds,tpseedt,tpseeds;// random number seed, seedt(Random number for the topology), seeds(Random number for the intial state)
float p;            // Probability that outputs of Boolean functions is 1. Ex: p=0.5 is unbias
int show;    // Flag parameter for showing the info


/* Global vector */
int *node;   // node's Configuration
int *tnode;  // Configuration of the starting period (This is for tracking the length of attractor)
int *frozen; // information of Frozen nodes

/* Global matrix */
int **connect;  // Connection table
int **functions; // Boolean functions for all nodes

/* Functions */
void result();     // plot the result of It, Sxyt, Sxt, len, seed
void free_all();   // free the memory 
void init();       // initialized the RBN
void evolution();  // evolution base on the states and Boolean functions
int compare();     // Comparing two node Configurations

int main(int argc, char *argv[]){
    int flag, onum, inum; // flags for pruneing process
    int value;      // Condition for finding an attractor
    int t,i,j,k;          // variable for intializied the network(time step)
     
    if (argc == 8){
        // require argc be equal to number of command line entries
        N = atol(argv[1]);
        K = atol(argv[2]);
        p = atof(argv[3]);
        seeds = atol(argv[4]);
        seedt = atol(argv[5]);
        tmax = atol(argv[6]);
        show = atol(argv[7]);

        tpseeds = seeds;
        tpseedt = seedt;
    } 
    else {
        // error input value of argc 
        fprintf(stderr,"\n Initialization error:\n");
        fprintf(stderr,"Usage: RBN03.x N(node numbers) k(in-degree numbers) p(probability) seeds(intial state) seedt(topology) tmax show\n");
        return 1;
    }

    // Number of possible input states
    F = pow(2,K);
    // State of each node will be 1 or 0. Index means node number; elements means the state
    node = ivector(0,N-1);
    // information for abslute frozen nodes
    frozen = ivector(0,N-1);
    // Configuration of a starting period (This is for measuring the length of attractor)
    tnode = ivector(0,N-1);
    // Boolean functions for each node. Row(node number); Columns(Truth table number); elements(output of truth table for specific node)
    functions = imatrix(0,N-1,0,F-1);
    // Connection list for in-degree. Row(node number); Columns(number of links); elements(connected nodes)
    connect = imatrix(0,N-1,0,K-1);
    
    // initialized
    init();


    // Generate intial state
    for (i = 0; i<N; ++i){
        node[i] = seeds % 2;
        seeds = seeds/2;
    }

    // Copy the configuration for finding attractor length
    for (i=0;i<N;++i){ tnode[i] = node[i];}
    
    // Print the evolution

    for (t=0; t<tmax; ++t){
        ++len; evolution(); value = compare();
        if (value == 1){printf("Find at first time \n"); break;}
    }
    if (len == tmax){
        len = 0; 
        for (i=0;i<N;++i){ tnode[i] = node[i];}
        for (t=0; t<tmax; ++t){
            ++len; evolution(); value = compare();
            if (value == 1){printf("Find at the second time \n");break;}
        }
    }

    // find frozen node by doing for loop
    if (len != 1){
        for (t=0; t<len; t++){
            for (i=0;i<N;++i){ tnode[i] = node[i];}
            evolution();
            for (i=0;i<N;++i){if (frozen[i]==1 && tnode[i]!=node[i]){frozen[i] = 3;}
            }
        }
    }

    // Pruneing process for relevent and irrelevant node
    
    do {
        flag=0;
        onum=0; // numbers of a nodes output
        inum=0; // numbers of a node's output is frozen or irrelevant
        // go through the property list
        for (i=0; i<N; ++i){
            if (frozen[i] == 3){
                // Check if all outputs are frozen or irrelevant nodes
                for (j=0; j<N; ++j) {
                    for (k=0; k<K; ++k){
                        if (connect[j][k] == i){
                            ++onum;
                            if (frozen[connect[j][k]]==1 || frozen[connect[j][k]]==2){
                                ++inum;
                            }
                        }

                        if (onum != inum || onum+inum == 0){frozen[i] = 3;}
                        else{frozen[i] = 2; flag = 1;}
                    }
                }
            }
        }
    } while ( flag == 1 );

    printf("Node's property\n");
    for (i=0; i<N; ++i){printf("%d ", frozen[i]);}
    printf("\n");
    
    result();

    free_all();

    return 0;
}

void result(){
    int t;
    TIME = clock() - TIME;
    double time_lasp = ((double) TIME)/CLOCKS_PER_SEC; // calculate the elapsed time
    printf("len\tconfig\tstructure\ttime \n");
    printf("%d\t%d\t%d\t%f\n", len, tpseeds, tpseedt, time_lasp);
}

void free_all(){
    free_ivector(node, 0, N-1);
    free_ivector(frozen, 0, N-1);
    free_ivector(tnode,0, N-1);
    free_imatrix(connect, 0, N-1, 0, K-1);
    free_imatrix(functions, 0, N-1, 0, F-1);
    return;
}

int compare() {
  int j;
  for(j = 0; j < N; ++j) {
    if (tnode[j] != node[j]) return 0;
  }
  return 1;
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
        dice = ran2(&seedt);
        if (dice < 0.5){
            for (j=0; j<F; ++j){
                dice = ran2(&seedt);
                if (dice < p) {functions[i][j] = 1;} // bias output
                else {functions[i][j]=0;}
            }
        }
        else{
            for (j=0; j<F; ++j){
                dice = ran2(&seedt);
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


    // Generate intial frozen nodes
    for (i=0; i<N; ++i){frozen[i] = 1;}

    return;
}