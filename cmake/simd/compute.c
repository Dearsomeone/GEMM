#include <math.h>
#include "compute.h"

float Jacobi(int Ni, int Nj, float* T, float* TOld)
{
	int i, j, index;
	float residual = 0.0f;

	for (j = 1; j < (Nj - 1); j++)
	{
		for (i = 1; i < (Ni - 1); i++)
		{
			index = i + j * Ni;
			T[index] = 0.25f * (TOld[index + 1] + TOld[index - 1] + TOld[index + Ni] + TOld[index - Ni]);
			residual += (T[index] - TOld[index]) * (T[index] - TOld[index]);
		}
	}

	residual = (float)sqrt(residual);
	return residual;
}