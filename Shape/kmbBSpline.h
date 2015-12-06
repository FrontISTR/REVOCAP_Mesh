/*----------------------------------------------------------------------
#                                                                      #
# Software Name : REVOCAP_PrePost version 1.6                          #
# Class Name : BSpline                                                 #
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
// B-Spline ���֐��̊�b�m��
// 0 ���� knots �̋�Ԃ� step �֐�
// p ���� �i�敪�I�jp ��������
// knots �� k_0,k_1,...,k_m-1 �Ƃ���
// i �Ԗڂ� 0 �����֐��� [k_i,k_i+1) ���Ɏ���
// i �Ԗڂ� p �����֐��� [k_i,k_i+p+1) ���Ɏ���
// p ���̊��֐��̌��Fm - (p+1)
// n �̐���_���� p ���̋Ȑ������ɂ� n+p+1 �� knot ���K�v

#pragma once

#include <vector>

namespace kmb{

class BSpline
{
private:
	std::vector< double > knots;
	// �Ȑ��p�����[�^�̐��x
	static double precision;
public:
	BSpline(void);
	~BSpline(void);
	void clear(void);
	bool appendKnot(double k);
	double getKnot(int index) const;
	void setKnots(int num,double* knots);
	int getKnotCount(void) const;
	// knotsCount = m �̂Ƃ��Aindex �� 0,...,m-deg-2 �܂�
	double getValue(int index,int degree,double u) const;
	// knotsCount = m �̂Ƃ��Aindex �� 0,...,m-deg-3 �܂�
	double getDerivative(int index,int degree,double u) const;
	// knotsCount = m �̂Ƃ��Aindex �� 0,...,m-deg-4 �܂�
	double getSecondDerivative(int index,int degree,double u) const;
	// ��`��Ɋ܂܂�邩�ǂ��� = knots �̍ŏ��ƍŌ�̊Ԃɂ��邩�ǂ���
	bool isDomain(double t) const;
	void getDomain(double &min_t,double &max_t) const;
	// origin �����_�� unit ��P�ʂƂ������W�ł̍ő�ƍŏ������߂�
	void getDomainOnFrame( double origin, double unit, double &min_t,double &max_t) const;
private:
	// t �� knot �� i �Ԗڈȏ� i+1 �Ԗڂ�菬����
	// t < knots[0] �̏ꍇ�� -1
	int getInterval(double t) const;
};

}
