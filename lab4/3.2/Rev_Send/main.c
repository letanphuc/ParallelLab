#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

float function(float x){
	return x*x;
}




int main( int argc, char* argv[] ){
	
	float a,b,delta;
	int n;
	float h,d;
	
	int i;
	int rank, size;
	int isend;
	int* irecv;
	
	float rev,rev1,rev2, ans;
	
	char end = 0;
	double t1, t2;
	
	
	MPI_Status status;
	MPI_Init( &argc, &argv );
	MPI_Comm_size( MPI_COMM_WORLD, &size );
	MPI_Comm_rank( MPI_COMM_WORLD, &rank );
	
	t1 = MPI_Wtime();
	
	srand (time(NULL));
	
	/* Do */
	if (rank == 0){
		a = 2;
		b = 5;
		printf("\nn= ");
		scanf("%d",&n);
		
		delta = (b-a)/n;
		
		
		
		ans = 0.0;
		
		for (i = 0; i< n; i++){
			d = a + i*delta + delta/2;
			h = function(d);
			
			MPI_Send(&end, 1, MPI_CHAR, i%(size-1)+1, 11, MPI_COMM_WORLD);
			MPI_Send(&delta, 1, MPI_DOUBLE, i%(size-1)+1, 11, MPI_COMM_WORLD);
			MPI_Send(&h, 1, MPI_DOUBLE, i%(size-1)+1, 11, MPI_COMM_WORLD);
		}
		
		end = 1;
		
		for (i = 0; i< size; i++){
			MPI_Send(&end, 1, MPI_CHAR, i%(size-1)+1, 11, MPI_COMM_WORLD);
		}
		for (i = 0; i< n; i++){
			MPI_Recv(&rev, 1, MPI_DOUBLE, MPI_ANY_SOURCE, MPI_ANY_TAG,MPI_COMM_WORLD,&status);
			ans += rev;
		}
		printf("\nAns = %f\n",ans);
		t2 = MPI_Wtime();
		printf("Time = %f\n",t2 - t1);
		
	}
	else {
		while (!end){
			MPI_Recv(&end, 1, MPI_CHAR, MPI_ANY_SOURCE, MPI_ANY_TAG,MPI_COMM_WORLD,&status);
			if (end == 0){
				MPI_Recv(&rev1, 1, MPI_DOUBLE, MPI_ANY_SOURCE, MPI_ANY_TAG,MPI_COMM_WORLD,&status);
				MPI_Recv(&rev2, 1, MPI_DOUBLE, MPI_ANY_SOURCE, MPI_ANY_TAG,MPI_COMM_WORLD,&status);
				rev1*=rev2;
				MPI_Send(&rev1, 1, MPI_DOUBLE, 0, 11, MPI_COMM_WORLD);
			}
		}
	}
	
	MPI_Finalize();
	return 0;
}
