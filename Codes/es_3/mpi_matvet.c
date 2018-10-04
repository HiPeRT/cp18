#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#define MIN(X, Y) (((X) < (Y)) ? (X) : (Y))

int main(int argc, char *argv[])
{
  const int MAX_ROWS = 1000, MAX_COLS = 1000;
  int myid, master, numprocs, rows, cols;
  int i, j, numsent, sender, anstype, row;
  double *Aloc, *Bloc, *Cloc, *Btemp;
	double ans;
  MPI_Status status;
  
  double *a =(double *) malloc(MAX_ROWS*MAX_COLS*sizeof(double));
  double *b=(double *) malloc(MAX_COLS*sizeof(double));
  double *c=(double *) malloc(MAX_ROWS*sizeof(double));
  double *buffer=(double *) malloc(MAX_COLS*sizeof(double));

  MPI_Init(&argc, &argv);
  MPI_Comm_size(MPI_COMM_WORLD, &numprocs);
  MPI_Comm_rank(MPI_COMM_WORLD, &myid);

  master = 0;
  rows = 100;
  cols = 100;

	if ( myid == master ){
	  //master initializes and then dispatches, initialize a and b (arbitrary)
		for(j=0; j<cols; j++){
			b[j] = 1;
			for(i=0; i<row; i++)
				a[i*cols + j];		  
		}

		//send b to each slave process
		numsent = 0;
		MPI_Bcast(b, cols, MPI_DOUBLE_PRECISION, master, MPI_COMM_WORLD);

		//send a row to each slave process; tag with row number
		for(i = 0; i < MIN(numprocs-1,rows); i++){
			for (j = 0; j<cols; j++){
         buffer[j] = a[i*cols +j];
			}
      MPI_Send(buffer, cols, MPI_DOUBLE_PRECISION, i+1, i, MPI_COMM_WORLD);
      numsent = numsent+1;
		}

		for(i = 0; i<rows; i++){
      MPI_Recv(&ans, 1, MPI_DOUBLE_PRECISION, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
      sender = status.MPI_SOURCE;    
      anstype = status.MPI_TAG; //row is tag value
      c[anstype] = ans;
      if (numsent < rows){ //send another row
         for(j = 0; j<cols; j++)
            buffer[j] = a[numsent*cols +j];
         MPI_Send(buffer, cols, MPI_DOUBLE_PRECISION, sender, numsent, MPI_COMM_WORLD);
         numsent = numsent+1;
      } 
			else{ //Tell sender that there is no more work
         MPI_Send(MPI_BOTTOM, 0, MPI_DOUBLE_PRECISION, sender, rows, MPI_COMM_WORLD);
			}
   	}

	}	else { //Slaves code
		
		//slaves receive b, then compute dot products until done message received
		MPI_Bcast(b, cols, MPI_DOUBLE_PRECISION, master, MPI_COMM_WORLD);

		if (myid <= rows){ //skip if more processes than work
			while(1){

				MPI_Recv(buffer, cols, MPI_DOUBLE_PRECISION, master, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
				if (status.MPI_TAG == rows){
						MPI_Finalize(); 
						return 0;
				}
      	row = status.MPI_TAG;
      	ans = 0.0;
      	for(i = 0; i<cols; i++)
      			ans = ans+buffer[i]*b[i];
      	MPI_Send(&ans, 1, MPI_DOUBLE_PRECISION, master, row, MPI_COMM_WORLD);
			}
  	}
	}

MPI_Finalize();
return 0;
}
