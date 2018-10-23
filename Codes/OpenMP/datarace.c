#include <stdio.h>
#include <omp.h>

int main()
{
	int a = 0;
	
	omp_lock_t lck;
	omp_init_lock(&lck);
	
	#pragma omp parallel num_threads(128) shared(a)
	{
		omp_set_lock(&lck);
		printf("I am thread %d and I am about to modify a\n", omp_get_thread_num());
		
		/*
			LD r1, [a]
			ADD, r3, #1, r1   ; r3 = r1 + 1
			ST [a], r3
		 */
		a++;
		
		printf("I am thread %d and I just modified a\n", omp_get_thread_num());
		omp_unset_lock(&lck);
		
	} // Implicit barrier/join
	
	omp_destroy_lock(&lck);
		
	printf("Hello world! After parreg, a is %d\n", a);
	
	return 0;
}