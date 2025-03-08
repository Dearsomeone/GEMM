#include <math.h>
#include <stdio.h>
#include <immintrin.h>
#include "compute.h"

void multiply(int N, float* A, float* B, float* C)
{
	int i, j, k;

	for (i = 0; i < N; i++)
        for (k = 0; k < N; k++)
        {
            j = 0;
            __m256 ra = _mm256_set1_ps(A[i * N + k]);
            
            for (; j <= N - 8; j += 8)
            {
                _mm256_storeu_ps(C + i * N + j, _mm256_add_ps(*(__m256 *)(C + i * N + j), _mm256_mul_ps(*(__m256 *)(B + k * N + j), ra)));
            }

            for (; j < N; j++)
                C[i * N + j] += A[i * N + k] * B[k * N + j];
        }
}

void printMatrix(int N, float* matrix)
{
	int i, j;
	for (i = 0; i < N; i++) 
    {
        for (j = 0; j < N; j++) 
        {
            printf("%8.2f ", matrix[i * N + j]);
        }
        printf("\n");
    }
}