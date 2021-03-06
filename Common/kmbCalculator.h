/*----------------------------------------------------------------------
#                                                                      #
# Software Name : REVOCAP_PrePost version 1.6                          #
# Class Name : Calculator                                              #
#                                                                      #
#                                Written by                            #
#                                           K. Tokunaga 2012/03/23     #
#                                                                      #
#      Contact Address: IIS, The University of Tokyo CISS              #
#                                                                      #
#      "Large Scale Assembly, Structural Correspondence,               #
#                                     Multi Dynamics Simulator"        #
#                                                                      #
# Software Name : RevocapMesh version 1.2                              #
#                                                                      #
#                                Written by                            #
#                                           K. Tokunaga 2008/03/14     #
#                                           T. Takeda   2008/03/14     #
#                                           K. Amemiya  2008/03/14     #
#                                                                      #
#      Contact Address: RSS21 Project, IIS, The University of Tokyo    #
#                                                                      #
#      "Innovative General-Purpose Coupled Analysis System"            #
#                                                                      #
----------------------------------------------------------------------*/
#pragma once

#include <cfloat>

namespace kmb{

class AverageCalculator
{
public:
	AverageCalculator(void);
	virtual ~AverageCalculator(void);
	void initialize(void);
	void add(double value);
	double getAverage(void) const;
protected:
	double sum;
	int counter;
};

class Minimizer
{
public:
	Minimizer(void);
	virtual ~Minimizer(void);
	double getMin(void) const{ return minValue; }
	void initialize(void);
	/// 最小値が更新された時に true を返す
	bool update(double value);
	static double getMin(double x,double y);
	static double getMin(double x,double y,double z);
	// 最小値の順番を返す
	template <typename T> static int minIndex(T x0, T x1, T x2) {
		if (x0 <= x1) {
			if (x0 <= x2) {
				return 0;
			}
			else {
				// 2 < 0 <= 1
				return 2;
			}
		}
		else {
			if (x1 <= x2) {
				return 1;
			}
			else {
				// 2 < 1 < 0
				return 2;
			}
		}
	}
protected:
	double minValue;
};

class Maximizer
{
public:
	Maximizer(void);
	virtual ~Maximizer(void);
	double getMax(void) const{ return maxValue; }
	void initialize(void);
	/// 最大値が更新された時に true を返す
	bool update(double value);
	static double getMax(double x,double y);
	static double getMax(double x,double y,double z);
	// 最大値の順番を返す
	template <typename T> static int maxIndex(T x0, T x1, T x2) {
		if (x0 >= x1) {
			if (x0 >= x2) {
				return 0;
			}else {
				// 2 > 0 >= 1
				return 2;
			}
		}else {
			if (x1 >= x2) {
				return 1;
			}
			else {
				// 2 > 1 > 0
				return 2;
			}
		}
	}
protected:
	double maxValue;
};

// 型 T で与えられる Id と一緒に保存
template<typename T>
class MinMaxWithId
{
protected:
	double minValue;
	double maxValue;
	T minId;
	T maxId;
public:
	MinMaxWithId(void)
		: minValue(DBL_MAX), maxValue(-DBL_MAX), minId(-1), maxId(-1)
	{
	}
	virtual ~MinMaxWithId(void){};
	double getMin(void) const{ return minValue; }
	double getMax(void) const{ return maxValue; }
	T getMinId(void) const{ return minId; }
	T getMaxId(void) const{ return maxId; }
	MinMaxWithId<T>& operator=(const MinMaxWithId<T> &other){
		this->minId = other.minId;
		this->maxId = other.maxId;
		this->minValue = other.minValue;
		this->maxValue = other.maxValue;
		return *this;
	}
	void initialize(void)
	{
		minValue = DBL_MAX;
		maxValue = -DBL_MAX;
	}
	// 更新された時に true を返す
	bool update(double value,T id)
	{
		bool flag = false;
		if( value > this->maxValue ){
			this->maxValue = value;
			this->maxId = id;
			flag = true;
		}
		if( value < this->minValue ){
			this->minValue = value;
			this->minId = id;
			flag = true;
		}
		return flag;
	}
};

class Calculator
{
protected:
	double maxValue;
	double minValue;
	double sum;
	int counter;
public:
	Calculator(void);
	virtual ~Calculator(void);
	double getMax(void) const{ return maxValue; }
	double getMin(void) const{ return minValue; }
	double getAverage(void) const;
	double getMaxAbs(void) const;
	double getSum(void) const;
	void initialize(void);
	/// 最大値が更新された時に true を返す
	bool update(double value);
};

class IntegerCalculator
{
protected:
	long maxValue;
	long minValue;
	long sum;
	int counter;
public:
	IntegerCalculator(void);
	virtual ~IntegerCalculator(void);
	long getMax(void) const{ return maxValue; }
	long getMin(void) const{ return minValue; }
	long getAverage(void) const;
	void initialize(void);
	/// 最大値が更新された時に true を返す
	bool update(long value);
	static long getMax(long x,long y);
	static long getMax(long x,long y,long z);
	static long getMin(long x,long y);
	static long getMin(long x,long y,long z);
	static long getAve(long x,long y);
	static long getAve(long x,long y,long z);
};

}
