#pragma once

#include "Matrix/kmbMatrix.h"

template <typename T> double kmb::SquareMatrix::quadratic(const T x, const T y) const
{
	double sum = 0.0;
	int size = getSize();
	for (int i = 0; i<size; ++i) {
		for (int j = 0; j<size; ++j) {
			sum += x[i] * get(i, j) * y[j];
		}
	}
	return sum;
}
