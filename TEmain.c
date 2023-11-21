#include <string.h>     // standard string library
#include <stdio.h>	    // standard io library
#include <stdlib.h>	    // standard library with lots of functions
#include <math.h>	      // standard math library
#include "TE.h"         // Transfer Entropy
#include "random.h"     // random number generator
#include "my_nrutil.h"  // dynamic malloc for vector, matrix, and tensor
#include "Generate.h"   // Generate structure of the netwrok
#include "save.h"       // save info
#include "show.h"       // show info
#include "Evolution.h"  // Evolution function

int main(int argc, char *argv[]){
  /* Global variables*/
  int N;        // Node numbers 
  int K;        // in-degree number for each node
  int S;        // Population size
  int G;        // Number of Generation
  int length;   // cycle length
  float p, eps, alp;  // Boolean function probability, Mutuation probability , alpha(for fitness function)
  double sumTE; // Populations Transfer Entropy
  long seedS, seedB, seedI; // random seed for Structure, Boolean function and Initial condition
  int i,j,g,flag;

  /* Global vector for node configuration*/
  int *node;   // Node's Configuration
  int *mnode;  // Memory for Node's Configuration
  int *tnode;  // Configuration of the starting period (This is for tracking length of attractor)

  /* Global matrix for structure*/
  int **connect;    // in-degree for connection list
  int **ftable;     // Boolean functions table for one population

  /* Global vector & matrix for Transfer Entropy*/
  double *S_xbt;          // S(Xt-1) Node X time t-1's entropy
  double *S_xnt_xbt;      // S(Xt, Xt-1) Node X at t and X at t-1 Joint entropy
  double *S_ybt_xbt;      // S(Yt-1, Xt-1) Node Y at t-1 and X at t-1 Joint entropy
  double *S_xnt_ybt_xbt;  // S(Xt, Yt-1, Xt-1) Node X at t & Y at t-1 & X at t-1 Joint entropy
  int **P_xbt;          // Node X time t-1's probability
  int **P_xnt_xbt;      // Node X at t and X at t-1 Joint probability
  int **P_ybt_xbt;      // Node X at t and Y at t-1 Joint probability
  int **P_xnt_ybt_xbt;  // Node X at t & Y at t-1 & X at t-1 Joint probability

  /* Populations of the system*/
  int **IniS;        // population of Initial condition of nodes
  int **BoolfS;      // population of Boolean functions of nodes
  int **BoolfSN;     // Next generation's Boolean functions population
  double *TE;        // Transfer entropy of the populations
  double *fitness;   // Fitness function
  int *rep;          // Replicate vector
  int *Att;          // Attractors length
  

  if (argc == 16){
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
    alp = atof(argv[10]);
  } 
  else {
    // error input value of argc 
    fprintf(stderr,"\n Initialization error:\n");
    fprintf(stderr,"Usage: Program.x N K S(num Population) G(Generations)\
    seedS(Structure) seedB(Boolean) seedI(Initial states) p(Probability for Boolean function) eps(Mutation Probability)\
    alp(Fitness variable) ConnectF BoolfSF IniSF TEF AttF\n");
    return 1;
  }
  
  int F = (int) pow(2,K); // number of total boolean functions
  int C = N*N*N;      // Combinations of choosing 3 nodes in N number of nodes where nodes can can repeat
  int H = N*N;            // Combinations of choosing 2 nodes in N number of nodes where two can repeat
  
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
  S_xbt = dvector(0,N-1); // Node X time t-1's entropy
  S_xnt_xbt = dvector(0, H-1);     // Node X at t and X at t-1 Joint entropy
  S_ybt_xbt = dvector(0, H-1);      // Node Y at t-1 and X at t-1 Joint entropy
  S_xnt_ybt_xbt = dvector(0, C-1);  // Node X at t & Y at t-1 & X at t-1 Joint entropy

  P_xbt = imatrix(0,N-1,0,1);          // Node X time t-1's probability
  P_xnt_xbt = imatrix(0, H-1, 0, 3);      // Node X at t and X at t-1 Joint probability
  P_ybt_xbt = imatrix(0, H-1, 0, 3);      // Node Y at t-1 and X at t-1 Joint probability
  P_xnt_ybt_xbt = imatrix(0, N*N*N-1, 0, 7); 
  
  // Populations
  IniS = imatrix(0,S-1,0,N-1);
  Zeros2d_int(IniS, S, N);
  BoolfS = imatrix(0,S-1,0,N-1);
  Zeros2d_int(BoolfS, S, N);
  BoolfSN = imatrix(0,S-1,0,N-1);
  Zeros2d_int(BoolfSN, S, N);
  TE = dvector(0, S-1);
  Zeros1d_dobule(TE, S);
  fitness = dvector(0, S-1);
  Zeros1d_dobule(fitness, S);
  rep = ivector(0,S-1);           // duplicate vector.
  Zeros1d_int(rep, S);
  Att = ivector(0,S-1);
  Zeros1d_int(Att, S);
  
  // Generating everything 
  Gindeg(connect, N, K, seedS);
  saveconnect(argv[11], connect, N, K);
  GBoolfS(BoolfS, N, K, S, p, seedB);
  GIniS2(IniS, N, S, seedI);
  seedI = seedI - 1;

  
  /* Start the evolution*/
  for (g=0; g<G; g++){
    sumTE = 0;
    // Each generation go through every population
    for (i=0; i < S; i++){
      AIP2node(IniS[i], node, N);            // copy inital cofig to node
      ABP2ftable(BoolfS[i], ftable, N, K);   // BoolfS[i] to ftable
      for (j=0; j < 50*N; j++){Nextconfig(connect, ftable, node, mnode, N, K);}

      Anode2tnode(node, tnode, N); // copy the starting config
      length = 0;
      flag = 0;
      while (flag == 0 && length < 10*N){
        Nextconfig(connect, ftable, node, mnode, N, K);
        length++;
        flag = compare(node, tnode, N);
      }

      // If cycle period is fixed point there's no need to find probability
      if (length > 1){
        for (j=0; j<length; j++){
          Anode2tnode(node, tnode, N); // copy the previous config
          Nextconfig(connect, ftable, node, mnode, N, K);
          shownode(node, N);
          shownode(tnode, N);
          TE_P(P_xbt, P_xnt_xbt, P_ybt_xbt, P_xnt_ybt_xbt, tnode, node, N);
        }
        Att[i] = length;
        TE_E(P_xbt, P_xnt_xbt, P_ybt_xbt, P_xnt_ybt_xbt, S_xbt, S_xnt_xbt, S_ybt_xbt, S_xnt_ybt_xbt, N, length);
      }
      else{Att[i] = length;}
      printf("Attractor length %d \n", length);
      //showPx(P_xbt, N);
      //showPxy(P_xnt_xbt, H);
      //showPxy(P_ybt_xbt, H);
      //showPxyz(P_xnt_ybt_xbt, C);
      printf("S_xbt \n");
      showS(S_xbt, N);
      printf("S_xnt_xbt \n");
      showS(S_xnt_xbt, H);
      printf("S_ybt_xbt \n");
      showS(S_ybt_xbt, H);
      printf("S_xnt_ybt_xbt \n");
      showS(S_xnt_ybt_xbt, C);

      // getting MI
      TE[i] = Transfer_Entropy(S_xbt, S_xnt_xbt, S_ybt_xbt, S_xnt_ybt_xbt, N)/C;
      sumTE = sumTE + pow(TE[i], alp);
      
      // Reset All probabilities
      Zeros2d_int(P_xbt, N, 2);
      Zeros2d_int(P_xnt_xbt, H, 4);
      Zeros2d_int(P_ybt_xbt, H, 4);
      Zeros2d_int(P_xnt_ybt_xbt, C, 8);
      Zeros1d_dobule(S_xbt, N);
      Zeros1d_dobule(S_xnt_xbt, H);
      Zeros1d_dobule(S_ybt_xbt, H);
      Zeros1d_dobule(S_xnt_ybt_xbt, C);
    }
    
    // Save all the info
    //saveBoolfS(argv[12], BoolfS, N, K, S, g);
    //showBoolfS(BoolfS, N, K, S);
    //saveIniS(argv[13], IniS, N, S, g);
    //showIniS(IniS, N, S);
    //saveMI(argv[14], TE, S, g);
    showTE(TE, S);
    //saveAttractor(argv[15], Att, S, g);

    // Measuring fitness function
    for (i=0; i<S; i++){fitness[i] = pow(TE[i],alp)/sumTE;}
    //showfitness(fitness, S);

    // Choosing high MI individual base on MI
    Replicate(rep, fitness, S, seedB);
    seedB = seedB - 1;
    
    // Duplicate or Mutation the Boolean function
    GenerateNextS(BoolfS, BoolfSN, rep, eps, N, K, S, seedB);
    seedB = seedB - 1;
    cpNextG_BoolfS(BoolfS, BoolfSN, S, N);

    //reset TE, fitness, initial config and rep population
    Zeros1d_int(rep, S);
    Zeros1d_dobule(TE, S);
    Zeros1d_dobule(fitness, S);
    GIniS2(IniS, N, S, seedI);
    seedI = seedI - 1;
  }
  
  free_ivector(node, 0, N-1);
  free_ivector(mnode, 0, N-1);
  free_ivector(tnode, 0, N-1);

  //////////////////////////////////////////
  free_dvector(S_xbt, 0, N-1); // Node X time t's entropy
  free_dvector(S_xnt_xbt, 0, H-1);     // Node X at t and X at t-1 Joint entropy
  free_dvector(S_ybt_xbt, 0, H-1);      // Node Y at t and Y at t-1 Joint entropy
  free_dvector(S_xnt_ybt_xbt, 0, C-1);  // Node X at t & Y at t-1 & X at t-1 Joint entropy

  free_imatrix(P_xbt, 0, N-1, 0, 1);          // Node X time t's probability
  free_imatrix(P_xnt_xbt, 0, H-1, 0, 3);      // Node X at t and X at t-1 Joint probability
  free_imatrix(P_ybt_xbt, 0, H-1, 0, 3);      // Node Y at t and Y at t-1 Joint probability
  free_imatrix(P_xnt_ybt_xbt, 0, C-1, 0, 7); 
  ///////////////////////////////////////////

  free_imatrix(connect, 0, N-1, 0, K-1);
  free_imatrix(ftable, 0, N-1, 0, F-1);
  free_imatrix(IniS, 0, S-1, 0, N-1);
  free_imatrix(BoolfS, 0, S-1, 0, N-1);
  free_imatrix(BoolfSN, 0, S-1, 0, N-1);
  free_dvector(TE, 0, S-1);
  free_dvector(fitness, 0, S-1);
  free_ivector(rep, 0, S-1);
  free_ivector(Att, 0, S-1);
  return 0;
}