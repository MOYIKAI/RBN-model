// Simple C program that explores the RBN(Random Boolean Network) configuration space network . Originally written by Yi-Kai Mo 08/2023
// Program uses Numerical Recipies (NR) arrays, random numbers.
//
// Parameters:
// N - number of Nodes for the Network 
// k - number of in-degree links for each node
// 
/* From configuration map to find all simple cycles(include self loops) */

#include <string.h>   // standard string library
#include <stdio.h>	  // standard io library
#include <stdlib.h>	  // standard library with lots of functions
#include <math.h>	  // standard math library
#include "my_nrutil.h"// include NR header files
#define NRANSI		  // needed for NR

/* Global variable*/
long N; // Node 
long K; // in-degree number for each node
long edges;

/* Global vector */
int *boolnode;	   // Boolean flag for nodes

/* Global matrix */
int **configspace;    // Configuration space (functional graph)

/* Functions */
int succ(int x);

int main(int argc, char *argv[]){

    int j, a, b, first, length;
    
    FILE *conf; // pointer to the configuration network file

    if (argc == 4){
        // require argc be equal to number of command line entries
        N = atol(argv[1]);
        edges = atol(argv[2]);
    } 
    else {
        // error input value of argc 
        fprintf(stderr,"\n Initialization error:\n");
        fprintf(stderr,"Usage: Fcycle.x N edges configef\n");
        return 1;
    }

    // Boolean flag for nodes
    boolnode = ivector(0, N-1);
    // information Configuration space
    configspace = imatrix(0, edges, 0, 1);   
    
    // initialized
    // Set all node none test
    for (j=0; j<N; ++j){boolnode[j] = 0;}

    // Read configspace file
    conf = fopen(argv[3],"r");
    for (j=0; j< (int) pow(2,N); ++j){fscanf(conf, "%d %d", &configspace[j][0], &configspace[j][1]);}
    fclose(conf);

    for (j=0; j< edges; ++j){
        a = succ(j);
	    b = succ(succ(j));
	    while (a != b){
            a = succ(a);
	        b = succ(succ(b));
        }

        a = j;
        boolnode[a] = 1;
        while (a != b){
            a = succ(a);
            b = succ(b);
        }
        first = a;
        
        b = succ(a);
        length = 1;
        printf("%d", a);
        while (a != b){
            printf(" -> ");
            printf("%d", b);
            b = succ(b);
            length++;
        }
        printf(" %d\n", length);
    }

    free_ivector(boolnode, 0, N-1);
    free_imatrix(configspace, 0, (int) pow(2,N) - 1, 0, 1);
    
    return 0;
}

int succ(int sor){
    int des;
    des = configspace[sor][1];
    return des;
}
