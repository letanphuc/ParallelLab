#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

void *thread_task(void* input){
	printf("End thread number");
	pthread_exit(NULL);
}

#define N 10

int main(){
	int i;
	pthread_t threads[N+1];
	int rc;
	
	for (i=0; i< N; i++){
		rc = pthread_create(&threads[i], NULL, thread_task, NULL);
		if (rc){
		   printf("ERROR; return code from pthread_create() is %d\n", rc);
		   return;
		};
	}
	pthread_exit(NULL);
	return 0;
}
	
