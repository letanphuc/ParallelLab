#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>


#define N 10000000
#define N_PER_THREAD 10


struct vector{
	float *data;
	int length;
};

struct data_pthread{
	float* a;
	float* b;
	float *ans;
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
		v.data[i] = 1;//random()%10000/100.0;
	return v;
}

void printVector(struct vector v){
	int i = 0;
	printf("\nVector: ");
	for (i; i < v.length; i++)
		printf("%f  ",*(v.data + i));
	printf("\n");
}

void printData(struct data_pthread d){
	//printf("\n%f %f %f\n",*d.a, *d.b, *d.ans);
}


void* calculate(void* arg){
	struct data_pthread d = *(struct data_pthread*)arg;
	int i = 0;
	for (i =0; i< N_PER_THREAD; i++){
		*(d.ans) += (d.a[i]*d.b[i]);
	}
	return 0;
	pthread_exit(NULL);
}





int main( int argc, char* argv[] ){
	int i,rc;
	int rank, size, count;
	struct vector v1, v2;
	float ans, result;
	float* rev1;
	float* rev2;
	pthread_t* thr;
	struct data_pthread d_thread;
	int n_thread;
	double t1;
	
	
	MPI_Status status;
	MPI_Init( &argc, &argv );
	MPI_Comm_size( MPI_COMM_WORLD, &size );
	MPI_Comm_rank( MPI_COMM_WORLD, &rank );
	
	t1 = MPI_Wtime();
	
	srand (time(NULL));
	
	/* Do */
	if (rank == 0){
		//v1 = initVector();
		//v1 = initVector();
		
		v1 = randomVector(N);
		v2 = randomVector(N);
		
		//printVector(v1);
		//printVector(v2);
		
		
		
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
	n_thread = count/N_PER_THREAD;
	thr = malloc(n_thread*sizeof(pthread_t));	
	
	for (i=0; i< n_thread; i++){
		
		d_thread.a = rev1+i*N_PER_THREAD;
		d_thread.b = rev2+i*N_PER_THREAD;
		d_thread.ans = &ans;
		
		
		rc = pthread_create(&thr[i], NULL, calculate,(void*)(&d_thread));
		pthread_join(thr[i],NULL);
		if (rc){
			printf("\nERROR can not create process\n");
			return 0;
		}
	}
	
	MPI_Barrier(MPI_COMM_WORLD);
	
	printf("__ (^_^) __ Pro #%d : %f\n",rank, ans);
	
	MPI_Reduce(&ans, &result, 1, MPI_FLOAT, MPI_SUM, 0, MPI_COMM_WORLD);
	
	MPI_Barrier(MPI_COMM_WORLD);
	
	if (rank == 0){
		t1 = MPI_Wtime() -t1;
		printf("\n\n_______Result = %f _________\nTime = %f\n", result, t1);
	}
	MPI_Finalize();
	return 0;
}
