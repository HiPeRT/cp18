#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include <time.h>

#define pi 3.1415926535897932384626433

/*funzione per la il prodotto di due matrici*/

void prodotto(double *A, double *B, double *C, int n)
{
int i, j, k;
  for (i=0; i<n; i++)
    for (j=0; j<n; j++)
      for (k=0; k<n; k++)    
              C[i*n+k]+=A[i*n+j]*B[j*n+k];
}

int main(int argc, char *argv[])
{
int i, j, k, col, row, N; 
double m;
clock_t start, diff;

N = atoi(argv[1]);

  // Allochiamo A, U e L
double *A = (double *)malloc(N*N*sizeof(double));
double *U = (double *)malloc(N*N*sizeof(double));
double *L = (double *)malloc(N*N*sizeof(double));

// Azzero A, U e copio A in U in modo da non perderla
memset(A, 0, N*N*sizeof(double));
memset(U, 0, N*N*sizeof(double));
for (i = 0; i < N; i++){
  for (j = 0; j < N; j++){
    A[i*N + j] = cos((j-1.)*(((2.*i-1.)*pi)/(2.*N)));
	U[i*N + j] = A[i*N + j];
  }
  A[i*N+i] += N;
  U[i*N + i] = A[i*N + i];
}    

//********************Algoritmo LU***************************
  
memset(L, 0, N*N*sizeof(double));
  
for (i = 0; i < N; i++)
  L[i*N+i] = 1.0;
  
//TIMING
start = clock();

for (col=0; col<N-1; col++)
{
  for (row = col+1; row < N; row++)
  {
    m = U[row*N + col] / U[col*N + col];
    L[row*N + col] = m;
    for (k = col; k < N; k++)
      U[row*N + k] -= m*U[col*N + k];
  }
} 

//TIMING
diff = clock() - start;
   
printf("\n");
  
double *T = (double *)malloc(N*N*sizeof(double));
prodotto(L,U,T,N);
  
double app,sum,sum2;
sum = 0.0;
sum2 = 0.0;
for (i=0; i<N; i++)
{
  for (j=0; j<N; j++)
  {
    app = T[i*N+j]-A[i*N+j];
    sum += app*app;
    sum2 += A[i*N+j]*A[i*N+j];
  }
}
  
printf("Error = %g\n",sqrt(sum)/sqrt(sum2));
   
int msec = diff * 1000 / CLOCKS_PER_SEC;
printf("Time taken %d seconds %d milliseconds\n", msec/1000, msec%1000);
  
free(T);  
free(L);  
free(A);  
free(U);  
return 0;
}     
    

