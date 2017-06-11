﻿/*----------------------------------------------------------------------
#                                                                      #
# Software Name : REVOCAP_PrePost version 1.6                          #
# Class Name : BezierCurve3D                                           #
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

#include "Shape/kmbBezierCurve3D.h"
#include "Shape/kmbBernstein.h"

kmb::BezierCurve3D::BezierCurve3D(void)
: kmb::Curve3D()
{
}

kmb::BezierCurve3D::~BezierCurve3D(void)
{
}

void
kmb::BezierCurve3D::clear(void)
{
	ctrlPts.clear();
}

bool
kmb::BezierCurve3D::isDomain( double t ) const
{
	return 0.0 <= t && t <= 1.0;
}

void
kmb::BezierCurve3D::getDomain( double &min_t, double &max_t ) const
{
	min_t = 0.0;
	max_t = 1.0;
}

int
kmb::BezierCurve3D::getOrder(void) const
{
	return static_cast<int>( ctrlPts.size() );
}

int
kmb::BezierCurve3D::getDegree(void) const
{
	return static_cast<int>( ctrlPts.size() ) - 1;
}

void
kmb::BezierCurve3D::appendCtrlPt(double x,double y,double z)
{
	this->ctrlPts.push_back( kmb::Point3D(x,y,z) );
}

bool
kmb::BezierCurve3D::getCtrlPt(int i,kmb::Point3D &pt) const
{
	pt.set( ctrlPts[i] );
	return true;
}

bool
kmb::BezierCurve3D::getPoint( double t, kmb::Point3D& point ) const
{
	point.zero();
	int order = static_cast<int>( ctrlPts.size() );
	for(int i=0;i<order;++i){
		double u = kmb::Bernstein::getValue(order-1,i,t);
		point.addCoordinate( u*ctrlPts[i].x(), u*ctrlPts[i].y(), u*ctrlPts[i].z() );
	}
	return true;
}

bool
kmb::BezierCurve3D::getDerivative( double t, kmb::Vector3D& tangent ) const
{
	tangent.zero();
	int order = static_cast<int>( ctrlPts.size() );
	for(int i=0;i<order-1;++i){
		double u = kmb::Bernstein::getValue(order-2,i,t) * (order-1);
		tangent.addCoordinate(
			u*(ctrlPts[i+1].x()-ctrlPts[i].x()),
			u*(ctrlPts[i+1].y()-ctrlPts[i].y()),
			u*(ctrlPts[i+1].z()-ctrlPts[i].z()) );
	}
	return true;
}

bool
kmb::BezierCurve3D::getSecondDerivative( double t, kmb::Vector3D& tangent ) const
{
	tangent.zero();
	int order = static_cast<int>( ctrlPts.size() );
	for(int i=0;i<order-2;++i){
		double u = kmb::Bernstein::getValue(order-3,i,t) * (order-1) * (order-2);
		tangent.addCoordinate(
			u*(ctrlPts[i+2].x()-2.0*ctrlPts[i+1].x()+ctrlPts[i].x()),
			u*(ctrlPts[i+2].y()-2.0*ctrlPts[i+1].y()+ctrlPts[i].y()),
			u*(ctrlPts[i+2].z()-2.0*ctrlPts[i+1].z()+ctrlPts[i].z()) );
	}
	return true;
}
