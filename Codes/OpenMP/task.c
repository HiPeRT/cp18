#include <stdio.h>
#include <omp.h>

int main()
{
	#pragma omp parallel num_threads(2)
	{
		#pragma omp single
		{
			printf("*************** I am thread %d and I am PUSHING t0\n", omp_get_thread_num());
			#pragma omp task
			{
				// t0()
				printf("I am thread %d and I am executing t0\n", omp_get_thread_num());
			}
			
			printf("*************** I am thread %d and I am PUSHING t1\n", omp_get_thread_num());
			#pragma omp task
			{
				// t1()
				printf("I am thread %d and I am executing t1\n", omp_get_thread_num());
			}
		}
	} // barrier
	
	return 0;
}