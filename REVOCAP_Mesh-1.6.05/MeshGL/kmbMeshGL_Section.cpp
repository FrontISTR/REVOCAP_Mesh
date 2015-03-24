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
----------------------------------------------------------------------*/
#include "MeshGL/kmbMeshGL.h"

#include "MeshDB/kmbMeshData.h"
#include "MeshDB/kmbDataBindings.h"
#include "MeshDB/kmbTetrahedron.h"
#include "Geometry/kmbPlane.h"
#include "MeshGL/kmbColorMap.h"

#include <cmath>



void
kmb::MeshGL::drawIntersection(kmb::bodyIdType bodyId,kmb::Plane* plane)
{
	const kmb::Body* body = NULL;
	if( mesh != NULL &&
		(body = mesh->getBodyPtr(bodyId)) != NULL &&
		body->isUniqueDim(3) && plane != NULL )
	{
		kmb::Node n0, n1, n2, n3;
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
						drawIntersectionTetrahedron(
							n0, n1, n2, n3,
							plane->evaluate(n0), plane->evaluate(n1), plane->evaluate(n2), plane->evaluate(n3) );
					}
				}
				break;
			default:
				break;
			}
			++eIter;
		}
		::glFlush();
	}
}


void
kmb::MeshGL::drawIntersectionContour(kmb::bodyIdType bodyId,kmb::Plane* plane,const char* physicalValue,kmb::ColorMap* colorMap,int comp)
{
	const kmb::Body* body = NULL;
	const kmb::DataBindings* data = NULL;
	if( mesh != NULL && colorMap != NULL &&
		(body = mesh->getBodyPtr(bodyId)) != NULL &&
		body->isUniqueDim(3) && plane != NULL &&
		(data = mesh->getDataBindingsPtr( physicalValue )) != NULL &&
		data->getBindingMode() == kmb::DataBindings::ElementVariable )
	{
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
								drawIntersectionTetrahedron(
									n0, n1, n2, n3,
									plane->evaluate(n0), plane->evaluate(n1), plane->evaluate(n2), plane->evaluate(n3) );
							}
						}
						break;
					default:
						break;
					}
				}
				++eIter;
			}
		}else if( data->getValueType() == kmb::PhysicalValue::Vector3 ){
			kmb::Node n0, n1, n2, n3;
			double v[3] = {0.0, 0.0, 0.0};
			if( comp==-1 ){
				kmb::ElementContainer::const_iterator eIter = body->begin();
				while( eIter != body->end() )
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
									drawIntersectionTetrahedron(
										n0, n1, n2, n3,
										plane->evaluate(n0), plane->evaluate(n1), plane->evaluate(n2), plane->evaluate(n3) );
								}
							}
							break;
						default:
							break;
						}
					}
					++eIter;
				}
			}else if(0 <= comp && comp < 3 ){
				kmb::ElementContainer::const_iterator eIter = body->begin();
				while( eIter != body->end() )
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
									drawIntersectionTetrahedron(
										n0, n1, n2, n3,
										plane->evaluate(n0), plane->evaluate(n1), plane->evaluate(n2), plane->evaluate(n3) );
								}
							}
							break;
						default:
							break;
						}
					}
					++eIter;
				}
			}
		}else if( data->getValueType() == kmb::PhysicalValue::Integer ){
			kmb::Node n0, n1, n2, n3;
			long v = 0L;
			kmb::ElementContainer::const_iterator eIter = body->begin();
			while( !eIter.isFinished() )
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
								colorMap->setGLColorByStep( v );
								drawIntersectionTetrahedron(
									n0, n1, n2, n3,
									plane->evaluate(n0), plane->evaluate(n1), plane->evaluate(n2), plane->evaluate(n3) );
							}
						}
						break;
					default:
						break;
					}
				}
				++eIter;
			}
		}
		::glFlush();
	}
}

