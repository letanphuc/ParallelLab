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

#define T_COUNT 10
	 
int main(int argc, char **argv){
	int rank,size;
	int count = 0;
	int i;
	int next,back;
	MPI_Status status;
	MPI_Init(&argc,&argv);
	MPI_Comm_rank(MPI_COMM_WORLD,&rank);
	MPI_Comm_size(MPI_COMM_WORLD,&size);
	
	fprintf(stdout,"\n Process %d of %d processes starts \n", rank, size);
	
	for (i = 0; i < T_COUNT; i++)	
		if(rank == 0) {
			next = 1;
			back = size -1;	
			
			//printf("Process #%d next = %d, back = %d\n",rank, next, back);
			
			printf("\nProcess 0: Start #%d\n\n",count);
			
			MPI_Send(&count, 1, MPI_INT, next, 11, MPI_COMM_WORLD);
			MPI_Recv(&count, 1, MPI_INT, back, MPI_ANY_TAG,MPI_COMM_WORLD,&status);
			
			printf("\nProcess 0: End, result is %d\n",count);
			
			count++;
		}
		else {
			next = (rank + 1) % size;
			back = rank - 1;
			//printf("Process #%d next = %d, back = %d\n",rank, next, back);
			
			MPI_Recv(&count, 1, MPI_INT, back, MPI_ANY_TAG,MPI_COMM_WORLD,&status);
			
			printf("-----Process %d: Receive #%d\n", rank, count);

			MPI_Send(&count, 1, MPI_INT, next, 11, MPI_COMM_WORLD);
		}
	
	MPI_Finalize();
	return 0;
}
