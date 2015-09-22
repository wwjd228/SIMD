#include <immintrin.h>
#include <stdint.h>
#include <stdio.h>
#include <time.h>

double compute_pi( size_t dt )
{
    double pi = 0.0;
    double delta = 1.0 / dt;
    register __m256d ymm0, ymm1, ymm2, ymm3, ymm4, ymm5, ymm6, ymm7, ymm8;
    ymm0 = _mm256_set1_pd(1.0);
    ymm1 = _mm256_set1_pd(delta);
    ymm2 = _mm256_set_pd(delta * 3, delta * 2, delta * 1, 0.0);
    ymm4 = _mm256_setzero_pd();
    ymm5 = _mm256_setzero_pd();
    ymm7 = _mm256_setzero_pd();

    for ( int i = 0 ; i < dt - 12 ; i+=12 ) {
        ymm3 = _mm256_set1_pd(i * delta);  
        ymm3 = _mm256_add_pd(ymm3, ymm2);  
	ymm3 = _mm256_mul_pd(ymm3, ymm3);  
        ymm3 = _mm256_add_pd(ymm0, ymm3); 
	ymm3 = _mm256_div_pd(ymm1, ymm3);  
        ymm4 = _mm256_add_pd(ymm4, ymm3);  
		       
        ymm6 = _mm256_set1_pd((i+4)* delta);  
        ymm6 = _mm256_add_pd(ymm6, ymm2);  
	ymm6 = _mm256_mul_pd(ymm6, ymm6);  
	ymm6 = _mm256_add_pd(ymm0, ymm6);  
	ymm6 = _mm256_div_pd(ymm1, ymm6);  
	ymm5 = _mm256_add_pd(ymm5, ymm6); 

	ymm8 = _mm256_set1_pd((i +8)* delta);
	ymm8 = _mm256_add_pd(ymm8, ymm2);
	ymm8 = _mm256_mul_pd(ymm8, ymm8);
	ymm8 = _mm256_add_pd(ymm0, ymm8);
	ymm8 = _mm256_div_pd(ymm1, ymm8);
	ymm7 = _mm256_add_pd(ymm7, ymm8);
    }
    
    ymm4 = _mm256_add_pd(ymm4, ymm5);
    ymm4 = _mm256_add_pd(ymm4, ymm7);
    double tmp[4] __attribute__((aligned(32)));
    _mm256_store_pd(tmp, ymm4);
    pi += tmp[0] + tmp[1] + tmp[2] + tmp[3];

    return pi *= 4;

}

int main(void)
{
    clock_t begin, end;
    double spend_time;
    begin = clock();
    double pi = compute_pi(128000000);
    end = clock();
    spend_time = (double) (end - begin) / CLOCKS_PER_SEC;
    printf( "%lf\nexcution time : %lf\n", pi, spend_time );
    return 0;
}
