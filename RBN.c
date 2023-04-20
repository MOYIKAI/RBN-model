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
    2. Randomly assigned the in-degree links for each node. It allows self-connected and double connected.
    3. Given the Probability p, p is the probability that output of Boolean function is 1
    4. After chosing the p, starts generating each node's Boolean functions table
    5. Finally, find the attractor length also measure Total Mutual information
After setting up, let system evolves. Do not do any pertubations on Nodes, Connections and Boolean functions.
All nodes are updated simultaneously */

#include <string.h>   // standard string library
#include <stdio.h>	  // standard io library
#include <stdlib.h>	  // standard library with lots of functions
#include <time.h>     // standard library for measuring the time
#include <math.h>	  // standard math library
#include "my_nrutil.h"// include NR header files
#define NRANSI		  // needed for NR

/* Global variable*/
long N; // Node 
long K; // in-degree number for each node
long F; // number of possible input states which is 2^K
long C; // Combination (Combination of choosing 2 nodes in N number of nodes)
long C3;// Combination (Combination of choosing 3 nodes in N number of nodes)
long len;    // attractor length 
long tmax;   // evolution timestep (simulation steps)
long seedt,seeds,tpseedt,tpseeds;// random number seed, seedt(Random number for the topology), seeds(Random number for the intial state)
float p;            // Probability that outputs of Boolean functions is 1. Ex: p=0.5 is unbias
int show;    // Flag parameter for showing the info
clock_t TIME; // time for program


/* Global vector*/
int *node;   // Node's Configuration
int *mnode;  // Memory for Node's Configuration
int *tnode;  // Configuration of the starting period (This is for tracking the length of attractor)
double *Sx;  // Single variable entropy
double *Sxy; // Joint entropy 2 variables
double *Sxyz;// Joint entropy 3 variables
double *It;  // Array for Total entropy St, Total Joint entorpy; Total Mutual information It

/* Global matrix*/
int **list;  // Connection table
int **Pabc;  // Joint Probability for 3 variables
int **Pab;   // Joint Probability for 2 variables
int **Pa;    // Probability of state
int **functions; // Boolean functions for all nodes

/* Functions */
void result();     // plot the result of It, Sxyt, Sxt, len, seed
void free_all();   // free the memory 
void plot();       // plot important info about RBN. Such as the node configuration, in-degree list and Boolean functions.
void init();       // initialized the RBN
void evolution();  // evolution base on the states and Boolean functions
void sum();        // Measuring the output state in order to get Mutual information
int compare();     // Comparing two node Configurations
int length();      // Measuring the length of attractor
double *I();       // Sum up all pairwise mutual information and get Total Mutual information of the network
float ran2(long *idum);	  // typecast ran2
float ran1(long *idum);   // typecast ran1

int main(int argc, char *argv[]){
    TIME = clock();
    int value;      // Condition for finding an attractor
    int t,i;          // variable for intializied the network(time step)

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
        fprintf(stderr,"Usage: RBN.x N(node numbers) k(in-degree numbers) p(probability) seeds(intial state) seedt(topology) tmax show(1 or 0)\n");
        return 1;
    }

    // Number of possible input states
    F = pow(2,K);
    // Combination( Combination of choosing 2 nodes in N number of nodes)
    C = N*(N-1)/2;
    // Combination( Combination of choosing 3 nodes in N number of nodes)
    C3  = N*(N-1)*(N-2)/6;
    // State of each node will be 1 or 0. Index means node number; elements means the state
    node = ivector(0,N-1);
    // Memory configuration of previous node (This is for measuring the length of attractor)
    mnode = ivector(0,N-1);
    // Configuration of a starting period (This is for measuring the length of attractor)
    tnode = ivector(0,N-1);
    // Boolean functions for each node. Row(node number); Columns(Truth table number); elements(output of truth table for specific node)
    functions = imatrix(0,N-1,0,F-1);
    // Connection list for in-degree. Row(node number); Columns(number of links); elements(connected nodes)
    list = imatrix(0,N-1,0,K-1);
    // Single variable entropy
    Sx = dvector(0,N-1);
    // Joint entropy 2 variables
    Sxy = dvector(0,C-1);
    // Joint entropy 3 variables
    Sxyz = dvector(0,C3-1);
    // Probability Matrix. Row(Nodes); Columns(states 1 0), elements(Probility)
    Pa  = imatrix(0,N-1,0,1);
    // Joint Probability Matrix. Row(); Columns(states 00 01 10 11), elements(Joint Probability)
    Pab = imatrix(0,C-1,0,3);
    // Joint Probability Matrix Pabc. Row(); Columns(states 000 001 010 011 100 101 110 111), elements(Joint Probability)
    Pabc = imatrix(0,C3-1,0,7);
    
    
    
    // initialized
    init();

    // DO FEW RUNS THAT TRY TO AVOID BASIAN STATE
    for (t=0; t<10*N; t++){evolution();}

    // Copy the configuration for finding attractor length
    for (i=0;i<N;++i){ tnode[i] = node[i];}

    value = 0;
    // Check for the first time
    value = length(1);
    if (value == 1){result();if(show==1){plot();} free_all();}
    else{
        // Copy the configuration
        for (i=0;i<N;++i){ tnode[i] = node[i];}
        // Check for the second time
        value = length(10);
        if (value == 1){
            result();
            if (show==1){plot();}
            free_all();
        }
        else{
            result();
            if (show==1){plot();}
            free_all();
        }
    }

    return 0;
}