void
kmb::MeshGL::drawSectionTetrahedron(
	kmb::Node& n0,kmb::Node& n1,kmb::Node& n2,kmb::Node& n3,
	double t0, double t1, double t2, double t3)
{

	unsigned int flag = 0;
	if( t0 > 0 ) flag |= 0x01;
	if( t1 > 0 ) flag |= 0x02;
	if( t2 > 0 ) flag |= 0x04;
	if( t3 > 0 ) flag |= 0x08;
	if( flag == 0x00 || flag == 0x0f ){
		return;
	}
	switch( flag )
	{

	case 0x01:
		{

			drawTriangleByDividingPoint(
				&n0,&n1,t0,t1,
				&n0,&n3,t0,t3,
				&n0,&n2,t0,t2);
		}
		break;
	case 0x02:
		{

			drawTriangleByDividingPoint(
				&n1,&n0,t1,t0,
				&n1,&n2,t1,t2,
				&n1,&n3,t1,t3);
		}
		break;
	case 0x04:
		{

			drawTriangleByDividingPoint(
				&n2,&n0,t2,t0,
				&n2,&n3,t2,t3,
				&n2,&n1,t2,t1);
		}
		break;
	case 0x08:
		{

			drawTriangleByDividingPoint(
				&n3,&n0,t3,t0,
				&n3,&n1,t3,t1,
				&n3,&n2,t3,t2);
		}
		break;

	case 0x03:
		{

			drawQuadByDividingPoint(
				&n0,&n2,t0,t2,
				&n1,&n2,t1,t2,
				&n1,&n3,t1,t3,
				&n0,&n3,t0,t3);
		}
		break;
	case 0x05:
		{

			drawQuadByDividingPoint(
				&n0,&n3,t0,t3,
				&n2,&n3,t2,t3,
				&n2,&n1,t2,t1,
				&n0,&n1,t0,t1);
		}
		break;
	case 0x06:
		{

			drawQuadByDividingPoint(
				&n1,&n0,t1,t0,
				&n2,&n0,t2,t0,
				&n2,&n3,t2,t3,
				&n1,&n3,t1,t3);
		}
		break;
	case 0x09:
		{

			drawQuadByDividingPoint(
				&n0,&n1,t0,t1,
				&n3,&n1,t3,t1,
				&n3,&n2,t3,t2,
				&n0,&n2,t0,t2);
		}
		break;
	case 0x0a:
		{

			drawQuadByDividingPoint(
				&n1,&n2,t1,t2,
				&n3,&n2,t3,t2,
				&n3,&n0,t3,t0,
				&n1,&n0,t1,t0);
		}
		break;
	case 0x0c:
		{

			drawQuadByDividingPoint(
				&n2,&n0,t2,t0,
				&n3,&n0,t3,t0,
				&n3,&n1,t3,t1,
				&n2,&n1,t2,t1);
		}
		break;

	case 0x07:
		{

			drawTriangleByDividingPoint(
				&n3,&n0,t3,t0,
				&n3,&n2,t3,t2,
				&n3,&n1,t3,t1);
		}
		break;
	case 0x0b:
		{

			drawTriangleByDividingPoint(
				&n2,&n0,t2,t0,
				&n2,&n1,t2,t1,
				&n2,&n3,t2,t3);
		}
		break;
	case 0x0d:
		{

			drawTriangleByDividingPoint(
				&n1,&n0,t1,t0,
				&n1,&n3,t1,t3,
				&n1,&n2,t1,t2);
		}
		break;
	case 0x0e:
		{

			drawTriangleByDividingPoint(
				&n0,&n1,t0,t1,
				&n0,&n2,t0,t2,
				&n0,&n3,t0,t3);
		}
		break;
	}
	::glFlush();
}

void
kmb::MeshGL::drawIntersectionTetrahedron(
	kmb::Node& n0,kmb::Node& n1,kmb::Node& n2,kmb::Node& n3,
	double t0, double t1, double t2, double t3)
{

	unsigned int flag = 0;
	if( t0 > 0 ) flag |= 0x01;
	if( t1 > 0 ) flag |= 0x02;
	if( t2 > 0 ) flag |= 0x04;
	if( t3 > 0 ) flag |= 0x08;
	if( flag == 0x00 || flag == 0x0f ){
		return;
	}
	switch( flag )
	{
	case 0x07:
		{
			drawTriangle(
				n0.x(), n0.y(), n0.z(),
				n1.x(), n1.y(), n1.z(),
				n2.x(), n2.y(), n2.z() );
		}
		break;
	case 0x0b:
		{
			drawTriangle(
				n0.x(), n0.y(), n0.z(),
				n1.x(), n1.y(), n1.z(),
				n3.x(), n3.y(), n3.z() );
		}
		break;
	case 0x0d:
		{
			drawTriangle(
				n0.x(), n0.y(), n0.z(),
				n3.x(), n3.y(), n3.z(),
				n2.x(), n2.y(), n2.z() );
		}
		break;
	case 0x0e:
		{
			drawTriangle(
				n1.x(), n1.y(), n1.z(),
				n2.x(), n2.y(), n2.z(),
				n3.x(), n3.y(), n3.z() );
		}
		break;
	default:
		break;
	}
	::glFlush();
}
