#include <mpi.h>
#include <stdio.h>

#define N 5
float dataArray[N];
float sumLocal(long offset, long chunk);

int main(int argc, char** argv){
    int rank,size;
    long i,offset,chunk;
    float localSum, globalSum;
    MPI_Status status;
    MPI_Init(&argc,&argv);
    MPI_Comm_rank(MPI_COMM_WORLD,&rank);
    MPI_Comm_size(MPI_COMM_WORLD,&size);
    chunk = N/size;
	/* divide data to other processes */
    if(rank == 0){
      for(i=0; i < N; i++)
           dataArray[i] = random() % 1000;
      offset = chunk;
      for(i=1; i < size; i++){
           MPI_Send(&offset,1,MPI_LONG,i,111,MPI_COMM_WORLD);
           MPI_Send(&dataArray[offset],chunk,MPI_FLOAT,i, 222,MPI_COMM_WORLD);
           offset += chunk;
      }
      /* process 0 execute its work */
      offset = 0;
      localSum = sumLocal(offset,chunk);
      /* process 0 receive resutls from other processes */
      globalSum = localSum;
      for(i=1; i < size; i++){
           MPI_Recv(&localSum, 1, MPI_FLOAT, i, 333, MPI_COMM_WORLD, &status);
           globalSum += localSum;
      }
      fprintf(stdout,"\n The result is : %f \n",globalSum);
    }
    else {
      MPI_Recv(&offset,1, MPI_LONG, 0, 111, MPI_COMM_WORLD, &status);
      MPI_Recv(&dataArray[offset],chunk,MPI_FLOAT,0,222,MPI_COMM_WORLD,&status);
      localSum = sumLocal(offset,chunk);
      MPI_Send(&localSum, 1, MPI_FLOAT, 0, 333, MPI_COMM_WORLD);
    }
    MPI_Finalize();
    return 0;
}
float sumLocal(long offset, long chunk){
    long i;
    float s=0;
    for(i=offset; i < offset+chunk; i++)
      s += dataArray[i];
    return s;
}