void result(){
    int t;
    double Mxyz, Mxy, Sxyz, Sxy, Sx;

    // Measure the probability and Mutual information
    for (t=0; t<len; t++){
        sum();
        evolution();
    }
    It = I(); // T[Sxyz, Sxy, Sx] Mxyz, Mxy 
    Mxyz = (It[2]*(N-1)*(N-2)/2 - It[1]*(N-2) + It[0])/C3;
    Mxy =  (It[2]*(N-1) - It[1])/C;
    TIME = clock() - TIME;
    double time_lasp = ((double) TIME)/CLOCKS_PER_SEC; // calculate the elapsed time
    printf("%f\t%f\t%f\t%f\t%f\t%d\t%d\t%d\t%f\n", Mxyz, Mxy, It[0]/C3, It[1]/C, It[2]/N, len, tpseeds, tpseedt, time_lasp);
}

void free_all(){
    free_dvector(Sx, 0, N-1);
    free_dvector(Sxy,0, C-1);
    free_dvector(Sxyz,0,C3-1);
    free_ivector(node, 0, N-1);
    free_ivector(mnode,0, N-1);
    free_ivector(tnode,0, N-1);
    free_imatrix(list, 0, N-1, 0, K-1);
    free_imatrix(functions, 0, N-1, 0, F-1);
    free_imatrix(Pabc, 0, C3-1, 0, 7);
    free_imatrix(Pab, 0, C-1, 0, 3);
    free_imatrix(Pa, 0, N-1, 0, 1);
    return;
}

double *I(){
    static double T[2]; // T[Sxyz, Sxy, Sx]
    double Pxyz, Pxy, Px;
    int i,j,k;
    int c,c3;
    int x,xy,xyz;
    c=0; // Combination number 
    c3=0;// Combination number
    // Assigned Joint entropy and entropy
    for (i=0; i<N; i++){
        for (x=0; x<2; x++){
            Px  = (double) Pa[i][x]/len;
            if (Px!=0) {Sx[i] = Sx[i] - Px*log2(Px);}
        }
        
        for (j=i+1; j<N; j++){
            for (xy=0; xy<4; xy++){
                Pxy = (double) Pab[c][xy]/len;
                if (Pxy!=0) {Sxy[c] = Sxy[c] - Pxy*log2(Pxy);}
            }
            c++;

            for (k=j+1; k < N; k++){
                for (xyz=0; xyz<8; xyz++){
                    Pxyz = (double) Pabc[c3][xyz]/len;
                    if (Pxyz!=0) {Sxyz[c3] = Sxyz[c3] - Pxyz*log2(Pxyz);}
                }
                c3++;
            }
        }


        
    }
    // Sum all Joint entropy and entropy T[Sxyz, Sxy, Sx]
    c = 0;
    c3= 0;
    for (i=0; i<N; i++){
        T[2] = Sx[i] + T[2]; // Entropy
        for (j=i+1; j<N; j++){
            T[1] = Sxy[c] + T[1]; // Joint entropy with 2 variables
            c++;
            for (k=j+1; k<N; k++){
                T[0] = Sxyz[c3] + T[0];
                c3++;
            }
        }
    }
    
    return T;
}

void sum(){
    int i,j,k;
    int w, w3;
    int c, c3;
    c=0;
    c3=0;
    for (i=0; i<N; i++){
        Pa[i][node[i]]++;
        for (j=i+1; j<N; j++){
            w=2*node[i]+node[j];
            Pab[c][w]++;
            c++;
            for (k=j+1; k<N; k++){
                w3=4*node[i]+2*node[j]+node[k];
                Pabc[c3][w3]++;
                c3++;
            }
        }
    }

    return;
}

