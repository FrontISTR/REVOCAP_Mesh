/*----------------------------------------------------------------------
#                                                                      #
# Software Name : REVOCAP_PrePost version 1.6                          #
# Class Name : Pyramid2                                                #
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
#include "MeshDB/kmbPyramid2.h"
#include "MeshDB/kmbTetrahedron.h"
#include "MeshDB/kmbMeshData.h"
#include "MeshDB/kmbElementRelation.h"

/********************************************************************************
=begin

=== 2次四角錐要素 (PYRAMID2)

接続行列

	{ 0, 1, 1, 1, 1, 2, 2, 2, 2, 0, 0, 0, 0},
	{ 1, 0, 1, 0, 1, 2, 0, 0, 0, 2, 0, 0, 2},
	{ 1, 1, 0, 1, 0, 0, 2, 0, 0, 2, 2, 0, 0},
	{ 1, 0, 1, 0, 1, 0, 0, 2, 0, 0, 2, 2, 0},
	{ 1, 1, 0, 1, 0, 0, 0, 0, 2, 0, 0, 2, 2},
	{ 2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{ 2, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{ 2, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{ 2, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0},
	{ 0, 2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{ 0, 0, 2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{ 0, 0, 0, 2, 2, 0, 0, 0, 0, 0, 0, 0, 0},
	{ 0, 2, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0},

面

	{ 0, 1, 2, 9, 6, 5},
	{ 0, 2, 3,10, 7, 6},
	{ 0, 3, 4,11, 8, 7},
	{ 0, 4, 1,12, 5, 8},
	{ 4, 3, 2, 1,11,10, 9,12}

辺

	{ 0, 1, 5},
	{ 0, 2, 6},
	{ 0, 3, 7},
	{ 0, 4, 8},
	{ 1, 2, 9},
	{ 2, 3, 10},
	{ 3, 4, 11},
	{ 1, 4, 12}

=end

形状関数
0 : 1/2*u*(1.0+u)                                                                   => (s,t,u) = (   0,   0,  1)
1 : -1/16*(1.0-s)*(1.0-t)*(1.0-u)*(4.0+3.0*s+3.0*t+2.0*s*t+2.0*u+s*u+u*t+2.0*s*t*u) => (s,t,u) = (  -1,  -1, -1)
2 : -1/16*(1.0+s)*(1.0-t)*(1.0-u)*(4.0-3.0*s+3.0*t-2.0*s*t+2.0*u-s*u+u*t-2.0*s*t*u) => (s,t,u) = (   1,  -1, -1)
3 : -1/16*(1.0+s)*(1.0+t)*(1.0-u)*(4.0-3.0*s-3.0*t+2.0*s*t+2.0*u-s*u-u*t+2.0*s*t*u) => (s,t,u) = (   1,   1, -1)
4 : -1/16*(1.0-s)*(1.0+t)*(1.0-u)*(4.0+3.0*s-3.0*t-2.0*s*t+2.0*u+s*u-u*t-2.0*s*t*u) => (s,t,u) = (  -1,   1, -1)
5 : 1/4*(1.0-s)*(1.0-t)*(1.0-u*u)                                                   => (s,t,u) = (  -1,  -1,  0)
6 : 1/4*(1.0+s)*(1.0-t)*(1.0-u*u)                                                   => (s,t,u) = (   1,  -1,  0)
7 : 1/4*(1.0+s)*(1.0+t)*(1.0-u*u)                                                   => (s,t,u) = (   1,   1,  0)
8 : 1/4*(1.0-s)*(1.0+t)*(1.0-u*u)                                                   => (s,t,u) = (  -1,   1,  0)
9 : 1/8*(1.0-s*s)*(1.0-t)*(1.0-u)*(2.0+t+t*u)                                       => (s,t,u) = (   0,  -1, -1)
10: 1/8*(1.0+s)*(1.0-t*t)*(1.0-u)*(2.0-s-s*u)                                       => (s,t,u) = (   1,   0, -1)
11: 1/8*(1.0-s*s)*(1.0+t)*(1.0-u)*(2.0-t-t*u)                                       => (s,t,u) = (   0,   1, -1)
12: 1/8*(1.0-s)*(1.0-t*t)*(1.0-u)*(2.0+s+s*u)                                       => (s,t,u) = (  -1,   0, -1)

*********************************************************************************/
const int kmb::Pyramid2::nodeCount = 13;

