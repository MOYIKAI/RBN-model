// Simple C program that explores the RBN(Random Boolean Network) cycles. Originally written by Yi-Kai Mo 11/2023
// Program uses Numerical Recipies (NR) arrays, random numbers.
//
// Parameters:
// N - number of Nodes for the Network 
// k - number of in-degree links for each node
// 
/* Find the Attractors by giving network structure, BF & initial state  */

#include <string.h>   // standard string library
#include <stdio.h>	  // standard io library
#include <stdlib.h>	  // standard library with lots of functions
#include <math.h>	  // standard math library
#include "my_nrutil.h"// include NR header files
#define NRANSI		  // needed for NR

/* Global variable*/
long N; // Node 
long K; // in-degree number for each node
long F; // number of possible input states which is 2^K 

/* Global vector */
int *node;         // node's Configuration
int *tnode;        // tmp node's Configuration
int *DecBF;        // Decimal representation of Boolean function

/* Global matrix */
int **connect;   // in-degree for connection list
int **functions; // Boolean functions for all nodes

/* Functions */
void Anode2tnode(); // copy node states to tnode
void Nextconfig();  // evolution base on the states and Boolean functions
int compare();      // comparing node with tnode 

int main(int argc, char *argv[]){

    int i,j,k,num;
    int tmp, b;
    int length, flag, Dec;
    
    FILE *stf;  // pointer to in-degree file (structure file)
    FILE *ibf;	// input boolean functions file
    FILE *Ini;  // pointer to initial condition file

    if (argc == 6){
        // require argc be equal to number of command line entries
        N = atol(argv[1]);
        K = atol(argv[2]);
    } 
    else {
        // error input value of argc 
        fprintf(stderr,"\n Initialization error:\n");
        fprintf(stderr,"Usage: FAL.x N k stucturef intiailboolf initialf\n");
        return 1;
    }

    // Number of possible input states
    F = pow(2,K);
    // State of each node will be 1 or 0. Index means node number; elements means the state
    node = ivector(0,N-1);
    // State of each node will be 1 or 0. Index means node number; elements means the state
    tnode = ivector(0,N-1);
    // Index means node number; elements means the number of BF
    DecBF = ivector(0,N-1);  
    // Boolean functions for each node. Row(node number); Columns(Truth table number); elements(output of truth table for specific node)
    functions = imatrix(0,N-1,0,F-1);
    // Connection list for in-degree. Row(node number); Columns(number of links); elements(connected nodes)
    connect = imatrix(0,N-1,0,K-1);
    
    // initialized
    // Read file and connected the in-degree
    stf = fopen(argv[3],"r");
    for (i=0; i<N; ++i){
        for (j=0; j<K; ++j){fscanf(stf, "%d", &connect[i][j]);}
    }
    fclose(stf);

    // Read file and fill in the Boolean functions
    ibf = fopen(argv[4],"r");
    for (i=0; i<N; ++i){
        fscanf(ibf, "%d", &DecBF[i]);
        tmp = DecBF[i];
        for (j=0; j < (int) F; ++j){b=tmp%2; tmp/=2; functions[i][j] = b;}
    }
    fclose(ibf);

    // Read file and fill in the Initial state
    Ini = fopen(argv[5],"r");
    for (i=0; i<N; ++i){fscanf(Ini, "%d", &node[i]);}
    fclose(ibf);

    // Find the Attractor
    for (j=0; j < 20*N; j++){Nextconfig(node, connect, functions, N, K);}
    Anode2tnode(node, tnode, N); // copy the starting config
    length = 0;
    flag = 0;
    while (flag == 0 && length < 10*N)
    {
        Nextconfig(node, connect, functions, N, K);
        length++;
        flag = compare(node, tnode, N);
    }

    printf("AL = %d \n",length);
    for (j=0; j < length; j++){
        Dec = 0;
        for (i=0; i<N; i++){printf("%d ", node[i]);Dec = Dec + node[i]*pow(2,i);}
        printf("%d\n", Dec);
        Nextconfig(node, connect, functions, N, K);
    }

    free_ivector(node, 0, N-1);
    free_ivector(tnode, 0, N-1);
    free_ivector(DecBF, 0, N-1);
    free_imatrix(connect, 0, N-1, 0, K-1);
    free_imatrix(functions, 0, N-1, 0, F-1);
    
    return 0;
}

int compare(int *node, int *tnode, int N) {
  int i;
  for(i = 0; i < N; ++i) {
    if (tnode[i] != node[i]) return 0;
  }
  return 1;
}

void Anode2tnode(int *node, int *tnode, int N){
  int i;
  for (i=0; i<N; ++i){tnode[i] = node[i];}
}

void Nextconfig(int *state, int** structure, int **BF, int N, int K){
    int i,j,k,n;
    int mnode[N];
    for (i=0; i<N; ++i){
        n=0;
        for (j=0; j<K; ++j){
            n=n+state[structure[i][j]]*pow(2,j);
             printf("%d ",j);
        }
        printf("%d BFout=%d\n",n,BF[i][n]);
        mnode[i] = BF[i][n];
    }
    // Refreshing old configuration into new one
    for (i=0;i<N;++i){ state[i] = mnode[i];}

    return ;
}