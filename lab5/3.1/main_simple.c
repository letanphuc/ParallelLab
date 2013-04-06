#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

int master( int );
int slave(void);
int minFunc(int , int);


#define N 10000

int main(int argc, char ** argv){
    int rank,size;
    MPI_Init(&argc,&argv);
    MPI_Comm_rank(MPI_COMM_WORLD,&rank);
    MPI_Comm_size(MPI_COMM_WORLD,&size);
    if(rank == 0)
      master(size);
    else
      slave();
    MPI_Finalize();
    return 0;
}

int master(int procs){
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
			MPI_Send(&numsent, 1, MPI_LONG, sender, numsent+1, MPI_COMM_WORLD);
			numsent++;
		}
		else {
			/* SV gui thong diep thong bao ket thuc cong viec */
			MPI_Send(MPI_BOTTOM, 0, MPI_LONG, sender, 0, MPI_COMM_WORLD);
		}
		
	}
	
	/* In ket qua de xac dinh tinh dung dan cua chuong trinh */
	printf("\n%Le\n",ans*4);
	
	return 0;
}

int slave(void){
	MPI_Status status;
	long rev;
	long double ans;
	int i;
	
	do {
		MPI_Recv(&rev, 1, MPI_LONG, 0, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
		
		ans = 1.0/(2*rev+1);
		if (rev%2 == 1)
			ans = -ans;
		
		if (status.MPI_TAG != 0)
			MPI_Send(&ans, 1, MPI_LONG_DOUBLE, 0, status.MPI_TAG, MPI_COMM_WORLD);
	}
	while (status.MPI_TAG != 0);
	
	return 0;
}

int minFunc(int a, int b){
	return (a<b)? a:b;
}
