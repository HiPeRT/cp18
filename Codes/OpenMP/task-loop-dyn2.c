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
			
			int CHUNK = 2;
			
			for(int i=0; i<16/CHUNK; i++)
			#pragma omp task firstprivate(i)
			{
				for(int j=0; j<CHUNK; j++)
				{
					int idx = i*CHUNK + j;
					printf("I am thread %d and I am executing %d\n", omp_get_thread_num(), idx);
				}
			} // end task
		} // bar
		
	} // implicit barrier
	
	return 0;
}