const int kmb::Pyramid2::connectionTable[13][13] =
{
	{ 0, 1, 1, 1, 1, 2, 2, 2, 2, 0, 0, 0, 0},
	{ 1, 0, 1, 0, 1, 2, 0, 0, 0, 2, 0, 0, 2},
	{ 1, 1, 0, 1, 0, 0, 2, 0, 0, 2, 2, 0, 0},
	{ 1, 0, 1, 0, 1, 0, 0, 2, 0, 0, 2, 2, 0},
	{ 1, 1, 0, 1, 0, 0, 0, 0, 2, 0, 0, 2, 2},
	{ 2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},  // 5   [0,1]
	{ 2, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},  // 6   [0,2]
	{ 2, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0},  // 7   [0,3]
	{ 2, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0},  // 8   [0,4]
	{ 0, 2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},  // 9   [1,2]
	{ 0, 0, 2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0},  // 10  [2,3]
	{ 0, 0, 0, 2, 2, 0, 0, 0, 0, 0, 0, 0, 0},  // 11  [3,4]
	{ 0, 2, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0},  // 12  [4,1]
};

// 面を構成する多角形の添え字番号
// 外側から見て左回りが正
// 0 を天頂とし
// 1234 が四角形
// 面を構成する多角形の添え字番号
// 0 1 2
// 0 2 3
// 0 3 4
// 0 4 1
// 4 3 2 1
//
// Triangle2 Quad2 形式

const int kmb::Pyramid2::faceTable[5][8] =
{
	{ 0, 1, 2, 9, 6, 5,-1,-1},
	{ 0, 2, 3,10, 7, 6,-1,-1},
	{ 0, 3, 4,11, 8, 7,-1,-1},
	{ 0, 4, 1,12, 5, 8,-1,-1},
	{ 4, 3, 2, 1,11,10, 9,12}
};

const int kmb::Pyramid2::edgeTable[8][3] =
{
	{ 0, 1, 5},
	{ 0, 2, 6},
	{ 0, 3, 7},
	{ 0, 4, 8},
	{ 1, 2, 9},
	{ 2, 3, 10},
	{ 3, 4, 11},
	{ 1, 4, 12}
};

kmb::Pyramid2::Pyramid2(void)
: kmb::Element(kmb::PYRAMID2)
{
	cell = new kmb::nodeIdType[13];
}

kmb::Pyramid2::Pyramid2(kmb::nodeIdType* ary)
: kmb::Element(kmb::PYRAMID2)
{
	cell = ary;
}

kmb::Pyramid2::~Pyramid2(void)
{
}

kmb::nodeIdType kmb::Pyramid2::operator()(const int index,const int i) const
{
	return cell[kmb::Pyramid2::faceTable[index][i]];
}

kmb::nodeIdType& kmb::Pyramid2::operator()(const int index,const int i)
{
	return cell[kmb::Pyramid2::faceTable[index][i]];
}

void
kmb::Pyramid2::shapeFunction(double s,double t,double u,double* coeff)
{
	coeff[0]  = 0.5*u*(1.0+u);
	coeff[1]  = -(1.0-s)*(1.0-t)*(1.0-u)*(4.0+3.0*s+3.0*t+2.0*s*t+2.0*u+s*u+u*t+2.0*s*t*u)/16.0;
	coeff[2]  = -(1.0+s)*(1.0-t)*(1.0-u)*(4.0-3.0*s+3.0*t-2.0*s*t+2.0*u-s*u+u*t-2.0*s*t*u)/16.0;
	coeff[3]  = -(1.0+s)*(1.0+t)*(1.0-u)*(4.0-3.0*s-3.0*t+2.0*s*t+2.0*u-s*u-u*t+2.0*s*t*u)/16.0;
	coeff[4]  = -(1.0-s)*(1.0+t)*(1.0-u)*(4.0+3.0*s-3.0*t-2.0*s*t+2.0*u+s*u-u*t-2.0*s*t*u)/16.0;
	coeff[5]  = 0.25*(1.0-s)*(1.0-t)*(1.0-u*u);
	coeff[6]  = 0.25*(1.0+s)*(1.0-t)*(1.0-u*u);
	coeff[7]  = 0.25*(1.0+s)*(1.0+t)*(1.0-u*u);
	coeff[8]  = 0.25*(1.0-s)*(1.0+t)*(1.0-u*u);
	coeff[9]  = 0.125*(1.0-s*s)*(1.0-t)*(1.0-u)*(2.0+t+t*u);
	coeff[10] = 0.125*(1.0+s)*(1.0-t*t)*(1.0-u)*(2.0-s-s*u);
	coeff[11] = 0.125*(1.0-s*s)*(1.0+t)*(1.0-u)*(2.0-t-t*u);
	coeff[12] = 0.125*(1.0-s)*(1.0-t*t)*(1.0-u)*(2.0+s+s*u);
}

double
kmb::Pyramid2::checkShapeFunctionDomain(double s,double t,double u)
{
	kmb::Minimizer minimizer;
	minimizer.update( 1.0-s );
	minimizer.update( 1.0+s );
	minimizer.update( 1.0-t );
	minimizer.update( 1.0+t );
	minimizer.update( 1.0-u );
	minimizer.update( 1.0+u );
	return minimizer.getMin();
}
