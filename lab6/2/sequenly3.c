#include <stdlib.h>
#include <stdio.h>
#include <sys/time.h>

/* change dimension size as needed */
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
        unsigned long int i, j, k;
        double *A, *B, *C, start, end;

        A = (double*)malloc(dimension*dimension*sizeof(double));
        B = (double*)malloc(dimension*dimension*sizeof(double));
        C = (double*)malloc(dimension*dimension*sizeof(double));

        srand(292);

        for(i = 0; i < dimension; i++)
                for(j = 0; j < dimension; j++)
                {   
                        A[dimension*i+j] = 1; //(rand()/(RAND_MAX + 1.0));
                        B[dimension*i+j] = 1; //(rand()/(RAND_MAX + 1.0));
                        C[dimension*i+j] = 0.0;
                }   

        start = timestamp();
        for(i = 0; i < dimension; i++)
                for(j = 0; j < dimension; j++)
                        for(k = 0; k < dimension; k++)
                                C[dimension*i+j] += A[dimension*i+k] *
                                        B[dimension*k+j];

        end = timestamp();
        printf("\nsecs:%f\n", end-start);

        free(A);
        free(B);
        free(C);

        return 0;
}
