#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include <omp.h> 
#include "compute.h"

int main(int argc, char** argv)
{
	int N = 1000;
	unsigned int size = 1000000;
	int batchSize = 1;
	int i, j;
	float* A;
	float* B;
	float* C;
	float* timeData;
	double timeCost;
	char* endptr;
	double start, finish;
	FILE* file;

	/* 测试omp是否开启成功 */
	#pragma omp parallel for
	for (int i = 0; i < 10; i++) 
	{
		printf("Hello World %d from thread = %d\n", i, omp_get_thread_num());
	}

	/* 解析命令行参数 */ 
	for (i = 1; i < argc; i++) 
	{
		switch(i)
		{
			case 1:
				N = (int)strtol(argv[i], &endptr, 10);
				printf("Command Line Argument %d: %d\n", i, N);
				break;
			case 2:
				batchSize = (int)strtol(argv[i], &endptr, 10);
				printf("Command Line Argument %d: %d\n", i, batchSize);
				break;
			default:
				printf("Warning: Ignore redundant command line arguments!");
				break;
		}
    }

	size = N * N;
	/* 分配内存空间 */
	A = (float*)malloc(sizeof(float) * size);
	B = (float*)malloc(sizeof(float) * size);
	C = (float*)malloc(sizeof(float) * size);
	timeData = (float*)malloc(sizeof(float) * batchSize);

	if (A == NULL || B == NULL || C == NULL || timeData == NULL) 
	{
		printf("Memory allocation failed!\n");
		return 1;
	}

	for (i = 0; i < batchSize; i++)
	{
		/* 初始化数组 */
		memset(C, 0, size * sizeof(float));
		for (j = 0; j < size; j++)
		{
			A[j] = sin(j);
			B[j] = cos(j);
		}

		/* 开始计算 */
		start = omp_get_wtime();
		multiply(N, A, B, C);
		finish = omp_get_wtime();
		timeCost = finish - start;
		timeData[i] = timeCost;
		printf("\nTime to solution of the %dth test: %.1f [sec]\n", i, timeCost);
		/*printMatrix(N, A);
		printMatrix(N, B);
		printMatrix(N, C);*/
	}
	
	/* 计时数据存入文件 */
	file = fopen("timing.txt", "w");
    if (file == NULL) 
	{
        perror("Failed to open file");
        return 1;
    }
    for (i = 0; i < batchSize; i++) 
	{
        fprintf(file, "%.1f\n", timeData[i]);
    }
    fclose(file);

	/* 释放内存 */
	free(A);
	free(B);
	free(C);
	free(timeData);
	A = B = C = timeData = NULL;

	return 0;
}