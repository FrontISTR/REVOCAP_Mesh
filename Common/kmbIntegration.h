/*----------------------------------------------------------------------
#                                                                      #
# Software Name : REVOCAP_PrePost version 1.6                          #
# Class Name : Integration                                             #
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

#pragma once

//
// 使い方
// 1変数関数fを2次のガウス・ルジャンドル積分公式を使う場合
// double f(double)
// GaussQuadrature<2>::evaluate(f);
// 1変数m成分関数fを2次のガウス・ルジャンドル積分公式を使う場合
// void f(double,double*,int m)
// double s[m];
// GaussQuadrature<2>::evaluate_n(f,s);
// 2変数関数fを第1変数を2次で、第2変数を3次で積分公式を使う場合
// GaussQuadrature2<2,3>::evaluate(f)
// 2変数n成分関数fを第1変数を2次で、第2変数を2次で積分公式を使う場合
// double s[m];
// GaussQuadrature2<2,2>::evaluate_n(f,s);
//

namespace kmb{

// 線分 [-1,1]
template<int m>
class GaussQuadrature{
public:
	static const double IntegrationPoint[m];
	static const double Weight[m];
	template<typename F> static double evaluate(F f){
		double s = 0.0;
		for(int i=0;i<m;++i){
			s += Weight[i] * f(IntegrationPoint[i]);
		}
		return s;
	}
	template<typename F> static void evaluate_n(F f,double* s,int n){
		double* t = new double[n];
		for(int j=0;j<n;++j){
			s[j] = 0.0;
		}
		for(int i=0;i<m;++i){
			f(IntegrationPoint[i],t,n);
			for(int j=0;j<n;++j){
				s[j] += Weight[i] * t[j];
			}
		}
		delete[] t;
	}
};

// 正方形 [-1,1]
template<int m0,int m1>
class GaussQuadrature2{
public:
	template<typename F> static double evaluate(F f){
		double s = 0.0;
		for(int i0=0;i0<m0;++i0){
			for(int i1=0;i1<m1;++i1){
				s += GaussQuadrature<m0>::Weight[i0] * GaussQuadrature<m1>::Weight[i1] * f(GaussQuadrature<m0>::IntegrationPoint[i0],GaussQuadrature<m1>::IntegrationPoint[i1]);
			}
		}
		return s;
	}
	template<typename F> static void evaluate_n(F f,double* s,int n){
		double* t = new double[n];
		for(int j=0;j<n;++j){
			s[j] = 0.0;
		}
		for(int i0=0;i0<m0;++i0){
			for(int i1=0;i1<m1;++i1){
				f(GaussQuadrature<m0>::IntegrationPoint[i0],GaussQuadrature<m1>::IntegrationPoint[i1],t,n);
				for(int j=0;j<n;++j){
					s[j] += GaussQuadrature<m0>::Weight[i0] * GaussQuadrature<m1>::Weight[i1] * t[j];
				}
			}
		}
		delete[] t;
	}
};

// 立方体 [-1,1]x[-1,1]x[-1,1]
template<int m0,int m1,int m2>
class GaussQuadrature3{
public:
	template<typename F> static double evaluate(F f){
		double s = 0.0;
		for(int i0=0;i0<m0;++i0){
			for(int i1=0;i1<m1;++i1){
				for(int i2=0;i2<m2;++i2){
					s += GaussQuadrature<m0>::Weight[i0] * GaussQuadrature<m1>::Weight[i1] * GaussQuadrature<m2>::Weight[i2] *
						f(GaussQuadrature<m0>::IntegrationPoint[i0],GaussQuadrature<m1>::IntegrationPoint[i1],GaussQuadrature<m2>::IntegrationPoint[i2]);
				}
			}
		}
		return s;
	}
	template<typename F> static void evaluate_n(F f,double* s,int n){
		double* t = new double[n];
		for(int j=0;j<n;++j){
			s[j] = 0.0;
		}
		for(int i0=0;i0<m0;++i0){
			for(int i1=0;i1<m1;++i1){
				for(int i2=0;i2<m2;++i2){
					f(GaussQuadrature<m0>::IntegrationPoint[i0],GaussQuadrature<m1>::IntegrationPoint[i1],GaussQuadrature<m2>::IntegrationPoint[i2],t,n);
					for(int j=0;j<n;++j){
						s[j] += GaussQuadrature<m0>::Weight[i0] * GaussQuadrature<m1>::Weight[i1] * GaussQuadrature<m2>::Weight[i2] * t[j];
					}
				}
			}
		}
		delete[] t;
	}
};

template<> const double kmb::GaussQuadrature<2>::IntegrationPoint[] = {
	-0.57735026918962573105886804114561527967453,
	0.57735026918962573105886804114561527967453
};

template<> const double kmb::GaussQuadrature<2>::Weight[] = {
	1.0,
	1.0
};

template<> const double kmb::GaussQuadrature<3>::IntegrationPoint[] = {
	-0.774596669241483404277914814883843064308167,
	0.0,
	0.774596669241483404277914814883843064308167
};

template<> const double kmb::GaussQuadrature<3>::Weight[] = {
	0.555555555555555555555555555555555555555556,
	0.888888888888888888888888888888888888888889,
	0.555555555555555555555555555555555555555556,
};

template<> const double GaussQuadrature<4>::IntegrationPoint[] = {
	-0.861136311594052572537805190222570672631264,
	-0.339981043584856312822495283398893661797047,
	0.339981043584856312822495283398893661797047,
	0.861136311594052572537805190222570672631264
};

template<> const double kmb::GaussQuadrature<4>::Weight[] = {
	0.347854845137453849712727560472558252513409,
	0.652145154862546205798423670785268768668175,
	0.652145154862546205798423670785268768668175,
	0.347854845137453849712727560472558252513409
};

template<> const double kmb::GaussQuadrature<5>::IntegrationPoint[] = {
	-0.90617984593866399279762687829939,
	-0.53846931010568309103631442070021,
	0.0,
	0.53846931010568309103631442070021,
	0.90617984593866399279762687829939
};

template<> const double kmb::GaussQuadrature<5>::Weight[] = {
	0.23692688505618908751426404071992,
	0.47862867049936646804129151483564,
	0.56888888888888888888888888888889,
	0.47862867049936646804129151483564,
	0.23692688505618908751426404071992
};

}
