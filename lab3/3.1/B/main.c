#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

struct vector{
	float* data;
	int length;
};

struct vector initVector(){
	struct vector v;
	int i;
	printf("\nVector length = ");
	scanf("%d", &v.length);
	
	v.data = malloc(v.length*sizeof(float));
	
	for (i = 0; i < v.length; i++)
		scanf("%f", v.data+i);
	return v;
}

struct vector randomVector(int n){
	struct vector v;
	int i;
	
	v.length = n;
	
	v.data = malloc(v.length*sizeof(float));
	
	for (i = 0; i < v.length; i++)
		v.data[i] = random();
	return v;
}

void printVector(struct vector v){
	int i = 0;
	printf("\n");
	for (i; i < v.length; i++)
		printf("%f  ",*(v.data + i));
	printf("\n");
}





int main( int argc, char* argv[] ){
	

	
	int i;
	int rank, size;
	int isend;
	int* irecv;
	struct vector v1, v2;
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
		printf("\nInput vector v1: \n");
		v1 = randomVector(50000);
		printf("\nInput vector v2: \n");
		v2 = randomVector(50000);
		//printVector(v1);
		//printVector(v2);
		
		if (v1.length != v2.length){
			printf("Exit -1");
			return -1;
		}
		
		
		ans = 0.0;
		
		for (i = 0; i< v1.length; i++){
			MPI_Send(&end, 1, MPI_CHAR, i%(size-1)+1, 11, MPI_COMM_WORLD);
			MPI_Send(v1.data + i, 1, MPI_DOUBLE, i%(size-1)+1, 11, MPI_COMM_WORLD);
			MPI_Send(v2.data + i, 1, MPI_DOUBLE, i%(size-1)+1, 11, MPI_COMM_WORLD);
		}
		
		end = 1;
		
		for (i = 0; i< size; i++){
			MPI_Send(&end, 1, MPI_CHAR, i%(size-1)+1, 11, MPI_COMM_WORLD);
		}
		for (i = 0; i< v1.length; i++){
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
