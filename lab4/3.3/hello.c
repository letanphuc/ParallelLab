#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>


void *thread_task(void* input){
	printf("End thread number\n");
	pthread_exit(NULL);
}

int main(int argc, char **argv){ 
	int i,rank,size,rc; 
	pthread_t threads[5];
	
	
	MPI_Init(&argc,&argv); 
	MPI_Comm_rank(MPI_COMM_WORLD,&rank); 
	MPI_Comm_size(MPI_COMM_WORLD,&size); 
	printf("Hello world, I have rank %d out of %d processes \n",rank,size); 
	
	
	for (i=0; i< 4; i++){
		rc = pthread_create(&threads[i], NULL, thread_task, NULL);
		pthread_join(threads[i],NULL);
		if (rc){
		   printf("ERROR; return code from pthread_create() is %d\n", rc);
		   return;
		}
	}
	
	MPI_Finalize(); 
	return 0; 
} 