int length(int m){
    int i,l;
    int accepted;
    l=0;
    for (i=0; i<tmax*m; ++i){
        l=l+1;
        evolution();
        accepted = compare();
        if (accepted == 0){}
        else{break;}   
    }
    len = l;
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
            rollint = N*ran2(&seedt); // unifrom distrubution between 0 to N-1 integer numbers
            //printf("ran2 %d\n",rollint1);
            list[i][j] = rollint;
        }
    }

    // Generate intial state
    for (i=0; i<N; ++i){
        roll = ran1(&seeds);
        //printf("ran1 %f\n",roll);
        if (roll < 0.5){node[i]= 1;} // uniform distribution between 0 and 1 
        else {node[i]=0;}
    }
    
    // Intialized Pabc, Pab and Pa
    for (i=0; i < C3; ++i){
        for (j=0; j < 8; ++j){Pabc[i][j] = 0;}
    }
    for (i=0; i < C; ++i){
        for (j=0; j < 4; ++j){Pab[i][j] = 0;}
    }
    for (i=0; i < N; ++i){
        for (j=0; j < 2; ++j){Pa[i][j] = 0;}
    }
    return;
}

// Plot info of the system
void plot(){
    int i,j;
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

    // Print the Pa
    fprintf(stderr,"Number of occuring state in Pa: \n");
    for (i=0; i<N; ++i){
        for (j=0; j<2; ++j){
            fprintf(stderr,"%d ",Pa[i][j]);
        }
        fprintf(stderr,"\n");
    }

    // Print the Pab
    fprintf(stderr,"Number of occuring state in Pab: \n");
    for (i=0; i<C; ++i){
        for (j=0; j<4; ++j){
            fprintf(stderr,"%d ",Pab[i][j]);
        }
        fprintf(stderr,"\n");
    }

    // Print the Pabc
    fprintf(stderr,"Number of occuring state in Pabc: \n");
    for (i=0; i<C3; ++i){
        for (j=0; j<8; ++j){
            fprintf(stderr,"%d ",Pabc[i][j]);
        }
        fprintf(stderr,"\n");
    }

    // Print the Sx
    fprintf(stderr,"Sx: \n");
    for (i=0; i<N; ++i){ fprintf(stderr,"Sx%d: %f ",i, Sx[i]);}
    fprintf(stderr,"\n");

    // Print the Sxy
    fprintf(stderr,"Sxy: \n");
    for (i=0; i<C; ++i){ fprintf(stderr,"Sxy%d: %f ", i, Sxy[i]);}
    fprintf(stderr,"\n");

    // Print the Sxyz
    fprintf(stderr,"Sxyz: \n");
    for (i=0; i<C3; ++i){ fprintf(stderr,"Sxyz%d: %f ", i, Sxyz[i]);}
    fprintf(stderr,"\n");

    fprintf(stderr,"\n\n");

    return;
}

// below is a NR random number generator. It generated float numbers evenly over range [0,1)
#define IM1 2147483563
#define IM2 2147483399
#define AM1 (1.0/IM1)
#define IMM1 (IM1-1)
#define IA1 40014
#define IA2 40692
#define IQ1 53668
#define IQ2 52774
#define IR1 12211
#define IR2 3791
#define NTAB 32
#define NDIV1 (1+IMM1/NTAB)
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
	j=iy/NDIV1;
	iy=iv[j]-idum2;
	iv[j] = *idum;
	if (iy < 1) iy += IMM1;
	if ((temp=AM1*iy) > RNMX) return RNMX;
	else return temp;
}

// “Minimal” random number generator of Park and Miller with Bays-Durham shuffle and added safeguards. It generated float numbers evenly over range [0,1)
#define IA 16807
#define IM 2147483647
#define AM (1.0/IM)
#define IQ 127773
#define IR 2836
#define NDIV (1+(IM-1)/NTAB)

float ran1(long *idum){
    int j;
    long k;
    static long iy=0;
    static long iv[NTAB];
    float temp;
    if (*idum <= 0 || !iy) {
        if (-(*idum) < 1) *idum=1; else *idum = -(*idum);
        for (j=NTAB+7;j>=0;j--) {
            k=(*idum)/IQ;
            *idum=IA*(*idum-k*IQ)-IR*k;
            if (*idum < 0) *idum += IM;
            if (j < NTAB) iv[j] = *idum;
        }
        iy=iv[0];
    }
    k=(*idum)/IQ;
    *idum=IA*(*idum-k*IQ)-IR*k;
    if (*idum < 0) *idum += IM;
    j=iy/NDIV;
    iy=iv[j];
    iv[j] = *idum;
    if ((temp=AM*iy) > RNMX) return RNMX;
    else return temp;
}


#undef IM1
#undef IM2
#undef AM1
#undef IMM1
#undef IA1
#undef IA2
#undef IQ1
#undef IQ2
#undef IR1
#undef IR2
#undef NTAB
#undef NDIV1
#undef EPS
#undef RNMX

#undef IA 
#undef IM 
#undef AM
#undef IQ 
#undef IR
#undef NDIV
/* (C) Copr. 1986-92 Numerical Recipes Software *1(.|a. */