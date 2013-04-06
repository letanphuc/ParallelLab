/*
 * LE TAN PHUC
 * MSSV: 50902003
 * MT09KTTN
 * 
 * LAB 2 Parallel Programming witH MPI
 * PROBLEM 3.3 FIND TARGET IN ARRAY
 */

#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define N 100000
#define TARGET 1426717520.000000
	 
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
	
	printf("\n Process %d of %d processes starts \n", rank, size);
	
	if (rank == 0){
		while (!file)
			file = fopen("result","w");
		sprintf(string, "FIND TARGET %f IN ARRAY\n",TARGET);
		fwrite(string, strlen(string), 1, file);
		
		
		srand ( time(NULL) );
		for (j = 1; j< size; j++){
			arr = malloc(numberPerProcess*sizeof(double));
			for (i =0; i< numberPerProcess; i++)
				arr[i] = rand();
			
			MPI_Send(arr, numberPerProcess , MPI_DOUBLE, j,11, MPI_COMM_WORLD);
			
			free(arr);
		}
		for (j = 1; j< size; j++){
			MPI_Recv(arr, 1, MPI_DOUBLE, j, MPI_ANY_TAG,MPI_COMM_WORLD,&status);
		}
		
		printf("\nFinish\n");
		
	}
	else {
		arr = malloc(numberPerProcess*sizeof(double));
		MPI_Recv(arr, numberPerProcess, MPI_DOUBLE, 0, MPI_ANY_TAG,MPI_COMM_WORLD,&status);
		for (i=0; i< numberPerProcess; i++){
			//printf("process #%d: %f\n",rank, arr[i]);
			if (arr[i] == TARGET){
				while (!file)
					file = fopen("result","a");
				sprintf(string, "Process #%d found TARGET at %d\n",rank,rank*numberPerProcess + i);
				fwrite(string, strlen(string), 1, file);
				fclose(file);
			}
		}
		MPI_Send(arr, 1 , MPI_DOUBLE, 0,11, MPI_COMM_WORLD);
	}

	
	MPI_Finalize();
	return 0;
}

