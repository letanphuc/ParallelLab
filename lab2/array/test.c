/*
 * LE TAN PHUC
 * MSSV: 50902003
 * MT09KTTN
 * 
 * LAB 2 Parallel Programming witH MPI
 * PROBLEM 3.2 RING PROCESS
 */

#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define N 50
#define TARGET 10
	 
int main(int argc, char **argv){
	int rank,size;
	int numberPerProcess;
	int i,j;
	FILE* file = NULL;
	char string[255];
	MPI_Status status;
	double* arr;
	
	MPI_Init(&argc,&argv);
	MPI_Comm_rank(MPI_COMM_WORLD,&rank);
	MPI_Comm_size(MPI_COMM_WORLD,&size);
	
	numberPerProcess = N/size;
	
	arr = malloc(numberPerProcess*sizeof(double));
	for (i =0; i< numberPerProcess; i++){
		arr[i] = rand();
		printf("%f\n",arr[i]);
	}
	
	MPI_Finalize();
	return 0;
}


