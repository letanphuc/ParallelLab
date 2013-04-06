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
	
	MPI_Init(&argc,&argv);
	MPI_Comm_rank(MPI_COMM_WORLD,&rank);
	MPI_Comm_size(MPI_COMM_WORLD,&size);
	
	fprintf(stdout,"\n Process %d of %d processes starts \n", rank, size);
	while (count < T_COUNT){
		if (rank != 0) {
			MPI_Recv(&count, 1, MPI_INT, rank - 1, 0,
			 MPI_COMM_WORLD, MPI_STATUS_IGNORE);
			printf("Process %d received count %d from process %d\n",rank, count, rank - 1);
		} else {
			// Set the count's value if you are process 0
			count++;
		}
		MPI_Send(&count, 1, MPI_INT, (rank + 1) % size,0, MPI_COMM_WORLD);
		// Now process 0 can receive from the last process.
		if (rank == 0) {
			MPI_Recv(&count, 1, MPI_INT, size - 1, 0,MPI_COMM_WORLD, MPI_STATUS_IGNORE);
			printf("Process %d received count %d from process %d\n",rank, count, size - 1);
		}
		sleep(1);
	}
	
	MPI_Finalize();
	return 0;
}

