// Simple C program that explores the RBN(Random Boolean Network) by Yi-Kai Mo, originally written 12/2022
// Program uses Numerical Recipies (NR) arrays, random numbers.
//
// Parameters:
// N - number of Nodes for the Network 
// k - number of in-degree links for each node
/*
This RBN model is setup by 3 steps:
    1. First randomly choose the state(0 or 1) for each node
    2. Randomly assigned the in-degree links for each node. Where allows self-connected.
    3. Randomly give outputs for Boolean functions
After setting up, let system evolves. Do not do any pertubations on Nodes, List and Boolean functions.
All nodes are updated simultaneously */

#include <string.h>   // standard string library
#include <stdio.h>	  // standard io library
#include <stdlib.h>	  // standard library with lots of functions
#include <math.h>	  // standard math library
#include "my_nrutil.h"    // include NR header files
#define NRANSI		  // needed for NR

/* Global variable*/
long N; // Node 
long K; // in-degree number for each node
long F; // number of Boolean functions
long seed;
long tmax;
int *node;   // Configuration of nodes
int *mnode;  // memory Configuration of nodes
int *tnode;  // Configuration of the starting period
int *truth;  // Boolean function table
int **list;  // Connection table


void plot(); // plot important info about RBN. Such as the node configuration, in-degree list and Boolean functions.
void init(); // initialized the RBN
void evolution(); // evolution base on the states and Boolean functions
int compare();  // Comparing two arrays
int length();
float ran2(long *idum);	  // typecast ran2

int main(int argc, char *argv[]){   // argc and argv used for command line input
    int len, alen, value;      // variables for attractor
    int t;                     // variable for intializied the network

    FILE *outfile;	// pointer to filename

    if (argc == 5){		// require argc be equal to number of command line entries
        N = atol(argv[1]);		// read long variable from command line spot 1
        K = atol(argv[2]);
        seed = atol(argv[3]);
        tmax = atol(argv[4]);
    } 
    else {			// error in value of argc 
        fprintf(stderr,"\n Initialization error:\n");
        fprintf(stderr,"Usage: RBN1_SA.x N(node numbers) k(in-degree numbers) seed tmax \n");  // correct input syntax
        return 1;
    }

    F = pow(2,K);
    // State of each node 1 or 0. Index means node number; elements means the state
    node = ivector(0,N-1);
    // Memory configuration of previous node
    mnode = ivector(0,N-1);
    // Save configue after t=N*N steps
    tnode = ivector(0,N-1);
    // Boolean functions table. Row(numbers of function); elements(output states).
    truth = ivector(0,F);
    // Connection list for in-degree. Row(node number); Columns(number of k); elements(connected nodes)
    list = imatrix(0,N-1,0,K-1);
    
    fprintf(stderr,"Number of nodes %d \n", N);
    fprintf(stderr,"Number of in-degrees %d \n", K);
    fprintf(stderr,"Number of functions %d \n\n", F);

    init();
    fprintf(stderr,"Initialized configuration, list and functions \n");
    plot();

    // DO FEW RUNS THAT TRY TO AVOID BASIAN STATE
    for (t=0; t<N; t++){
        evolution();
        plot();
        getchar();
    }

    
    len = 0; // Setting attractor length
    alen = 0;// Setting attractor length
    value = 0;
    memcpy(tnode, node, N*sizeof(*node)); // Copy the testing configuration

    // Check for the first time
    value = length(&alen);
    if (value == 1){fprintf(stderr,"Find attractor. Length: %d \n", alen);}
    else{len=alen+len;fprintf(stderr, "New loop for finding attractor \n");}

    // Check for the second time
    value = length(&alen);
    if (value == 1){len=alen+len; fprintf(stderr,"Find attractor. Length: %d \n", len);}
    else{fprintf(stderr,"Not sure about the lenght\n");}


    free_ivector(node, 0, N-1);
    free_ivector(mnode,0, N-1);
    free_ivector(tnode,0, N-1);
    free_ivector(truth, 0, F);
    free_imatrix(list, 0, N-1, 0, K-1);

    return 0;
}
int length(int *add_len){
    int i,n,l;
    int accepted;
    l=0;
    for (i=0; i<tmax; ++i){
        l=l+1;
        evolution();
        accepted = compare();
        fprintf(stderr, "Accept = %d \n", accepted);
        if (accepted == 0){
            fprintf(stderr,"Number of steps %d \n", i+1);
            for (n=0; n<N; n++){fprintf(stderr,"tnode[%d] = %d, node[%d] = %d \n", n, tnode[n],n,node[n]);}
            getchar();
        }
        else{
            fprintf(stderr, "Find the attractor length: %d \n", l);
            for (n=0; n<N; n++){fprintf(stderr,"tnode[%d] = %d, node[%d] = %d \n", n, tnode[n],n,node[n]);}
            break;}   
    }
    fprintf(stderr, "%d, %d \n", l, accepted);
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

void evolution(){
    int i,j,k,n;
    for (i=0; i<N; ++i){
        n=0;
        k=K-1;
        //fprintf(stderr, "Input:");
        for (j=0; j<K; ++j){
            n=n+node[list[i][j]]*pow(2,k);
            //fprintf(stderr, "%d, ", node[list[i][j]]);
            k = k-1;
        }
        //fprintf(stderr, "output: Boolean function number %d, value %d\n", n, truth[n]);
        mnode[i] = truth[n];

    }
    // Refreshing old configuration into new one
    for (i=0;i<N;++i){ node[i] = mnode[i];}

    return ;
}

void init(){
    int i,j;
    int c,d;
    float roll;
    int rollint;
    // Generate intial state
    for (i=0; i<N; ++i){
        roll = ran2(&seed);
        if (roll < 0.5){node[i]= 1;} // uniform distribution between 0 and 1 
        else {node[i]=0;}
    }
    // Randomly assigned the truth table output
    for (i=0; i<F; ++i){
        roll = ran2(&seed);
        if (roll < 0.5) {truth[i] = 1;} // unifrom distrubution between 0 to N-1
        else {truth[i]=0;}
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
    long i,j;
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
    for (i=0; i<F; ++i){
        fprintf(stderr,"%d ",truth[i]);
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