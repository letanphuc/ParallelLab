/*
 * LE TAN PHUC
 * MSSV: 50902003
 * MT09KTTN
 * 
 * LAB 2 Parallel Programming witH MPI
 * PROBLEM 3.1 PING-PONG PROCESS
 */

#include <mpi.h>
#include <stdio.h>

#define T_COUNT 30
	 
int main(int argc, char **argv){
	int rank,size;
	int count = 0;
	MPI_Status status;
	MPI_Init(&argc,&argv);
	MPI_Comm_rank(MPI_COMM_WORLD,&rank);
	MPI_Comm_size(MPI_COMM_WORLD,&size);
	
	fprintf(stdout,"\n Process %d of %d processes starts \n", rank, size);
	
	if(rank == 0) {
		while (count < T_COUNT){
			printf("Ping #%d\n", count);
			MPI_Send(&count, 1, MPI_DOUBLE, 1, 11, MPI_COMM_WORLD);
			MPI_Recv(&count, 1, MPI_DOUBLE, 1,MPI_ANY_TAG,MPI_COMM_WORLD,&status);
			count++;
		}
	}
	else if (rank == 1){
		while (count < T_COUNT - 1){
			MPI_Recv(&count, 1, MPI_DOUBLE, 0,MPI_ANY_TAG,MPI_COMM_WORLD,&status);
			printf("Pong #%d\n",count);
			MPI_Send(&count, 1, MPI_DOUBLE, 0, 11, MPI_COMM_WORLD);
		}
	}
	
	MPI_Finalize();
	return 0;
}
