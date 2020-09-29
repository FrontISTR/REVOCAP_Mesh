#pragma once

#include "Matrix/kmbMatrix.h"
#include "Matrix/kmbMatrix_DoubleArray.h"

template <typename T1, typename T2> bool solveByGauss(const kmb::SquareMatrix& a,T1 b, T2 x)
{
	int size = a.getSize();
	if (size == 1) {
		double d = a.get(0, 0);
		if (d != 0.0) {
			x[0] = b[0] / d;
			return true;
		}
		else {
			return false;
		}
	}
	else if (size == 2) {
		double d = a.determinant();
		if (d != 0.0) {
			x[0] = (a.get(1, 1)*b[0] - a.get(0, 1)*b[1]) / d;
			x[1] = (-a.get(1, 0)*b[0] + a.get(0, 0)*b[1]) / d;
			return true;
		}
		else {
			return false;
		}
	}
	else {
		// 消去法
		kmb::Matrix_DoubleArray mat(size, size + 1);
		for (int j = 0; j<size; ++j) {
			for (int i = 0; i<size; ++i) {
				mat.set(i, j, a.get(i, j));
			}
		}
		for (int i = 0; i<size; ++i) {
			mat.set(i, size, b[i]);
		}

		for (int j = 0; j<size; ++j) {
			// j から size-1 で絶対値最大を探す
			int ind = -1;
			double m = 0.0;
			for (int i = j; i<size; ++i) {
				double d = fabs(mat.get(i, j));
				if (d > m) {
					ind = i;
					m = d;
				}
			}
			if (m == 0.0) {
				return false;
			}
			else if (ind != j) {
				mat.row_exchange(j, ind);
			}
			mat.row_multi(j, 1.0 / mat.get(j, j));
			for (int i = 0; i<size; ++i) {
				if (i != j) {
					mat.row_transf(j, i, -mat.get(i, j));
				}
			}
		}
		for (int i = 0; i<size; ++i) {
			x[i] = mat.get(i, size);
		}
		return true;
	}
}
