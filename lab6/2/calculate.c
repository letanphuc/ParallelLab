#include <sys/time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int main(int argc, char ** argv){
	double *t;
	double speedup_max = 0;
	double speedup;
	double sum, avr;
	double sequency_time = 0;
	FILE* f;
	FILE* fout = fopen("result.txt","w");
	int i;
	char cmd[255];
	int num_progs = 5;
	int num_test = atoi(argv[1]);
	int max_num_progs = atoi(argv[2]);
	
	t = malloc(num_test*sizeof(double));
	
	/* Compile */
	i = system("gcc -o sequen sequenly.c && mpicc -o para parallel.c");
	printf("Compile %d\n", i);
	
	if (i!=0)
		return -1;
	printf("Compile OK\nRunning\n");
	
	// Sequency
	sum = 0.0;
	printf("Sequency\n");
	fprintf(fout,"Sequenly run %d times:\n", num_test);
	for (i=0; i<num_test; i++){
		system("rm temp.txt");
		system("./sequen >> temp.txt");
		
		f = fopen("temp.txt","r");
		fscanf(f,"%lf",&t[i]);
		
		sum += t[i];
		fprintf(fout,"\t%d:\t%f\n",i,t[i]);
	}
	avr = sum/num_test;
	fprintf(fout,"Sequenly run %d times, avr = %f\n",num_test,avr);
	sequency_time = avr;
	fprintf(fout, "################################################\n\n");
	fclose(f);
	
	//Parallel
	for (num_progs = 2; num_progs <= max_num_progs; num_progs++){
		sum = 0.0;
		printf("Parallel with %d process\n",num_progs);
		fprintf(fout,"Parallel run with %d process %d times:\n", num_progs, num_test);
		for (i=0; i<num_test; i++){
			system("rm temp.txt");
			sprintf(cmd,"mpirun -np %d ./para >> temp.txt",num_progs);
			system(cmd);
			
			f = fopen("temp.txt","r");
			fscanf(f,"%lf",&t[i]);
			
			sum += t[i];
			fprintf(fout,"\t%d:\t%f\n",i,t[i]);
		}
		avr = sum/num_test;
		fprintf(fout,"Parallel run (%d process) %d times, avr = %f\n",num_progs,num_test,avr);
		speedup = sequency_time/avr;
		speedup_max = (speedup>speedup_max)? speedup:speedup_max;
		fprintf(fout,"----------> SPEED UP = %f\n",speedup);
		fprintf(fout, "################################################\n\n");
		fclose(f);
	}
	
	printf("Finish, best speedup is %f\n", speedup_max);
	fprintf(fout, "Finish, best speedup is %f\n", speedup_max);
	fclose(fout);
	return 0;
}
