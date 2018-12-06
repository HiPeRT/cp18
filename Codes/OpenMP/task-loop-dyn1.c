#include <stdio.h>
#include <omp.h>

int main()
{
	#pragma omp parallel num_threads(4)
	{
		//#pragma omp for schedule(dynamic, 1)
		#pragma omp single
		{
			printf("*************** I am thread %d and I am PUSHING all tasks\n", omp_get_thread_num());
			
			for(int i=0; i<16; i++)
			#pragma omp task firstprivate(i)
			{
				printf("I am thread %d and I am executing %d\n", omp_get_thread_num(), i);
			} // end task
		} // bar
		
	} // implicit barrier
	
	return 0;
}