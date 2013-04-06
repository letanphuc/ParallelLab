 /* 
 c program:
 --------------------------------
  1. draws Mandelbrot set for Fc(z)=z*z +c
  using Mandelbrot algorithm ( boolean escape time )
 -------------------------------         
 2. technique of creating ppm file is  based on the code of Claudio Rocchini
 http://en.wikipedia.org/wiki/Image:Color_complex_plot.jpg
 create 24 bit color graphic file ,  portable pixmap file = PPM 
 see http://en.wikipedia.org/wiki/Portable_pixmap
 to see the file use external application ( graphic viewer)
  */
#include <stdio.h>
#include <math.h>
#include <mpi.h>
#include <stdlib.h>
 
 
#define iXmax 			200
#define iYmax 			200

#define N				iXmax* iXmax
#define IterationMax 	100
#define MaxColorComponentValue 255
#define EscapeRadius	1
#define ER2			EscapeRadius*EscapeRadius

#define CxMin			2.5
#define CxMax			1.5
#define CyMin			-2.0
#define CyMax			2.0


int master( int );
int slave(void);
int minFunc(int , int);
 
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
	int data_send[2];
	int rev_ans[2];
	int arr_buf[N];
	int numsent = 0;
	int i,j, sender;
	MPI_Status status;
	
	
	/* screen ( integer) coordinate */
	int iX,iY;

	/* world ( double) coordinate = parameter plane*/
	double Cx,Cy;
		
	/* color component ( R or G or B) is coded from 0 to 255 */
	/* it is 24 bit color RGB file */
	FILE * fp;
	char *filename="new1.ppm";
	char *comment="# ";/* comment should start with # */
	static unsigned char color[3];
	
	

	/*create new file,give it a name and open it in binary mode  */
	fp= fopen(filename,"wb"); /* b -  binary mode */
	/*write ASCII header to the file*/
	fprintf(fp,"P6\n %s\n %d\n %d\n %d\n",comment,iXmax,iYmax,MaxColorComponentValue);
	

	/* distribute data to slave */
	for(i=1; i < minFunc(procs, N); i++){
		MPI_Send(&numsent, 1, MPI_INT, i, i, MPI_COMM_WORLD );
		numsent++;
	}
	
	/* receive result and distribute data */
	for(i=0; i < N; i++){
		MPI_Recv(rev_ans, 2, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
		/* SV xac dinh process gui ket qua ve va gui tiep du lieu cho no ??? */
		arr_buf[rev_ans[0]] = rev_ans[1];
		printf("%d %d\n",rev_ans[0], rev_ans[1]);
		sender = status.MPI_SOURCE;
		
		if(numsent < N) {
			MPI_Send(&numsent, 1, MPI_INT, sender, numsent+1, MPI_COMM_WORLD);
			numsent++;
		}
		else {
			MPI_Send(MPI_BOTTOM, 0, MPI_INT, sender, 0, MPI_COMM_WORLD);
		}
		
	}
	// write to file
	for (i =0; i< iYmax; i++)
	for (j =0; j< iXmax; j++){
		if (arr_buf[i*iYmax+j] == 1)
		{ /*  interior of Mandelbrot set = black */
		   color[0]=0;
		   color[1]=0;
		   color[2]=0;                           
		}
		else 
		{ /* exterior of Mandelbrot set = white */
			color[0]=255;
			color[1]=255;
			color[2]=255;
		};
		fwrite(color,1,3,fp);
	}
	fclose(fp);
}

int slave(void){
	MPI_Status status;
	int rev;
	int send_buff[2];
	long double ans;
	int i;
	int iX, iY;
	int Cx, Cy;
	/*  */
	int Iteration;
	
	double PixelWidth=(CxMax-CxMin)/iXmax;
	double PixelHeight=(CyMax-CyMin)/iYmax;
	/* Z=Zx+Zy*i  ;   Z0 = 0 */
	double Zx, Zy;
	double Zx2, Zy2; /* Zx2=Zx*Zx;  Zy2=Zy*Zy  */
	
	do {
		MPI_Recv(&rev, 1, MPI_INT, 0, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
		if (status.MPI_TAG != 0){
			iX = rev % iXmax;
			iY = rev / iXmax;
			
			Cy = CyMin + iY*PixelHeight;
            if (fabs(Cy)< PixelHeight/2) Cy=0.0;
            
			Cx = CxMin + iX*PixelWidth;
			
			
			/* initial value of orbit = critical point Z= 0 */
			Zx=0.0;
			Zy=0.0;
			Zx2=Zx*Zx;
			Zy2=Zy*Zy;
			/* */
			for (Iteration=0;Iteration<IterationMax && ((Zx2+Zy2)<ER2);Iteration++)
			{
				Zy=2*Zx*Zy + Cy;
				Zx=Zx2-Zy2 + Cx;
				Zx2=Zx*Zx;
				Zy2=Zy*Zy;
			};
			
			/* compute  pixel color (24 bit = 3 bytes) */
			if (Iteration==IterationMax)
			{
			   send_buff[1] = 1;  
			}
			else 
			{
				send_buff[1] = 0;
				//printf("----------------------------------------------------------------");
			};
			
			send_buff[0] = rev;
			
			MPI_Send(&send_buff, 2, MPI_INT, 0, status.MPI_TAG, MPI_COMM_WORLD);
		}
	}
	while (status.MPI_TAG != 0);
	return 0;
}

int minFunc(int a, int b){
	return (a<b)? a:b;
}
