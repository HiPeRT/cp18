#include <stdio.h>
#include <omp.h>

int main()
{
	#pragma omp parallel num_threads(16)
	{
		printf("Hello world! I am BEFORE barrier\n");
		
		if(omp_get_thread_num() == 0)
		{
			#pragma omp barrier
		}
		
		printf("Hello world! I am AFTER barrier\n");
	} // Implicit barrier/join
	return 0;
}