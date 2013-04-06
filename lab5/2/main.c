#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

int master( int );
int slave(void);
int minFunc(int , int);


#define N 10

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
	long matrixA[N][N], vectorC[N];
	long i,j,dotp, sender, row, numsent=0;
	MPI_Status status;

	/* Initialize data */
	for(i=0; i < N; i++)
		for(j=0; j < N; j++)
			matrixA[i][j] = 1;
	
	/* distribute data to slave */
	for(i=1; i < minFunc(procs, N); i++){
		MPI_Send(&matrixA[i-1][0], N, MPI_LONG, i, i, MPI_COMM_WORLD );
		numsent++;
	}
	
	/* receive result and distribute data */
	for(i=0; i < N; i++){
		MPI_Recv(&dotp, 1, MPI_LONG, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
		/* SV xac dinh process gui ket qua ve va gui tiep du lieu cho no ??? */
		sender = status.MPI_SOURCE;
		row     = status.MPI_TAG - 1;
		vectorC[row] = dotp;
		
		if(numsent < N) {
			MPI_Send(&matrixA[numsent][0], N, MPI_LONG, sender, numsent+1, MPI_COMM_WORLD);
			numsent++;
		}
		else {
			/* SV gui thong diep thong bao ket thuc cong viec */
			MPI_Send(MPI_BOTTOM, 0, MPI_LONG, sender, 0, MPI_COMM_WORLD);
		}
		
	}
	
	/* In ket qua de xac dinh tinh dung dan cua chuong trinh */
	for(i = 0; i < 10; i++)
		fprintf(stdout,"%ld ",vectorC[i]);
	printf("\n");
	
	return 0;
}

int slave(void){
	MPI_Status status;
	long rev[N];
	long vector[N];
	
	long ans;
	int i;
	
	for (i=0; i<N; i++)
		vector[N] = 1;
	
	do {
		MPI_Recv(rev, N, MPI_LONG, 0, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
		
		ans = 0;
		for (i=0; i<N; i++)
			ans+= rev[i]*vector[i];
		
		if (status.MPI_TAG != 0)
			MPI_Send(&ans, 1, MPI_LONG, 0, status.MPI_TAG, MPI_COMM_WORLD);
	}
	while (status.MPI_TAG != 0);
	
	return 0;
}

int minFunc(int a, int b){
	return (a<b)? a:b;
}
