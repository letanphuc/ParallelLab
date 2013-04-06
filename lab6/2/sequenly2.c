#include <stdlib.h>
#include <stdio.h>
#include <sys/time.h>

const int dimension = 10000;
struct timeval tv; 

double timestamp()
{
	double t;
	gettimeofday(&tv, NULL);
	t = tv.tv_sec + (tv.tv_usec/1000000.0);
	return t;
}

int main(int argc, char *argv[])
{
	long int i, j, k;
	double start, end;
	long *Matrix, *Vextor, *Ans;

	Matrix = (long*)malloc(dimension*dimension*sizeof(long));
	Vextor = (long*)malloc(dimension*sizeof(double));
	Ans = (long*)malloc(dimension*sizeof(double));

	srand(292);

	for(i = 0; i < dimension; i++){
		Vextor[i] = 1;
		Ans[i] = 0;
		for(j = 0; j < dimension; j++)
		{   
			Matrix[i] = 1;
		}
	}   

	start = timestamp();
	
	for(i = 0; i < dimension; i++)
		for(j = 0; j < dimension; j++)
			*(Ans+i) += *(Matrix + dimension*i+j) * *(Vextor + j);

	end = timestamp();
	printf("\nsecs:%f\n", end-start);

	free(Matrix);
	free(Vextor);
	free(Ans);

	return 0;
}
