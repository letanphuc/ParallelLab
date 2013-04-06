#include <sys/time.h>
#include <stdio.h>
#include <stdlib.h>

#define N 1000

struct timeval tv; 
double timestamp();

int main(int argc, char ** argv){
	double runTime;
	long *matrix;
	long *vector;
	long *ans;
	unsigned long int i,j;
	
	matrix = malloc(N*N*sizeof(long));
	vector = malloc(N*sizeof(long));
	ans = malloc(N*sizeof(long));
	
	/* init value */
	for (i = 0; i<N; i++){
		vector[i] = 1;
		ans[i] = 0;
		for (j = 0; j<N; j++)
			matrix[i*N + j] = 1;
	}
	
	runTime = timestamp();
	
	/* Caculate */
	for (i=0; i< N; i++){
		for (j=0; j<N; j++)
			ans[i] += (matrix[i*N + j]*vector[j]);
		//printf("%ld ",ans[i]);
	}
	//printf("\n");
	
	runTime = timestamp() - runTime;
	
	//printf ("Run time is %f seconds\n", runTime);
	printf("%f\n",runTime);
	return 0;
}


double timestamp(){
	double t;
	gettimeofday(&tv, NULL);
	t = ((double)tv.tv_sec + (tv.tv_usec/1000000.0));
	return t;
}
