#include <math.h>
#include "compute.h"

float Jacobi(int Ni, int Nj, float* temp, float* tempOld)
{
	int i, j, ij;
	float residual = 0.0f;

	for (j = 1; j < (Nj - 1); j++)
	{
		for (i = 1; i < (Ni - 1); i++)
		{
			ij = i + j * Ni;
			temp[ij] = 0.25f * (tempOld[ij + 1] + tempOld[ij - 1] + tempOld[ij + Ni] + tempOld[ij - Ni]);
			residual += (temp[ij] - tempOld[ij]) * (temp[ij] - tempOld[ij]);
		}
	}

	residual = (float)sqrt(residual);
	return residual;
}