/*
 *  Maths.cpp
 *  spinify
 *
 *  Created by Loïc Séguin-Charbonneau on 10-02-18.
 *  Copyright 2010 Loïc Séguin-Charbonneau. All rights reserved.
 *
 */

#include <cmath>

#include "Maths.h"

double
avg(double* pV, const int nV)
{
	double sum = 0;
	for (int i = 0; i < nV; i++) {
		sum += pV[i];
	}
	return sum / nV;
}

double
stdDev(double *pV, const int nV)
{
	double avgV = avg(pV, nV);
	double sum = 0;
	for (int i = 0; i < nV; i++) {
		sum += (pV[i] - avgV)*(pV[i] - avgV);
	}
	return sqrt(sum / nV);
}
