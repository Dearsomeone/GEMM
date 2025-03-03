#include <math.h>
#include <stdio.h>
#include <immintrin.h>
#include "compute.h"

void multiply(int N, float* A, float* B, float* C)
{
	int i, j, k;

	for (i = 0; i < N; i++) 
    {
        for (j = 0; j < N; j++) 
        {
            /* 初始化 C[i][j] 为 0 */
            __m256 c = _mm256_setzero_ps();
            for (k = 0; k < N; k += 8) 
            {
                /* 加载 A[i][k] 到 AVX2 寄存器 */
                __m256 a = _mm256_loadu_ps(&A[i * N + k]);
                /* 加载 B[k][j] 到 AVX2 寄存器 */
                __m256 b = _mm256_setr_ps(
                    B[k * N + j],       B[(k + 1) * N + j],
                    B[(k + 2) * N + j], B[(k + 3) * N + j],
                    B[(k + 4) * N + j], B[(k + 5) * N + j],
                    B[(k + 6) * N + j], B[(k + 7) * N + j]
                );
                /* 乘法操作：a * b */
                __m256 mul = _mm256_mul_ps(a, b);
                /* 加法操作：c += a * b */
                c = _mm256_add_ps(c, mul);
            }
            /* 将 AVX2 寄存器中的结果累加并存储到 C[i][j] */
            float temp[8];
            _mm256_storeu_ps(temp, c);
            C[i * N + j] = temp[0] + temp[1] + temp[2] + temp[3] +
                           temp[4] + temp[5] + temp[6] + temp[7];
        }
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