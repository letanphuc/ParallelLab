#include <stdio.h>
#include <stdlib.h>

#define N 1000000000
#define A 2
#define B 5

double function(double x){
	return x*x;
}

int main( int argc, char* argv[] ){
	unsigned long long int i,j,k;
	long double delta, index, finalResult;
	unsigned long long int chunk;
	
	delta = ((long double)B - (long double)A)/(long double)N;
	finalResult = 0;
	
	for (i=0; i<N; i++){
		index = A + delta*(i+0.5);
		finalResult += function(index);
	}
	finalResult *= delta;
	
	printf("\nFinal Result is %Le\n", finalResult);
	
	
	return 0;
}
	
