#include <stdio.h>
#include <omp.h>

int main()
{
	/* Sequential code: 1 thrd */
	int a = 11; // Why 11?
	
	#pragma omp parallel num_threads(4) firstprivate(a) // private(a) // shared(a)
	{
		if(omp_get_thread_num() == 0) // Only thread 0
			a = 5;
		
		/* Parallel code: 4 thrds */
		printf("Hello world! a is %d\n", a);
		
	} // Implicit barrier
	
	/* Sequential code: 1 thrd (again)*/
	printf("After the parreg, a is %d\n", a);
	
	return 0;
}