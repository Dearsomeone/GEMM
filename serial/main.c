#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "compute.h"

int main()
{
	int Ni = 512;
	int Nj = 512;
	int maxIter = 10000;
	int gridNum = Ni * Nj;
	int i, j, iter;
	float  residual, residual0;
	float* T = (float*)malloc(sizeof(float) * gridNum);
	float* TOld = (float*)malloc(sizeof(float) * gridNum);
	clock_t start, finish;

	if (T == NULL || TOld == NULL) {
		printf("Memory allocation failed!\n");
		return 1;
	}

	// 初始化
	memset(T, 0, gridNum);
	memset(TOld, 0, gridNum);

	// 设置边界条件
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
	printf("\nTime to solution: %.1f [sec]\n", (double)(finish - start) / CLOCKS_PER_SEC);

	// 释放内存
	free(T);
	free(TOld);
	T = TOld = NULL;

	return 0;
}