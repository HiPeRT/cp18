#include <stdio.h>
#include <omp.h>

int main()
{
	int var = 0;
	// init lock
	omp_lock_t lock;
	
	omp_init_lock(&lock);

	#pragma omp parallel num_threads(1024) shared(lock)
	{
		// set lock
		omp_set_lock(&lock);

		// increment var
		var++;
		
		// unset lock
		//omp_unset_lock(&lock); 

	} // barrier
	
	printf("var is %d (should be 1024)\n", var);
	
	// destroy lock 
	omp_destroy_lock(&lock);

	
	return 0;
}