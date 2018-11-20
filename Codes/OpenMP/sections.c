#include <stdio.h>
#include <omp.h>

int main()
{
	#pragma omp parallel
	{
		// Knowing NUM
		int myid = omp_get_thread_num();
		int num_threads = omp_get_num_threads();
		
		#pragma omp sections
		{
			#pragma omp section
			{   // A
				printf("I am thread %d and I am executing 'A'\n", myid);
			}
			
			#pragma omp section
			{   // B
				printf("I am thread %d and I am executing 'B'\n", myid);
			}
			
			#pragma omp section
			{   // C
				printf("I am thread %d and I am executing 'C'\n", myid);
			}
			
			#pragma omp section
			{   // D
				printf("I am thread %d and I am executing 'D'\n", myid);
			}
			
			#pragma omp section
			{   // E
				printf("I am thread %d and I am executing 'E'\n", myid);
			}
			
		} // End of sections: implicit barrier
	}
}