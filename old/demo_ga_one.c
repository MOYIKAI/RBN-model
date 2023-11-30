#include <string.h>     // standard string library
#include <stdio.h>	    // standard io library
#include <stdlib.h>	    // standard library with lots of functions
#include <math.h>	      // standard math library
#include "random.h"     // random number generator
#include "my_nrutil.h"  // dynamic malloc for vector, matrix, and tensor
#include "Generate.h"   // Generate structure of the netwrok
#include "show.h"       // show info
#include "Evolution.h"  // Evolution function
#define NRANSI		      // needed for NR

int main(int argc, char *argv[]){
  /* Global variables*/
  int N;        // Node numbers 
  int K;        // in-degree number for each node
  int S;        // populations size
  int G;        // Number of Generation
  int length;   // cycle length
  float p, eps;  // Boolean function probability, Mutuation probability
  double sumMI; // Populations Mutual information
  long seedS, seedB, seedI; // random seed for Structure, Boolean function and Initial condition
  int i,j,g,flag;

  /* Global vector for node configuration*/
  int *inode;  // Initial Configuration
  int *node;   // Node's Configuration
  int *mnode;  // Memory for Node's Configuration
  int *tnode;  // Configuration of the starting period (This is for tracking length of attractor)

  /* Global matrix for structure*/
  int **connect;    // in-degree for connection list
  int **ftable;     // Boolean functions table for one population

  /* Global vector & matrix for Mutual information*/
  double *Sx;   // Single variable entropy
  double *Sxy;  // Joint entropy 2 variables
  int **Px;     // Joint Srobability for 2 variables
  int **Pxy;    // Probability of state

  /* Populations of the system*/
  int **BoolfS;      // populations of Boolean functions of nodes
  int **BoolfSN;     // Next generation's Boolean functions populations
  double *MI;        // Mutual information of the populations
  double *fitness;   // Fitness function
  int *rep;          // Replicate vector
  

  if (argc == 10){
    // require argc be equal to number of command line entries
    N = atol(argv[1]);
    K = atol(argv[2]);
    S = atol(argv[3]);
    G = atol(argv[4]);
    seedS = atol(argv[5]);
    seedB = atol(argv[6]);
    seedI = atol(argv[7]);
    p = atof(argv[8]);
    eps = atof(argv[9]);
  } 
  else {
    // error input value of argc 
    fprintf(stderr,"\n Initialization error:\n");
    fprintf(stderr,"Usage: Program.x N K P(population) G(Num of Generation) seedS(Structure) seedB(Boolean)\
    seedI(Initial states) p(Probability for Boolean function) eps(Mutation Probability) ga(Fitness variable)\n");
    return 1;
  }
  
  int F = (int) pow(2,K); // number of total boolean functions
  int C = N*(N-1)/2;      // Combinations of choosing 2 nodes in N number of nodes
  
  // Initial node config.
  inode = ivector(0,N-1);
  Zeros1d_int(inode, N);
  // State of each node will be 1 or 0. Index means node number; elements means the state
  node = ivector(0,N-1);
  Zeros1d_int(node, N);
  // Memory configuration of previous node(This is for measuring the length of attractor)
  mnode = ivector(0,N-1);
  Zeros1d_int(mnode, N);
  // Configuration of a starting period (This is for measuring the length of attractor)
  tnode = ivector(0,N-1);
  Zeros1d_int(tnode, N);
  // Connection list for in-degree. Row(node number); Columns(number of links); elements(connected nodes)
  connect = imatrix(0,N-1,0,K-1);
  Zeros2d_int(connect, N, K);
  // Boolean functions table for each node. Row(node number); Columns(Truth table number); elements(output of truth table for specific node)
  ftable = imatrix(0,N-1,0,F-1);
  Zeros2d_int(ftable, N, F);
  
  // Single variable entropy
  Sx = dvector(0,N-1);
  Zeros1d_dobule(Sx, N);
  // Joint entropy 2 variables
  Sxy = dvector(0,C-1);
  Zeros1d_dobule(Sxy, C);
  // Probability Matrix. Row(Nodes); Columns(states 1 0), elements(Probility)
  Px  = imatrix(0,N-1,0,1);
  Zeros2d_int(Px, N, 2);
  // Joint Srobability Matrix. Row(); Columns(states 00 01 10 11), elements(Joint Srobability)
  Pxy = imatrix(0,C-1,0,3);
  Zeros2d_int(Pxy, C, 4);
  
  // Populations
  BoolfS = imatrix(0,S-1,0,N-1);
  Zeros2d_int(BoolfS, S, N);
  BoolfSN = imatrix(0,S-1,0,N-1);
  Zeros2d_int(BoolfSN, S, N);
  MI = dvector(0, S-1);
  Zeros1d_dobule(MI, S);
  fitness = dvector(0, S-1);
  Zeros1d_dobule(fitness, S);
  rep = ivector(0,S-1);           // duplicate vector.
  Zeros1d_int(rep, S);
  
  // Generating everything 
  Gindeg(connect, N, K, seedS);
  GBoolfS(BoolfS, N, K, S, p, seedB);
  GIniS1(inode, N, seedI);
  seedI = seedI - 1;

  
  /* Start the evolution*/
  for (g=0; g<G; g++){
    printf("\nGeneration %d\n", g);
    shownode(inode, N);
    showBoolfS(BoolfS, N, K, S);
    sumMI = 0;

    // Pach Generation
    for (i=0; i < S; i++){
      AIP2node(inode, node, N);            // copy inital cofig to node
      ABP2ftable(BoolfS[i], ftable, N, K); // BoolfS[i] to ftable
      for (j=0; j < 10*N; j++){Nextconfig(connect, ftable, node, mnode, N, K);}

      Anode2tnode(node, tnode, N); // copy the starting config
      length = 0;
      flag = 0;
      while (flag == 0 && length < 4*N){
        Nextconfig(connect, ftable, node, mnode, N, K);
        length++;
        flag = compare(node, tnode, N);
      }

      // If cycle period is fixed point there's no need to find probability
      if (length > 1){
        for (j=0; j<length; j++){
          SumProbability(Pxy, Px, node, N);
          Nextconfig(connect, ftable, node, mnode, N, K);
        }
        printf("cycle %d P %d\n", length,i);
        Entropy(Pxy, Px, Sxy, Sx, N, length);
      }
      else{printf("cycle %d. P %d\n", length,i);
      }

      // getting MI
      MI[i] = 2*Mutual(Sxy, Sx, N)/((N-1)*N);
      sumMI = sumMI + MI[i];
      
      // Reset Px, Py, Sy, Sxy
      Zeros2d_int(Px, N, 2);
      Zeros2d_int(Pxy, C, 4);
      Zeros1d_dobule(Sx, N);
      Zeros1d_dobule(Sxy, C);
    }
    // Measuring fitness function
    for (i=0; i<S; i++){fitness[i] = MI[i]/sumMI;}
    showMI(MI, S);
    showfitness(fitness, S);

    // Choosing high MI individual base on MI
    Replicate(rep, fitness, S, seedB);
    seedB = seedB - 1;
    showrep(rep, S);
    
    // Duplicate or Mutation
    GenerateNextP(BoolfS, BoolfSN, rep, eps, N, K, S, seedB);
    seedB = seedB - 1;
    cpNextG_BoolfS(BoolfS, BoolfSN, S, N);

    //reset MI, fitness, initial config and rep population
    Zeros1d_int(rep, S);
    Zeros1d_dobule(MI, S);
    Zeros1d_dobule(fitness, S);
    GIniS1(inode, N, seedI);
    seedI = seedI - 1;
    getchar();
  }
  
  free_ivector(node, 0, N-1);
  free_ivector(inode, 0, N-1);
  free_ivector(mnode, 0, N-1);
  free_ivector(tnode, 0, N-1);
  free_dvector(Sx, 0, N-1);
  free_dvector(Sxy,0, C-1);
  free_imatrix(Pxy, 0, C-1, 0, 3);
  free_imatrix(Px, 0, N-1, 0, 1);
  free_imatrix(connect, 0, N-1, 0, K-1);
  free_imatrix(ftable, 0, N-1, 0, F-1);
  free_imatrix(BoolfS, 0, S-1, 0, N-1);
  free_imatrix(BoolfSN, 0, S-1, 0, N-1);
  free_dvector(MI, 0, S-1);
  free_dvector(fitness, 0, S-1);
  free_ivector(rep, 0, S-1);
  return 0;
}