#include <stdio.h>
#include <omp.h>

int main()
{
	int counter = 3;
	
	#pragma omp parallel num_threads(4) reduction(+:counter)
	{
		counter += 5;
	} // Implicit barrier
	
	printf("counter now is %d\n", counter);
	
	return 0;
}