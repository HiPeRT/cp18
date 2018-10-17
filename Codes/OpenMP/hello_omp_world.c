#include <stdio.h>
#include <omp.h>

int main()
{
	printf("Hello sequential OpenMP world! I am thread nr %d out of %d\n\n",
			omp_get_thread_num(), omp_get_num_threads());
			
	#pragma omp parallel num_threads(8)
	{
		int myid = omp_get_thread_num();
		printf("Hello parallel OpenMP world! I am thread nr %d out of %d\n",
				myid, omp_get_num_threads());
	} // (implicit) barrier
	
	printf("\nHello sequential OpenMP world! I am thread nr %d out of %d\n\n",
			omp_get_thread_num(), omp_get_num_threads());
	
	return 0;
}