#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

long double master(int );
int slave(long int);
long int minFunc(long int , long int);
long int power(long int, long int);

#define N 10000

int main(int argc, char ** argv){
    int rank,size;
    long double a,b, pi;
    MPI_Init(&argc,&argv);
    MPI_Comm_rank(MPI_COMM_WORLD,&rank);
    MPI_Comm_size(MPI_COMM_WORLD,&size);
    
    // Compute arctan(1/5);
	if(rank == 0)
      a = master(size);
    else
      slave(5);
      
    // Compute arctan(1/239);
	if(rank == 0)
      b = master(size);
    else
      slave(239);
    
    if (rank==0){
		pi = 16*a - 4*b;
		printf("\nPi = %Le\n",pi);
	}
    
    MPI_Finalize();
    return 0;
}
	


long double master(int procs){
	long i, sender, row, numsent=0;
	
	long double ans = 0;
	long double dotp = 0;
	
	MPI_Status status;
	
	/* distribute data to slave */
	for(i=1; i < minFunc(procs, N); i++){
		MPI_Send(&numsent, 1, MPI_LONG, i, i, MPI_COMM_WORLD );
		numsent++;
	}
	
	/* receive result and distribute data */
	for(i=0; i < N; i++){
		MPI_Recv(&dotp, 1, MPI_LONG_DOUBLE, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
		/* SV xac dinh process gui ket qua ve va gui tiep du lieu cho no ??? */
		sender = status.MPI_SOURCE;
		row     = status.MPI_TAG - 1;
		ans += dotp;
		
		if(numsent < N) {
			//printf("Master send here\n");
			MPI_Send(&numsent, 1, MPI_LONG, sender, numsent+1, MPI_COMM_WORLD);
			numsent++;
		}
		else {
			/* SV gui thong diep thong bao ket thuc cong viec */
			MPI_Send(MPI_BOTTOM, 0, MPI_LONG, sender, 0, MPI_COMM_WORLD);
		}
		
	}
	
	return ans;
}

int slave(long int x){
	MPI_Status status;
	long rev;
	long double ans;
	long int i;
	
	do {
		MPI_Recv(&rev, 1, MPI_LONG, 0, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
		
		ans = 1.0/(2*rev+1)/power(x,rev*2+1);
		if (rev%2 == 1)
			ans = -ans;
		if (status.MPI_TAG != 0){
			MPI_Send(&ans, 1, MPI_LONG_DOUBLE, 0, status.MPI_TAG, MPI_COMM_WORLD);
			//printf("Slave rev = %lu ans = %Le\n",rev,ans);
		}
	}
	while (status.MPI_TAG != 0);
	
	return 0;
}

long int minFunc(long int a, long int b){
	return (a<b)? a:b;
}

long int power(long int x, long int n){
	long int i,ans;
	ans = 1;
	for (i=0; i<n; i++)
		ans*= x;
	return ans;
}
