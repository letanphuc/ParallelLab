#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

struct vector{
	float *data;
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
		v.data[i] = random()/1000000.0;
	return v;
}

void printVector(struct vector v){
	int i = 0;
	printf("\n");
	for (i; i < v.length; i++)
		printf("%f  ",*(v.data + i));
	printf("\n");
}

#define N 5000


int main( int argc, char* argv[] ){
	int i;
	int rank, size, count;
	struct vector v1, v2;
	float ans, result;
	float* rev1;
	float* rev2;
	
	
	MPI_Status status;
	MPI_Init( &argc, &argv );
	MPI_Comm_size( MPI_COMM_WORLD, &size );
	MPI_Comm_rank( MPI_COMM_WORLD, &rank );
	
	
	srand (time(NULL));
	
	/* Do */
	if (rank == 0){
		v1 = randomVector(N);
		v2 = randomVector(N);
		
		printVector(v1);
		printVector(v2);
		
		if (v1.length != v2.length){
			printf("Exit -1");
			return -1;
		}	
		count = v1.length / size;
	}
	
	MPI_Bcast(&count,1,MPI_INT,0,MPI_COMM_WORLD);
	
	
	rev1 = malloc(count*sizeof(float));
	rev2 = malloc(count*sizeof(float));
	
	MPI_Scatter(v1.data, count, MPI_FLOAT, rev1, count, MPI_FLOAT, 0, MPI_COMM_WORLD);
	MPI_Scatter(v2.data, count, MPI_FLOAT, rev2, count, MPI_FLOAT, 0, MPI_COMM_WORLD);
	
	ans = 0;	
	for (i=0; i< count; i++){
		ans+= rev1[i]*rev2[i];
	}
	
	MPI_Barrier(MPI_COMM_WORLD);
	
	printf("__*****____Pro #%d : %f\n",rank, ans);
	
	MPI_Reduce(&ans, &result, 1, MPI_FLOAT, MPI_SUM, 0, MPI_COMM_WORLD);
	
	if (!rank)
		printf("\n\n_______Result = %f _________\n", result);

	MPI_Finalize();
	return 0;
}
