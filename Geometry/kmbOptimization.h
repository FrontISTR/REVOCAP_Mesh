/*----------------------------------------------------------------------
#                                                                      #
# Software Name : REVOCAP_PrePost version 1.6                          #
# Class Name : Optimization                                            #
#                                                                      #
#                                Written by                            #
#                                           K. Tokunaga 2012/03/23     #
#                                                                      #
#      Contact Address: IIS, The University of Tokyo CISS              #
#                                                                      #
#      "Large Scale Assembly, Structural Correspondence,               #
#                                     Multi Dynamics Simulator"        #
#                                                                      #
----------------------------------------------------------------------*/
/**
 *
 * ����`�֐��̍œK�����s�����߂̃A���S���Y���W
 *
 */
#pragma once

#include "Common/kmbCommon.h"

#include <cfloat>

namespace kmb{

class Matrix;
class ColumnVector;
class Region;

// scalar => scalar
// �����Ȃ�
class OptTargetSS_0{
public:
	virtual ~OptTargetSS_0(void){};
	virtual double f(double t) = 0;
};

// vector => scalar
// �����Ȃ�
class OptTargetSV_0{
public:
	virtual ~OptTargetSV_0(void){};
	virtual int getDomainDim(void) const = 0;
	virtual double f(const double* t) = 0;
};

// vector => vector
// �����Ȃ�
class OptTargetVV_0{
public:
	virtual ~OptTargetVV_0(void){};
	virtual int getDomainDim(void) const = 0;
	virtual int getRangeDim(void) const = 0;
	virtual bool f(const ColumnVector &t,ColumnVector &val) = 0;
};

// scalar => scalar
class OptTargetSS : public OptTargetSS_0{
public:
	virtual ~OptTargetSS(void){};
	virtual double df(double t) = 0;
};

// vector => scalar
class OptTargetSV : public OptTargetSV_0{
public:
	virtual ~OptTargetSV(void){};
	virtual bool df(const double* t,double* deriv) = 0;
};

// vector => vector
class OptTargetVV : public OptTargetVV_0{
public:
	virtual ~OptTargetVV(void){};
	virtual bool df(const ColumnVector &t,Matrix &jac) = 0;
};

class Optimization
{
public:
	typedef double (*FuncSS)(double);
private:
	int iterCount;
	// f(x) = 0 �����߂�ꍇ�ɁAf(x) �̒l
	// �܂��� f(x) �̍ŏ��l�����߂�Ƃ��� df(x) �̒l
	double residual;
	// f(x) �̍ŏ��l�����߂�ꍇ�ɁAx_n - x_(n-1)
	double diff;
	int iterMax;
	double epsilon;
public:
	Optimization(void);
	~Optimization(void){}
	void initialize(void);
	int getIterCount(void) const;
	double getResidual(void) const;
	double getDiff(void) const;
	void setIterMax(int max);
	void setEpsilon(double e);
	

	// min_t, max_t �̗̈�� div �ŕ������� grid �̒��_�ōŏ��̂Ƃ����T��
	// �߂�l�� grid �̒��_�̍��W
	// 1�����̋�ԏ�̓��Ԋu�̓_�̒�����ŏ���T��
	double calcMinOnGrid( OptTargetSS_0 &obj, double& opt_t, const double min_t, const double max_t, const int div );
	// 2,3������Ԃ̊i�q��̓_�̒�����ŏ���T��
	bool calcMinOnGrid( OptTargetSV_0 &obj, double* opt_t, const double* min_t, const double* max_t, const int* div );
	// 3������Ԃ̗̈搧�����ōŏ���T��
	bool calcMinOnGridWithRegion( OptTargetSV_0 &obj, double* opt_t, const double* min_t, const double* max_t, const int* div, const kmb::Region *region );
	// min_t, max_t �̗̈�� div �ŕ������� grid �̒��S�ōŏ��̂Ƃ����T��
	bool calcMinOnCell( OptTargetSV_0 &obj, double* optmin_t, double* optmax_t, const double* min_t, const double* max_t, const int* div );


	// Newton-Raphson �@
	double calcZero_NR( FuncSS f, FuncSS df, double min_t, double max_t, double init_t=-DBL_MAX );
	double calcZero_NR( OptTargetSS &obj, double min_t, double max_t, double init_t=-DBL_MAX );
	bool calcZero_NR( OptTargetVV &obj, double* opt_t, const double* min_t, const double* max_t, const double* init_t=NULL );

	// Damped Newton Method �����j���[�g���@
	double calcZero_DN( OptTargetSS &obj, double init_t );
	bool calcZero_DN( OptTargetVV &obj, double* opt_t, const double* init_t );
	
	// �Z�J���g�@
	// init_t0 �� init_t1 �̊Ԃɉ�������
	double calcZero_secant( OptTargetSS_0 &obj, double init_t0, double init_t1 );

	// Golden Section ���������@
	// min_t �� max_t �̊Ԃɍŏ��l��1�������邱�Ɓi�P�����j������
	double calcMin_GS( FuncSS f, double min_t, double max_t );
	double calcMin_GS( OptTargetSS_0 &obj, double min_t, double max_t );
	bool calcMin_GS( OptTargetSV_0 &obj, double* opt_t, const double* min_t, const double* max_t );

	// Conjugate Gradient �������z�@
	bool calcMin_CG( OptTargetSV &obj, double* opt_t, const double* min_t, const double* max_t, const double* init_t=NULL );

	// ���j���[�g��(BFGS)�@
	bool calcMin_BFGS( OptTargetSV &obj, double* opt_t, double* init_t );
private:
	// ������ rho = 1 + (1-tau)
	static const double tau;
};

}
