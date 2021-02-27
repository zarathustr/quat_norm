#include <string.h> /* needed for memcpy() */
#include <stdio.h>
#include <stdlib.h>
#include <time.h> 
#include <math.h>

#define MAX(a, b) (a > b ? a : b)
#define SORT(a, b, c, d) (MAX(MAX(a, b), MAX(a, b)))

float invSqrtf( float number )
{
	long i;
	float x2, y;
	const float threehalfs = 1.5F;

	x2 = number * 0.5F;
	y  = number;
	i  = * ( long * ) &y;                       
	i  = 0x5f3759df - ( i >> 1 );               
	y  = * ( float * ) &i;
	y  = y * ( threehalfs - ( x2 * y * y ) );  

	return y;
}


double invSqrt( const double x )
{    
    double y = x;    
    double xhalf = ( double )0.5 * y;    
    long long i = *( long long* )( &y );    
    i = 0x5fe6ec85e7de30daLL - ( i >> 1 ); //LL suffix for (long long) type for GCC    
    y = *( double* )( &i );    
    y = y * ( ( double )1.5 - xhalf * y * y );        
    return y;
}




int main()
{
    srand(time(0)); 
    int iter;
    printf("Please enter the number of iteration:\n");
    scanf("%d", &iter);
    
    clock_t start, end;
    double cpu_time_used;
    double time_invSqrtf = 0, time_COROf = 0, 
           time_sqrtf = 0, time_invSqrt = 0, 
           time_CORO = 0, time_sqrt = 0;
    
    double err_invSqrtf = 0, err_COROf = 0, err_invSqrt = 0, 
           err_CORO = 0;
    
    double err_q0, err_q1, err_q2, err_q3;
    
    FILE* fp = fopen("quat_norm.txt", "w+");
    fprintf(fp, "\n");
    
    for(int i = 0; i < iter; ++i)
    {
        int num = rand();
        float qq0 = ((float) num);
        num = rand();
        float qq1 = ((float) num);
        num = rand();
        float qq2 = ((float) num);
        num = rand();
        float qq3 = ((float) num);
        
        float q0, q1, q2, q3, N;
        float q0_, q1_, q2_, q3_;
        q0_ = qq0; q1_ = qq1; q2_ = qq2; q3_ = qq3;
        start = clock();
        N = 1.0f / sqrtf(q0_ * q0_ + q1_ * q1_ + q2_ * q2_ + q3_ * q3_);
        q0_ = q0_ * N;
        q1_ = q1_ * N;
        q2_ = q2_ * N;
        q3_ = q3_ * N;
        end = clock();
        cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
        time_sqrtf = time_sqrtf + cpu_time_used;
        
        fprintf(fp, "Sqrtf: %5.16f\t%5.16f\t%5.16f\t%5.16f\n", (double)q0_, (double)q1_, (double)q2_, (double)q3_);
        
        q0 = qq0; q1 = qq1; q2 = qq2; q3 = qq3;
        start = clock();
        N = fabsf(invSqrtf(q0 * q0 + q1 * q1 + q2 * q2 + q3 * q3));
        q0 = q0 * N;
        q1 = q1 * N;
        q2 = q2 * N;
        q3 = q3 * N;
        end = clock();
        cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
        time_invSqrtf = time_invSqrtf + cpu_time_used;
        err_q0 = q0 - q0_;
        err_q1 = q1 - q1_;
        err_q2 = q2 - q2_;
        err_q3 = q3 - q3_;
        err_invSqrtf = err_invSqrtf + 1.0f / ((float) iter) * sqrtf(err_q0 * err_q0 + err_q1 * err_q1 + err_q2 * err_q2 + err_q3 * err_q3);
        
        
        fprintf(fp, "iqrtf: %5.16f\t%5.16f\t%5.16f\t%5.16f\n", (double)q0, (double)q1, (double)q2, (double)q3);
        
        q0 = qq0; q1 = qq1; q2 = qq2; q3 = qq3;
        start = clock();
        float maximum = 1.0f / SORT(fabsf(q0), fabsf(q1), fabsf(q2), fabsf(q3));
        q0 = q0 * maximum;
        q1 = q1 * maximum;
        q2 = q2 * maximum;
        q3 = q3 * maximum;
        for(int j = 0; j < 3; ++j)
        {
            N = (q0 * q0 + q1 * q1 + q2 * q2 + q3 * q3);
            N = (5.0f + N) / (2.0f + 4.0f * N);
            q0 = q0 * N;
            q1 = q1 * N;
            q2 = q2 * N;
            q3 = q3 * N;
        }
        end = clock();
        cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
        time_COROf = time_COROf + cpu_time_used;
        err_q0 = q0 - q0_;
        err_q1 = q1 - q1_;
        err_q2 = q2 - q2_;
        err_q3 = q3 - q3_;
        err_COROf = err_COROf + 1.0f / ((float) iter) * sqrtf(err_q0 * err_q0 + err_q1 * err_q1 + err_q2 * err_q2 + err_q3 * err_q3);
        
        fprintf(fp, "COROf: %5.16f\t%5.16f\t%5.16f\t%5.16f\n\n\n", (double)q0, (double)q1, (double)q2, (double)q3);
       
    }
    
    
    
    for(int i = 0; i < iter; ++i)
    {
        int num = rand();
        double qq0 = ((double) num);
        num = rand();
        double qq1 = ((double) num);
        num = rand();
        double qq2 = ((double) num);
        num = rand();
        double qq3 = ((double) num);
        
        double q0, q1, q2, q3, N;
        double q0_, q1_, q2_, q3_;
        q0_ = qq0; q1_ = qq1; q2_ = qq2; q3_ = qq3;
        start = clock();
        N = 1.0 / sqrt(q0_ * q0_ + q1_ * q1_ + q2_ * q2_ + q3_ * q3_);
        q0_ = q0_ * N;
        q1_ = q1_ * N;
        q2_ = q2_ * N;
        q3_ = q3_ * N;
        end = clock();
        cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
        time_sqrt = time_sqrt + cpu_time_used;
        
        
        fprintf(fp, "Sqrt: %5.16f\t%5.16f\t%5.16f\t%5.16f\n", q0_, q1_, q2_, q3_);
        
        q0 = qq0; q1 = qq1; q2 = qq2; q3 = qq3;
        start = clock();
        N = fabs(invSqrt(q0 * q0 + q1 * q1 + q2 * q2 + q3 * q3));
        q0 = q0 * N;
        q1 = q1 * N;
        q2 = q2 * N;
        q3 = q3 * N;
        end = clock();
        cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
        time_invSqrt = time_invSqrt + cpu_time_used;
        err_q0 = q0 - q0_;
        err_q1 = q1 - q1_;
        err_q2 = q2 - q2_;
        err_q3 = q3 - q3_;
        err_invSqrt = err_invSqrt + 1.0 / ((double) iter) * sqrt(err_q0 * err_q0 + err_q1 * err_q1 + err_q2 * err_q2 + err_q3 * err_q3);
        
        
        fprintf(fp, "iqrt: %5.16f\t%5.16f\t%5.16f\t%5.16f\n", q0, q1, q2, q3);
        
        q0 = qq0; q1 = qq1; q2 = qq2; q3 = qq3;
        start = clock();
        double maximum = 1.0 / SORT(fabs(q0), fabs(q1), fabs(q2), fabs(q3));
        q0 = q0 * maximum;
        q1 = q1 * maximum;
        q2 = q2 * maximum;
        q3 = q3 * maximum;
        for(int j = 0; j < 3; ++j)
        {
            N = (q0 * q0 + q1 * q1 + q2 * q2 + q3 * q3);
            N = (5.0 + N) / (2.0 + 4.0 * N);
            q0 = q0 * N;
            q1 = q1 * N;
            q2 = q2 * N;
            q3 = q3 * N;
        }
        end = clock();
        cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
        time_CORO = time_CORO + cpu_time_used;
        err_q0 = q0 - q0_;
        err_q1 = q1 - q1_;
        err_q2 = q2 - q2_;
        err_q3 = q3 - q3_;
        err_CORO = err_CORO + 1.0 / ((double) iter) * sqrt(err_q0 * err_q0 + err_q1 * err_q1 + err_q2 * err_q2 + err_q3 * err_q3);
        
        fprintf(fp, "CORO: %5.16f\t%5.16f\t%5.16f\t%5.16f\n\n\n", q0, q1, q2, q3);

    }
    fclose(fp);
    
    printf("time: \n invSqrtf: %5.8f\t COROf: %5.8f\t sqrtf: %5.8f\t\n", time_invSqrtf, time_COROf, time_sqrtf);
    printf("time: \n invSqrt: %5.8f\t CORO: %5.8f\t sqrt: %5.8f\t\n", time_invSqrt, time_CORO, time_sqrt);
    printf("\n\n");
    printf("error: \n invSqrtf: %5.8f\t COROf: %5.8f\t\n", err_invSqrtf, err_COROf);
    printf("error: \n invSqrt: %5.8f\t CORO: %5.8f\t\n", err_invSqrt, err_CORO);
    return 0;
}