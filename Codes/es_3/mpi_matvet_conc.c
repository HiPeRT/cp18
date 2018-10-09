#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <math.h>
#include <string.h>
#include <time.h>
#define MIN(X, Y) (((X) < (Y)) ? (X) : (Y))

int main(int argc, char *argv[])
{
	int N = 10;
	int myid, master, numprocs, numrows, cols;
	int i, j, numsent, sender, anstype, row;
	double *A_loc, *c_loc;
	double *A, *b, *c;
	double start, stop;
	MPI_Status status;
	clock_t t; 

	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &numprocs);
	MPI_Comm_rank(MPI_COMM_WORLD, &myid);

	master = 0;
	if (argc > 1)
		N = atoi(argv[1]);

	numrows = N/numprocs;

	//everyone needs b
	b =(double *) malloc(N*sizeof(double));

	if ( myid == master )
	{
		//master initializes and then dispatches, initialize a and b (arbitrary)
		A =(double *) malloc(N*N*sizeof(double));
		c =(double *) malloc(N*sizeof(double));
		for(j=0; j<N; j++){
			b[j] = 1;
			for(i=0; i<N; i++)
				A[j*N + i] = sin(i)+cos(j);		  
		}
	}

	if ( myid == master )
	{
		start = MPI_Wtime();
		t = clock(); 
	}

	//send b to all
	MPI_Bcast(b, N, MPI_DOUBLE_PRECISION, master, MPI_COMM_WORLD);

	A_loc =(double *) malloc(N*numrows*sizeof(double));
	c_loc =(double *) malloc(numrows*sizeof(double));
	memset(c_loc,0,numrows*sizeof(double));

	//divide the matrix
	MPI_Scatter(A, N*numrows, MPI_DOUBLE_PRECISION,
				A_loc, N*numrows, MPI_DOUBLE_PRECISION, master,
				MPI_COMM_WORLD);

	for(i = 0; i<numrows; i++)
		for(j = 0; j<N; j++)
			c_loc[i] += A_loc[i*N+j]*b[j];
	
	MPI_Gather(c_loc, numrows, MPI_DOUBLE_PRECISION,
				c, numrows, MPI_DOUBLE_PRECISION, master,
				MPI_COMM_WORLD);
	
	if (myid==master)
	{		
		stop = MPI_Wtime();
		t = clock() - t;
		double time_taken = ((double)t)/CLOCKS_PER_SEC;
		printf("Elapsed time is %g\n", time_taken);
		for (i=0; i<10; i++)
			printf("%g\n", c[i]);
	}

MPI_Finalize();
return 0;
}
