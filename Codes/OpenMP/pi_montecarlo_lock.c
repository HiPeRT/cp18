#include <math.h>
#include <string.h>
#include <stdio.h>

#include <omp.h>
#define USE_OMP
#define N_THREADS 4

#define SECONDS 1000000000
#define _printf(...) printf(__VA_ARGS__)
//#define _printf(...)

#include <time.h>
#include <sys/time.h>
unsigned long long gettime(void)
{
    struct timespec t;
    int r;

    r = clock_gettime(CLOCK_MONOTONIC, &t);
    if (r < 0) {
        printf("Error to get time! (%i)\n", r);
        return -1;
    }

    return (unsigned long long) t.tv_sec * SECONDS + t.tv_nsec;
}

#include <stdlib.h>
float randNumGen(){
    int random_value = 
#ifdef USE_OMP
	rand_r(omp_get_thread_num());
#else
	rand(); //Generate a random number
#endif
    float unit_random = random_value / (float) RAND_MAX; //make it between 0 and 1
    return unit_random;
}

int main()
{
    unsigned long long t_start, t_delta, counter=0;
    float in_count = 0, tot_count = 0;
	unsigned long tot_iterations = 10000L;
	omp_lock_t lock;
	omp_init_lock(&lock);

#ifdef USE_OMP
	tot_iterations /= N_THREADS;
#endif
	_printf("[get_pi] tot_iterations = %llu\n", tot_iterations); 

    t_start = gettime();
    
    srand ( time(NULL) ); // initiate random seed

#ifdef USE_OMP
	#pragma omp parallel reduction(+:in_count, tot_count) num_threads(N_THREADS) private(counter)
#endif
	{
#ifdef USE_OMP
		_printf("[Thread %d/%d]\n", omp_get_thread_num(), omp_get_num_threads());
#else
		_printf("OpenMP is disabled\n");
#endif
		for(counter=0;counter<tot_iterations;counter++){

			float x = randNumGen();
			float y = randNumGen();

			float result = sqrt((x*x) + (y*y));

			omp_set_lock(&lock);
			if(result<1){
				in_count+=1;         //check if the generated value is inside a unit circle
			}
			tot_count+=1;
			omp_unset_lock(&lock);
		}
	}

	omp_destroy_lock(&lock);
    float pi = 4 * in_count/tot_count;
	
    t_delta = gettime() - t_start;
    
    printf("pi :\t \t3.1415926535897932384626433832795028841971693993751058209\n");
    printf("get_pi: \t%.10f \ttime:\t%llu \tus\n", pi, t_delta/1000);

	return 0;
}
