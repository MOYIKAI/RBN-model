#include <stdbool.h>
#include <string.h>   // standard string library
#include <stdio.h>	  // standard io library
#include <stdlib.h>	  // standard library with lots of functions
#include <math.h>	  // standard math library
#include "random.h"   // random number generator
#include "my_nrutil.h"// dynamic malloc for vector, matrix, and tensor
#include "Generate.h"   // Generate structure of the netwrok
#include "Evolution.h"
#define NRANSI		  // needed for NR

int main(int argc, char *argv[]){
  /* Global variables*/
  int N; // Node 
  int K; // in-degree number for each node
  int E; // ensembles size
  int F = (int) pow(2,K);
  int length; // cycle length
  float p;
  long seedS, seedI; // random seed for Structure, Boolean function and Initial condition
  int i,j,flag;
  
  /* Global vector for node configuration*/
  int *node;   // Node's Configuration
  int *mnode;  // Memory for Node's Configuration
  int *tnode;  // Configuration of the starting period (This is for tracking length of attractor)
  /* Global matrix for structure*/
  int **connect;    // in-degree for connection list
  int **ftable;     // Boolean functions table for one ensemble
  /* Ensembles of the system*/
  int **IniE;        // ensembles of Initial conition of nodes
  int **BoolfE;     // ensembles of Boolean functions of nodes


  if (argc == 7){
    // require argc be equal to number of command line entries
    N = atol(argv[1]);
    K = atol(argv[2]);
    E = atol(argv[3]);
    seedS = atol(argv[4]);
    seedI = atol(argv[5]);
    p = atof(argv[6]);
  } 
  else {
    // error input value of argc 
    fprintf(stderr,"\n Initialization error:\n");
    fprintf(stderr,"Usage: main.x N K E(ensemble) seedS(Structure) seedI(Boolean) p(Probability for Boolean function)\n");
    return 1;
  }

  // State of each node will be 1 or 0. Index means node number; elements means the state
  node = ivector(0,N-1);
  // Memory configuration of previous node(This is for measuring the length of attractor)
  mnode = ivector(0,N-1);
  // Configuration of a starting period (This is for measuring the length of attractor)
  tnode = ivector(0,N-1);

  // Connection list for in-degree. Row(node number); Columns(number of links); elements(connected nodes)
  connect = imatrix(0,N-1,0,K-1);
  // Boolean functions table for each node. Row(node number); Columns(Truth table number); elements(output of truth table for specific node)
  ftable = imatrix(0,N-1,0,F-1);
  
  
  // Ensembles
  IniE = imatrix(0,E-1,0,N-1);
  BoolfE = imatrix(0,E-1,0,N-1);
  
  // Generating everything
  Gindeg(connect, N, K, seedS);
  GBoolfE(BoolfE, N, K, E, seedI, p);
  GIniE(IniE, N, E, seedI);

  /* Start the evolution*/
  // First copy Boolf[i]E to ftable and IniE[i] to node
  
  /* Testing 
  for (i=0; i < E; i++){

    AIE2node(IniE[i], node, N);
    ABE2ftable(BoolfE[i], ftable, N, K);
    for (j=0; j < N; j++){Nextconfig(connect, ftable, node, mnode, N, K);}

    Anode2tnode(node, tnode, N); // copy the starting config
    length = 0;
    for (j=0; j < 10*N; j++){
      Nextconfig(connect, ftable, node, mnode, N, K);
      length++;
      flag = compare(node, tnode, N);
      if (flag){
        
        printf("Find the cycle %d. Ensemble %d\n", length,i);
        break;
      }
    }

  }*/
  // Print in-degree info
   showconnect(connect, N, K);


  free_ivector(node, 0, N-1);
  free_ivector(mnode, 0, N-1);
  free_ivector(tnode, 0, N-1);
  free_imatrix(connect, 0, N-1, 0, K-1);
  free_imatrix(ftable, 0, N-1, 0, F-1);
  free_imatrix(IniE, 0, E-1, 0, N-1);
  free_imatrix(BoolfE, 0, E-1, 0, N-1);
  return 0;
}
