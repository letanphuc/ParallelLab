#include <mpi.h>
#include <stdio.h>

#define N 10000
float dataArray[N];
float sumLocal(float A[], long chunk);

int main(int argc, char** argv){
    int rank,size;
    long i,offset,chunk;
    float localSum, globalSum;
    MPI_Status status;
    MPI_Init(&argc,&argv);
    MPI_Comm_rank(MPI_COMM_WORLD,&rank);
    MPI_Comm_size(MPI_COMM_WORLD,&size);
    chunk = N/size;
    float bufArray[chunk];
    /* divide data to other processes */
    if(rank == 0){
      for(i=0; i < N; i++)
          // dataArray[i] = random() % 1000;
          dataArray[i] = 1;
    }
    /* divide data to every process */
    MPI_Scatter(dataArray, chunk, MPI_FLOAT, bufArray, chunk, MPI_FLOAT, 0, MPI_COMM_WORLD);
    
    /* compute localSum */
    localSum = sumLocal(bufArray,chunk);
    
    /* compute globalSum */
    MPI_Reduce(&localSum, &globalSum, 1, MPI_FLOAT, MPI_SUM, 0, MPI_COMM_WORLD);
    
    if(rank == 0) {
      fprintf(stdout,"\n The result is : %f \n",globalSum);
    }
    MPI_Finalize();
    return 0;
}
float sumLocal(float A[], long chunk){
    long i;
    float s=0;
    for(i=0; i < chunk; i++)
      s += A[i];
    return s;
}
