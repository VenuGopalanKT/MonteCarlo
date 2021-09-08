#include <stdlib.h>
#include <stdio.h>
#include <omp.h>
#include <time.h>
#include <sys/time.h>

double getRandomNumber(unsigned int *seed) {
    return (double) rand_r(seed) * 2 / (double) (RAND_MAX) - 1;
} //Random Number generation

long double Caluclate_Seq_Pi(long long itr) {
    long long circle_points = 0,square_points=0;
    unsigned int seed = (unsigned int) time(NULL);
    
    for (long long int i = 0; i < itr; i++) {
        double x = getRandomNumber(&seed);
        double y = getRandomNumber(&seed);
        double d = x*x + y*y; 
        
        if (d <= 1) {
        	circle_points = circle_points + 1;
        }
        square_points = square_points + 1;
    }
//Count of Points
printf("\nNmber of Circle Points = %lld\nNumber of Square Points = %lld\n", circle_points,square_points);
//
    return 4 * (double)circle_points / (double)square_points;
}

long double Caluclate_Par_Pi (long long itr) {
    long long circle_points = 0, square_points = 0;
	#pragma omp parallel num_threads(4)
	{
    // rand() is not thread-safe, 
    // So we generate a seed per thread and use rand_r() to 
		//retrieve a thread-safe random value. 
    	unsigned int seed = (unsigned int) time(NULL) + (unsigned int) omp_get_thread_num();
    	#pragma omp for reduction(+: circle_points)
        for (long long int i = 0; i < itr; i++) {
            double x = getRandomNumber(&seed);
            double y = getRandomNumber(&seed);
            double d = x*x + y*y;
            	
            if (d <= 1) {
            	circle_points = circle_points + 1;
            }
            square_points = square_points + 1;
        }
    }
    //Count of Points
printf("\nNmber of Circle Points = %lld\nNumber of Square Points = %lld\n", circle_points,square_points);
    return 4 * (double) circle_points/((double) square_points);
}

int main() {
    struct timeval start, end;

    long long iterations = 100000000;

    printf("\nTiming Sequential: \n");
    gettimeofday(&start, NULL);
    long double SeqPi = Caluclate_Seq_Pi(iterations);
    gettimeofday(&end, NULL);
    printf("Took %f seconds\n", end.tv_sec - start.tv_sec + (double) (end.tv_usec - start.tv_usec) / 1000000);

    printf("\nTiming Parallel: \n");
    gettimeofday(&start, NULL);
    long double ParPi = Caluclate_Par_Pi(iterations);
    gettimeofday(&end, NULL);
    printf("Took %f seconds\n\n", end.tv_sec - start.tv_sec + (double) (end.tv_usec - start.tv_usec) / 1000000);

    printf("Sequential Pi Value(Estimated) : %.10Lf \n", SeqPi);
    printf("Parallel Pi Valus(Estimated)  : %.10Lf \n", ParPi);

    return 0;
}