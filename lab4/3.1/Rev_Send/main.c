#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

#define N 1000000
#define TARGET 10

int main( int argc, char* argv[] ){
	unsigned long int i,j,k;
	int rank, size;
	float* rev1;
	float* array;
	unsigned long int chunk;
	FILE *f;
	char str[10];
	
	
	MPI_Status status;
	MPI_Init( &argc, &argv );
	MPI_Comm_size( MPI_COMM_WORLD, &size );
	MPI_Comm_rank( MPI_COMM_WORLD, &rank );
	
	chunk = N/size/1000;
	
	for (k=0; k<1000; k++){
		if (rank == 0){
			array = malloc(chunk*sizeof(float));
			for (i=0; i<size; i++){
				for (j=0; j<chunk; j++){
					array[j] = random()%100;
					//printf(" %f ",array[j]);
				}
				//printf("\n");
				MPI_Send(array, chunk, MPI_FLOAT, i, 11, MPI_COMM_WORLD);
			}
			if (k==0){
				f = fopen("output.txt","w");
				if (f){
					fprintf(f,"__RESULT____\n");
					fclose(f);
				}
			}
		}
		
		
		
		rev1 = malloc(chunk*sizeof(float));
		MPI_Recv(rev1, chunk, MPI_FLOAT, 0, 11, MPI_COMM_WORLD, &status);
		
		
		for (j=0; j<chunk; j++){
			MPI_Barrier(MPI_COMM_WORLD);
			if (rev1[j] == TARGET){
				//printf("Process #%d found TARGET at %lu\n", rank, k*1000+rank*chunk + j);
				f = fopen("output.txt","a");
				if (f){
					//fprintf(f,"Process #%d found TARGET at %lu\n", rank, k*1000 + rank*chunk + j);
					fclose(f);
				}
				else printf("__ERRROR FILE___\n");
			}
		}
	}
	MPI_Finalize();
	return 0;
}
	
