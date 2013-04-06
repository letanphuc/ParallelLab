#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>


float function(float x){
	return 3*x*x;
}


int main( int argc, char* argv[] ){
	int i;
	int rank, size, n;
	float* data;
	float ans, result;
	float* rev1;
	float a,b,delta,index;
	
	
	MPI_Status status;
	MPI_Init( &argc, &argv );
	MPI_Comm_size( MPI_COMM_WORLD, &size );
	MPI_Comm_rank( MPI_COMM_WORLD, &rank );
	
	
	srand (time(NULL));
	
	/* Do */
	if (rank == 0){
		printf("Input a,b = ");
		scanf("%f",&a);
		scanf("%f",&b);
		printf("\nn= ");
		scanf("%d",&n);
		
		delta = (b-a)/n;
		count = n/size;
		
		data = malloc(n*sizeof(float));
	
	
		for (i = 0; i< n; i++){
			data[i] = a + i*delta + delta/2;
		}
	}
	
	MPI_Bcast(&count,1,MPI_INT,0,MPI_COMM_WORLD);
	MPI_Bcast(&delta,1,MPI_FLOAT,0,MPI_COMM_WORLD);
	
	rev1 = malloc(count*sizeof(float));
	
	MPI_Scatter(data, count, MPI_FLOAT, rev1, count, MPI_FLOAT, 0, MPI_COMM_WORLD);
	
	ans = 0;	
	for (i=0; i< count; i++){
		ans += function(data[i]);
	}
	ans*= delta;
	
	
	MPI_Barrier(MPI_COMM_WORLD);
	
	printf("__*****____Pro #%d : %f\n",rank, ans);
	
	MPI_Reduce(&ans, &result, 1, MPI_FLOAT, MPI_SUM, 0, MPI_COMM_WORLD);
	
	if (!rank)
		printf("\n\n_______Result = %f _________\n", result);

	MPI_Finalize();
	return 0;
}
