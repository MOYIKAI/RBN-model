// Simple C program that explores the RBN(Random Boolean Network). Originally written by Yi-Kai Mo 12/2022
// Program uses Numerical Recipies (NR) arrays, random numbers.
//
// Parameters:
// N - number of Nodes for the Network 
// k - number of in-degree links for each node
// p - probability of the output
/*
This RBN model is setup by 3 steps:
    1. First randomly choose the state(0 or 1) for each node
    2. Randomly assigned the in-degree links for each node. It allows self-connected.
    3. Given the Probability p, p is the probability that output of Boolean function is 1
    4. After chosing the p, starts generating each node's Boolean functions table
After setting up, let system evolves. Do not do any pertubations on Nodes, Connections and Boolean functions.
All nodes are updated simultaneously */

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
long seed;   // random number seed
long tmax;   // evolution timestep (simulation steps)
float p;     // Probability that outputs of Boolean functions is 1. p=0.5 is unbias
int *node;   // Node's Configuration
int *mnode;  // Memory Configuration
int *tnode;  // Configuration of the starting period (This is for tracking the length of attractor)
int *dnode;  // Frozen, irrelevant, relevant node, undecide (1,2,3,0)
int **list;  // Connection table
int **functions; // Boolean functions for all nodes


void plot(); // plot important info about RBN. Such as the node configuration, in-degree list and Boolean functions.
void init(); // initialized the RBN
void evolution(); // evolution base on the states and Boolean functions
void detect();	  // detect the property of nodes (Frozen, irrelevant, relevant, undecide)
int compare();    // Comparing two node Configurations
int length();     // Measuring the length of attractor
float ran2(long *idum);	  // typecast ran2

int main(int argc, char *argv[]){
    int len, value;      // variables for attractor
    int t;               // variable for intializied the network(time step)

    FILE *outfile;	// pointer to filename

    if (argc == 6){
        // require argc be equal to number of command line entries
        N = atol(argv[1]);
        K = atol(argv[2]);
        p = atof(argv[3]);
        seed = atol(argv[4]);
        tmax = atol(argv[5]);
    } 
    else {
        // error input value of argc 
        fprintf(stderr,"\n Initialization error:\n");
        fprintf(stderr,"Usage: RBN.x N(node numbers) k(in-degree numbers) p(probability) seed tmax \n");
        return 1;
    }

    // Number of possible input states
    F = pow(2,K);
    // State of each node will be 1 or 0. Index means node number; elements means the state
    node = ivector(0,N-1);
    // Memory configuration of previous node (This is for measuring the length of attractor)
    mnode = ivector(0,N-1);
    // Configuration of a starting period (This is for measuring the length of attractor)
    tnode = ivector(0,N-1);
    // information that each node is belongs to Frozen(0), irrelevant(1), relevant(2) and undecide(4) nodes
    dnode = ivector(0,N-1);
    // Boolean functions for each node. Row(node number); Columns(Truth table number); elements(output of truth table for specific node)
    functions = imatrix(0,N-1,0,F-1);
    // Connection list for in-degree. Row(node number); Columns(number of links); elements(connected nodes)
    list = imatrix(0,N-1,0,K-1);
    
    // initialized and info
    init();
    plot();
    detect();
    plot();

    // DO FEW RUNS THAT TRY TO AVOID BASIAN STATE
    for (t=0; t<N; t++){
        evolution();
    }


    len = 0;
    value = 0;
    /*
    // Copy the configuration for First loop that finds attractor length
    memcpy(tnode, node, N*sizeof(*node)); 

    // Check for the first time
    value = length(1, &len);
    if (value == 1){printf("%d \n", len);}
    else{
        // Copy the configuration for Second loop
        memcpy(tnode, node, N*sizeof(*node)); 
        // Check for the second time
        value = length(2, &len);
        if (value == 1){printf("%d \n", len);}
        else{printf("%d \n", len);}
    }*/


    free_ivector(node, 0, N-1);
    free_ivector(mnode,0, N-1);
    free_ivector(tnode,0, N-1);
    free_ivector(dnode,0, N-1);
    free_imatrix(list, 0, N-1, 0, K-1);
    free_imatrix(functions, 0, N-1, 0, F-1);

    return 0;
}
int length(int m, int *add_len){
    int i,n,l;
    int accepted;
    l=0;
    for (i=0; i<tmax*m; ++i){
        l=l+1;
        evolution();
        accepted = compare();
        if (accepted == 0){}
        else{break;}   
    }
    *add_len = l;
    return accepted;
}
int compare() {
  int j;
  for(j = 0; j < N; ++j) {
    if (tnode[j] != node[j]) return 0;
  }
  return 1;
}

void detect(){
  int i,j,k;
  int frozen = 1 ;
  int irr=0;
  int n=N;
  int tmp;
  // detect the Frozen nodes
  for (i=0; i<N; ++i){
    for (j=0; j<F; ++j){
        frozen = frozen + functions[i][j];
	//printf("%d \n",frozen);
    }
    if ( frozen == 1 || frozen == pow(2,K)+1 ) { dnode[i] = 1; n=n-1;}
    frozen = 1;
  }
  tmp = n;
  // detect irrelevant nodes
  for (k=0; k<N; ++k){
    

    for (i=0; i<N; ++i){
      if ( dnode[i] != 1 || dnode[i] != 2 ){
        for(j=0; j<K; ++j){
	  if (dnode[list[i][j]]==1 || dnode[list[i][j]]==2){irr = irr+1;}
	  else {break;}
        }
        if (irr == K && dnode[i] == 0){dnode[i]=2;n = n - 1;break;}
	printf("%d %d %d\n",irr, n, tmp);
        irr = 0;
      }
    }
    if ( tmp == n ){break;}
    else {tmp = n;}
  }
  
    

}

