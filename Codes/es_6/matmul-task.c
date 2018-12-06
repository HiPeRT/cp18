#include <stdio.h>
#include <omp.h>

 
int main()
{
	int rows_A, cols_A, rows_B, cols_B, row, col, k, sum = 0;
	int A[10][10], B[10][10], Result[10][10];

	rows_A = 8;
	cols_A = 4;
	rows_B = 4;
	cols_B = 4;
	
	if (cols_A != rows_B)
		printf("Matrices with entered orders can't be multiplied with each other.\n");

	for (row = 0; row < rows_A; row++)
		for (col = 0; col < cols_A; col++)
		  A[row][col] = row * 100 + col;
 
	printf("A matrix\n");
	for (row = 0; row < rows_A; row++) {
	  for (col = 0; col < cols_A; col++)
		printf("%d\t", A[row][col]);

	  printf("\n");
	}

	// Identity matrix
	for (row = 0; row < rows_B; row++)
	  for (col = 0; col < cols_B; col++)
		  B[row][col] = row == col ? 2 : 0;

	printf("B matrix\n");
	for (row = 0; row < rows_B; row++) {
	  for (col = 0; col < cols_B; col++)
		printf("%d\t", B[row][col]);

	  printf("\n");
	}
	unsigned int CHUNK =2;
	
	#pragma omp parallel shared (A, B, Result) firstprivate(rows_A, rows_B, cols_A, cols_B, CHUNK) private(row, col, k, sum) num_threads(16)
	{
		int _row;
		for (_row = 0; _row < rows_A/CHUNK; _row++)
		{
			#pragma omp single
			#pragma omp task firstprivate(_row, rows_A, cols_B, rows_B, CHUNK) private(row, sum, col, k) shared(A, B, Result)
			{
				int i;
#ifdef _OPENMP
					printf("[Thread %ld/%ld] CHUNK is %d\n", omp_get_thread_num(), omp_get_num_threads(), CHUNK);
#else
					printf(" CHUNK is %d\n", CHUNK);
#endif
				for(i=0; i<CHUNK; i++)
				{
					row = _row*CHUNK + i;
#ifdef _OPENMP
					printf("[Thread %ld/%ld] Working on row %d out of %d\n", omp_get_thread_num(), omp_get_num_threads(), row, rows_A);
#else
					printf("Working on row %d out of %d\n", row, rows_A);
#endif
					for (col = 0; col < cols_B; col++)
					{
					  
						sum = 0;
						for (k = 0; k < rows_B; k++)
						{
							
							
						  sum = sum + A[row][k] * B[k][col];
						  
						  
						}

#ifdef _OPENMP
						printf("[Thread %ld/%ld] Sum is %d\n", omp_get_thread_num(), omp_get_num_threads(), sum);
#else
						printf("Sum is %d\n", sum);
#endif
						// Actually, not needed here, as output sets are disjoint
						#pragma omp critical
						Result[row][col] = sum;
						sum = 0;


					}
				}
			}
		  
		  
		}
	}// parreg

	printf("Product of entered matrices:-\n");

	for (row = 0; row < rows_A; row++) {
	  for (col = 0; col < cols_B; col++)
		printf("%d\t", Result[row][col]);

	  printf("\n");
	}
 
  return 0;
}