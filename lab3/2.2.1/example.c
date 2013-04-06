#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv){
	int i,rank,size;
	
	
	/* Initiate */
	MPI_Init(&argc,&argv);
	MPI_Comm_rank(MPI_COMM_WORLD,&rank);
	MPI_Comm_size(MPI_COMM_WORLD,&size);
	
	for (i=0; i< size; i++){
		MPI_Barrier(MPI_COMM_WORLD);
		if (rank == i)
			printf("Hello world, I have rank %d out of %d processes \n",rank,size);
	}
	
	MPI_Finalize();
	return 0;
}
