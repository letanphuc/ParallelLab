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
	unsigned long int chunk, amount;
	FILE *f;
	char str[10];
	
	
	MPI_Status status;
	MPI_Init( &argc, &argv );
	MPI_Comm_size( MPI_COMM_WORLD, &size );
	MPI_Comm_rank( MPI_COMM_WORLD, &rank );
	
	chunk = N/size/10000;
	amount = N/10000;
	
	for (k=0; k<10000; k++){
		if (rank == 0){
			array = malloc(amount*sizeof(float));
			for (i=0; i<size; i++){
				for (j=0; j<amount; j++){
					array[j] = random()%100;
					//printf(" %f ",array[j]);
				}
				//printf("\n");
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
		MPI_Scatter(array, chunk, MPI_FLOAT, rev1, chunk, MPI_FLOAT, 0, MPI_COMM_WORLD);
		
		
		for (j=0; j<chunk; j++){
			MPI_Barrier(MPI_COMM_WORLD);
			if (rev1[j] == TARGET){
				printf("Process #%d found TARGET at %lu\n", rank, k*10000+rank*chunk + j);
				f = fopen("output.txt","a");
				if (f){
					fprintf(f,"Process #%d found TARGET at %lu\n", rank, k*10000 + rank*chunk + j);
					fclose(f);
				}
				else printf("__ERRROR FILE___\n");
			}
		}
	}
	MPI_Finalize();
	return 0;
}
	
