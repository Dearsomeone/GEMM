#include <stdio.h>
#include <stdlib.h>
#include "compute.h"

int main()
{
	int Ni = 512;
	int Nj = 512;
	int maxIter = 10000;
	int     i, j, iter;
	float    residual, residual0;
	float* temp = (float*)malloc(sizeof(float) * Ni * Nj);
	float* tempOld = (float*)malloc(sizeof(float) * Ni * Nj);

	// ≥ı ºªØ
	for (j = 0; j < Nj; j++)
	{
		for (i = 0; i < Ni; i++)
		{
			temp[i + Ni * j] = 0.0f;
			tempOld[i + Ni * j] = 0.0f;
		}
	}

	for (i = 0; i < Ni; i++)
	{
		temp[i + Ni * 0] = tempOld[i + Ni * 0] = 5.0f;
		temp[i + Ni * (Nj - 1)] = tempOld[i + Ni * (Nj - 1)] = 5.0f;
	}

	// set boundary conditions at i=0, i=Ni-1

	for (j = 1; j < (Nj - 1); j++)
	{
		temp[0 + Ni * j] = tempOld[0 + Ni * j] = 5.0f;
		temp[Ni - 1 + Ni * j] = tempOld[Ni - 1 + Ni * j] = 5.0f;
	}

	for (iter = 1; iter <= maxIter; iter++)
	{
		residual = Jacobi(Ni, Nj, temp, tempOld);

		float* tmp = tempOld;
		tempOld = temp;
		temp = tmp;

		if (iter == 1) residual0 = residual;
		if (iter == 1 || iter % 1000 == 0)
		{
			printf("%5d %14.6f\n", iter, residual / residual0);
		}
	}
	return 0;
}