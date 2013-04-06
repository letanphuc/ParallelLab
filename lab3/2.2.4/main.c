#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

int main( int argc, char* argv[] ){
	
	int i;
	int rank, size;
	int isend;
	int* irecv;
	
	MPI_Init( &argc, &argv );
	MPI_Comm_size( MPI_COMM_WORLD, &size );
	MPI_Comm_rank( MPI_COMM_WORLD, &rank );
	
	isend = rank + 1;
	irecv = malloc(size*sizeof(int));
	
	MPI_Gather( &isend, 1, MPI_INT, irecv, 1,MPI_INT, 0, MPI_COMM_WORLD);
	
	if(rank == 0) {
		for(i=0; i<size; i++)
			printf("irecv = %d\n", irecv[i]);
	}
	
	MPI_Finalize();
}