void evolution(){
    int i,j,k,n;
    for (i=0; i<N; ++i){
        n=0;
        k=K-1;
        for (j=0; j<K; ++j){
            n=n+node[list[i][j]]*pow(2,k);
            k = k-1;
        }
        mnode[i] = functions[i][n];

    }
    // Refreshing old configuration into new one
    for (i=0;i<N;++i){ node[i] = mnode[i];}

    return ;
}

void init(){
    int i,j;
    float roll,dice;
    int rollint;
    // Generate property of the node (all of them are undecide at the first hand)
    for (i=0; i<N; ++i){ dnode[i]=0;} 

    // Generate intial state
    for (i=0; i<N; ++i){
        roll = ran2(&seed);
        if (roll < 0.5){node[i]= 1;} // uniform distribution between 0 and 1 
        else {node[i]=0;}
    }
    // Randomly assigned the truth table output
    for (i=0; i<N; ++i){
        dice = ran2(&seed);
        if (dice < 0.5){
            for (j=0; j<F; ++j){
                roll = ran2(&seed);
                if (roll < p) {functions[i][j] = 1;} // bias output
                else {functions[i][j]=0;}
            }
        }
        else{
            for (j=0; j<F; ++j){
                roll = ran2(&seed);
                if (roll < (1-p)) {functions[i][j] = 1;} // bias output
                else {functions[i][j]=0;}
            }
        }
    }
    // Randomly connected the in-degree
    for (i=0; i<N; ++i){
        for (j=0; j<K; ++j){
            rollint = N*ran2(&seed); // unifrom distrubution between 0 to N-1 integer numbers
            list[i][j] = rollint;
        }
    }
    return;
}
// Plot info of the system
void plot(){
    int i,j;
    // Print nodes configuration
    fprintf(stderr,"Nodes property: \n");
    for (i=0; i<N; ++i){ fprintf(stderr,"%d ",dnode[i]);}
    fprintf(stderr,"\n");
    // Print nodes configuration
    fprintf(stderr,"Nodes Configuration: \n");
    for (i=0; i<N; ++i){ fprintf(stderr,"%d ",node[i]);}
    fprintf(stderr,"\n");
    // Print connection list
    fprintf(stderr,"Connection list: \n");
    for (i=0; i<N; ++i){
        for (j=0; j<K; ++j){ fprintf(stderr,"%d ",list[i][j]+1);}
        fprintf(stderr,"\n");
    }
    // Print Output of Boolean functions
    fprintf(stderr,"Boolean functions: \n");
    for (i=0; i<N; ++i){
        for (j=0; j<F; ++j){
            fprintf(stderr,"%d ",functions[i][j]);
        }
        fprintf(stderr,"\n");
    }
    fprintf(stderr,"\n\n");

    return;
}


// below is a NR random number generator. It generated float numbers evenly over range [0,1)
#define IM1 2147483563
#define IM2 2147483399
#define AM (1.0/IM1)
#define IMM1 (IM1-1)
#define IA1 40014
#define IA2 40692
#define IQ1 53668
#define IQ2 52774
#define IR1 12211
#define IR2 3791
#define NTAB 32
#define NDIV (1+IMM1/NTAB)
#define EPS 1.2e-7
#define RNMX (1.0-EPS)

float ran2(long *idum)
{
	int j;
	long k;
	static long idum2=123456789;
	static long iy=0;
	static long iv[NTAB];
	float temp;

	if (*idum <= 0) {
		if (-(*idum) < 1) *idum=1;
		else *idum = -(*idum);
		idum2=(*idum);
		for (j=NTAB+7;j>=0;j--) {
			k=(*idum)/IQ1;
			*idum=IA1*(*idum-k*IQ1)-k*IR1;
			if (*idum < 0) *idum += IM1;
			if (j < NTAB) iv[j] = *idum;
		}
		iy=iv[0];
	}
    k=(*idum)/IQ1;
    *idum=IA1*(*idum-k*IQ1)-k*IR1;
    if (*idum < 0) *idum += IM1;
    k=idum2/IQ2;
    idum2=IA2*(idum2-k*IQ2)-k*IR2;
	if (idum2 < 0) idum2 += IM2;
	j=iy/NDIV;
	iy=iv[j]-idum2;
	iv[j] = *idum;
	if (iy < 1) iy += IMM1;
	if ((temp=AM*iy) > RNMX) return RNMX;
	else return temp;
}
#undef IM1
#undef IM2
#undef AM
#undef IMM1
#undef IA1
#undef IA2
#undef IQ1
#undef IQ2
#undef IR1
#undef IR2
#undef NTAB
#undef NDIV
#undef EPS
#undef RNMX
/* (C) Copr. 1986-92 Numerical Recipes Software *1(.|a. */