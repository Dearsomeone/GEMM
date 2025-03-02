#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "compute.h"

int main(int argc, char** argv)
{
	int Ni = 512;
	int Nj = 512;
	int maxIter = 10000;
	int gridNum = Ni * Nj;
	int i, j, iter;
	int batchSize;	// 批量测试的次数
	float  residual, residual0;
	float* T = (float*)malloc(sizeof(float) * gridNum);
	float* TOld = (float*)malloc(sizeof(float) * gridNum);
	float* timeData = (float*)malloc(sizeof(float) * batchSize);
	double timeCost;
	clock_t start, finish;
	FILE* file;
	char* endptr;

	if (T == NULL || TOld == NULL || timeData == NULL) {
		printf("Memory allocation failed!\n");
		return 1;
	}

	// 解析命令行参数
	for (i = 1; i < argc; i++) {
        batchSize = (int)strtol(argv[i], &endptr, 10);
        printf("Command Line Argument %d: %d\n", i, batchSize);
    }

	for (int n = 0; n < batchSize; n++)
	{
		// 初始化数组
		memset(T, 0, gridNum);
		memset(TOld, 0, gridNum);

		// 初始化边界条件
		for (i = 0; i < Ni; i++)
		{
			T[i] = TOld[i] = 0.0f;
			T[i + Ni * (Nj - 1)] = TOld[i + Ni * (Nj - 1)] = 100.0f;
		}

		for (j = 1; j < (Nj - 1); j++)
		{
			T[Ni * j] = TOld[Ni * j] = 0.0f;
			T[Ni - 1 + Ni * j] = TOld[Ni - 1 + Ni * j] = 0.0f;
		}

		// 开始计算
		start = clock();
		for (iter = 1; iter <= maxIter; iter++)
		{
			residual = Jacobi(Ni, Nj, T, TOld);

			float* tmp = TOld;
			TOld = T;
			T = tmp;

			/*if (iter == 1) residual0 = residual;
			if (iter == 1 || iter % 1000 == 0)
			{
				printf("%5d %14.6f\n", iter, residual / residual0);
			}*/
		}
		finish = clock();
		timeCost = (double)(finish - start) / CLOCKS_PER_SEC;
		timeData[n] = timeCost;
		printf("\nTime to solution of the %dth test: %.1f [sec]\n", n, timeCost);
	}
	
	// 计时数据存入文件
	file = fopen("output.txt", "w"); // 打开文件
    if (file == NULL) {
        perror("Failed to open file");
        return 1;
    }

    for (i = 0; i < batchSize; i++) {
        fprintf(file, "%.1f\n", timeData[i]); // 每行写入一个数据
    }

    fclose(file); // 关闭文件
	// 释放内存
	free(T);
	free(TOld);
	free(timeData);
	T = TOld = NULL;

	return 0;
}