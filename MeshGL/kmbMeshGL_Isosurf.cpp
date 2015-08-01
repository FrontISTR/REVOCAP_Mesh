/*----------------------------------------------------------------------
#                                                                      #
# Software Name : REVOCAP_PrePost version 1.6                          #
# Class Name : MeshGL                                                  #
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
#include "MeshGL/kmbMeshGL.h"
#include "MeshGL/kmbColorMap.h"

#include "MeshDB/kmbMeshData.h"
#include "MeshDB/kmbDataBindings.h"
#include "MeshDB/kmbTetrahedron.h"
#include "Geometry/kmbPlane.h"

#include <cmath>

void kmb::MeshGL::drawTriangleByDividingPoint
(kmb::Node* n00,kmb::Node* n01,double t00,double t01,
 kmb::Node* n10,kmb::Node* n11,double t10,double t11,
 kmb::Node* n20,kmb::Node* n21,double t20,double t21)
{
	double s0 = 1.0/(t01 - t00);
	double s1 = 1.0/(t11 - t10);
	double s2 = 1.0/(t21 - t20);
	drawTriangle(
		s0*t01*n00->x() - s0*t00*n01->x(),
		s0*t01*n00->y() - s0*t00*n01->y(),
		s0*t01*n00->z() - s0*t00*n01->z(),
		s1*t11*n10->x() - s1*t10*n11->x(),
		s1*t11*n10->y() - s1*t10*n11->y(),
		s1*t11*n10->z() - s1*t10*n11->z(),
		s2*t21*n20->x() - s2*t20*n21->x(),
		s2*t21*n20->y() - s2*t20*n21->y(),
		s2*t21*n20->z() - s2*t20*n21->z());
}

void kmb::MeshGL::drawQuadByDividingPoint
(kmb::Node* n00,kmb::Node* n01,double t00,double t01,
 kmb::Node* n10,kmb::Node* n11,double t10,double t11,
 kmb::Node* n20,kmb::Node* n21,double t20,double t21,
 kmb::Node* n30,kmb::Node* n31,double t30,double t31)
{
	double s0 = 1.0/(t01 - t00);
	double s1 = 1.0/(t11 - t10);
	double s2 = 1.0/(t21 - t20);
	double s3 = 1.0/(t31 - t30);
	drawQuad(
		s0*t01*n00->x() - s0*t00*n01->x(),
		s0*t01*n00->y() - s0*t00*n01->y(),
		s0*t01*n00->z() - s0*t00*n01->z(),
		s1*t11*n10->x() - s1*t10*n11->x(),
		s1*t11*n10->y() - s1*t10*n11->y(),
		s1*t11*n10->z() - s1*t10*n11->z(),
		s2*t21*n20->x() - s2*t20*n21->x(),
		s2*t21*n20->y() - s2*t20*n21->y(),
		s2*t21*n20->z() - s2*t20*n21->z(),
		s3*t31*n30->x() - s3*t30*n31->x(),
		s3*t31*n30->y() - s3*t30*n31->y(),
		s3*t31*n30->z() - s3*t30*n31->z());
}

// n0 , n1 è„Ç… t0, t1 Ç™Ç†ÇÈÇ∆Ç´Ç… 0 ÇÃì_Çó^Ç¶ÇÈ
//void glVertex3dAtZero(kmb::Node* n0,kmb::Node* n1,double t0,double t1){
//	double s = 1.0/(t1-t0);
//	::glVertex3d( 
//		s*t1*n0->x() - s*t0*n1->x(),
//		s*t1*n0->y() - s*t0*n1->y(),
//		s*t1*n0->z() - s*t0*n1->z() );
//}

void
kmb::MeshGL::drawIsosurfTetrahedron
(kmb::nodeIdType nodeId0, kmb::nodeIdType nodeId1, kmb::nodeIdType nodeId2, kmb::nodeIdType nodeId3,const kmb::DataBindings* isoData,double val,int comp)
{
	// no null check and no type check!!
	kmb::Node node[4];
	kmb::nodeIdType nodeId[4] = {nodeId0,nodeId1,nodeId2,nodeId3};
	double t[4] = {0.0, 0.0, 0.0, 0.0};

	for(int i=0;i<4;++i){
		if( !mesh->getNode( nodeId[i], node[i] ) ){
			return;
		}
	}

	if( isoData->getValueType() == kmb::PhysicalValue::Scalar ){
		for(int i=0;i<4;++i){
			if( isoData->getPhysicalValue( nodeId[i], &t[i] ) ){
				t[i] -= val;
			}else{
				return;
			}
		}
	}else if( isoData->getValueType() == kmb::PhysicalValue::Vector3 && comp == -1){
		double vec[3] = {0.0, 0.0, 0.0};
		for(int i=0;i<4;++i){
			if( isoData->getPhysicalValue( nodeId[i], vec ) ){
				t[i] = kmb::Vector3D::abs( vec ) - val;
			}else{
				return;
			}
		}
	}else if( isoData->getValueType() == kmb::PhysicalValue::Vector3 && 0 <= comp && comp < 3){
		double vec[3] = {0.0, 0.0, 0.0};
		for(int i=0;i<4;++i){
			if( isoData->getPhysicalValue( nodeId[i], vec ) ){
				t[i] = vec[comp] - val;
			}else{
				return;
			}
		}
	}else if( isoData->getValueType() == kmb::PhysicalValue::Tensor6 && 0 <= comp && comp < 6 ){
	  double ten[6] = {0.0, 0.0, 0.0,0.0, 0.0, 0.0};
		for(int i=0;i<4;++i){
			if( isoData->getPhysicalValue( nodeId[i], ten ) ){
				t[i] = ten[comp] - val;
			}else{
				return;
			}
		}
	}

	drawSectionTetrahedron( node[0], node[1], node[2], node[3], t[0], t[1], t[2], t[3] );
}

double
kmb::MeshGL::getMiddleNode
( kmb::Node* node0, kmb::Node* node1,
  double t0, double t1,
  double v0, double v1,
  kmb::Node &middle )
{
	double s = 1.0/(t1-t0);
	middle.setCoordinate(
		s*t1*node0->x() - s*t0*node1->x(),
		s*t1*node0->y() - s*t0*node1->y(),
		s*t1*node0->z() - s*t0*node1->z() );
	return s*t1*v0 - s*t0*v1;
}

void
kmb::MeshGL::getMiddleNodeVector
( kmb::Node& node0, kmb::Node& node1,
  double t0, double t1,
  double v0[3], double v1[3],
  kmb::Node &middle, double middleVec[3] )
{
	double s = 1.0/(t1-t0);
	middle.setCoordinate(
		s*t1*node0.x() - s*t0*node1.x(),
		s*t1*node0.y() - s*t0*node1.y(),
		s*t1*node0.z() - s*t0*node1.z() );
	middleVec[0] = s*t1*v0[0] - s*t0*v1[0];
	middleVec[1] = s*t1*v0[1] - s*t0*v1[1];
	middleVec[2] = s*t1*v0[2] - s*t0*v1[2];
}

void
kmb::MeshGL::drawIsosurfNodeContourTetrahedron
(kmb::nodeIdType nodeId0, kmb::nodeIdType nodeId1, kmb::nodeIdType nodeId2, kmb::nodeIdType nodeId3,
 const kmb::DataBindings* isoData, double iso,
 const kmb::DataBindings* data, kmb::ColorMap* colorMap)
{
	// no null check and no type check!!
	kmb::Node node[4];
	kmb::nodeIdType nodeId[4] = {nodeId0,nodeId1,nodeId2,nodeId3};
	double t[4] = {0.0, 0.0, 0.0, 0.0};
	double v[4] = {0.0, 0.0, 0.0, 0.0};

	for(int i=0;i<4;++i){
		if( !mesh->getNode( nodeId[i], node[i] ) ){
			return;
		}
	}

	if( isoData->getValueType() == kmb::PhysicalValue::Scalar ){
		for(int i=0;i<4;++i){
			if( isoData->getPhysicalValue( nodeId[i], &t[i] ) ){
				t[i] -= iso;
			}else{
				return;
			}
		}
	}else if( isoData->getValueType() == kmb::PhysicalValue::Vector3 ){
		double vec[3] = {0.0, 0.0, 0.0};
		for(int i=0;i<4;++i){
			if( isoData->getPhysicalValue( nodeId[i], vec ) ){
				t[i] = kmb::Vector3D::abs( vec ) - iso;
			}else{
				return;
			}
		}
	}

	if( data->getValueType() == kmb::PhysicalValue::Scalar ){
		for(int i=0;i<4;++i){
			if( !data->getPhysicalValue( nodeId[i], &v[i] ) ){
				return;
			}
		}
	}else if( data->getValueType() == kmb::PhysicalValue::Vector3 ){
		double vec[3] = {0.0, 0.0, 0.0};
		for(int i=0;i<4;++i){
			if( data->getPhysicalValue( nodeId[i], vec ) ){
				v[i] = kmb::Vector3D::abs( v );
			}else{
				return;
			}
		}
	}

	drawIsosurfTetrahedronContour( &node[0], &node[1], &node[2], &node[3],
		t[0], t[1], t[2], t[3],
		v[0], v[1], v[2], v[3], colorMap );
}

void
kmb::MeshGL::drawIsosurfTetrahedronContour
(kmb::Node* n0,kmb::Node* n1,kmb::Node* n2,kmb::Node* n3,
 double t0, double t1, double t2, double t3,
 double v0, double v1, double v2, double v3,
 kmb::ColorMap* colorMap)
{
//	std::cout << "_drawIsosurfTetrahedronContour" << std::endl;
	// signature check
	unsigned int flag = 0;
	if( t0 > 0 ) flag |= 0x01;
	if( t1 > 0 ) flag |= 0x02;
	if( t2 > 0 ) flag |= 0x04;
	if( t3 > 0 ) flag |= 0x08;
	if( flag == 0x00 || flag == 0x0f ){
		return;
	}
	double u0, u1, u2, u3, u4;
	switch( flag )
	{
//	std::cout << "_drawIsosurfTetrahedronContour " << flag << std::endl;
	// ÇPÇ¬ÇæÇØê≥
	case 0x01: // 0
		{
			// REMARK ORDER!!
			u0 = getMiddleNode( n0, n1, t0, t1, v0, v1, m0 );
			u1 = getMiddleNode( n0, n3, t0, t3, v0, v3, m1 );
			u2 = getMiddleNode( n0, n2, t0, t2, v0, v2, m2 );
			if( colorMap->getStep() == 0 ){
				drawContourTriangle(
					m0.x(), m0.y(), m0.z(), u0,
					m1.x(), m1.y(), m1.z(), u1,
					m2.x(), m2.y(), m2.z(), u2,
					colorMap );
			}else{
				drawSolidContourTriangle( &m0, &m1, &m2, u0, u1, u2, colorMap );
			}
		}
		break;
	case 0x02: // 1
		{
			// REMARK ORDER!!
			u0 = getMiddleNode( n1, n0, t1, t0, v1, v0, m0 );
			u1 = getMiddleNode( n1, n2, t1, t2, v1, v2, m1 );
			u2 = getMiddleNode( n1, n3, t1, t3, v1, v3, m2 );
			if( colorMap->getStep() == 0 ){
				drawContourTriangle(
					m0.x(), m0.y(), m0.z(), u0,
					m1.x(), m1.y(), m1.z(), u1,
					m2.x(), m2.y(), m2.z(), u2,
					colorMap );
			}else{
				drawSolidContourTriangle( &m0, &m1, &m2, u0, u1, u2, colorMap );
			}
		}
		break;
	case 0x04: // 2
		{
			// REMARK ORDER!!
			u0 = getMiddleNode( n2, n0, t2, t0, v2, v0, m0 );
			u1 = getMiddleNode( n2, n3, t2, t3, v2, v3, m1 );
			u2 = getMiddleNode( n2, n1, t2, t1, v2, v1, m2 );
			if( colorMap->getStep() == 0 ){
				drawContourTriangle(
					m0.x(), m0.y(), m0.z(), u0,
					m1.x(), m1.y(), m1.z(), u1,
					m2.x(), m2.y(), m2.z(), u2,
					colorMap );
			}else{
				drawSolidContourTriangle( &m0, &m1, &m2, u0, u1, u2, colorMap );
			}
		}
		break;
	case 0x08: // 3
		{
			// REMARK ORDER!!
			u0 = getMiddleNode( n3, n0, t3, t0, v3, v0, m0 );
			u1 = getMiddleNode( n3, n1, t3, t1, v3, v1, m1 );
			u2 = getMiddleNode( n3, n2, t3, t2, v3, v2, m2 );
			if( colorMap->getStep() == 0 ){
				drawContourTriangle(
					m0.x(), m0.y(), m0.z(), u0,
					m1.x(), m1.y(), m1.z(), u1,
					m2.x(), m2.y(), m2.z(), u2,
					colorMap );
			}else{
				drawSolidContourTriangle( &m0, &m1, &m2, u0, u1, u2, colorMap );
			}
		}
		break;
	// ÇQÇ¬ÇæÇØê≥
	case 0x03: // 0 1
		{
			// REMARK ORDER!!
			u0 = getMiddleNode( n0, n2, t0, t2, v0, v2, m0 );
			u1 = getMiddleNode( n1, n2, t1, t2, v1, v2, m1 );
			u2 = getMiddleNode( n1, n3, t1, t3, v1, v3, m2 );
			u3 = getMiddleNode( n0, n3, t0, t3, v0, v3, m3 );
			if( colorMap->getStep() == 0 ){
				drawContourQuad(
					m0.x(), m0.y(), m0.z(), u0,
					m1.x(), m1.y(), m1.z(), u1,
					m2.x(), m2.y(), m2.z(), u2,
					m3.x(), m3.y(), m3.z(), u3,
					colorMap );
			}else{
				m4.setCoordinate(
					0.25 * ( m0.x() + m1.x() + m2.x() + m3.x() ),
					0.25 * ( m0.y() + m1.y() + m2.y() + m3.y() ),
					0.25 * ( m0.z() + m1.z() + m2.z() + m3.z() ) );
				u4 = 0.25 * (u0+u1+u2+u3);
				drawSolidContourTriangle( &m0, &m1, &m4, u0, u1, u4, colorMap );
				drawSolidContourTriangle( &m1, &m2, &m4, u1, u2, u4, colorMap );
				drawSolidContourTriangle( &m2, &m3, &m4, u2, u3, u4, colorMap );
				drawSolidContourTriangle( &m3, &m0, &m4, u3, u0, u4, colorMap );
			}
		}
		break;
	case 0x05: // 0 2
		{
			// REMARK ORDER!!
			u0 = getMiddleNode( n0, n3, t0, t3, v0, v3, m0 );
			u1 = getMiddleNode( n2, n3, t2, t3, v2, v3, m1 );
			u2 = getMiddleNode( n2, n1, t2, t1, v2, v1, m2 );
			u3 = getMiddleNode( n0, n1, t0, t1, v0, v1, m3 );
			if( colorMap->getStep() == 0 ){
				drawContourQuad(
					m0.x(), m0.y(), m0.z(), u0,
					m1.x(), m1.y(), m1.z(), u1,
					m2.x(), m2.y(), m2.z(), u2,
					m3.x(), m3.y(), m3.z(), u3,
					colorMap );
			}else{
				m4.setCoordinate(
					0.25 * ( m0.x() + m1.x() + m2.x() + m3.x() ),
					0.25 * ( m0.y() + m1.y() + m2.y() + m3.y() ),
					0.25 * ( m0.z() + m1.z() + m2.z() + m3.z() ) );
				u4 = 0.25 * (u0+u1+u2+u3);
				drawSolidContourTriangle( &m0, &m1, &m4, u0, u1, u4, colorMap );
				drawSolidContourTriangle( &m1, &m2, &m4, u1, u2, u4, colorMap );
				drawSolidContourTriangle( &m2, &m3, &m4, u2, u3, u4, colorMap );
				drawSolidContourTriangle( &m3, &m0, &m4, u3, u0, u4, colorMap );
			}
		}
		break;
	case 0x06: // 1 2
		{
			// REMARK ORDER!!
			u0 = getMiddleNode( n1, n0, t1, t0, v1, v0, m0 );
			u1 = getMiddleNode( n2, n0, t2, t0, v2, v0, m1 );
			u2 = getMiddleNode( n2, n3, t2, t3, v2, v3, m2 );
			u3 = getMiddleNode( n1, n3, t1, t3, v1, v3, m3 );
			if( colorMap->getStep() == 0 ){
				drawContourQuad(
					m0.x(), m0.y(), m0.z(), u0,
					m1.x(), m1.y(), m1.z(), u1,
					m2.x(), m2.y(), m2.z(), u2,
					m3.x(), m3.y(), m3.z(), u3,
					colorMap );
			}else{
				m4.setCoordinate(
					0.25 * ( m0.x() + m1.x() + m2.x() + m3.x() ),
					0.25 * ( m0.y() + m1.y() + m2.y() + m3.y() ),
					0.25 * ( m0.z() + m1.z() + m2.z() + m3.z() ) );
				u4 = 0.25 * (u0+u1+u2+u3);
				drawSolidContourTriangle( &m0, &m1, &m4, u0, u1, u4, colorMap );
				drawSolidContourTriangle( &m1, &m2, &m4, u1, u2, u4, colorMap );
				drawSolidContourTriangle( &m2, &m3, &m4, u2, u3, u4, colorMap );
				drawSolidContourTriangle( &m3, &m0, &m4, u3, u0, u4, colorMap );
			}
		}
		break;
	case 0x09: // 0 3
		{
			// REMARK ORDER!!
			u0 = getMiddleNode( n0, n1, t0, t1, v0, v1, m0 );
			u1 = getMiddleNode( n3, n1, t3, t1, v3, v1, m1 );
			u2 = getMiddleNode( n3, n2, t3, t2, v3, v2, m2 );
			u3 = getMiddleNode( n0, n2, t0, t2, v0, v2, m3 );
			if( colorMap->getStep() == 0 ){
				drawContourQuad(
					m0.x(), m0.y(), m0.z(), u0,
					m1.x(), m1.y(), m1.z(), u1,
					m2.x(), m2.y(), m2.z(), u2,
					m3.x(), m3.y(), m3.z(), u3,
					colorMap );
			}else{
				m4.setCoordinate(
					0.25 * ( m0.x() + m1.x() + m2.x() + m3.x() ),
					0.25 * ( m0.y() + m1.y() + m2.y() + m3.y() ),
					0.25 * ( m0.z() + m1.z() + m2.z() + m3.z() ) );
				u4 = 0.25 * (u0+u1+u2+u3);
				drawSolidContourTriangle( &m0, &m1, &m4, u0, u1, u4, colorMap );
				drawSolidContourTriangle( &m1, &m2, &m4, u1, u2, u4, colorMap );
				drawSolidContourTriangle( &m2, &m3, &m4, u2, u3, u4, colorMap );
				drawSolidContourTriangle( &m3, &m0, &m4, u3, u0, u4, colorMap );
			}
		}
		break;
	case 0x0a: // 1 3
		{
			// REMARK ORDER!!
			u0 = getMiddleNode( n1, n2, t1, t2, v1, v2, m0 );
			u1 = getMiddleNode( n3, n2, t3, t2, v3, v2, m1 );
			u2 = getMiddleNode( n3, n0, t3, t0, v3, v0, m2 );
			u3 = getMiddleNode( n1, n0, t1, t0, v1, v0, m3 );
			if( colorMap->getStep() == 0 ){
				drawContourQuad(
					m0.x(), m0.y(), m0.z(), u0,
					m1.x(), m1.y(), m1.z(), u1,
					m2.x(), m2.y(), m2.z(), u2,
					m3.x(), m3.y(), m3.z(), u3,
					colorMap );
			}else{
				m4.setCoordinate(
					0.25 * ( m0.x() + m1.x() + m2.x() + m3.x() ),
					0.25 * ( m0.y() + m1.y() + m2.y() + m3.y() ),
					0.25 * ( m0.z() + m1.z() + m2.z() + m3.z() ) );
				u4 = 0.25 * (u0+u1+u2+u3);
				drawSolidContourTriangle( &m0, &m1, &m4, u0, u1, u4, colorMap );
				drawSolidContourTriangle( &m1, &m2, &m4, u1, u2, u4, colorMap );
				drawSolidContourTriangle( &m2, &m3, &m4, u2, u3, u4, colorMap );
				drawSolidContourTriangle( &m3, &m0, &m4, u3, u0, u4, colorMap );
			}
		}
		break;
	case 0x0c: // 2 3
		{
			// REMARK ORDER!!
			u0 = getMiddleNode( n2, n0, t2, t0, v2, v0, m0 );
			u1 = getMiddleNode( n3, n0, t3, t0, v3, v0, m1 );
			u2 = getMiddleNode( n3, n1, t3, t1, v3, v1, m2 );
			u3 = getMiddleNode( n2, n1, t2, t1, v2, v1, m3 );
			if( colorMap->getStep() == 0 ){
				drawContourQuad(
					m0.x(), m0.y(), m0.z(), u0,
					m1.x(), m1.y(), m1.z(), u1,
					m2.x(), m2.y(), m2.z(), u2,
					m3.x(), m3.y(), m3.z(), u3,
					colorMap );
			}else{
				m4.setCoordinate(
					0.25 * ( m0.x() + m1.x() + m2.x() + m3.x() ),
					0.25 * ( m0.y() + m1.y() + m2.y() + m3.y() ),
					0.25 * ( m0.z() + m1.z() + m2.z() + m3.z() ) );
				u4 = 0.25 * (u0+u1+u2+u3);
				drawSolidContourTriangle( &m0, &m1, &m4, u0, u1, u4, colorMap );
				drawSolidContourTriangle( &m1, &m2, &m4, u1, u2, u4, colorMap );
				drawSolidContourTriangle( &m2, &m3, &m4, u2, u3, u4, colorMap );
				drawSolidContourTriangle( &m3, &m0, &m4, u3, u0, u4, colorMap );
			}
		}
		break;
	// ÇRÇ¬ÇæÇØê≥
	case 0x07: // 0 1 2
		{
			// REMARK ORDER!!
			u0 = getMiddleNode( n3, n0, t3, t0, v3, v0, m0 );
			u1 = getMiddleNode( n3, n2, t3, t2, v3, v2, m1 );
			u2 = getMiddleNode( n3, n1, t3, t1, v3, v1, m2 );
			if( colorMap->getStep() == 0 ){
				drawContourTriangle(
					m0.x(), m0.y(), m0.z(), u0,
					m1.x(), m1.y(), m1.z(), u1,
					m2.x(), m2.y(), m2.z(), u2,
					colorMap );
			}else{
				drawSolidContourTriangle( &m0, &m1, &m2, u0, u1, u2, colorMap );
			}
		}
		break;
	case 0x0b: // 0 1 3
		{
			// REMARK ORDER!!
			u0 = getMiddleNode( n2, n0, t2, t0, v2, v0, m0 );
			u1 = getMiddleNode( n2, n1, t2, t1, v2, v1, m1 );
			u2 = getMiddleNode( n2, n3, t2, t3, v2, v3, m2 );
			if( colorMap->getStep() == 0 ){
				drawContourTriangle(
					m0.x(), m0.y(), m0.z(), u0,
					m1.x(), m1.y(), m1.z(), u1,
					m2.x(), m2.y(), m2.z(), u2,
					colorMap );
			}else{
				drawSolidContourTriangle( &m0, &m1, &m2, u0, u1, u2, colorMap );
			}
		}
		break;
	case 0x0d: // 0 2 3
		{
			// REMARK ORDER!!
			u0 = getMiddleNode( n1, n0, t1, t0, v1, v0, m0 );
			u1 = getMiddleNode( n1, n3, t1, t3, v1, v3, m1 );
			u2 = getMiddleNode( n1, n2, t1, t2, v1, v2, m2 );
			if( colorMap->getStep() == 0 ){
				drawContourTriangle(
					m0.x(), m0.y(), m0.z(), u0,
					m1.x(), m1.y(), m1.z(), u1,
					m2.x(), m2.y(), m2.z(), u2,
					colorMap );
			}else{
				drawSolidContourTriangle( &m0, &m1, &m2, u0, u1, u2, colorMap );
			}
		}
		break;
	case 0x0e: // 1 2 3
		{
			// REMARK ORDER!!
			u0 = getMiddleNode( n0, n1, t0, t1, v0, v1, m0 );
			u1 = getMiddleNode( n0, n2, t0, t2, v0, v2, m1 );
			u2 = getMiddleNode( n0, n3, t0, t3, v0, v3, m2 );
			if( colorMap->getStep() == 0 ){
				drawContourTriangle(
					m0.x(), m0.y(), m0.z(), u0,
					m1.x(), m1.y(), m1.z(), u1,
					m2.x(), m2.y(), m2.z(), u2,
					colorMap );
			}else{
				drawSolidContourTriangle( &m0, &m1, &m2, u0, u1, u2, colorMap );
			}
		}
		break;
	}
	::glFlush();
}

void
kmb::MeshGL::drawInterpolatedVector(kmb::Node& n0,kmb::Node& n1,double t0, double t1,double v0[3], double v1[3],double factor,kmb::ColorMap* colorMap)
{
	// signature check
	unsigned int flag = 0;
	if( t0 > 0 ) flag |= 0x01;
	if( t1 > 0 ) flag |= 0x02;
	if( flag == 0x00 || flag == 0x03 ){
		return;
	}
	kmb::Node m;
	double u[3]={0.0,0.0,0.0};
	getMiddleNodeVector( n0, n1, t0, t1, v0, v1, m, u );
	drawVector( m.x(), m.y(), m.z(), u[0], u[1], u[2], factor, colorMap );
	::glFlush();
}

void kmb::MeshGL::drawInterpolatedField(kmb::Node& n0,kmb::Node& n1,double t0, double t1,double v0[3], double v1[3],double arrowSize,kmb::ColorMap* colorMap)
{
	// signature check
	unsigned int flag = 0;
	if( t0 > 0 ) flag |= 0x01;
	if( t1 > 0 ) flag |= 0x02;
	if( flag == 0x00 || flag == 0x03 ){
		return;
	}
	kmb::Node m;
	double u[3]={0.0,0.0,0.0};
	getMiddleNodeVector( n0, n1, t0, t1, v0, v1, m, u );
	drawArrow( m.x(), m.y(), m.z(), u[0], u[1], u[2], arrowSize, colorMap );
	::glFlush();
}

void
kmb::MeshGL::drawIsosurface
(kmb::bodyIdType bodyId, const char* nodeValue, double val)
{
	const kmb::DataBindings* data = NULL;
	const kmb::Body* body = NULL;
	if( mesh != NULL &&
		(body = mesh->getBodyPtr(bodyId)) != NULL &&
		body->isUniqueDim(3) &&
		(data = mesh->getDataBindingsPtr( nodeValue )) != NULL &&
		data->getBindingMode() == kmb::DataBindings::NodeVariable )
	{
		::glPushAttrib( GL_ENABLE_BIT );
		::glEnable( GL_COLOR_MATERIAL );
		kmb::nodeIdType tetras[6][4];
		for(int i=0;i<6;++i){
			for(int j=0;j<4;++j){
				tetras[i][j] = kmb::nullNodeId;
			}
		}
		kmb::ElementContainer::const_iterator eIter = body->begin();
		while( !eIter.isFinished() )
		{
			switch( eIter.getType() )
			{
			case kmb::TETRAHEDRON:
			case kmb::TETRAHEDRON2:
				{
					drawIsosurfTetrahedron(
						eIter.getCellId(0), eIter.getCellId(1), eIter.getCellId(2), eIter.getCellId(3),
						data, val );
				}
				break;
			default:
				{
					int len = eIter.divideIntoTetrahedrons( tetras );
					for(int i=0;i<len;++i){
						drawIsosurfTetrahedron(
							tetras[i][0], tetras[i][1], tetras[i][2], tetras[i][3],
							data, val );
					}
				}
				break;
			}
			++eIter;
		}
		::glPopAttrib();
		::glFlush();
	}
}

void kmb::MeshGL::drawSection(kmb::bodyIdType bodyId,kmb::Plane* plane)
{
	const kmb::Body* body = NULL;
	if( mesh != NULL &&
		(body = mesh->getBodyPtr(bodyId)) != NULL &&
		body->isUniqueDim(3) && plane != NULL )
	{
		kmb::Node n0,n1,n2,n3;
		kmb::nodeIdType tetras[6][4];
		for(int i=0;i<6;++i){
			for(int j=0;j<4;++j){
				tetras[i][j] = kmb::nullNodeId;
			}
		}
		kmb::ElementContainer::const_iterator eIter = body->begin();
		while( !eIter.isFinished() )
		{
			switch( eIter.getType() )
			{
			case kmb::TETRAHEDRON:
			case kmb::TETRAHEDRON2:
				{
					if( mesh->getNode( eIter.getCellId(0), n0 ) &&
						mesh->getNode( eIter.getCellId(1), n1 ) &&
						mesh->getNode( eIter.getCellId(2), n2 ) &&
						mesh->getNode( eIter.getCellId(3), n3 ) )
					{
						drawSectionTetrahedron( n0, n1, n2, n3,
							plane->evaluate(n0),
							plane->evaluate(n1),
							plane->evaluate(n2),
							plane->evaluate(n3) );
					}
				}
				break;
			default:
				{
					int len = eIter.divideIntoTetrahedrons( tetras );
					for(int i=0;i<len;++i){
						if( mesh->getNode( tetras[i][0], n0 ) &&
							mesh->getNode( tetras[i][1], n1 ) &&
							mesh->getNode( tetras[i][2], n2 ) &&
							mesh->getNode( tetras[i][3], n3 ) )
						{
							drawSectionTetrahedron( n0, n1, n2, n3,
								plane->evaluate(n0),
								plane->evaluate(n1),
								plane->evaluate(n2),
								plane->evaluate(n3) );
						}
					}
				}
				break;
			}
			++eIter;
		}
		::glFlush();
	}
}

void
kmb::MeshGL::drawSectionNodeContour
(kmb::bodyIdType bodyId,kmb::Plane* plane,const char* physicalValue, kmb::ColorMap* colorMap, int comp )
{
	const kmb::DataBindings* data = NULL;
	const kmb::Body* body = NULL;
	if( mesh != NULL &&
		(body = mesh->getBodyPtr(bodyId)) != NULL &&
		body->isUniqueDim(3) && plane != NULL && colorMap != NULL &&
		(data = mesh->getDataBindingsPtr( physicalValue )) != NULL &&
		data->getBindingMode() == kmb::DataBindings::NodeVariable )
	{
		::glPushAttrib( GL_ENABLE_BIT );
		::glEnable( GL_COLOR_MATERIAL );
		kmb::nodeIdType tetras[6][4];
		for(int i=0;i<6;++i){
			for(int j=0;j<4;++j){
				tetras[i][j] = kmb::nullNodeId;
			}
		}
		kmb::Node n0,n1,n2,n3;
		if( data->getValueType() == kmb::PhysicalValue::Scalar ){
			double v0=0.0,v1=0.0,v2=0.0,v3=0.0;
			kmb::ElementContainer::const_iterator eIter = body->begin();
			while( !eIter.isFinished() )
			{
				switch( eIter.getType() )
				{
				case kmb::TETRAHEDRON:
				case kmb::TETRAHEDRON2:
					{
						if( mesh->getNode( eIter.getCellId(0), n0 )
							&& mesh->getNode( eIter.getCellId(1), n1 )
							&& mesh->getNode( eIter.getCellId(2), n2 )
							&& mesh->getNode( eIter.getCellId(3), n3 )
							&& data->getPhysicalValue( eIter.getCellId(0), &v0 )
							&& data->getPhysicalValue( eIter.getCellId(1), &v1 )
							&& data->getPhysicalValue( eIter.getCellId(2), &v2 )
							&& data->getPhysicalValue( eIter.getCellId(3), &v3 ) )
						{
							drawIsosurfTetrahedronContour( &n0, &n1, &n2, &n3,
								plane->evaluate(n0),
								plane->evaluate(n1),
								plane->evaluate(n2),
								plane->evaluate(n3),
								v0, v1, v2, v3, colorMap );
						}
					}
					break;
				default:
					{
						int len = eIter.divideIntoTetrahedrons( tetras );
						for(int i=0;i<len;++i){
							if( mesh->getNode( tetras[i][0], n0 )
								&& mesh->getNode( tetras[i][1], n1 )
								&& mesh->getNode( tetras[i][2], n2 )
								&& mesh->getNode( tetras[i][3], n3 )
								&& data->getPhysicalValue( tetras[i][0], &v0 )
								&& data->getPhysicalValue( tetras[i][1], &v1 )
								&& data->getPhysicalValue( tetras[i][2], &v2 )
								&& data->getPhysicalValue( tetras[i][3], &v3 ) )
							{
								drawIsosurfTetrahedronContour( &n0, &n1, &n2, &n3,
									plane->evaluate(n0),
									plane->evaluate(n1),
									plane->evaluate(n2),
									plane->evaluate(n3),
									v0, v1, v2, v3, colorMap );
							}
						}
					}
					break;
				}
				++eIter;
			}
			::glFlush();
		}else if( data->getValueType() == kmb::PhysicalValue::Vector3 && comp == -1 ){
			double v0[3]={0.0,0.0,0.0},v1[3]={0.0,0.0,0.0},v2[3]={0.0,0.0,0.0},v3[3]={0.0,0.0,0.0};
			kmb::ElementContainer::const_iterator eIter = body->begin();
			while( !eIter.isFinished() )
			{
				switch( eIter.getType() )
				{
				case kmb::TETRAHEDRON:
				case kmb::TETRAHEDRON2:
					{
						if( mesh->getNode( eIter.getCellId(0), n0 )
							&& mesh->getNode( eIter.getCellId(1), n1 )
							&& mesh->getNode( eIter.getCellId(2), n2 )
							&& mesh->getNode( eIter.getCellId(3), n3 )
							&& data->getPhysicalValue( eIter.getCellId(0), v0 )
							&& data->getPhysicalValue( eIter.getCellId(1), v1 )
							&& data->getPhysicalValue( eIter.getCellId(2), v2 )
							&& data->getPhysicalValue( eIter.getCellId(3), v3 ) )
						{
							drawIsosurfTetrahedronContour( &n0, &n1, &n2, &n3,
								plane->evaluate(n0),
								plane->evaluate(n1),
								plane->evaluate(n2),
								plane->evaluate(n3),
								kmb::Vector3D::abs(v0),
								kmb::Vector3D::abs(v1),
								kmb::Vector3D::abs(v2),
								kmb::Vector3D::abs(v3),
								colorMap );
						}
					}
					break;
				default:
					{
						int len = eIter.divideIntoTetrahedrons( tetras );
						for(int i=0;i<len;++i){
							if( mesh->getNode( tetras[i][0], n0 )
								&& mesh->getNode( tetras[i][1], n1 )
								&& mesh->getNode( tetras[i][2], n2 )
								&& mesh->getNode( tetras[i][3], n3 )
								&& data->getPhysicalValue( tetras[i][0], v0 )
								&& data->getPhysicalValue( tetras[i][1], v1 )
								&& data->getPhysicalValue( tetras[i][2], v2 )
								&& data->getPhysicalValue( tetras[i][3], v3 ) )
							{
								drawIsosurfTetrahedronContour( &n0, &n1, &n2, &n3,
									plane->evaluate(n0),
									plane->evaluate(n1),
									plane->evaluate(n2),
									plane->evaluate(n3),
									kmb::Vector3D::abs(v0),
									kmb::Vector3D::abs(v1),
									kmb::Vector3D::abs(v2),
									kmb::Vector3D::abs(v3),
									colorMap );
							}
						}
					}
					break;
				}
				++eIter;
			}
			::glFlush();
		}else if( data->getValueType() == kmb::PhysicalValue::Vector3 && 0 <= comp && comp < 3 ){
			double v0[3]={0.0,0.0,0.0},v1[3]={0.0,0.0,0.0},v2[3]={0.0,0.0,0.0},v3[3]={0.0,0.0,0.0};
			kmb::ElementContainer::const_iterator eIter = body->begin();
			while( !eIter.isFinished() )
			{
				switch( eIter.getType() )
				{
				case kmb::TETRAHEDRON:
				case kmb::TETRAHEDRON2:
					{
						if( mesh->getNode( eIter.getCellId(0), n0 )
							&& mesh->getNode( eIter.getCellId(1), n1 )
							&& mesh->getNode( eIter.getCellId(2), n2 )
							&& mesh->getNode( eIter.getCellId(3), n3 )
							&& data->getPhysicalValue( eIter.getCellId(0), v0 )
							&& data->getPhysicalValue( eIter.getCellId(1), v1 )
							&& data->getPhysicalValue( eIter.getCellId(2), v2 )
							&& data->getPhysicalValue( eIter.getCellId(3), v3 ) )
						{
							drawIsosurfTetrahedronContour( &n0, &n1, &n2, &n3,
								plane->evaluate(n0),
								plane->evaluate(n1),
								plane->evaluate(n2),
								plane->evaluate(n3),
								v0[comp],
								v1[comp],
								v2[comp],
								v3[comp],
								colorMap );
						}
					}
					break;
				default:
					{
						int len = eIter.divideIntoTetrahedrons( tetras );
						for(int i=0;i<len;++i){
							if( mesh->getNode( tetras[i][0], n0 )
								&& mesh->getNode( tetras[i][1], n1 )
								&& mesh->getNode( tetras[i][2], n2 )
								&& mesh->getNode( tetras[i][3], n3 )
								&& data->getPhysicalValue( tetras[i][0], v0 )
								&& data->getPhysicalValue( tetras[i][1], v1 )
								&& data->getPhysicalValue( tetras[i][2], v2 )
								&& data->getPhysicalValue( tetras[i][3], v3 ) )
							{
								drawIsosurfTetrahedronContour( &n0, &n1, &n2, &n3,
									plane->evaluate(n0),
									plane->evaluate(n1),
									plane->evaluate(n2),
									plane->evaluate(n3),
									v0[comp],
									v1[comp],
									v2[comp],
									v3[comp],
									colorMap );
							}
						}
					}
					break;
				}
				++eIter;
			}
			::glFlush();
		}else if( data->getValueType() == kmb::PhysicalValue::Tensor6 && 0 <= comp && comp < 6 ){
			double v0[6]={0.0,0.0,0.0,0.0,0.0,0.0},v1[6]={0.0,0.0,0.0,0.0,0.0,0.0},v2[6]={0.0,0.0,0.0,0.0,0.0,0.0},v3[6]={0.0,0.0,0.0,0.0,0.0,0.0};
			kmb::ElementContainer::const_iterator eIter = body->begin();
			while( !eIter.isFinished() )
			{
				switch( eIter.getType() )
				{
				case kmb::TETRAHEDRON:
				case kmb::TETRAHEDRON2:
					{
						if( mesh->getNode( eIter.getCellId(0), n0 )
							&& mesh->getNode( eIter.getCellId(1), n1 )
							&& mesh->getNode( eIter.getCellId(2), n2 )
							&& mesh->getNode( eIter.getCellId(3), n3 )
							&& data->getPhysicalValue( eIter.getCellId(0), v0 )
							&& data->getPhysicalValue( eIter.getCellId(1), v1 )
							&& data->getPhysicalValue( eIter.getCellId(2), v2 )
							&& data->getPhysicalValue( eIter.getCellId(3), v3 ) )
						{
							drawIsosurfTetrahedronContour( &n0, &n1, &n2, &n3,
								plane->evaluate(n0),
								plane->evaluate(n1),
								plane->evaluate(n2),
								plane->evaluate(n3),
								v0[comp],
								v1[comp],
								v2[comp],
								v3[comp],
								colorMap );
						}
					}
					break;
				default:
					{
						int len = eIter.divideIntoTetrahedrons( tetras );
						for(int i=0;i<len;++i){
							if( mesh->getNode( tetras[i][0], n0 )
								&& mesh->getNode( tetras[i][1], n1 )
								&& mesh->getNode( tetras[i][2], n2 )
								&& mesh->getNode( tetras[i][3], n3 )
								&& data->getPhysicalValue( tetras[i][0], v0 )
								&& data->getPhysicalValue( tetras[i][1], v1 )
								&& data->getPhysicalValue( tetras[i][2], v2 )
								&& data->getPhysicalValue( tetras[i][3], v3 ) )
							{
								drawIsosurfTetrahedronContour( &n0, &n1, &n2, &n3,
									plane->evaluate(n0),
									plane->evaluate(n1),
									plane->evaluate(n2),
									plane->evaluate(n3),
									v0[comp],
									v1[comp],
									v2[comp],
									v3[comp],
									colorMap );
							}
						}
					}
					break;
				}
				++eIter;
			}
		}
		::glPopAttrib();
		::glFlush();
	}
}

void kmb::MeshGL::drawSectionContour(kmb::bodyIdType bodyId,kmb::Plane* plane,const char* physicalValue, kmb::ColorMap* colorMap, int comp )
{
	const kmb::DataBindings* data = NULL;
	const kmb::Body* body = NULL;
	if( mesh != NULL &&
		(body = mesh->getBodyPtr(bodyId)) != NULL &&
		body->isUniqueDim(3) && plane != NULL && colorMap != NULL &&
		(data = mesh->getDataBindingsPtr( physicalValue )) != NULL &&
		data->getBindingMode() == kmb::DataBindings::ElementVariable )
	{
		::glPushAttrib( GL_ENABLE_BIT );
		::glEnable( GL_COLOR_MATERIAL );
		kmb::nodeIdType tetras[6][4];
		for(int i=0;i<6;++i){
			for(int j=0;j<4;++j){
				tetras[i][j] = kmb::nullNodeId;
			}
		}
		if( data->getValueType() == kmb::PhysicalValue::Scalar ){
			kmb::Node n0, n1, n2, n3;
			double v = 0.0;
			kmb::ElementContainer::const_iterator eIter = body->begin();
			while( eIter != body->end() )
			{
				if( data->getPhysicalValue( eIter.getId(), &v ) ){
					switch( eIter.getType() )
					{
					case kmb::TETRAHEDRON:
					case kmb::TETRAHEDRON2:
						{
							if( mesh->getNode( eIter.getCellId(0), n0 ) &&
								mesh->getNode( eIter.getCellId(1), n1 ) &&
								mesh->getNode( eIter.getCellId(2), n2 ) &&
								mesh->getNode( eIter.getCellId(3), n3 ) )
							{
								colorMap->setGLColor( v );
								drawSectionTetrahedron(
									n0, n1, n2, n3,
									plane->evaluate(n0), plane->evaluate(n1), plane->evaluate(n2), plane->evaluate(n3) );
							}
						}
						break;
					default:
						{
							int len = eIter.divideIntoTetrahedrons( tetras );
							for(int i=0;i<len;++i){
								if( mesh->getNode( tetras[i][0], n0 )
									&& mesh->getNode( tetras[i][1], n1 )
									&& mesh->getNode( tetras[i][2], n2 )
									&& mesh->getNode( tetras[i][3], n3 ) )
								{
									colorMap->setGLColor( v );
									drawSectionTetrahedron(
										n0, n1, n2, n3,
										plane->evaluate(n0), plane->evaluate(n1), plane->evaluate(n2), plane->evaluate(n3) );
								}
							}
						}
						break;
					}
				}
				++eIter;
			}
		}else if( data->getValueType() == kmb::PhysicalValue::Vector3 && comp == -1 ){
			kmb::Node n0, n1, n2, n3;
			double v[3] = {0.0,0.0,0.0};
			kmb::ElementContainer::const_iterator eIter = body->begin();
			while( !eIter.isFinished() )
			{
				if( data->getPhysicalValue( eIter.getId(), v ) ){
					switch( eIter.getType() )
					{
					case kmb::TETRAHEDRON:
					case kmb::TETRAHEDRON2:
						{
							if( mesh->getNode( eIter.getCellId(0), n0 ) &&
								mesh->getNode( eIter.getCellId(1), n1 ) &&
								mesh->getNode( eIter.getCellId(2), n2 ) &&
								mesh->getNode( eIter.getCellId(3), n3 ) )
							{
								colorMap->setGLColor( kmb::Vector3D::abs(v) );
								drawSectionTetrahedron(
									n0, n1, n2, n3,
									plane->evaluate(n0), plane->evaluate(n1), plane->evaluate(n2), plane->evaluate(n3) );
							}
						}
						break;
					default:
						{
							int len = eIter.divideIntoTetrahedrons( tetras );
							for(int i=0;i<len;++i){
								if( mesh->getNode( tetras[i][0], n0 )
									&& mesh->getNode( tetras[i][1], n1 )
									&& mesh->getNode( tetras[i][2], n2 )
									&& mesh->getNode( tetras[i][3], n3 ) )
								{
									colorMap->setGLColor( kmb::Vector3D::abs(v) );
									drawSectionTetrahedron(
										n0, n1, n2, n3,
										plane->evaluate(n0), plane->evaluate(n1), plane->evaluate(n2), plane->evaluate(n3) );
								}
							}
						}
						break;
					}
				}
				++eIter;
			}
		}else if( data->getValueType() == kmb::PhysicalValue::Vector3 && 0 <= comp && comp < 3 ){
			kmb::Node n0, n1, n2, n3;
			double v[3] = {0.0,0.0,0.0};
			kmb::ElementContainer::const_iterator eIter = body->begin();
			while( !eIter.isFinished() )
			{
				if( data->getPhysicalValue( eIter.getId(), v ) ){
					switch( eIter.getType() )
					{
					case kmb::TETRAHEDRON:
					case kmb::TETRAHEDRON2:
						{
							if( mesh->getNode( eIter.getCellId(0), n0 ) &&
								mesh->getNode( eIter.getCellId(1), n1 ) &&
								mesh->getNode( eIter.getCellId(2), n2 ) &&
								mesh->getNode( eIter.getCellId(3), n3 ) )
							{
								colorMap->setGLColor( v[comp] );
								drawSectionTetrahedron(
									n0, n1, n2, n3,
									plane->evaluate(n0), plane->evaluate(n1), plane->evaluate(n2), plane->evaluate(n3) );
							}
						}
						break;
					default:
						{
							int len = eIter.divideIntoTetrahedrons( tetras );
							for(int i=0;i<len;++i){
								if( mesh->getNode( tetras[i][0], n0 )
									&& mesh->getNode( tetras[i][1], n1 )
									&& mesh->getNode( tetras[i][2], n2 )
									&& mesh->getNode( tetras[i][3], n3 ) )
								{
									colorMap->setGLColor( v[comp] );
									drawSectionTetrahedron(
										n0, n1, n2, n3,
										plane->evaluate(n0), plane->evaluate(n1), plane->evaluate(n2), plane->evaluate(n3) );
								}
							}
						}
						break;
					}
				}
				++eIter;
			}
		}else if( data->getValueType() == kmb::PhysicalValue::Tensor6 && 0 <= comp && comp < 6 ){
			kmb::Node n0, n1, n2, n3;
			double v[6] = {0.0,0.0,0.0,0.0,0.0,0.0};
			kmb::ElementContainer::const_iterator eIter = body->begin();
			while( !eIter.isFinished() )
			{
				if( data->getPhysicalValue( eIter.getId(), v ) ){
					switch( eIter.getType() )
					{
					case kmb::TETRAHEDRON:
					case kmb::TETRAHEDRON2:
						{
							if( mesh->getNode( eIter.getCellId(0), n0 ) &&
								mesh->getNode( eIter.getCellId(1), n1 ) &&
								mesh->getNode( eIter.getCellId(2), n2 ) &&
								mesh->getNode( eIter.getCellId(3), n3 ) )
							{
								colorMap->setGLColor( v[comp] );
								drawSectionTetrahedron(
									n0, n1, n2, n3,
									plane->evaluate(n0), plane->evaluate(n1), plane->evaluate(n2), plane->evaluate(n3) );
							}
						}
						break;
					default:
						{
							int len = eIter.divideIntoTetrahedrons( tetras );
							for(int i=0;i<len;++i){
								if( mesh->getNode( tetras[i][0], n0 )
									&& mesh->getNode( tetras[i][1], n1 )
									&& mesh->getNode( tetras[i][2], n2 )
									&& mesh->getNode( tetras[i][3], n3 ) )
								{
									colorMap->setGLColor( v[comp] );
									drawSectionTetrahedron(
										n0, n1, n2, n3,
										plane->evaluate(n0), plane->evaluate(n1), plane->evaluate(n2), plane->evaluate(n3) );
								}
							}
						}
						break;
					}
				}
				++eIter;
			}
		}
		::glPopAttrib();
		::glFlush();
	}
}

void
kmb::MeshGL::drawSectionVector(kmb::bodyIdType bodyId,kmb::Plane* plane,const char* vectorName, double factor, kmb::ColorMap* colorMap)
{
	const kmb::DataBindings* data = NULL;
	const kmb::Body* body = NULL;
	if( mesh != NULL &&
		(body = mesh->getBodyPtr(bodyId)) != NULL &&
		body->isUniqueDim(3) && plane != NULL && colorMap != NULL &&
		(data = mesh->getDataBindingsPtr( vectorName )) != NULL &&
		data->getBindingMode() == kmb::DataBindings::NodeVariable &&
		data->getValueType() == kmb::PhysicalValue::Vector3 )
	{
		::glPushAttrib( GL_ENABLE_BIT );
		::glEnable( GL_COLOR_MATERIAL );
		// èdï°ÇñhÇÆÇΩÇﬂÇÃÉLÉÉÉbÉVÉÖ
		// è¨Ç≥Ç¢ï˚Ç™ëÊÇPê¨ï™
		std::set< std::pair<kmb::nodeIdType,kmb::nodeIdType> > nodePairs;
		kmb::Node n0, n1;
		double v0[3], v1[3];
		kmb::ElementContainer::const_iterator eIter = body->begin();
		while( !eIter.isFinished() )
		{
			int edgeCount = eIter.getEdgeCount();
			for(int i =0;i< edgeCount;++i){
				kmb::nodeIdType nodeId0 = eIter.getEdgeCellId(i,0);
				kmb::nodeIdType nodeId1 = eIter.getEdgeCellId(i,1);
				if( nodeId0 > nodeId1 ){
					kmb::nodeIdType tmp = nodeId0;
					nodeId0 = nodeId1;
					nodeId1 = tmp;
				}
				std::pair<kmb::nodeIdType,kmb::nodeIdType> pair = std::pair<kmb::nodeIdType,kmb::nodeIdType>(nodeId0,nodeId1);
				if( nodePairs.find( pair ) == nodePairs.end() ){
					if( mesh->getNode( nodeId0, n0 ) &&
						mesh->getNode( nodeId1, n1 ) &&
						data->getPhysicalValue( nodeId0, v0 ) &&
						data->getPhysicalValue( nodeId1, v1 ) )
					{
						nodePairs.insert( pair );
						drawInterpolatedVector(n0,n1,plane->evaluate(n0),plane->evaluate(n1),v0,v1,factor,colorMap);
					}
				}
			}
			++eIter;
		}
		::glPopAttrib();
		::glFlush();
	}
}

void kmb::MeshGL::drawSectionField(kmb::bodyIdType bodyId,kmb::Plane* plane,const char* vectorName, double arrowSize, kmb::ColorMap* colorMap)
{
	const kmb::DataBindings* data = NULL;
	const kmb::Body* body = NULL;
	if( mesh != NULL &&
		(body = mesh->getBodyPtr(bodyId)) != NULL &&
		body->isUniqueDim(3) && plane != NULL && colorMap != NULL &&
		(data = mesh->getDataBindingsPtr( vectorName )) != NULL &&
		data->getBindingMode() == kmb::DataBindings::NodeVariable &&
		data->getValueType() == kmb::PhysicalValue::Vector3 )
	{
		::glPushAttrib( GL_ENABLE_BIT );
		::glEnable( GL_COLOR_MATERIAL );
		// èdï°ÇñhÇÆÇΩÇﬂÇÃÉLÉÉÉbÉVÉÖ
		// è¨Ç≥Ç¢ï˚Ç™ëÊÇPê¨ï™
		std::set< std::pair<kmb::nodeIdType,kmb::nodeIdType> > nodePairs;
		kmb::Node n0, n1;
		double v0[3], v1[3];
		kmb::ElementContainer::const_iterator eIter = body->begin();
		while( !eIter.isFinished() )
		{
			int edgeCount = eIter.getEdgeCount();
			for(int i =0;i< edgeCount;++i){
				kmb::nodeIdType nodeId0 = eIter.getEdgeCellId(i,0);
				kmb::nodeIdType nodeId1 = eIter.getEdgeCellId(i,1);
				if( nodeId0 > nodeId1 ){
					kmb::nodeIdType tmp = nodeId0;
					nodeId0 = nodeId1;
					nodeId1 = tmp;
				}
				std::pair<kmb::nodeIdType,kmb::nodeIdType> pair = std::pair<kmb::nodeIdType,kmb::nodeIdType>(nodeId0,nodeId1);
				if( nodePairs.find( pair ) == nodePairs.end() ){
					if( mesh->getNode( nodeId0, n0 ) &&
						mesh->getNode( nodeId1, n1 ) &&
						data->getPhysicalValue( nodeId0, v0 ) &&
						data->getPhysicalValue( nodeId1, v1 ) )
					{
						nodePairs.insert( pair );
						drawInterpolatedField(n0,n1,plane->evaluate(n0),plane->evaluate(n1),v0,v1,arrowSize,colorMap);
					}
				}
			}
			++eIter;
		}
		::glPopAttrib();
		::glFlush();
	}
}
