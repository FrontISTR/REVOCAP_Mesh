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

#include "MeshDB/kmbMeshData.h"
#include "MeshDB/kmbDataBindings.h"
#include "MeshDB/kmbElementContainerOpenGLDraw.h"
#include "MeshGL/kmbColorMap.h"
#include "Geometry/kmbPoint3DContainerArray.h"

#include <cmath>
#include <cstring>

double
kmb::MeshGL::getMeanLengthVector( kmb::PhysicalValue* val0, kmb::PhysicalValue* val1, kmb::PhysicalValue* val2, kmb::PhysicalValue* val3)
{
	// no null check
	double vx = 0.25 * (
		static_cast<kmb::Vector3Value*>(val0)->getValue(0) +
		static_cast<kmb::Vector3Value*>(val1)->getValue(0) +
		static_cast<kmb::Vector3Value*>(val2)->getValue(0) +
		static_cast<kmb::Vector3Value*>(val3)->getValue(0) );
	double vy = 0.25 * (
		static_cast<kmb::Vector3Value*>(val0)->getValue(1) +
		static_cast<kmb::Vector3Value*>(val1)->getValue(1) +
		static_cast<kmb::Vector3Value*>(val2)->getValue(1) +
		static_cast<kmb::Vector3Value*>(val3)->getValue(1) );
	double vz = 0.25 * (
		static_cast<kmb::Vector3Value*>(val0)->getValue(2) +
		static_cast<kmb::Vector3Value*>(val1)->getValue(2) +
		static_cast<kmb::Vector3Value*>(val2)->getValue(2) +
		static_cast<kmb::Vector3Value*>(val3)->getValue(2) );
	return sqrt(vx*vx + vy*vy + vz*vz);
}

void
kmb::MeshGL::drawSurfaceContour(kmb::bodyIdType bodyId,const char* physicalValue,kmb::ColorMap* colorMap,int comp)
{
	const kmb::DataBindings* data = NULL;
	const kmb::Body* body = NULL;
	if( mesh != NULL && colorMap != NULL &&
		(body = mesh->getBodyPtr(bodyId)) != NULL &&
		body->isUniqueDim(2) &&
		(data = mesh->getDataBindingsPtr( physicalValue )) != NULL &&
		data->getBindingMode() == kmb::DataBindings::ElementVariable )
	{
		::glPushAttrib(GL_ENABLE_BIT);
		::glEnable(GL_COLOR_MATERIAL);

		kmb::Node node0,node1,node2,node3;
		if( data->getValueType() == kmb::PhysicalValue::Scalar ){
			kmb::ElementContainer::const_iterator eIter = body->begin();
			double v = 0.0;
			while( !eIter.isFinished() )
			{
				kmb::elementIdType id = eIter.getId();
				if( data->getPhysicalValue( id, &v ) ){
					switch( eIter.getType() ){
					case kmb::TRIANGLE:
					case kmb::TRIANGLE2:
						if( mesh->getNode( eIter[0], node0 ) &&
							mesh->getNode( eIter[1], node1 ) &&
							mesh->getNode( eIter[2], node2 ) )
						{
							colorMap->setGLColor( v );
							drawTriangle
								(node0.x(), node0.y(), node0.z(),
								 node1.x(), node1.y(), node1.z(),
								 node2.x(), node2.y(), node2.z());
						}
						break;
					case kmb::QUAD:
					case kmb::QUAD2:
						if( mesh->getNode( eIter[0], node0 ) &&
							mesh->getNode( eIter[1], node1 ) &&
							mesh->getNode( eIter[2], node2 ) &&
							mesh->getNode( eIter[3], node3 ) )
						{
							colorMap->setGLColor( v );
							drawQuad
								(node0.x(), node0.y(), node0.z(),
								 node1.x(), node1.y(), node1.z(),
								 node2.x(), node2.y(), node2.z(),
								 node3.x(), node3.y(), node3.z());
						}
						break;
					default:
						break;
					}
				}
				++eIter;
			}
		}else if( data->getValueType() == kmb::PhysicalValue::Vector3 ){
			if( comp==-1 ){
				kmb::ElementContainer::const_iterator eIter = body->begin();
				double v[3] = {0.0, 0.0, 0.0};
				while( !eIter.isFinished() )
				{
					kmb::elementIdType id = eIter.getId();
					if( data->getPhysicalValue( id, v ) ){
						switch( eIter.getType() ){
						case kmb::TRIANGLE:
						case kmb::TRIANGLE2:
							if( mesh->getNode( eIter[0], node0 ) &&
								mesh->getNode( eIter[1], node1 ) &&
								mesh->getNode( eIter[2], node2 ) )
							{
								colorMap->setGLColor( kmb::Vector3D::abs(v) );
								drawTriangle
									(node0.x(), node0.y(), node0.z(),
									 node1.x(), node1.y(), node1.z(),
									 node2.x(), node2.y(), node2.z());
							}
							break;
						case kmb::QUAD:
						case kmb::QUAD2:
							if( mesh->getNode( eIter[0], node0 ) &&
								mesh->getNode( eIter[1], node1 ) &&
								mesh->getNode( eIter[2], node2 ) &&
								mesh->getNode( eIter[3], node3 ) )
							{
								colorMap->setGLColor( kmb::Vector3D::abs(v) );
								drawQuad
									(node0.x(), node0.y(), node0.z(),
									 node1.x(), node1.y(), node1.z(),
									 node2.x(), node2.y(), node2.z(),
									 node3.x(), node3.y(), node3.z());
							}
							break;
						default:
							break;
						}
					}
					++eIter;
				}
			}else if( 0 <= comp && comp < 3 ){
				kmb::ElementContainer::const_iterator eIter = body->begin();
				double v[3] = {0.0, 0.0, 0.0};
				while( !eIter.isFinished() )
				{
					kmb::elementIdType id = eIter.getId();
					if( data->getPhysicalValue( id, v ) ){
						switch( eIter.getType() ){
						case kmb::TRIANGLE:
						case kmb::TRIANGLE2:
							if( mesh->getNode( eIter[0], node0 ) &&
								mesh->getNode( eIter[1], node1 ) &&
								mesh->getNode( eIter[2], node2 ) )
							{
								colorMap->setGLColor( v[comp] );
								drawTriangle
									(node0.x(), node0.y(), node0.z(),
									 node1.x(), node1.y(), node1.z(),
									 node2.x(), node2.y(), node2.z());
							}
							break;
						case kmb::QUAD:
						case kmb::QUAD2:
							if( mesh->getNode( eIter[0], node0 ) &&
								mesh->getNode( eIter[1], node1 ) &&
								mesh->getNode( eIter[2], node2 ) &&
								mesh->getNode( eIter[3], node3 ) )
							{
								colorMap->setGLColor( v[comp] );
								drawQuad
									(node0.x(), node0.y(), node0.z(),
									 node1.x(), node1.y(), node1.z(),
									 node2.x(), node2.y(), node2.z(),
									 node3.x(), node3.y(), node3.z());
							}
							break;
						default:
							break;
						}
					}
					++eIter;
				}
			}
		}else if( data->getValueType() == kmb::PhysicalValue::Tensor6 ){
			if( 0 <= comp && comp < 6 ){
				kmb::ElementContainer::const_iterator eIter = body->begin();
				double v[6] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
				while( !eIter.isFinished() )
				{
					kmb::elementIdType id = eIter.getId();
					if( data->getPhysicalValue( id, v ) ){
						switch( eIter.getType() ){
						case kmb::TRIANGLE:
						case kmb::TRIANGLE2:
							if( mesh->getNode( eIter[0], node0 ) &&
								mesh->getNode( eIter[1], node1 ) &&
								mesh->getNode( eIter[2], node2 ) )
							{
								colorMap->setGLColor( v[comp] );
								drawTriangle
									(node0.x(), node0.y(), node0.z(),
									 node1.x(), node1.y(), node1.z(),
									 node2.x(), node2.y(), node2.z());
							}
							break;
						case kmb::QUAD:
						case kmb::QUAD2:
							if( mesh->getNode( eIter[0], node0 ) &&
								mesh->getNode( eIter[1], node1 ) &&
								mesh->getNode( eIter[2], node2 ) &&
								mesh->getNode( eIter[3], node3 ) )
							{
								colorMap->setGLColor( v[comp] );
								drawQuad
									(node0.x(), node0.y(), node0.z(),
									 node1.x(), node1.y(), node1.z(),
									 node2.x(), node2.y(), node2.z(),
									 node3.x(), node3.y(), node3.z());
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
			kmb::ElementContainer::const_iterator eIter = body->begin();
			long v = 0L;
			while( !eIter.isFinished() )
			{
				kmb::elementIdType id = eIter.getId();
				if( data->getPhysicalValue( id, &v ) ){
					switch( eIter.getType() ){
					case kmb::TRIANGLE:
					case kmb::TRIANGLE2:
						if( mesh->getNode( eIter[0], node0 ) &&
							mesh->getNode( eIter[1], node1 ) &&
							mesh->getNode( eIter[2], node2 ) )
						{
							colorMap->setGLColorByStep( v );
							drawTriangle
								(node0.x(), node0.y(), node0.z(),
								 node1.x(), node1.y(), node1.z(),
								 node2.x(), node2.y(), node2.z());
						}
						break;
					case kmb::QUAD:
					case kmb::QUAD2:
						if( mesh->getNode( eIter[0], node0 ) &&
							mesh->getNode( eIter[1], node1 ) &&
							mesh->getNode( eIter[2], node2 ) &&
							mesh->getNode( eIter[3], node3 ) )
						{
							colorMap->setGLColorByStep( v );
							drawQuad
								(node0.x(), node0.y(), node0.z(),
								 node1.x(), node1.y(), node1.z(),
								 node2.x(), node2.y(), node2.z(),
								 node3.x(), node3.y(), node3.z());
						}
						break;
					default:
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
kmb::MeshGL::drawSurfaceContour2D(kmb::bodyIdType bodyId,const char* physicalValue,kmb::ColorMap* colorMap,int comp)
{
	const kmb::DataBindings* data = NULL;
	const kmb::Point2DContainer* points = NULL;
	const kmb::Body* body = NULL;
	if( mesh != NULL &&
		(points = mesh->getNode2Ds()) != NULL &&
		colorMap != NULL &&
		(body = mesh->getBodyPtr(bodyId)) != NULL &&
		body->isUniqueDim(2) != false &&
		(data = mesh->getDataBindingsPtr( physicalValue )) != NULL &&
		data->getBindingMode() == kmb::DataBindings::ElementVariable )
	{
		::glPushAttrib( GL_ENABLE_BIT );
		::glEnable(GL_COLOR_MATERIAL);

		double x0=0.0, y0=0.0, x1=0.0, y1=0.0, x2=0.0, y2=0.0, x3=0.0, y3=0.0;
		if( data->getValueType() == kmb::PhysicalValue::Scalar ){
			kmb::ElementContainer::const_iterator eIter = body->begin();
			double v = 0.0;
			while( !eIter.isFinished() )
			{
				kmb::elementIdType id = eIter.getId();
				if( data->getPhysicalValue( id, &v ) ){
					switch( eIter.getType() ){
					case kmb::TRIANGLE:
					case kmb::TRIANGLE2:
						if( points->getXY( eIter[0], x0, y0 ) &&
							points->getXY( eIter[1], x1, y1 ) &&
							points->getXY( eIter[2], x2, y2 ) )
						{
							colorMap->setGLColor( v );
							drawTriangle2D(x0,y0,x1,y1,x2,y2);
						}
						break;
					case kmb::QUAD:
					case kmb::QUAD2:
						if( points->getXY( eIter[0], x0, y0 ) &&
							points->getXY( eIter[1], x1, y1 ) &&
							points->getXY( eIter[2], x2, y2 ) &&
							points->getXY( eIter[3], x3, y3 ) )
						{
							colorMap->setGLColor( v );
							drawQuad2D(x0,y0,x1,y1,x2,y2,x3,y3);
						}
						break;
					default:
						break;
					}
				}
				++eIter;
			}
		}else if( data->getValueType() == kmb::PhysicalValue::Vector2 ){
			if( comp == -1 ){
				kmb::ElementContainer::const_iterator eIter = body->begin();
				double v[2] = { 0.0, 0.0 };
				while( !eIter.isFinished() )
				{
					kmb::elementIdType id = eIter.getId();
					if( data->getPhysicalValue( id, v ) ){
						switch( eIter.getType() ){
						case kmb::TRIANGLE:
						case kmb::TRIANGLE2:
							if( points->getXY( eIter[0], x0, y0 ) &&
								points->getXY( eIter[1], x1, y1 ) &&
								points->getXY( eIter[2], x2, y2 ) )
							{
								colorMap->setGLColor( kmb::Vector2D::abs(v) );
								drawTriangle2D(x0,y0,x1,y1,x2,y2);
							}
							break;
						case kmb::QUAD:
						case kmb::QUAD2:
							if( points->getXY( eIter[0], x0, y0 ) &&
								points->getXY( eIter[1], x1, y1 ) &&
								points->getXY( eIter[2], x2, y2 ) &&
								points->getXY( eIter[3], x3, y3 ) )
							{
								colorMap->setGLColor( kmb::Vector2D::abs(v) );
								drawQuad2D(x0,y0,x1,y1,x2,y2,x3,y3);
							}
							break;
						default:
							break;
						}
					}
					++eIter;
				}
			}else if( 0 <= comp && comp < 2 ){
				kmb::ElementContainer::const_iterator eIter = body->begin();
				double v[2] = { 0.0, 0.0 };
				while( !eIter.isFinished() )
				{
					kmb::elementIdType id = eIter.getId();
					if( data->getPhysicalValue( id, v ) ){
						switch( eIter.getType() ){
						case kmb::TRIANGLE:
						case kmb::TRIANGLE2:
							if( points->getXY( eIter[0], x0, y0 ) &&
								points->getXY( eIter[1], x1, y1 ) &&
								points->getXY( eIter[2], x2, y2 ) )
							{
								colorMap->setGLColor( v[comp] );
								drawTriangle2D(x0,y0,x1,y1,x2,y2);
							}
							break;
						case kmb::QUAD:
						case kmb::QUAD2:
							if( points->getXY( eIter[0], x0, y0 ) &&
								points->getXY( eIter[1], x1, y1 ) &&
								points->getXY( eIter[2], x2, y2 ) &&
								points->getXY( eIter[3], x3, y3 ) )
							{
								colorMap->setGLColor( v[comp] );
								drawQuad2D(x0,y0,x1,y1,x2,y2,x3,y3);
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
			kmb::ElementContainer::const_iterator eIter = body->begin();
			long v = 0L;
			while( !eIter.isFinished() )
			{
				kmb::elementIdType id = eIter.getId();
				if( data->getPhysicalValue( id, &v ) ){
					switch( eIter.getType() ){
					case kmb::TRIANGLE:
					case kmb::TRIANGLE2:
						if( points->getXY( eIter[0], x0, y0 ) &&
							points->getXY( eIter[1], x1, y1 ) &&
							points->getXY( eIter[2], x2, y2 ) )
						{
							colorMap->setGLColorByStep( v );
							drawTriangle2D(x0,y0,x1,y1,x2,y2);
						}
						break;
					case kmb::QUAD:
					case kmb::QUAD2:
						if( points->getXY( eIter[0], x0, y0 ) &&
							points->getXY( eIter[1], x1, y1 ) &&
							points->getXY( eIter[2], x2, y2 ) &&
							points->getXY( eIter[3], x3, y3 ) )
						{
							colorMap->setGLColorByStep( v );
							drawQuad2D(x0,y0,x1,y1,x2,y2,x3,y3);
						}
						break;
					default:
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

// 親の要素に値が入っている時
void
kmb::MeshGL::drawFaceGroupContour(const char* faceName, const char* physicalValue, kmb::ColorMap* colorMap,int comp)
{
	const kmb::DataBindings* data = NULL;
	const kmb::DataBindings* value = NULL;
	if( mesh != NULL && colorMap != NULL &&
		(data = mesh->getDataBindingsPtr( faceName )) != NULL &&
		data->getBindingMode() == kmb::DataBindings::FaceGroup &&
		(value = mesh->getDataBindingsPtr( physicalValue )) != NULL &&
		value->getBindingMode() == kmb::DataBindings::ElementVariable )
	{
		::glPushAttrib( GL_ENABLE_BIT );
		::glEnable(GL_COLOR_MATERIAL);

		kmb::Node node0,node1,node2,node3,center;
		kmb::bodyIdType bodyId = data->getTargetBodyId();
		kmb::Face f;
		if( value->getValueType() == kmb::PhysicalValue::Scalar ){
			kmb::DataBindings::const_iterator fIter = data->begin();
			double v = 0.0;
			while( !fIter.isFinished() )
			{
				if( !fIter.getFace(f) ){
					++fIter;
					continue;
				}
				kmb::ElementContainer::const_iterator element = mesh->findElement( f.getElementId(), bodyId );
				kmb::idType localId = f.getLocalFaceId();
				if( !element.isFinished() && value->getPhysicalValue( f.getElementId(), &v ) ){
					switch( element.getBoundaryVertexCount(localId) )
					{
					case 3:
						if( mesh->getNode( element.getBoundaryCellId(localId,0), node0 ) &&
							mesh->getNode( element.getBoundaryCellId(localId,1), node1 ) &&
							mesh->getNode( element.getBoundaryCellId(localId,2), node2 ) )
						{
							colorMap->setGLColor( v );
							drawTriangle
								(node0.x(), node0.y(), node0.z(),
								 node1.x(), node1.y(), node1.z(),
								 node2.x(), node2.y(), node2.z());
						}
						break;
					case 4:
						if( mesh->getNode( element.getBoundaryCellId(localId,0), node0 ) &&
							mesh->getNode( element.getBoundaryCellId(localId,1), node1 ) &&
							mesh->getNode( element.getBoundaryCellId(localId,2), node2 ) &&
							mesh->getNode( element.getBoundaryCellId(localId,3), node3 ) )
						{
							colorMap->setGLColor( v );
							drawQuad
								(node0.x(), node0.y(), node0.z(),
								 node1.x(), node1.y(), node1.z(),
								 node2.x(), node2.y(), node2.z(),
								 node3.x(), node3.y(), node3.z());
						}
						break;
					}
				}
				++fIter;
			}
		}else if( value->getValueType() == kmb::PhysicalValue::Vector3 ){
			kmb::DataBindings::const_iterator fIter = data->begin();
			double v[3] = { 0.0, 0.0, 0.0 };
			if( comp == -1 ){
				while( !fIter.isFinished() )
				{
					if( !fIter.getFace(f) ){
						++fIter;
						continue;
					}
					kmb::ElementContainer::const_iterator element = mesh->findElement( f.getElementId(), bodyId );
					kmb::idType localId = f.getLocalFaceId();
					if( !element.isFinished() && value->getPhysicalValue( f.getElementId(), v ) ){
						switch( element.getBoundaryVertexCount(localId) )
						{
						case 3:
							if( mesh->getNode( element.getBoundaryCellId(localId,0), node0 ) &&
								mesh->getNode( element.getBoundaryCellId(localId,1), node1 ) &&
								mesh->getNode( element.getBoundaryCellId(localId,2), node2 ) )
							{
								colorMap->setGLColor( kmb::Vector3D::abs(v) );
								drawTriangle
									(node0.x(), node0.y(), node0.z(),
									 node1.x(), node1.y(), node1.z(),
									 node2.x(), node2.y(), node2.z());
							}
							break;
						case 4:
							if( mesh->getNode( element.getBoundaryCellId(localId,0), node0 ) &&
								mesh->getNode( element.getBoundaryCellId(localId,1), node1 ) &&
								mesh->getNode( element.getBoundaryCellId(localId,2), node2 ) &&
								mesh->getNode( element.getBoundaryCellId(localId,3), node3 ) )
							{
								colorMap->setGLColor( kmb::Vector3D::abs(v) );
								drawQuad
									(node0.x(), node0.y(), node0.z(),
									 node1.x(), node1.y(), node1.z(),
									 node2.x(), node2.y(), node2.z(),
									 node3.x(), node3.y(), node3.z());
							}
							break;
						}
					}
					++fIter;
				}
			}else if(0 <= comp && comp < 3 ){
				while( !fIter.isFinished() )
				{
					if( !fIter.getFace(f) ){
						++fIter;
						continue;
					}
					kmb::ElementContainer::const_iterator element = mesh->findElement( f.getElementId(), bodyId );
					kmb::idType localId = f.getLocalFaceId();
					if( !element.isFinished() && value->getPhysicalValue( f.getElementId(), v ) ){
						switch( element.getBoundaryVertexCount(localId) )
						{
						case 3:
							if( mesh->getNode( element.getBoundaryCellId(localId,0), node0 ) &&
								mesh->getNode( element.getBoundaryCellId(localId,1), node1 ) &&
								mesh->getNode( element.getBoundaryCellId(localId,2), node2 ) )
							{
								colorMap->setGLColor( v[comp] );
								drawTriangle
									(node0.x(), node0.y(), node0.z(),
									 node1.x(), node1.y(), node1.z(),
									 node2.x(), node2.y(), node2.z());
							}
							break;
						case 4:
							if( mesh->getNode( element.getBoundaryCellId(localId,0), node0 ) &&
								mesh->getNode( element.getBoundaryCellId(localId,1), node1 ) &&
								mesh->getNode( element.getBoundaryCellId(localId,2), node2 ) &&
								mesh->getNode( element.getBoundaryCellId(localId,3), node3 ) )
							{
								colorMap->setGLColor( v[comp] );
								drawQuad
									(node0.x(), node0.y(), node0.z(),
									 node1.x(), node1.y(), node1.z(),
									 node2.x(), node2.y(), node2.z(),
									 node3.x(), node3.y(), node3.z());
							}
							break;
						}
					}
					++fIter;
				}
			}
		}else if( value->getValueType() == kmb::PhysicalValue::Tensor6 ){
			kmb::DataBindings::const_iterator fIter = data->begin();
			double v[6] = { 0.0, 0.0, 0.0, 0.0, 0.0, 0.0 };
			if(0 <= comp && comp < 6 ){
				while( !fIter.isFinished() )
				{
					if( !fIter.getFace(f) ){
						++fIter;
						continue;
					}
					kmb::ElementContainer::const_iterator element = mesh->findElement( f.getElementId(), bodyId );
					kmb::idType localId = f.getLocalFaceId();
					if( !element.isFinished() && value->getPhysicalValue( f.getElementId(), v ) ){
						switch( element.getBoundaryVertexCount(localId) )
						{
						case 3:
							if( mesh->getNode( element.getBoundaryCellId(localId,0), node0 ) &&
								mesh->getNode( element.getBoundaryCellId(localId,1), node1 ) &&
								mesh->getNode( element.getBoundaryCellId(localId,2), node2 ) )
							{
								colorMap->setGLColor( v[comp] );
								drawTriangle
									(node0.x(), node0.y(), node0.z(),
									 node1.x(), node1.y(), node1.z(),
									 node2.x(), node2.y(), node2.z());
							}
							break;
						case 4:
							if( mesh->getNode( element.getBoundaryCellId(localId,0), node0 ) &&
								mesh->getNode( element.getBoundaryCellId(localId,1), node1 ) &&
								mesh->getNode( element.getBoundaryCellId(localId,2), node2 ) &&
								mesh->getNode( element.getBoundaryCellId(localId,3), node3 ) )
							{
								colorMap->setGLColor( v[comp] );
								drawQuad
									(node0.x(), node0.y(), node0.z(),
									 node1.x(), node1.y(), node1.z(),
									 node2.x(), node2.y(), node2.z(),
									 node3.x(), node3.y(), node3.z());
							}
							break;
						}
					}
					++fIter;
				}
			}
		}else if( value->getValueType() == kmb::PhysicalValue::Integer ){
			kmb::DataBindings::const_iterator fIter = data->begin();
			long v = 0L;
			while( !fIter.isFinished() )
			{
				if( !fIter.getFace(f) ){
					++fIter;
					continue;
				}
				kmb::ElementContainer::const_iterator element = mesh->findElement( f.getElementId(), bodyId );
				kmb::idType localId = f.getLocalFaceId();
				if( !element.isFinished() && value->getPhysicalValue( f.getElementId(), &v ) ){
					switch( element.getBoundaryVertexCount(localId) )
					{
					case 3:
						if( mesh->getNode( element.getBoundaryCellId(localId,0), node0 ) &&
							mesh->getNode( element.getBoundaryCellId(localId,1), node1 ) &&
							mesh->getNode( element.getBoundaryCellId(localId,2), node2 ) )
						{
							colorMap->setGLColorByStep( v );
							drawTriangle
								(node0.x(), node0.y(), node0.z(),
								 node1.x(), node1.y(), node1.z(),
								 node2.x(), node2.y(), node2.z());
						}
						break;
					case 4:
						if( mesh->getNode( element.getBoundaryCellId(localId,0), node0 ) &&
							mesh->getNode( element.getBoundaryCellId(localId,1), node1 ) &&
							mesh->getNode( element.getBoundaryCellId(localId,2), node2 ) &&
							mesh->getNode( element.getBoundaryCellId(localId,3), node3 ) )
						{
							colorMap->setGLColorByStep( v );
							drawQuad
								(node0.x(), node0.y(), node0.z(),
								 node1.x(), node1.y(), node1.z(),
								 node2.x(), node2.y(), node2.z(),
								 node3.x(), node3.y(), node3.z());
						}
						break;
					}
				}
				++fIter;
			}
		}
		::glPopAttrib();
		::glFlush();
	}
}

// 1次要素と2次要素を区別せずにコンターを書くため
// getVertexCount() で判定する
void
kmb::MeshGL::drawSurfaceNodeContour(kmb::bodyIdType bodyId,const char* physicalValue,kmb::ColorMap* colorMap,int comp)
{
	const kmb::DataBindings* data = NULL;
	const kmb::Body* body = NULL;
	const kmb::Point3DContainer* points = NULL;
	// 特別な場合
	if( mesh != NULL && colorMap != NULL && prefMode == kmb::MeshGL::SPEED &&
		(points = mesh->getNodes()) != NULL &&
		(body = mesh->getBodyPtr(bodyId)) != NULL &&
		0 == strcmp(points->getContainerType(),kmb::Point3DContainerArray::CONTAINER_TYPE) &&
		0 == strcmp(body->getContainerType(),kmb::ElementContainerOpenGLDraw::CONTAINER_TYPE) &&
		::glIsEnabled( GL_VERTEX_ARRAY ) == GL_TRUE &&
		(data = mesh->getDataBindingsPtr( physicalValue )) != NULL &&
		data->getBindingMode() == kmb::DataBindings::NodeVariable &&
		colorMap->getColorCache() != NULL &&
		colorMap->getStep() == 0 )
	{
		// 節点が配列で与えれらている
		// 色が配列で与えられている
		// 要素は三角形または四角形として配列で与えられている
		// glColorPointer はポリゴンごとに色を付ける場合に使えるのでここでは使えない
		::glPushAttrib(GL_ENABLE_BIT);
			::glEnable(GL_COLOR_MATERIAL);
			::glEnableClientState(GL_VERTEX_ARRAY);
			::glVertexPointer(3 , GL_DOUBLE , 0 ,
				reinterpret_cast<const kmb::Point3DContainerArray*>(points)->getDoubleArray() );
			const kmb::ElementContainerOpenGLDraw* elements
				= reinterpret_cast<const kmb::ElementContainerOpenGLDraw*>(body);
			const float* colors = colorMap->getColorCache();
			unsigned int nodeId = 0;
			if( elements->getTriCount() > 0 ){
				const size_t triCount = elements->getTriCount();
				const unsigned int* triNodeTable = elements->getTriNodeTable();
				const float* triNormals = elements->getTriNormals();
				::glBegin( GL_TRIANGLES );
				for(size_t i=0;i<triCount;++i){
					// 法線
					::glNormal3f( triNormals[3*i], triNormals[3*i+1], triNormals[3*i+2] );
					// 1点目
					nodeId = triNodeTable[3*i];
					::glColor3f( colors[3*nodeId], colors[3*nodeId+1], colors[3*nodeId+2] );
					::glArrayElement( nodeId );
					// 2点目
					nodeId = triNodeTable[3*i+1];
					::glColor3f( colors[3*nodeId], colors[3*nodeId+1], colors[3*nodeId+2] );
					::glArrayElement( nodeId );
					// 3点目
					nodeId = triNodeTable[3*i+2];
					::glColor3f( colors[3*nodeId], colors[3*nodeId+1], colors[3*nodeId+2] );
					::glArrayElement( nodeId );
				}
				::glEnd();
			}
			if( elements->getQuadCount() > 0 ){
				const size_t quadCount = elements->getQuadCount();
				const unsigned int* quadNodeTable = elements->getQuadNodeTable();
				const float* quadNormals = elements->getQuadNormals();
				::glBegin( GL_QUADS );
				for(size_t i=0;i<quadCount;++i){
					// 法線
					::glNormal3f( quadNormals[3*i], quadNormals[3*i+1], quadNormals[3*i+2] );
					// 1点目
					nodeId = quadNodeTable[4*i];
					::glColor3f( colors[3*nodeId], colors[3*nodeId+1], colors[3*nodeId+2] );
					::glArrayElement( nodeId );
					// 2点目
					nodeId = quadNodeTable[4*i+1];
					::glColor3f( colors[3*nodeId], colors[3*nodeId+1], colors[3*nodeId+2] );
					::glArrayElement( nodeId );
					// 3点目
					nodeId = quadNodeTable[4*i+2];
					::glColor3f( colors[3*nodeId], colors[3*nodeId+1], colors[3*nodeId+2] );
					::glArrayElement( nodeId );
					// 4点目
					nodeId = quadNodeTable[4*i+3];
					::glColor3f( colors[3*nodeId], colors[3*nodeId+1], colors[3*nodeId+2] );
					::glArrayElement( nodeId );
				}
				::glEnd();
			}
			::glDisableClientState(GL_COLOR_ARRAY);
			::glDisableClientState(GL_VERTEX_ARRAY);
		::glPopAttrib();
		::glFlush();
	}else
	if( mesh != NULL && colorMap != NULL &&
		(body = mesh->getBodyPtr(bodyId)) != NULL &&
		body->isUniqueDim(2) &&
		(data = mesh->getDataBindingsPtr( physicalValue )) != NULL &&
		data->getBindingMode() == kmb::DataBindings::NodeVariable )
	{
		::glPushAttrib( GL_ENABLE_BIT );
		::glEnable(GL_COLOR_MATERIAL);
		kmb::Node node0,node1,node2,node3;
		kmb::ElementContainer::const_iterator eIter = body->begin();
		if( data->getValueType() == kmb::PhysicalValue::Scalar ){
			double v0 = 0.0, v1 = 0.0, v2 = 0.0, v3 = 0.0, mean = 0.0;
			if( colorMap->getStep() == 0 ){
				while( !eIter.isFinished() )
				{
					if( eIter.getVertexCount() == 3 &&
						mesh->getNode( eIter[0], node0 ) &&
						mesh->getNode( eIter[1], node1 ) &&
						mesh->getNode( eIter[2], node2 ) &&
						data->getPhysicalValue( eIter[0], &v0 ) &&
						data->getPhysicalValue( eIter[1], &v1 ) &&
						data->getPhysicalValue( eIter[2], &v2 ) )
					{
						drawContourTriangle(
							node0.x(),node0.y(),node0.z(),v0,
							node1.x(),node1.y(),node1.z(),v1,
							node2.x(),node2.y(),node2.z(),v2,colorMap);
					}else if( eIter.getVertexCount() == 4 &&
						mesh->getNode( eIter[0], node0 ) &&
						mesh->getNode( eIter[1], node1 ) &&
						mesh->getNode( eIter[2], node2 ) &&
						mesh->getNode( eIter[3], node3 ) &&
						data->getPhysicalValue( eIter[0], &v0 ) &&
						data->getPhysicalValue( eIter[1], &v1 ) &&
						data->getPhysicalValue( eIter[2], &v2 ) &&
						data->getPhysicalValue( eIter[3], &v3 ) )
					{
						drawContourQuad(
							node0.x(), node0.y(), node0.z(), v0,
							node1.x(), node1.y(), node1.z(), v1,
							node2.x(), node2.y(), node2.z(), v2,
							node3.x(), node3.y(), node3.z(), v3, colorMap);
					}
					++eIter;
				}
			}else{
				while( !eIter.isFinished() )
				{
					if( eIter.getVertexCount() == 3 &&
						mesh->getNode( eIter[0], node0 ) &&
						mesh->getNode( eIter[1], node1 ) &&
						mesh->getNode( eIter[2], node2 ) &&
						data->getPhysicalValue( eIter[0], &v0 ) &&
						data->getPhysicalValue( eIter[1], &v1 ) &&
						data->getPhysicalValue( eIter[2], &v2 ) )
					{
						drawSolidContourTriangle(&node0,&node1,&node2,v0,v1,v2,colorMap);
					}else if( eIter.getVertexCount() == 4 &&
						mesh->getNode( eIter[0], node0 ) &&
						mesh->getNode( eIter[1], node1 ) &&
						mesh->getNode( eIter[2], node2 ) &&
						mesh->getNode( eIter[3], node3 ) &&
						data->getPhysicalValue( eIter[0], &v0 ) &&
						data->getPhysicalValue( eIter[1], &v1 ) &&
						data->getPhysicalValue( eIter[2], &v2 ) &&
						data->getPhysicalValue( eIter[3], &v3 ) )
					{
						mean = 0.25*(v0 + v1 + v2 + v3);
						kmb::Node node(
							0.25*(node0.x()+node1.x()+node2.x()+node3.x()),
							0.25*(node0.y()+node1.y()+node2.y()+node3.y()),
							0.25*(node0.z()+node1.z()+node2.z()+node3.z()));
						drawSolidContourTriangle(&node0,&node1,&node,v0,v1,mean,colorMap);
						drawSolidContourTriangle(&node1,&node2,&node,v1,v2,mean,colorMap);
						drawSolidContourTriangle(&node2,&node3,&node,v2,v3,mean,colorMap);
						drawSolidContourTriangle(&node3,&node0,&node,v3,v0,mean,colorMap);
					}
					++eIter;
				}
			}
		}else if( data->getValueType() == kmb::PhysicalValue::Vector3 ){
			double v0[3] = {0.0, 0.0, 0.0},
				v1[3] = {0.0, 0.0, 0.0},
				v2[3] = {0.0, 0.0, 0.0},
				v3[3] = {0.0, 0.0, 0.0},
				u0 = 0.0, u1 =0.0, u2= 0.0, u3 = 0.0, mean = 0.0;
			if( colorMap->getStep() == 0 ){
				if( comp==-1 ){
					while( !eIter.isFinished() )
					{
						if( eIter.getVertexCount() == 3 &&
							mesh->getNode( eIter[0], node0 ) &&
							mesh->getNode( eIter[1], node1 ) &&
							mesh->getNode( eIter[2], node2 ) &&
							data->getPhysicalValue( eIter[0], v0 ) &&
							data->getPhysicalValue( eIter[1], v1 ) &&
							data->getPhysicalValue( eIter[2], v2 ) )
						{
							drawContourTriangle(
								node0.x(), node0.y(), node0.z(), kmb::Vector3D::abs(v0),
								node1.x(), node1.y(), node1.z(), kmb::Vector3D::abs(v1),
								node2.x(), node2.y(), node2.z(), kmb::Vector3D::abs(v2), colorMap);
						}else if( eIter.getVertexCount() == 4 &&
							mesh->getNode( eIter[0], node0 ) &&
							mesh->getNode( eIter[1], node1 ) &&
							mesh->getNode( eIter[2], node2 ) &&
							mesh->getNode( eIter[3], node3 ) &&
							data->getPhysicalValue( eIter[0], v0 ) &&
							data->getPhysicalValue( eIter[1], v1 ) &&
							data->getPhysicalValue( eIter[2], v2 ) &&
							data->getPhysicalValue( eIter[3], v3 ) )
						{
							drawContourQuad(
								node0.x(), node0.y(), node0.z(), kmb::Vector3D::abs(v0),
								node1.x(), node1.y(), node1.z(), kmb::Vector3D::abs(v1),
								node2.x(), node2.y(), node2.z(), kmb::Vector3D::abs(v2),
								node3.x(), node3.y(), node3.z(), kmb::Vector3D::abs(v3), colorMap);
						}
						++eIter;
					}
				}else if(0 <= comp && comp < 3 ){
					while( !eIter.isFinished() )
					{
						if( eIter.getVertexCount() == 3 &&
							mesh->getNode( eIter[0], node0 ) &&
							mesh->getNode( eIter[1], node1 ) &&
							mesh->getNode( eIter[2], node2 ) &&
							data->getPhysicalValue( eIter[0], v0 ) &&
							data->getPhysicalValue( eIter[1], v1 ) &&
							data->getPhysicalValue( eIter[2], v2 ) )
						{
							drawContourTriangle(
								node0.x(), node0.y(), node0.z(), v0[comp],
								node1.x(), node1.y(), node1.z(), v1[comp],
								node2.x(), node2.y(), node2.z(), v2[comp], colorMap);
						}else if( eIter.getVertexCount() == 4 &&
							mesh->getNode( eIter[0], node0 ) &&
							mesh->getNode( eIter[1], node1 ) &&
							mesh->getNode( eIter[2], node2 ) &&
							mesh->getNode( eIter[3], node3 ) &&
							data->getPhysicalValue( eIter[0], v0 ) &&
							data->getPhysicalValue( eIter[1], v1 ) &&
							data->getPhysicalValue( eIter[2], v2 ) &&
							data->getPhysicalValue( eIter[3], v3 ) )
						{
							drawContourQuad(
								node0.x(), node0.y(), node0.z(), v0[comp],
								node1.x(), node1.y(), node1.z(), v1[comp],
								node2.x(), node2.y(), node2.z(), v2[comp],
								node3.x(), node3.y(), node3.z(), v3[comp], colorMap);
						}
						++eIter;
					}
				}
			}else{
				if( comp==-1 ){
					while( !eIter.isFinished() )
					{
						if( eIter.getVertexCount() == 3 &&
							mesh->getNode( eIter[0], node0 ) &&
							mesh->getNode( eIter[1], node1 ) &&
							mesh->getNode( eIter[2], node2 ) &&
							data->getPhysicalValue( eIter[0], v0 ) &&
							data->getPhysicalValue( eIter[1], v1 ) &&
							data->getPhysicalValue( eIter[2], v2 ) )
						{
							drawSolidContourTriangle(
								&node0, &node1, &node2,
								kmb::Vector3D::abs(v0),
								kmb::Vector3D::abs(v1),
								kmb::Vector3D::abs(v2), colorMap);
						}else if( eIter.getVertexCount() == 4 &&
							mesh->getNode( eIter[0], node0 ) &&
							mesh->getNode( eIter[1], node1 ) &&
							mesh->getNode( eIter[2], node2 ) &&
							mesh->getNode( eIter[3], node3 ) &&
							data->getPhysicalValue( eIter[0], v0 ) &&
							data->getPhysicalValue( eIter[1], v1 ) &&
							data->getPhysicalValue( eIter[2], v2 ) &&
							data->getPhysicalValue( eIter[3], v3 ) )
						{
							u0 = kmb::Vector3D::abs( v0 );
							u1 = kmb::Vector3D::abs( v1 );
							u2 = kmb::Vector3D::abs( v2 );
							u3 = kmb::Vector3D::abs( v3 );
							mean = 0.25*(u0 + u1 + u2 + u3);
							kmb::Node node(
								0.25*(node0.x()+node1.x()+node2.x()+node3.x()),
								0.25*(node0.y()+node1.y()+node2.y()+node3.y()),
								0.25*(node0.z()+node1.z()+node2.z()+node3.z()));
							drawSolidContourTriangle(&node0,&node1,&node,u0,u1,mean,colorMap);
							drawSolidContourTriangle(&node1,&node2,&node,u1,u2,mean,colorMap);
							drawSolidContourTriangle(&node2,&node3,&node,u2,u3,mean,colorMap);
							drawSolidContourTriangle(&node3,&node0,&node,u3,u0,mean,colorMap);
						}
						++eIter;
					}
				}else if(0 <= comp && comp < 3 ){
					while( !eIter.isFinished() )
					{
						if( eIter.getVertexCount() == 3 &&
							mesh->getNode( eIter[0], node0 ) &&
							mesh->getNode( eIter[1], node1 ) &&
							mesh->getNode( eIter[2], node2 ) &&
							data->getPhysicalValue( eIter[0], v0 ) &&
							data->getPhysicalValue( eIter[1], v1 ) &&
							data->getPhysicalValue( eIter[2], v2 ) )
						{
							drawSolidContourTriangle(
								&node0, &node1, &node2,
								v0[comp], v1[comp], v2[comp], colorMap);
						}else if( eIter.getVertexCount() == 4 &&
							mesh->getNode( eIter[0], node0 ) &&
							mesh->getNode( eIter[1], node1 ) &&
							mesh->getNode( eIter[2], node2 ) &&
							mesh->getNode( eIter[3], node3 ) &&
							data->getPhysicalValue( eIter[0], v0 ) &&
							data->getPhysicalValue( eIter[1], v1 ) &&
							data->getPhysicalValue( eIter[2], v2 ) &&
							data->getPhysicalValue( eIter[3], v3 ) )
						{
							u0 = v0[comp];
							u1 = v1[comp];
							u2 = v2[comp];
							u3 = v3[comp];
							mean = 0.25*(u0 + u1 + u2 + u3);
							kmb::Node node(
								0.25*(node0.x()+node1.x()+node2.x()+node3.x()),
								0.25*(node0.y()+node1.y()+node2.y()+node3.y()),
								0.25*(node0.z()+node1.z()+node2.z()+node3.z()));
							drawSolidContourTriangle(&node0,&node1,&node,u0,u1,mean,colorMap);
							drawSolidContourTriangle(&node1,&node2,&node,u1,u2,mean,colorMap);
							drawSolidContourTriangle(&node2,&node3,&node,u2,u3,mean,colorMap);
							drawSolidContourTriangle(&node3,&node0,&node,u3,u0,mean,colorMap);
						}
						++eIter;
					}
				}
			}
		}else if( data->getValueType() == kmb::PhysicalValue::Tensor6 ){
			double v0[6] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0},
				v1[6] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0},
				v2[6] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0},
				v3[6] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0},
				u0 = 0.0, u1 =0.0, u2= 0.0, u3 = 0.0, mean = 0.0;
			if( colorMap->getStep() == 0 ){
				if(0 <= comp && comp < 6 ){
					while( !eIter.isFinished() )
					{
						if( eIter.getVertexCount() == 3 &&
							mesh->getNode( eIter[0], node0 ) &&
							mesh->getNode( eIter[1], node1 ) &&
							mesh->getNode( eIter[2], node2 ) &&
							data->getPhysicalValue( eIter[0], v0 ) &&
							data->getPhysicalValue( eIter[1], v1 ) &&
							data->getPhysicalValue( eIter[2], v2 ) )
						{
							drawContourTriangle(
								node0.x(), node0.y(), node0.z(), v0[comp],
								node1.x(), node1.y(), node1.z(), v1[comp],
								node2.x(), node2.y(), node2.z(), v2[comp], colorMap);
						}else if( eIter.getVertexCount() == 4 &&
							mesh->getNode( eIter[0], node0 ) &&
							mesh->getNode( eIter[1], node1 ) &&
							mesh->getNode( eIter[2], node2 ) &&
							mesh->getNode( eIter[3], node3 ) &&
							data->getPhysicalValue( eIter[0], v0 ) &&
							data->getPhysicalValue( eIter[1], v1 ) &&
							data->getPhysicalValue( eIter[2], v2 ) &&
							data->getPhysicalValue( eIter[3], v3 ) )
						{
							drawContourQuad(
								node0.x(), node0.y(), node0.z(), v0[comp],
								node1.x(), node1.y(), node1.z(), v1[comp],
								node2.x(), node2.y(), node2.z(), v2[comp],
								node3.x(), node3.y(), node3.z(), v3[comp], colorMap);
						}
						++eIter;
					}
				}
			}else{
				if(0 <= comp && comp < 6 ){
					while( !eIter.isFinished() )
					{
						if( eIter.getVertexCount() == 3 &&
							mesh->getNode( eIter[0], node0 ) &&
							mesh->getNode( eIter[1], node1 ) &&
							mesh->getNode( eIter[2], node2 ) &&
							data->getPhysicalValue( eIter[0], v0 ) &&
							data->getPhysicalValue( eIter[1], v1 ) &&
							data->getPhysicalValue( eIter[2], v2 ) )
						{
							drawSolidContourTriangle(
								&node0, &node1, &node2,
								v0[comp], v1[comp], v2[comp], colorMap);
						}else if( eIter.getVertexCount() == 4 &&
							mesh->getNode( eIter[0], node0 ) &&
							mesh->getNode( eIter[1], node1 ) &&
							mesh->getNode( eIter[2], node2 ) &&
							mesh->getNode( eIter[3], node3 ) &&
							data->getPhysicalValue( eIter[0], v0 ) &&
							data->getPhysicalValue( eIter[1], v1 ) &&
							data->getPhysicalValue( eIter[2], v2 ) &&
							data->getPhysicalValue( eIter[3], v3 ) )
						{
							u0 = v0[comp];
							u1 = v1[comp];
							u2 = v2[comp];
							u3 = v3[comp];
							mean = 0.25*(u0 + u1 + u2 + u3);
							kmb::Node node(
								0.25*(node0.x()+node1.x()+node2.x()+node3.x()),
								0.25*(node0.y()+node1.y()+node2.y()+node3.y()),
								0.25*(node0.z()+node1.z()+node2.z()+node3.z()));
							drawSolidContourTriangle(&node0,&node1,&node,u0,u1,mean,colorMap);
							drawSolidContourTriangle(&node1,&node2,&node,u1,u2,mean,colorMap);
							drawSolidContourTriangle(&node2,&node3,&node,u2,u3,mean,colorMap);
							drawSolidContourTriangle(&node3,&node0,&node,u3,u0,mean,colorMap);
						}
						++eIter;
					}
				}
			}
		}else if( data->getValueType() == kmb::PhysicalValue::Vector2withInt ){
			double v0[2] = {0.0, 0.0},
				v1[2] = {0.0, 0.0},
				v2[2] = {0.0, 0.0},
				v3[2] = {0.0, 0.0},
				u0 = 0.0, u1 =0.0, u2= 0.0, u3 = 0.0, mean = 0.0;
			long l0 = 0L, l1 = 0L, l2 = 0L, l3 = 0L;
			if( colorMap->getStep() == 0 ){
				if(0 <= comp && comp < 2 ){
					while( !eIter.isFinished() )
					{
						if( eIter.getVertexCount() == 3 &&
							mesh->getNode( eIter[0], node0 ) &&
							mesh->getNode( eIter[1], node1 ) &&
							mesh->getNode( eIter[2], node2 ) &&
							data->getPhysicalValue( eIter[0], v0 ) &&
							data->getPhysicalValue( eIter[1], v1 ) &&
							data->getPhysicalValue( eIter[2], v2 ) )
						{
							drawContourTriangle(
								node0.x(), node0.y(), node0.z(), v0[comp],
								node1.x(), node1.y(), node1.z(), v1[comp],
								node2.x(), node2.y(), node2.z(), v2[comp], colorMap);
						}else if( eIter.getVertexCount() == 4 &&
							mesh->getNode( eIter[0], node0 ) &&
							mesh->getNode( eIter[1], node1 ) &&
							mesh->getNode( eIter[2], node2 ) &&
							mesh->getNode( eIter[3], node3 ) &&
							data->getPhysicalValue( eIter[0], v0 ) &&
							data->getPhysicalValue( eIter[1], v1 ) &&
							data->getPhysicalValue( eIter[2], v2 ) &&
							data->getPhysicalValue( eIter[3], v3 ) )
						{
							drawContourQuad(
								node0.x(), node0.y(), node0.z(), v0[comp],
								node1.x(), node1.y(), node1.z(), v1[comp],
								node2.x(), node2.y(), node2.z(), v2[comp],
								node3.x(), node3.y(), node3.z(), v3[comp], colorMap);
						}
						++eIter;
					}
				}else if(comp == 2 ){
					while( !eIter.isFinished() )
					{
						if( eIter.getVertexCount() == 3 &&
							mesh->getNode( eIter[0], node0 ) &&
							mesh->getNode( eIter[1], node1 ) &&
							mesh->getNode( eIter[2], node2 ) &&
							data->getPhysicalValue( eIter[0], &l0 ) &&
							data->getPhysicalValue( eIter[1], &l1 ) &&
							data->getPhysicalValue( eIter[2], &l2 ) )
						{
							drawContourTriangle(
								node0.x(), node0.y(), node0.z(), static_cast<double>(l0),
								node1.x(), node1.y(), node1.z(), static_cast<double>(l1),
								node2.x(), node2.y(), node2.z(), static_cast<double>(l2), colorMap);
						}else if( eIter.getVertexCount() == 4 &&
							mesh->getNode( eIter[0], node0 ) &&
							mesh->getNode( eIter[1], node1 ) &&
							mesh->getNode( eIter[2], node2 ) &&
							mesh->getNode( eIter[3], node3 ) &&
							data->getPhysicalValue( eIter[0], &l0 ) &&
							data->getPhysicalValue( eIter[1], &l1 ) &&
							data->getPhysicalValue( eIter[2], &l2 ) &&
							data->getPhysicalValue( eIter[3], &l3 ) )
						{
							drawContourQuad(
								node0.x(), node0.y(), node0.z(), static_cast<double>(l0),
								node1.x(), node1.y(), node1.z(), static_cast<double>(l1),
								node2.x(), node2.y(), node2.z(), static_cast<double>(l2),
								node3.x(), node3.y(), node3.z(), static_cast<double>(l3), colorMap);
						}
						++eIter;
					}
				}
			}else{
				if(0 <= comp && comp < 2 ){
					while( !eIter.isFinished() )
					{
						if( eIter.getVertexCount() == 3 &&
							mesh->getNode( eIter[0], node0 ) &&
							mesh->getNode( eIter[1], node1 ) &&
							mesh->getNode( eIter[2], node2 ) &&
							data->getPhysicalValue( eIter[0], v0 ) &&
							data->getPhysicalValue( eIter[1], v1 ) &&
							data->getPhysicalValue( eIter[2], v2 ) )
						{
							drawSolidContourTriangle(
								&node0, &node1, &node2,
								v0[comp], v1[comp], v2[comp], colorMap);
						}else if( eIter.getVertexCount() == 4 &&
							mesh->getNode( eIter[0], node0 ) &&
							mesh->getNode( eIter[1], node1 ) &&
							mesh->getNode( eIter[2], node2 ) &&
							mesh->getNode( eIter[3], node3 ) &&
							data->getPhysicalValue( eIter[0], v0 ) &&
							data->getPhysicalValue( eIter[1], v1 ) &&
							data->getPhysicalValue( eIter[2], v2 ) &&
							data->getPhysicalValue( eIter[3], v3 ) )
						{
							u0 = v0[comp];
							u1 = v1[comp];
							u2 = v2[comp];
							u3 = v3[comp];
							mean = 0.25*(u0 + u1 + u2 + u3);
							kmb::Node node(
								0.25*(node0.x()+node1.x()+node2.x()+node3.x()),
								0.25*(node0.y()+node1.y()+node2.y()+node3.y()),
								0.25*(node0.z()+node1.z()+node2.z()+node3.z()));
							drawSolidContourTriangle(&node0,&node1,&node,u0,u1,mean,colorMap);
							drawSolidContourTriangle(&node1,&node2,&node,u1,u2,mean,colorMap);
							drawSolidContourTriangle(&node2,&node3,&node,u2,u3,mean,colorMap);
							drawSolidContourTriangle(&node3,&node0,&node,u3,u0,mean,colorMap);
						}
						++eIter;
					}
				}else if(comp == 2 ){
					while( !eIter.isFinished() )
					{
						if( eIter.getVertexCount() == 3 &&
							mesh->getNode( eIter[0], node0 ) &&
							mesh->getNode( eIter[1], node1 ) &&
							mesh->getNode( eIter[2], node2 ) &&
							data->getPhysicalValue( eIter[0], &l0 ) &&
							data->getPhysicalValue( eIter[1], &l1 ) &&
							data->getPhysicalValue( eIter[2], &l2 ) )
						{
							drawSolidContourTriangle(
								&node0, &node1, &node2,
								static_cast<double>(l0), static_cast<double>(l1), static_cast<double>(l2), colorMap);
						}else if( eIter.getVertexCount() == 4 &&
							mesh->getNode( eIter[0], node0 ) &&
							mesh->getNode( eIter[1], node1 ) &&
							mesh->getNode( eIter[2], node2 ) &&
							mesh->getNode( eIter[3], node3 ) &&
							data->getPhysicalValue( eIter[0], &l0 ) &&
							data->getPhysicalValue( eIter[1], &l1 ) &&
							data->getPhysicalValue( eIter[2], &l2 ) &&
							data->getPhysicalValue( eIter[3], &l3 ) )
						{
							mean = 0.25 * static_cast<double>( (l0+l1+l2+l3) );
							kmb::Node node(
								0.25*(node0.x()+node1.x()+node2.x()+node3.x()),
								0.25*(node0.y()+node1.y()+node2.y()+node3.y()),
								0.25*(node0.z()+node1.z()+node2.z()+node3.z()));
							drawSolidContourTriangle(&node0,&node1,&node,
								static_cast<double>(l0),static_cast<double>(l1),mean,colorMap);
							drawSolidContourTriangle(&node1,&node2,&node,
								static_cast<double>(l1),static_cast<double>(l2),mean,colorMap);
							drawSolidContourTriangle(&node2,&node3,&node,
								static_cast<double>(l2),static_cast<double>(l3),mean,colorMap);
							drawSolidContourTriangle(&node3,&node0,&node,
								static_cast<double>(l3),static_cast<double>(l0),mean,colorMap);
						}
						++eIter;
					}
				}
			}
		}
		::glPopAttrib();
		::glFlush();
	}
}

// comp 未対応
void
kmb::MeshGL::drawSurfaceNodeContour2D(kmb::bodyIdType bodyId,const char* physicalValue,kmb::ColorMap* colorMap,int comp)
{
	const kmb::DataBindings* data = NULL;
	const kmb::Point2DContainer* points = NULL;
	const kmb::Body* body = NULL;
	if( mesh == NULL ||
		(points = mesh->getNode2Ds()) == NULL ||
		colorMap == NULL ||
		(body = mesh->getBodyPtr(bodyId)) == NULL ||
		body->isUniqueDim(2) == false ||
		(data = mesh->getDataBindingsPtr( physicalValue )) == NULL ||
		data->getBindingMode() != kmb::DataBindings::NodeVariable )
	{
		return;
	}

	::glPushAttrib( GL_ENABLE_BIT );
	::glEnable(GL_COLOR_MATERIAL);

	if( data->getValueType() == kmb::PhysicalValue::Scalar ){
		kmb::ElementContainer::const_iterator eIter = body->begin();
		double x0=0.0, y0=0.0, x1=0.0, y1=0.0, x2=0.0, y2=0.0, x3=0.0, y3=0.0;
		double v0 = 0.0, v1 = 0.0, v2 = 0.0, v3 = 0.0, mean = 0.0;
		double xm=0.0, ym=0.0;

		if( colorMap->getStep() == 0 ){
			while( !eIter.isFinished() )
			{
				if( eIter.getVertexCount() == 3 &&
					points->getXY( eIter[0], x0, y0 ) &&
					points->getXY( eIter[1], x1, y1 ) &&
					points->getXY( eIter[2], x2, y2 ) &&
					data->getPhysicalValue( eIter[0], &v0 ) &&
					data->getPhysicalValue( eIter[1], &v1 ) &&
					data->getPhysicalValue( eIter[2], &v2 ) )
				{
					drawContourTriangle2D(
						x0,y0,v0,
						x1,y1,v1,
						x2,y2,v2,colorMap);
				}else if( eIter.getVertexCount() == 4 &&
					points->getXY( eIter[0], x0, y0 ) &&
					points->getXY( eIter[1], x1, y1 ) &&
					points->getXY( eIter[2], x2, y2 ) &&
					points->getXY( eIter[3], x3, y3 ) &&
					data->getPhysicalValue( eIter[0], &v0 ) &&
					data->getPhysicalValue( eIter[1], &v1 ) &&
					data->getPhysicalValue( eIter[2], &v2 ) &&
					data->getPhysicalValue( eIter[3], &v3 ) )
				{
					drawContourQuad2D(
						x0,y0,v0,
						x1,y1,v1,
						x2,y2,v2,
						x3,y3,v3,colorMap);
				}
				++eIter;
			}
		}else{
			while( !eIter.isFinished() )
			{
				if( eIter.getVertexCount() == 3 &&
					points->getXY( eIter[0], x0, y0 ) &&
					points->getXY( eIter[1], x1, y1 ) &&
					points->getXY( eIter[2], x2, y2 ) &&
					data->getPhysicalValue( eIter[0], &v0 ) &&
					data->getPhysicalValue( eIter[1], &v1 ) &&
					data->getPhysicalValue( eIter[2], &v2 ) )
				{
					drawSolidContourTriangle2D(
						x0,y0,v0,
						x1,y1,v1,
						x2,y2,v2,
						colorMap);
				}else if( eIter.getVertexCount() == 4 &&
					points->getXY( eIter[0], x0, y0 ) &&
					points->getXY( eIter[1], x1, y1 ) &&
					points->getXY( eIter[2], x2, y2 ) &&
					points->getXY( eIter[3], x3, y3 ) &&
					data->getPhysicalValue( eIter[0], &v0 ) &&
					data->getPhysicalValue( eIter[1], &v1 ) &&
					data->getPhysicalValue( eIter[2], &v2 ) &&
					data->getPhysicalValue( eIter[3], &v3 ) )
				{
					mean = 0.25*(v0 + v1 + v2 + v3);
					xm = 0.25*(x0 + x1 + x2 + x3);
					ym = 0.25*(y0 + y1 + y2 + y3);
					drawSolidContourTriangle2D(
						x0,y0,v0,
						x1,y1,v1,
						xm,ym,mean,
						colorMap);
					drawSolidContourTriangle2D(
						x1,y1,v1,
						x2,y2,v2,
						xm,ym,mean,
						colorMap);
					drawSolidContourTriangle2D(
						x2,y2,v2,
						x3,y3,v3,
						xm,ym,mean,
						colorMap);
					drawSolidContourTriangle2D(
						x3,y3,v3,
						x0,y0,v0,
						xm,ym,mean,
						colorMap);
				}
				++eIter;
			}
		}
	}else if( data->getValueType() == kmb::PhysicalValue::Vector2 ){
		kmb::Point2D node0,node1,node2,node3;
		double v0[2] = {0.0, 0.0},
			v1[2] = {0.0, 0.0},
			v2[2] = {0.0, 0.0},
			v3[2] = {0.0, 0.0};
		if( colorMap->getStep() == 0 ){
			if( comp==-1 ){
				kmb::ElementContainer::const_iterator eIter = body->begin();
				while( !eIter.isFinished() ){
					if( eIter.getLinearType() == kmb::TRIANGLE &&
						points->getPoint( eIter[0], node0 ) &&
						points->getPoint( eIter[1], node1 ) &&
						points->getPoint( eIter[2], node2 ) &&
						data->getPhysicalValue( eIter[0], v0 ) &&
						data->getPhysicalValue( eIter[1], v1 ) &&
						data->getPhysicalValue( eIter[2], v2 ) )
					{
						drawContourTriangle2D(
							node0.x(), node0.y(), kmb::Vector2D::abs(v0),
							node1.x(), node1.y(), kmb::Vector2D::abs(v1),
							node2.x(), node2.y(), kmb::Vector2D::abs(v2),
							colorMap);
					}else if( eIter.getLinearType() == kmb::QUAD &&
						points->getPoint( eIter[0], node0 ) &&
						points->getPoint( eIter[1], node1 ) &&
						points->getPoint( eIter[2], node2 ) &&
						points->getPoint( eIter[3], node3 ) &&
						data->getPhysicalValue( eIter[0], v0 ) &&
						data->getPhysicalValue( eIter[1], v1 ) &&
						data->getPhysicalValue( eIter[2], v2 ) &&
						data->getPhysicalValue( eIter[3], v3 ) )
					{
						drawContourQuad2D(
							node0.x(), node0.y(), kmb::Vector2D::abs(v0),
							node1.x(), node1.y(), kmb::Vector2D::abs(v1),
							node2.x(), node2.y(), kmb::Vector2D::abs(v2),
							node3.x(), node3.y(), kmb::Vector2D::abs(v3),
							colorMap);
					}
					++eIter;
				}
			}else if(0 <= comp && comp < 2 ){
				kmb::ElementContainer::const_iterator eIter = body->begin();
				while( !eIter.isFinished() ){
					if( eIter.getLinearType() == kmb::TRIANGLE &&
						points->getPoint( eIter[0], node0 ) &&
						points->getPoint( eIter[1], node1 ) &&
						points->getPoint( eIter[2], node2 ) &&
						data->getPhysicalValue( eIter[0], v0 ) &&
						data->getPhysicalValue( eIter[1], v1 ) &&
						data->getPhysicalValue( eIter[2], v2 ) )
					{
						drawContourTriangle2D(
							node0.x(), node0.y(), v0[comp],
							node1.x(), node1.y(), v1[comp],
							node2.x(), node2.y(), v2[comp],
							colorMap);
					}else if( eIter.getLinearType() == kmb::QUAD &&
						points->getPoint( eIter[0], node0 ) &&
						points->getPoint( eIter[1], node1 ) &&
						points->getPoint( eIter[2], node2 ) &&
						points->getPoint( eIter[3], node3 ) &&
						data->getPhysicalValue( eIter[0], v0 ) &&
						data->getPhysicalValue( eIter[1], v1 ) &&
						data->getPhysicalValue( eIter[2], v2 ) &&
						data->getPhysicalValue( eIter[3], v3 ) )
					{
						drawContourQuad2D(
							node0.x(), node0.y(), v0[comp],
							node1.x(), node1.y(), v1[comp],
							node2.x(), node2.y(), v2[comp],
							node3.x(), node3.y(), v3[comp],
							colorMap);
					}
					++eIter;
				}
			}
		}else{
			double u0 = 0.0, u1 = 0.0, u2 = 0.0, u3 = 0.0, mean = 0.0;
			double xm=0.0, ym=0.0;
			if( comp==-1 ){
				kmb::ElementContainer::const_iterator eIter = body->begin();
				while( !eIter.isFinished() ){
					if( eIter.getLinearType() == kmb::TRIANGLE &&
						points->getPoint( eIter[0], node0 ) &&
						points->getPoint( eIter[1], node1 ) &&
						points->getPoint( eIter[2], node2 ) &&
						data->getPhysicalValue( eIter[0], v0 ) &&
						data->getPhysicalValue( eIter[1], v1 ) &&
						data->getPhysicalValue( eIter[2], v2 ) )
					{
						drawSolidContourTriangle2D(
							node0.x(), node0.y(), kmb::Vector2D::abs(v0),
							node1.x(), node1.y(), kmb::Vector2D::abs(v1),
							node2.x(), node2.y(), kmb::Vector2D::abs(v2),
							colorMap);
					}else if( eIter.getLinearType() == kmb::QUAD &&
						points->getPoint( eIter[0], node0 ) &&
						points->getPoint( eIter[1], node1 ) &&
						points->getPoint( eIter[2], node2 ) &&
						points->getPoint( eIter[3], node3 ) &&
						data->getPhysicalValue( eIter[0], v0 ) &&
						data->getPhysicalValue( eIter[1], v1 ) &&
						data->getPhysicalValue( eIter[2], v2 ) &&
						data->getPhysicalValue( eIter[3], v3 ) )
					{
						u0 = kmb::Vector2D::abs(v0),
						u1 = kmb::Vector2D::abs(v1),
						u2 = kmb::Vector2D::abs(v2),
						u3 = kmb::Vector2D::abs(v3),
						mean = 0.25*(u0+u1+u2+u3);
						xm = 0.25*(node0.x() + node1.x() + node2.x() + node3.x());
						ym = 0.25*(node0.y() + node1.y() + node2.y() + node3.y());
						drawSolidContourTriangle2D(
							node0.x(),node0.y(),u0,
							node1.x(),node1.y(),u1,
							xm,ym,mean,
							colorMap);
						drawSolidContourTriangle2D(
							node1.x(),node1.y(),u1,
							node2.x(),node2.y(),u2,
							xm,ym,mean,
							colorMap);
						drawSolidContourTriangle2D(
							node2.x(),node2.y(),u2,
							node3.x(),node3.y(),u3,
							xm,ym,mean,
							colorMap);
						drawSolidContourTriangle2D(
							node3.x(),node3.y(),u3,
							node0.x(),node0.y(),u0,
							xm,ym,mean,
							colorMap);
					}
					++eIter;
				}
			}else if(0 <= comp && comp < 2 ){
				kmb::ElementContainer::const_iterator eIter = body->begin();
				while( !eIter.isFinished() ){
					if( eIter.getLinearType() == kmb::TRIANGLE &&
						points->getPoint( eIter[0], node0 ) &&
						points->getPoint( eIter[1], node1 ) &&
						points->getPoint( eIter[2], node2 ) &&
						data->getPhysicalValue( eIter[0], v0 ) &&
						data->getPhysicalValue( eIter[1], v1 ) &&
						data->getPhysicalValue( eIter[2], v2 ) )
					{
						drawSolidContourTriangle2D(
							node0.x(), node0.y(), v0[comp],
							node1.x(), node1.y(), v1[comp],
							node2.x(), node2.y(), v2[comp],
							colorMap);
					}else if( eIter.getLinearType() == kmb::QUAD &&
						points->getPoint( eIter[0], node0 ) &&
						points->getPoint( eIter[1], node1 ) &&
						points->getPoint( eIter[2], node2 ) &&
						points->getPoint( eIter[3], node3 ) &&
						data->getPhysicalValue( eIter[0], v0 ) &&
						data->getPhysicalValue( eIter[1], v1 ) &&
						data->getPhysicalValue( eIter[2], v2 ) &&
						data->getPhysicalValue( eIter[3], v3 ) )
					{
						mean = 0.25*(v0[comp]+v1[comp]+v2[comp]+v3[comp]);
						xm = 0.25*(node0.x() + node1.x() + node2.x() + node3.x());
						ym = 0.25*(node0.y() + node1.y() + node2.y() + node3.y());
						drawSolidContourTriangle2D(
							node0.x(),node0.y(),v0[comp],
							node1.x(),node1.y(),v1[comp],
							xm,ym,mean,
							colorMap);
						drawSolidContourTriangle2D(
							node1.x(),node1.y(),v1[comp],
							node2.x(),node2.y(),v2[comp],
							xm,ym,mean,
							colorMap);
						drawSolidContourTriangle2D(
							node2.x(),node2.y(),v2[comp],
							node3.x(),node3.y(),v3[comp],
							xm,ym,mean,
							colorMap);
						drawSolidContourTriangle2D(
							node3.x(),node3.y(),v3[comp],
							node0.x(),node0.y(),v0[comp],
							xm,ym,mean,
							colorMap);
					}
					++eIter;
				}
			}
		}
	}
	::glPopAttrib();
	::glFlush();
}

void
kmb::MeshGL::drawNodeContourOnSurface(kmb::bodyIdType bodyId,const char* physicalValue,kmb::ColorMap* colorMap,int comp)
{
	const kmb::DataBindings* data = NULL;
	const kmb::Body* body = NULL;
	if( mesh != NULL && colorMap != NULL &&
		(body = mesh->getBodyPtr(bodyId)) != NULL &&
		body->isUniqueDim(2) &&
		(data = mesh->getDataBindingsPtr( physicalValue )) != NULL &&
		data->getBindingMode() == kmb::DataBindings::NodeVariable )
	{
		::glBegin( GL_POINTS );
		std::set< kmb::nodeIdType > nodeSet;
		kmb::Node node;
		kmb::ElementContainer::const_iterator eIter = body->begin();
		if( data->getValueType() == kmb::PhysicalValue::Scalar ){
			double v = 0.0;
			while( !eIter.isFinished() )
			{
				int nodeCount = eIter.getNodeCount();
				for(int i=0;i<nodeCount;++i){
					kmb::nodeIdType nodeId = eIter[i];
					if( nodeSet.insert(nodeId).second &&
						mesh->getNode( nodeId, node ) &&
						data->getPhysicalValue( nodeId, &v ) )
					{
						colorMap->setGLColor( v );
						::glVertex3d( node.x(), node.y(), node.z() );
					}
				}
				++eIter;
			}
		}else if( data->getValueType() == kmb::PhysicalValue::Vector3 ){
			double v[3] = {0.0, 0.0, 0.0};
			if( comp==-1 ){
				while( !eIter.isFinished() )
				{
					int nodeCount = eIter.getNodeCount();
					for(int i=0;i<nodeCount;++i){
						kmb::nodeIdType nodeId = eIter[i];
						if( nodeSet.insert(nodeId).second &&
							mesh->getNode( nodeId, node ) &&
							data->getPhysicalValue( nodeId, v ) )
						{
							colorMap->setGLColor( kmb::Vector3D::abs(v) );
							::glVertex3d( node.x(), node.y(), node.z() );
						}
					}
					++eIter;
				}
			}else if(0 <= comp && comp < 3 ){
				while( !eIter.isFinished() )
				{
					int nodeCount = eIter.getNodeCount();
					for(int i=0;i<nodeCount;++i){
						kmb::nodeIdType nodeId = eIter[i];
						if( nodeSet.insert(nodeId).second &&
							mesh->getNode( nodeId, node ) &&
							data->getPhysicalValue( nodeId, v ) )
						{
							colorMap->setGLColor( v[comp] );
							::glVertex3d( node.x(), node.y(), node.z() );
						}
					}
					++eIter;
				}
			}
		}else if( data->getValueType() == kmb::PhysicalValue::Tensor6 ){
			double v[6] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
			if(0 <= comp && comp < 6 ){
				while( !eIter.isFinished() )
				{
					int nodeCount = eIter.getNodeCount();
					for(int i=0;i<nodeCount;++i){
						kmb::nodeIdType nodeId = eIter[i];
						if( nodeSet.insert(nodeId).second &&
							mesh->getNode( nodeId, node ) &&
							data->getPhysicalValue( nodeId, v ) )
						{
							colorMap->setGLColor( v[comp] );
							::glVertex3d( node.x(), node.y(), node.z() );
						}
					}
					++eIter;
				}
			}
		}else if( data->getValueType() == kmb::PhysicalValue::Vector2withInt && 0 <= comp && comp < 2 ){
			double v[2] = {0.0, 0.0};
			while( !eIter.isFinished() )
			{
				int nodeCount = eIter.getNodeCount();
				for(int i=0;i<nodeCount;++i){
					kmb::nodeIdType nodeId = eIter[i];
					if( nodeSet.insert(nodeId).second &&
						mesh->getNode( nodeId, node ) &&
						data->getPhysicalValue( nodeId, v ) )
					{
						colorMap->setGLColor( v[comp] );
						::glVertex3d( node.x(), node.y(), node.z() );
					}
				}
				++eIter;
			}
		}else if( data->getValueType() == kmb::PhysicalValue::Vector2withInt && comp == 2 ){
			long v = 0L;
			while( !eIter.isFinished() )
			{
				int nodeCount = eIter.getNodeCount();
				for(int i=0;i<nodeCount;++i){
					kmb::nodeIdType nodeId = eIter[i];
					if( nodeSet.insert(nodeId).second &&
						mesh->getNode( nodeId, node ) &&
						data->getPhysicalValue( nodeId, &v ) )
					{
						colorMap->setGLColor( static_cast<double>(v) );
						::glVertex3d( node.x(), node.y(), node.z() );
					}
				}
				++eIter;
			}
		}else if( data->getValueType() == kmb::PhysicalValue::Integer ){
			long v = 0L;
			while( !eIter.isFinished() )
			{
				int nodeCount = eIter.getNodeCount();
				for(int i=0;i<nodeCount;++i){
					kmb::nodeIdType nodeId = eIter[i];
					if( nodeSet.insert(nodeId).second &&
						mesh->getNode( nodeId, node ) &&
						data->getPhysicalValue( nodeId, &v ) )
					{
						colorMap->setGLColorByStep( v );
						::glVertex3d( node.x(), node.y(), node.z() );
					}
				}
				++eIter;
			}
		}
		::glEnd();
		::glFlush();
	}
}

void
kmb::MeshGL::drawNodeContour2DOnSurface(kmb::bodyIdType bodyId,const char* physicalValue,kmb::ColorMap* colorMap,int comp)
{
	const kmb::DataBindings* data = NULL;
	const kmb::Point2DContainer* points = NULL;
	const kmb::Body* body = NULL;
	if( mesh != NULL && colorMap != NULL &&
		(points = mesh->getNode2Ds()) != NULL &&
		(body = mesh->getBodyPtr(bodyId)) != NULL &&
		body->isUniqueDim(2) &&
		(data = mesh->getDataBindingsPtr( physicalValue )) != NULL &&
		data->getBindingMode() == kmb::DataBindings::NodeVariable )
	{
		::glBegin( GL_POINTS );
		std::set< kmb::nodeIdType > nodeSet;
		double x=0.0,y=0.0;
		kmb::ElementContainer::const_iterator eIter = body->begin();
		if( data->getValueType() == kmb::PhysicalValue::Scalar ){
			double v = 0.0;
			while( !eIter.isFinished() )
			{
				int nodeCount = eIter.getNodeCount();
				for(int i=0;i<nodeCount;++i){
					kmb::nodeIdType nodeId = eIter[i];
					if( nodeSet.insert(nodeId).second &&
						points->getXY( nodeId, x, y ) &&
						data->getPhysicalValue( nodeId, &v ) )
					{
						colorMap->setGLColor( v );
						::glVertex2d( x, y );
					}
				}
				++eIter;
			}
		}else if( data->getValueType() == kmb::PhysicalValue::Vector3 ){
			double v[3] = {0.0, 0.0, 0.0};
			if( comp == -1 ){
				while( !eIter.isFinished() )
				{
					int nodeCount = eIter.getNodeCount();
					for(int i=0;i<nodeCount;++i){
						kmb::nodeIdType nodeId = eIter[i];
						if( nodeSet.insert(nodeId).second &&
							points->getXY( nodeId, x, y ) &&
							data->getPhysicalValue( nodeId, v ) )
						{
							colorMap->setGLColor( kmb::Vector3D::abs(v) );
							::glVertex2d( x, y );
						}
					}
					++eIter;
				}
			}else if( 0 <= comp && comp < 3 ){
				while( !eIter.isFinished() )
				{
					int nodeCount = eIter.getNodeCount();
					for(int i=0;i<nodeCount;++i){
						kmb::nodeIdType nodeId = eIter[i];
						if( nodeSet.insert(nodeId).second &&
							points->getXY( nodeId, x, y ) &&
							data->getPhysicalValue( nodeId, v ) )
						{
							colorMap->setGLColor( v[comp] );
							::glVertex2d( x, y );
						}
					}
					++eIter;
				}
			}
		}else if( data->getValueType() == kmb::PhysicalValue::Tensor6 ){
			double v[6] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
			if( 0 <= comp && comp < 6 ){
				while( !eIter.isFinished() )
				{
					int nodeCount = eIter.getNodeCount();
					for(int i=0;i<nodeCount;++i){
						kmb::nodeIdType nodeId = eIter[i];
						if( nodeSet.insert(nodeId).second &&
							points->getXY( nodeId, x, y ) &&
							data->getPhysicalValue( nodeId, v ) )
						{
							colorMap->setGLColor( v[comp] );
							::glVertex2d( x, y );
						}
					}
					++eIter;
				}
			}
		}else if( data->getValueType() == kmb::PhysicalValue::Integer ){
			long v = 0L;
			while( !eIter.isFinished() )
			{
				int nodeCount = eIter.getNodeCount();
				for(int i=0;i<nodeCount;++i){
					kmb::nodeIdType nodeId = eIter[i];
					if( nodeSet.insert(nodeId).second &&
						points->getXY( nodeId, x, y ) &&
						data->getPhysicalValue( nodeId, &v ) )
					{
						colorMap->setGLColorByStep( v );
						::glVertex2d( x, y );
					}
				}
				++eIter;
			}
		}
		::glEnd();
		::glFlush();
	}
}

void
kmb::MeshGL::deformSurfaceNodeContour(kmb::bodyIdType bodyId, const char* displacement, double factor, const char* physicalValue, kmb::ColorMap* colorMap,int comp)
{
	const kmb::Body* body = NULL;
	const kmb::DataBindings* displace = NULL;
	const kmb::DataBindings* nodeValue = NULL;
	if( mesh != NULL && colorMap != NULL &&
		(body = mesh->getBodyPtr(bodyId)) != NULL &&
		body->isUniqueDim(2) &&
		(displace = mesh->getDataBindingsPtr( displacement )) != NULL &&
		displace->getBindingMode() == kmb::DataBindings::NodeVariable &&
		displace->getValueType() == kmb::PhysicalValue::Vector3 &&
		(nodeValue = mesh->getDataBindingsPtr( physicalValue )) != NULL &&
		nodeValue->getBindingMode() == kmb::DataBindings::NodeVariable )
	{
		::glPushAttrib( GL_ENABLE_BIT );
		::glEnable( GL_COLOR_MATERIAL );

		kmb::nodeIdType
			nodeId0 = kmb::nullNodeId,
			nodeId1 = kmb::nullNodeId,
			nodeId2 = kmb::nullNodeId,
			nodeId3 = kmb::nullNodeId;
		kmb::Node node0,node1,node2,node3,n0,n1,n2,n3,center;
		double
			d0[3] = {0.0, 0.0, 0.0},
			d1[3] = {0.0, 0.0, 0.0},
			d2[3] = {0.0, 0.0, 0.0},
			d3[3] = {0.0, 0.0, 0.0};
		kmb::ElementContainer::const_iterator eIter = body->begin();
		if( nodeValue->getValueType() == kmb::PhysicalValue::Scalar ){
			double v0 = 0.0, v1 = 0.0, v2 = 0.0, v3 = 0.0, mean = 0.0;
			if( colorMap->getStep() == 0 ){
				while( !eIter.isFinished() ){
					switch( eIter.getVertexCount() ){
						case 3:
							nodeId0 = eIter[0];
							nodeId1 = eIter[1];
							nodeId2 = eIter[2];
							if( mesh->getNode( nodeId0, node0 ) &&
								mesh->getNode( nodeId1, node1 ) &&
								mesh->getNode( nodeId2, node2 ) &&
								displace->getPhysicalValue( nodeId0, d0 ) &&
								displace->getPhysicalValue( nodeId1, d1 ) &&
								displace->getPhysicalValue( nodeId2, d2 ) &&
								nodeValue->getPhysicalValue( nodeId0, &v0 ) &&
								nodeValue->getPhysicalValue( nodeId1, &v1 ) &&
								nodeValue->getPhysicalValue( nodeId2, &v2 ) )
							{
								drawContourTriangle(
									node0.x() + factor * d0[0],
									node0.y() + factor * d0[1],
									node0.z() + factor * d0[2],
									v0,
									node1.x() + factor * d1[0],
									node1.y() + factor * d1[1],
									node1.z() + factor * d1[2],
									v1,
									node2.x() + factor * d2[0],
									node2.y() + factor * d2[1],
									node2.z() + factor * d2[2],
									v2,
									colorMap );
							}
							break;
						case 4:
							nodeId0 = eIter[0];
							nodeId1 = eIter[1];
							nodeId2 = eIter[2];
							nodeId3 = eIter[3];
							if( mesh->getNode( nodeId0, node0 ) &&
								mesh->getNode( nodeId1, node1 ) &&
								mesh->getNode( nodeId2, node2 ) &&
								mesh->getNode( nodeId3, node3 ) &&
								displace->getPhysicalValue( nodeId0, d0 ) &&
								displace->getPhysicalValue( nodeId1, d1 ) &&
								displace->getPhysicalValue( nodeId2, d2 ) &&
								displace->getPhysicalValue( nodeId3, d3 ) &&
								nodeValue->getPhysicalValue( nodeId0, &v0 ) &&
								nodeValue->getPhysicalValue( nodeId1, &v1 ) &&
								nodeValue->getPhysicalValue( nodeId2, &v2 ) &&
								nodeValue->getPhysicalValue( nodeId3, &v3 ) )
							{
								drawContourQuad(
									node0.x() + factor * d0[0],
									node0.y() + factor * d0[1],
									node0.z() + factor * d0[2],
									v0,
									node1.x() + factor * d1[0],
									node1.y() + factor * d1[1],
									node1.z() + factor * d1[2],
									v1,
									node2.x() + factor * d2[0],
									node2.y() + factor * d2[1],
									node2.z() + factor * d2[2],
									v2,
									node3.x() + factor * d3[0],
									node3.y() + factor * d3[1],
									node3.z() + factor * d3[2],
									v3,
									colorMap );
							}
							break;
						default:
							break;
					}
					++eIter;
				}
			}else{ // solid contour
				while( !eIter.isFinished() ){
					switch( eIter.getVertexCount() ){
						case 3:
							nodeId0 = eIter[0];
							nodeId1 = eIter[1];
							nodeId2 = eIter[2];
							if( mesh->getNode( nodeId0, node0 ) &&
								mesh->getNode( nodeId1, node1 ) &&
								mesh->getNode( nodeId2, node2 ) &&
								displace->getPhysicalValue( nodeId0, d0 ) &&
								displace->getPhysicalValue( nodeId1, d1 ) &&
								displace->getPhysicalValue( nodeId2, d2 ) &&
								nodeValue->getPhysicalValue( nodeId0, &v0 ) &&
								nodeValue->getPhysicalValue( nodeId1, &v1 ) &&
								nodeValue->getPhysicalValue( nodeId2, &v2 ) )
							{
								n0.setCoordinate(
									node0.x() + factor * d0[0],
									node0.y() + factor * d0[1],
									node0.z() + factor * d0[2]);
								n1.setCoordinate(
									node1.x() + factor * d1[0],
									node1.y() + factor * d1[1],
									node1.z() + factor * d1[2]);
								n2.setCoordinate(
									node2.x() + factor * d2[0],
									node2.y() + factor * d2[1],
									node2.z() + factor * d2[2]);
								drawSolidContourTriangle(&n0,&n1,&n2,v0,v1,v2,colorMap);
							}
							break;
						case 4:
							nodeId0 = eIter[0];
							nodeId1 = eIter[1];
							nodeId2 = eIter[2];
							nodeId3 = eIter[3];
							if( mesh->getNode( nodeId0, node0 ) &&
								mesh->getNode( nodeId1, node1 ) &&
								mesh->getNode( nodeId2, node2 ) &&
								mesh->getNode( nodeId3, node3 ) &&
								displace->getPhysicalValue( nodeId0, d0 ) &&
								displace->getPhysicalValue( nodeId1, d1 ) &&
								displace->getPhysicalValue( nodeId2, d2 ) &&
								displace->getPhysicalValue( nodeId3, d3 ) &&
								nodeValue->getPhysicalValue( nodeId0, &v0 ) &&
								nodeValue->getPhysicalValue( nodeId1, &v1 ) &&
								nodeValue->getPhysicalValue( nodeId2, &v2 ) &&
								nodeValue->getPhysicalValue( nodeId3, &v3 ) )
							{
								n0.setCoordinate(
									node0.x() + factor * d0[0],
									node0.y() + factor * d0[1],
									node0.z() + factor * d0[2]);
								n1.setCoordinate(
									node1.x() + factor * d1[0],
									node1.y() + factor * d1[1],
									node1.z() + factor * d1[2]);
								n2.setCoordinate(
									node2.x() + factor * d2[0],
									node2.y() + factor * d2[1],
									node2.z() + factor * d2[2]);
								n3.setCoordinate(
									node3.x() + factor * d3[0],
									node3.y() + factor * d3[1],
									node3.z() + factor * d3[2]);
								center.setCoordinate(
									0.25*(n0.x()+n1.x()+n2.x()+n3.x()),
									0.25*(n0.y()+n1.y()+n2.y()+n3.y()),
									0.25*(n0.z()+n1.z()+n2.z()+n3.z()));
								mean = 0.25*(v0 + v1 + v2 + v3);
								drawSolidContourTriangle(&n0,&n1,&center,v0,v1,mean,colorMap);
								drawSolidContourTriangle(&n1,&n2,&center,v1,v2,mean,colorMap);
								drawSolidContourTriangle(&n2,&n3,&center,v2,v3,mean,colorMap);
								drawSolidContourTriangle(&n3,&n0,&center,v3,v0,mean,colorMap);
							}
							break;
						default:
							break;
					}
					++eIter;
				}
			}
		}else if( nodeValue->getValueType() == kmb::PhysicalValue::Vector3 && comp == -1){
			double v0[] = {0.0, 0.0, 0.0},
				v1[] = {0.0, 0.0, 0.0},
				v2[] = {0.0, 0.0, 0.0},
				v3[] = {0.0, 0.0, 0.0},
				u0 = 0.0, u1 =0.0, u2= 0.0, u3 = 0.0, mean = 0.0;
			if( colorMap->getStep() == 0 ){
				while( !eIter.isFinished() ){
					switch( eIter.getVertexCount() ){
						case 3:
							nodeId0 = eIter[0];
							nodeId1 = eIter[1];
							nodeId2 = eIter[2];
							if( mesh->getNode( nodeId0, node0 ) &&
								mesh->getNode( nodeId1, node1 ) &&
								mesh->getNode( nodeId2, node2 ) &&
								displace->getPhysicalValue( nodeId0, d0 ) &&
								displace->getPhysicalValue( nodeId1, d1 ) &&
								displace->getPhysicalValue( nodeId2, d2 ) &&
								nodeValue->getPhysicalValue( nodeId0, v0 ) &&
								nodeValue->getPhysicalValue( nodeId1, v1 ) &&
								nodeValue->getPhysicalValue( nodeId2, v2 ) )
							{
								drawContourTriangle(
									node0.x() + factor * d0[0],
									node0.y() + factor * d0[1],
									node0.z() + factor * d0[2],
									kmb::Vector3D::abs(v0),
									node1.x() + factor * d1[0],
									node1.y() + factor * d1[1],
									node1.z() + factor * d1[2],
									kmb::Vector3D::abs(v1),
									node2.x() + factor * d2[0],
									node2.y() + factor * d2[1],
									node2.z() + factor * d2[2],
									kmb::Vector3D::abs(v2),
									colorMap );
							}
							break;
						case 4:
							nodeId0 = eIter[0];
							nodeId1 = eIter[1];
							nodeId2 = eIter[2];
							nodeId3 = eIter[3];
							if( mesh->getNode( nodeId0, node0 ) &&
								mesh->getNode( nodeId1, node1 ) &&
								mesh->getNode( nodeId2, node2 ) &&
								mesh->getNode( nodeId3, node3 ) &&
								displace->getPhysicalValue( nodeId0, d0 ) &&
								displace->getPhysicalValue( nodeId1, d1 ) &&
								displace->getPhysicalValue( nodeId2, d2 ) &&
								displace->getPhysicalValue( nodeId3, d3 ) &&
								nodeValue->getPhysicalValue( nodeId0, v0 ) &&
								nodeValue->getPhysicalValue( nodeId1, v1 ) &&
								nodeValue->getPhysicalValue( nodeId2, v2 ) &&
								nodeValue->getPhysicalValue( nodeId3, v3 ) )
							{
								drawContourQuad(
									node0.x() + factor * d0[0],
									node0.y() + factor * d0[1],
									node0.z() + factor * d0[2],
									kmb::Vector3D::abs(v0),
									node1.x() + factor * d1[0],
									node1.y() + factor * d1[1],
									node1.z() + factor * d1[2],
									kmb::Vector3D::abs(v1),
									node2.x() + factor * d2[0],
									node2.y() + factor * d2[1],
									node2.z() + factor * d2[2],
									kmb::Vector3D::abs(v2),
									node3.x() + factor * d3[0],
									node3.y() + factor * d3[1],
									node3.z() + factor * d3[2],
									kmb::Vector3D::abs(v3),
									colorMap );
							}
							break;
						default:
							break;
					}
					++eIter;
				}
			}else{ // solid contour
				while( !eIter.isFinished() ){
					switch( eIter.getVertexCount() ){
						case 3:
							nodeId0 = eIter[0];
							nodeId1 = eIter[1];
							nodeId2 = eIter[2];
							if( mesh->getNode( nodeId0, node0 ) &&
								mesh->getNode( nodeId1, node1 ) &&
								mesh->getNode( nodeId2, node2 ) &&
								displace->getPhysicalValue( nodeId0, d0 ) &&
								displace->getPhysicalValue( nodeId1, d1 ) &&
								displace->getPhysicalValue( nodeId2, d2 ) &&
								nodeValue->getPhysicalValue( nodeId0, v0 ) &&
								nodeValue->getPhysicalValue( nodeId1, v1 ) &&
								nodeValue->getPhysicalValue( nodeId2, v2 ) )
							{
								n0.setCoordinate(
									node0.x() + factor * d0[0],
									node0.y() + factor * d0[1],
									node0.z() + factor * d0[2]);
								n1.setCoordinate(
									node1.x() + factor * d1[0],
									node1.y() + factor * d1[1],
									node1.z() + factor * d1[2]);
								n2.setCoordinate(
									node2.x() + factor * d2[0],
									node2.y() + factor * d2[1],
									node2.z() + factor * d2[2]);
								drawSolidContourTriangle(&n0,&n1,&n2,
									kmb::Vector3D::abs(v0),
									kmb::Vector3D::abs(v1),
									kmb::Vector3D::abs(v2),
									colorMap);
							}
							break;
						case 4:
							nodeId0 = eIter[0];
							nodeId1 = eIter[1];
							nodeId2 = eIter[2];
							nodeId3 = eIter[3];
							if( mesh->getNode( nodeId0, node0 ) &&
								mesh->getNode( nodeId1, node1 ) &&
								mesh->getNode( nodeId2, node2 ) &&
								mesh->getNode( nodeId3, node3 ) &&
								displace->getPhysicalValue( nodeId0, d0 ) &&
								displace->getPhysicalValue( nodeId1, d1 ) &&
								displace->getPhysicalValue( nodeId2, d2 ) &&
								displace->getPhysicalValue( nodeId3, d3 ) &&
								nodeValue->getPhysicalValue( nodeId0, v0 ) &&
								nodeValue->getPhysicalValue( nodeId1, v1 ) &&
								nodeValue->getPhysicalValue( nodeId1, v2 ) &&
								nodeValue->getPhysicalValue( nodeId2, v3 ) )
							{
								n0.setCoordinate(
									node0.x() + factor * d0[0],
									node0.y() + factor * d0[1],
									node0.z() + factor * d0[2]);
								n1.setCoordinate(
									node1.x() + factor * d1[0],
									node1.y() + factor * d1[1],
									node1.z() + factor * d1[2]);
								n2.setCoordinate(
									node2.x() + factor * d2[0],
									node2.y() + factor * d2[1],
									node2.z() + factor * d2[2]);
								n3.setCoordinate(
									node3.x() + factor * d3[0],
									node3.y() + factor * d3[1],
									node3.z() + factor * d3[2]);
								center.setCoordinate(
									0.25*(n0.x()+n1.x()+n2.x()+n3.x()),
									0.25*(n0.y()+n1.y()+n2.y()+n3.y()),
									0.25*(n0.z()+n1.z()+n2.z()+n3.z()));
								u0 = kmb::Vector3D::abs( v0 );
								u1 = kmb::Vector3D::abs( v1 );
								u2 = kmb::Vector3D::abs( v2 );
								u3 = kmb::Vector3D::abs( v3 );
								mean = 0.25*(u0 + u1 + u2 + u3);
								drawSolidContourTriangle(&n0,&n1,&center,u0,u1,mean,colorMap);
								drawSolidContourTriangle(&n1,&n2,&center,u1,u2,mean,colorMap);
								drawSolidContourTriangle(&n2,&n3,&center,u2,u3,mean,colorMap);
								drawSolidContourTriangle(&n3,&n0,&center,u3,u0,mean,colorMap);
							}
							break;
						default:
							break;
					}
					++eIter;
				}
			}
		}else if( nodeValue->getValueType() == kmb::PhysicalValue::Vector3 && 0 <= comp && comp < 3 ){
			double v0[] = {0.0, 0.0, 0.0},
				v1[] = {0.0, 0.0, 0.0},
				v2[] = {0.0, 0.0, 0.0},
				v3[] = {0.0, 0.0, 0.0},
				u0 = 0.0, u1 =0.0, u2= 0.0, u3 = 0.0, mean = 0.0;
			if( colorMap->getStep() == 0 ){
				while( !eIter.isFinished() ){
					switch( eIter.getVertexCount() ){
						case 3:
							nodeId0 = eIter[0];
							nodeId1 = eIter[1];
							nodeId2 = eIter[2];
							if( mesh->getNode( nodeId0, node0 ) &&
								mesh->getNode( nodeId1, node1 ) &&
								mesh->getNode( nodeId2, node2 ) &&
								displace->getPhysicalValue( nodeId0, d0 ) &&
								displace->getPhysicalValue( nodeId1, d1 ) &&
								displace->getPhysicalValue( nodeId2, d2 ) &&
								nodeValue->getPhysicalValue( nodeId0, v0 ) &&
								nodeValue->getPhysicalValue( nodeId1, v1 ) &&
								nodeValue->getPhysicalValue( nodeId2, v2 ) )
							{
								drawContourTriangle(
									node0.x() + factor * d0[0],
									node0.y() + factor * d0[1],
									node0.z() + factor * d0[2],
									v0[comp],
									node1.x() + factor * d1[0],
									node1.y() + factor * d1[1],
									node1.z() + factor * d1[2],
									v1[comp],
									node2.x() + factor * d2[0],
									node2.y() + factor * d2[1],
									node2.z() + factor * d2[2],
									v2[comp],
									colorMap );
							}
							break;
						case 4:
							nodeId0 = eIter[0];
							nodeId1 = eIter[1];
							nodeId2 = eIter[2];
							nodeId3 = eIter[3];
							if( mesh->getNode( nodeId0, node0 ) &&
								mesh->getNode( nodeId1, node1 ) &&
								mesh->getNode( nodeId2, node2 ) &&
								mesh->getNode( nodeId3, node3 ) &&
								displace->getPhysicalValue( nodeId0, d0 ) &&
								displace->getPhysicalValue( nodeId1, d1 ) &&
								displace->getPhysicalValue( nodeId2, d2 ) &&
								displace->getPhysicalValue( nodeId3, d3 ) &&
								nodeValue->getPhysicalValue( nodeId0, v0 ) &&
								nodeValue->getPhysicalValue( nodeId1, v1 ) &&
								nodeValue->getPhysicalValue( nodeId2, v2 ) &&
								nodeValue->getPhysicalValue( nodeId3, v3 ) )
							{
								drawContourQuad(
									node0.x() + factor * d0[0],
									node0.y() + factor * d0[1],
									node0.z() + factor * d0[2],
									v0[comp],
									node1.x() + factor * d1[0],
									node1.y() + factor * d1[1],
									node1.z() + factor * d1[2],
									v1[comp],
									node2.x() + factor * d2[0],
									node2.y() + factor * d2[1],
									node2.z() + factor * d2[2],
									v2[comp],
									node3.x() + factor * d3[0],
									node3.y() + factor * d3[1],
									node3.z() + factor * d3[2],
									v3[comp],
									colorMap );
							}
							break;
						default:
							break;
					}
					++eIter;
				}
			}else{ // solid contour
				while( !eIter.isFinished() ){
					switch( eIter.getVertexCount() ){
						case 3:
							nodeId0 = eIter[0];
							nodeId1 = eIter[1];
							nodeId2 = eIter[2];
							if( mesh->getNode( nodeId0, node0 ) &&
								mesh->getNode( nodeId1, node1 ) &&
								mesh->getNode( nodeId2, node2 ) &&
								displace->getPhysicalValue( nodeId0, d0 ) &&
								displace->getPhysicalValue( nodeId1, d1 ) &&
								displace->getPhysicalValue( nodeId2, d2 ) &&
								nodeValue->getPhysicalValue( nodeId0, v0 ) &&
								nodeValue->getPhysicalValue( nodeId1, v1 ) &&
								nodeValue->getPhysicalValue( nodeId2, v2 ) )
							{
								n0.setCoordinate(
									node0.x() + factor * d0[0],
									node0.y() + factor * d0[1],
									node0.z() + factor * d0[2]);
								n1.setCoordinate(
									node1.x() + factor * d1[0],
									node1.y() + factor * d1[1],
									node1.z() + factor * d1[2]);
								n2.setCoordinate(
									node2.x() + factor * d2[0],
									node2.y() + factor * d2[1],
									node2.z() + factor * d2[2]);
								drawSolidContourTriangle(&n0,&n1,&n2,
									v0[comp],
									v1[comp],
									v2[comp],
									colorMap);
							}
							break;
						case 4:
							nodeId0 = eIter[0];
							nodeId1 = eIter[1];
							nodeId2 = eIter[2];
							nodeId3 = eIter[3];
							if( mesh->getNode( nodeId0, node0 ) &&
								mesh->getNode( nodeId1, node1 ) &&
								mesh->getNode( nodeId2, node2 ) &&
								mesh->getNode( nodeId3, node3 ) &&
								displace->getPhysicalValue( nodeId0, d0 ) &&
								displace->getPhysicalValue( nodeId1, d1 ) &&
								displace->getPhysicalValue( nodeId2, d2 ) &&
								displace->getPhysicalValue( nodeId3, d3 ) &&
								nodeValue->getPhysicalValue( nodeId0, v0 ) &&
								nodeValue->getPhysicalValue( nodeId1, v1 ) &&
								nodeValue->getPhysicalValue( nodeId1, v2 ) &&
								nodeValue->getPhysicalValue( nodeId2, v3 ) )
							{
								n0.setCoordinate(
									node0.x() + factor * d0[0],
									node0.y() + factor * d0[1],
									node0.z() + factor * d0[2]);
								n1.setCoordinate(
									node1.x() + factor * d1[0],
									node1.y() + factor * d1[1],
									node1.z() + factor * d1[2]);
								n2.setCoordinate(
									node2.x() + factor * d2[0],
									node2.y() + factor * d2[1],
									node2.z() + factor * d2[2]);
								n3.setCoordinate(
									node3.x() + factor * d3[0],
									node3.y() + factor * d3[1],
									node3.z() + factor * d3[2]);
								center.setCoordinate(
									0.25*(n0.x()+n1.x()+n2.x()+n3.x()),
									0.25*(n0.y()+n1.y()+n2.y()+n3.y()),
									0.25*(n0.z()+n1.z()+n2.z()+n3.z()));
								u0 = v0[comp];
								u1 = v1[comp];
								u2 = v2[comp];
								u3 = v3[comp];
								mean = 0.25*(u0 + u1 + u2 + u3);
								drawSolidContourTriangle(&n0,&n1,&center,u0,u1,mean,colorMap);
								drawSolidContourTriangle(&n1,&n2,&center,u1,u2,mean,colorMap);
								drawSolidContourTriangle(&n2,&n3,&center,u2,u3,mean,colorMap);
								drawSolidContourTriangle(&n3,&n0,&center,u3,u0,mean,colorMap);
							}
							break;
						default:
							break;
					}
					++eIter;
				}
			}
		}else if( nodeValue->getValueType() == kmb::PhysicalValue::Tensor6 && 0 <= comp && comp < 6){
			double v0[] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0},
				v1[] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0},
				v2[] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0},
				v3[] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0},
				u0 = 0.0, u1 =0.0, u2= 0.0, u3 = 0.0, mean = 0.0;
			if( colorMap->getStep() == 0 ){
				while( !eIter.isFinished() ){
					switch( eIter.getVertexCount() ){
						case 3:
							nodeId0 = eIter[0];
							nodeId1 = eIter[1];
							nodeId2 = eIter[2];
							if( mesh->getNode( nodeId0, node0 ) &&
								mesh->getNode( nodeId1, node1 ) &&
								mesh->getNode( nodeId2, node2 ) &&
								displace->getPhysicalValue( nodeId0, d0 ) &&
								displace->getPhysicalValue( nodeId1, d1 ) &&
								displace->getPhysicalValue( nodeId2, d2 ) &&
								nodeValue->getPhysicalValue( nodeId0, v0 ) &&
								nodeValue->getPhysicalValue( nodeId1, v1 ) &&
								nodeValue->getPhysicalValue( nodeId2, v2 ) )
							{
								drawContourTriangle(
									node0.x() + factor * d0[0],
									node0.y() + factor * d0[1],
									node0.z() + factor * d0[2],
									v0[comp],
									node1.x() + factor * d1[0],
									node1.y() + factor * d1[1],
									node1.z() + factor * d1[2],
									v1[comp],
									node2.x() + factor * d2[0],
									node2.y() + factor * d2[1],
									node2.z() + factor * d2[2],
									v2[comp],
									colorMap );
							}
							break;
						case 4:
							nodeId0 = eIter[0];
							nodeId1 = eIter[1];
							nodeId2 = eIter[2];
							nodeId3 = eIter[3];
							if( mesh->getNode( nodeId0, node0 ) &&
								mesh->getNode( nodeId1, node1 ) &&
								mesh->getNode( nodeId2, node2 ) &&
								mesh->getNode( nodeId3, node3 ) &&
								displace->getPhysicalValue( nodeId0, d0 ) &&
								displace->getPhysicalValue( nodeId1, d1 ) &&
								displace->getPhysicalValue( nodeId2, d2 ) &&
								displace->getPhysicalValue( nodeId3, d3 ) &&
								nodeValue->getPhysicalValue( nodeId0, v0 ) &&
								nodeValue->getPhysicalValue( nodeId1, v1 ) &&
								nodeValue->getPhysicalValue( nodeId2, v2 ) &&
								nodeValue->getPhysicalValue( nodeId3, v3 ) )
							{
								drawContourQuad(
									node0.x() + factor * d0[0],
									node0.y() + factor * d0[1],
									node0.z() + factor * d0[2],
									v0[comp],
									node1.x() + factor * d1[0],
									node1.y() + factor * d1[1],
									node1.z() + factor * d1[2],
									v1[comp],
									node2.x() + factor * d2[0],
									node2.y() + factor * d2[1],
									node2.z() + factor * d2[2],
									v2[comp],
									node3.x() + factor * d3[0],
									node3.y() + factor * d3[1],
									node3.z() + factor * d3[2],
									v3[comp],
									colorMap );
							}
							break;
						default:
							break;
					}
					++eIter;
				}
			}else{ // solid contour
				while( !eIter.isFinished() ){
					switch( eIter.getVertexCount() ){
						case 3:
							nodeId0 = eIter[0];
							nodeId1 = eIter[1];
							nodeId2 = eIter[2];
							if( mesh->getNode( nodeId0, node0 ) &&
								mesh->getNode( nodeId1, node1 ) &&
								mesh->getNode( nodeId2, node2 ) &&
								displace->getPhysicalValue( nodeId0, d0 ) &&
								displace->getPhysicalValue( nodeId1, d1 ) &&
								displace->getPhysicalValue( nodeId2, d2 ) &&
								nodeValue->getPhysicalValue( nodeId0, v0 ) &&
								nodeValue->getPhysicalValue( nodeId1, v1 ) &&
								nodeValue->getPhysicalValue( nodeId2, v2 ) )
							{
								n0.setCoordinate(
									node0.x() + factor * d0[0],
									node0.y() + factor * d0[1],
									node0.z() + factor * d0[2]);
								n1.setCoordinate(
									node1.x() + factor * d1[0],
									node1.y() + factor * d1[1],
									node1.z() + factor * d1[2]);
								n2.setCoordinate(
									node2.x() + factor * d2[0],
									node2.y() + factor * d2[1],
									node2.z() + factor * d2[2]);
								drawSolidContourTriangle(&n0,&n1,&n2,
									v0[comp],
									v1[comp],
									v2[comp],
									colorMap);
							}
							break;
						case 4:
							nodeId0 = eIter[0];
							nodeId1 = eIter[1];
							nodeId2 = eIter[2];
							nodeId3 = eIter[3];
							if( mesh->getNode( nodeId0, node0 ) &&
								mesh->getNode( nodeId1, node1 ) &&
								mesh->getNode( nodeId2, node2 ) &&
								mesh->getNode( nodeId3, node3 ) &&
								displace->getPhysicalValue( nodeId0, d0 ) &&
								displace->getPhysicalValue( nodeId1, d1 ) &&
								displace->getPhysicalValue( nodeId2, d2 ) &&
								displace->getPhysicalValue( nodeId3, d3 ) &&
								nodeValue->getPhysicalValue( nodeId0, v0 ) &&
								nodeValue->getPhysicalValue( nodeId1, v1 ) &&
								nodeValue->getPhysicalValue( nodeId1, v2 ) &&
								nodeValue->getPhysicalValue( nodeId2, v3 ) )
							{
								n0.setCoordinate(
									node0.x() + factor * d0[0],
									node0.y() + factor * d0[1],
									node0.z() + factor * d0[2]);
								n1.setCoordinate(
									node1.x() + factor * d1[0],
									node1.y() + factor * d1[1],
									node1.z() + factor * d1[2]);
								n2.setCoordinate(
									node2.x() + factor * d2[0],
									node2.y() + factor * d2[1],
									node2.z() + factor * d2[2]);
								n3.setCoordinate(
									node3.x() + factor * d3[0],
									node3.y() + factor * d3[1],
									node3.z() + factor * d3[2]);
								center.setCoordinate(
									0.25*(n0.x()+n1.x()+n2.x()+n3.x()),
									0.25*(n0.y()+n1.y()+n2.y()+n3.y()),
									0.25*(n0.z()+n1.z()+n2.z()+n3.z()));
								u0 = v0[comp];
								u1 = v1[comp];
								u2 = v2[comp];
								u3 = v3[comp];
								mean = 0.25*(u0 + u1 + u2 + u3);
								drawSolidContourTriangle(&n0,&n1,&center,u0,u1,mean,colorMap);
								drawSolidContourTriangle(&n1,&n2,&center,u1,u2,mean,colorMap);
								drawSolidContourTriangle(&n2,&n3,&center,u2,u3,mean,colorMap);
								drawSolidContourTriangle(&n3,&n0,&center,u3,u0,mean,colorMap);
							}
							break;
						default:
							break;
					}
					++eIter;
				}
			}
		}
		::glPopAttrib();
		::glFlush();
	}
}

void
kmb::MeshGL::deformSurfaceContour(kmb::bodyIdType bodyId, const char* displacement, double factor, const char* physicalValue, kmb::ColorMap* colorMap, int comp)
{
	const kmb::Body* body = NULL;
	const kmb::DataBindings* displace = NULL;
	const kmb::DataBindings* elementValue = NULL;
	if( mesh != NULL && colorMap != NULL &&
		(body = mesh->getBodyPtr(bodyId)) != NULL &&
		body->isUniqueDim(2) &&
		(displace = mesh->getDataBindingsPtr( displacement )) != NULL &&
		displace->getBindingMode() == kmb::DataBindings::NodeVariable &&
		displace->getValueType() == kmb::PhysicalValue::Vector3 &&
		(elementValue = mesh->getDataBindingsPtr( physicalValue )) != NULL &&
		elementValue->getBindingMode() == kmb::DataBindings::ElementVariable )
	{
		::glPushAttrib( GL_ENABLE_BIT );
		::glEnable(GL_COLOR_MATERIAL);

		kmb::nodeIdType
			nodeId0 = kmb::nullNodeId,
			nodeId1 = kmb::nullNodeId,
			nodeId2 = kmb::nullNodeId,
			nodeId3 = kmb::nullNodeId;
		kmb::Node node0,node1,node2,node3;
		double
			d0[3] = {0.0, 0.0, 0.0},
			d1[3] = {0.0, 0.0, 0.0},
			d2[3] = {0.0, 0.0, 0.0},
			d3[3] = {0.0, 0.0, 0.0};
		kmb::ElementContainer::const_iterator eIter = body->begin();
		if( elementValue->getValueType() == kmb::PhysicalValue::Scalar ){
			double v = 0.0;
			while( !eIter.isFinished() ){
				kmb::elementIdType id = eIter.getId();
				if( elementValue->getPhysicalValue( id, &v ) ){
					switch( eIter.getVertexCount() ){
						case 3:
							nodeId0 = eIter[0];
							nodeId1 = eIter[1];
							nodeId2 = eIter[2];
							if( mesh->getNode( nodeId0, node0 ) &&
								mesh->getNode( nodeId1, node1 ) &&
								mesh->getNode( nodeId2, node2 ) &&
								displace->getPhysicalValue( nodeId0, d0 ) &&
								displace->getPhysicalValue( nodeId1, d1 ) &&
								displace->getPhysicalValue( nodeId2, d2 ) )
							{
								colorMap->setGLColor( v );
								drawTriangle
									(node0.x() + factor * d0[0], node0.y() + factor * d0[1], node0.z() + factor * d0[2],
									 node1.x() + factor * d1[0], node1.y() + factor * d1[1], node1.z() + factor * d1[2],
									 node2.x() + factor * d2[0], node2.y() + factor * d2[1], node2.z() + factor * d2[2]);
							}
							break;
						case 4:
							nodeId0 = eIter[0];
							nodeId1 = eIter[1];
							nodeId2 = eIter[2];
							nodeId3 = eIter[3];
							if( mesh->getNode( nodeId0, node0 ) &&
								mesh->getNode( nodeId1, node1 ) &&
								mesh->getNode( nodeId2, node2 ) &&
								mesh->getNode( nodeId3, node3 ) &&
								displace->getPhysicalValue( nodeId0, d0 ) &&
								displace->getPhysicalValue( nodeId1, d1 ) &&
								displace->getPhysicalValue( nodeId2, d2 ) &&
								displace->getPhysicalValue( nodeId3, d3 ) )
							{
								colorMap->setGLColor( v );
								drawQuad
									(node0.x() + factor * d0[0], node0.y() + factor * d0[1], node0.z() + factor * d0[2],
									 node1.x() + factor * d1[0], node1.y() + factor * d1[1], node1.z() + factor * d1[2],
									 node2.x() + factor * d2[0], node2.y() + factor * d2[1], node2.z() + factor * d2[2],
									 node3.x() + factor * d3[0], node3.y() + factor * d3[1], node3.z() + factor * d3[2]);
							}
							break;
						default:
							break;
					}
				}
				++eIter;
			}
		}else if( elementValue->getValueType() == kmb::PhysicalValue::Vector3 && comp == -1){
			double v[3] = {0.0, 0.0, 0.0};
			while( !eIter.isFinished() ){
				kmb::elementIdType id = eIter.getId();
				if( elementValue->getPhysicalValue( id, v ) ){
					switch( eIter.getVertexCount() ){
					case 3:
						nodeId0 = eIter[0];
						nodeId1 = eIter[1];
						nodeId2 = eIter[2];
						if( mesh->getNode( nodeId0, node0 ) &&
							mesh->getNode( nodeId1, node1 ) &&
							mesh->getNode( nodeId2, node2 ) &&
							displace->getPhysicalValue( nodeId0, d0 ) &&
							displace->getPhysicalValue( nodeId1, d1 ) &&
							displace->getPhysicalValue( nodeId2, d2 ) )
						{
							colorMap->setGLColor( kmb::Vector3D::abs(v) );
							drawTriangle
								(node0.x() + factor * d0[0], node0.y() + factor * d0[1], node0.z() + factor * d0[2],
								 node1.x() + factor * d1[0], node1.y() + factor * d1[1], node1.z() + factor * d1[2],
								 node2.x() + factor * d2[0], node2.y() + factor * d2[1], node2.z() + factor * d2[2]);
						}
						break;
					case 4:
						nodeId0 = eIter[0];
						nodeId1 = eIter[1];
						nodeId2 = eIter[2];
						nodeId3 = eIter[3];
						if( mesh->getNode( nodeId0, node0 ) &&
							mesh->getNode( nodeId1, node1 ) &&
							mesh->getNode( nodeId2, node2 ) &&
							mesh->getNode( nodeId3, node3 ) &&
							displace->getPhysicalValue( nodeId0, d0 ) &&
							displace->getPhysicalValue( nodeId1, d1 ) &&
							displace->getPhysicalValue( nodeId2, d2 ) &&
							displace->getPhysicalValue( nodeId3, d3 ) )
						{
							colorMap->setGLColor( kmb::Vector3D::abs(v) );
							drawQuad
								(node0.x() + factor * d0[0], node0.y() + factor * d0[1], node0.z() + factor * d0[2],
								 node1.x() + factor * d1[0], node1.y() + factor * d1[1], node1.z() + factor * d1[2],
								 node2.x() + factor * d2[0], node2.y() + factor * d2[1], node2.z() + factor * d2[2],
								 node3.x() + factor * d3[0], node3.y() + factor * d3[1], node3.z() + factor * d3[2]);
						}
						break;
					default:
						break;
					}
				}
				++eIter;
			}
		}else if( elementValue->getValueType() == kmb::PhysicalValue::Vector3 && 0 <= comp && comp < 3 ){
			double v[3] = {0.0, 0.0, 0.0};
			while( !eIter.isFinished() ){
				kmb::elementIdType id = eIter.getId();
				if( elementValue->getPhysicalValue( id, v ) ){
					switch( eIter.getVertexCount() ){
					case 3:
						nodeId0 = eIter[0];
						nodeId1 = eIter[1];
						nodeId2 = eIter[2];
						if( mesh->getNode( nodeId0, node0 ) &&
							mesh->getNode( nodeId1, node1 ) &&
							mesh->getNode( nodeId2, node2 ) &&
							displace->getPhysicalValue( nodeId0, d0 ) &&
							displace->getPhysicalValue( nodeId1, d1 ) &&
							displace->getPhysicalValue( nodeId2, d2 ) )
						{
							colorMap->setGLColor( v[comp] );
							drawTriangle
								(node0.x() + factor * d0[0], node0.y() + factor * d0[1], node0.z() + factor * d0[2],
								 node1.x() + factor * d1[0], node1.y() + factor * d1[1], node1.z() + factor * d1[2],
								 node2.x() + factor * d2[0], node2.y() + factor * d2[1], node2.z() + factor * d2[2]);
						}
						break;
					case 4:
						nodeId0 = eIter[0];
						nodeId1 = eIter[1];
						nodeId2 = eIter[2];
						nodeId3 = eIter[3];
						if( mesh->getNode( nodeId0, node0 ) &&
							mesh->getNode( nodeId1, node1 ) &&
							mesh->getNode( nodeId2, node2 ) &&
							mesh->getNode( nodeId3, node3 ) &&
							displace->getPhysicalValue( nodeId0, d0 ) &&
							displace->getPhysicalValue( nodeId1, d1 ) &&
							displace->getPhysicalValue( nodeId2, d2 ) &&
							displace->getPhysicalValue( nodeId3, d3 ) )
						{
							colorMap->setGLColor( v[comp] );
							drawQuad
								(node0.x() + factor * d0[0], node0.y() + factor * d0[1], node0.z() + factor * d0[2],
								 node1.x() + factor * d1[0], node1.y() + factor * d1[1], node1.z() + factor * d1[2],
								 node2.x() + factor * d2[0], node2.y() + factor * d2[1], node2.z() + factor * d2[2],
								 node3.x() + factor * d3[0], node3.y() + factor * d3[1], node3.z() + factor * d3[2]);
						}
						break;
					default:
						break;
					}
				}
				++eIter;
			}
		}else if( elementValue->getValueType() == kmb::PhysicalValue::Tensor6 && 0 <= comp && comp < 6){
			double v[] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
			while( !eIter.isFinished() ){
				kmb::elementIdType id = eIter.getId();
				if( elementValue->getPhysicalValue( id, v ) ){
					switch( eIter.getVertexCount() ){
					case 3:
						nodeId0 = eIter[0];
						nodeId1 = eIter[1];
						nodeId2 = eIter[2];
						if( mesh->getNode( nodeId0, node0 ) &&
							mesh->getNode( nodeId1, node1 ) &&
							mesh->getNode( nodeId2, node2 ) &&
							displace->getPhysicalValue( nodeId0, d0 ) &&
							displace->getPhysicalValue( nodeId1, d1 ) &&
							displace->getPhysicalValue( nodeId2, d2 ) )
						{
							colorMap->setGLColor( v[comp] );
							drawTriangle
								(node0.x() + factor * d0[0], node0.y() + factor * d0[1], node0.z() + factor * d0[2],
								 node1.x() + factor * d1[0], node1.y() + factor * d1[1], node1.z() + factor * d1[2],
								 node2.x() + factor * d2[0], node2.y() + factor * d2[1], node2.z() + factor * d2[2]);
						}
						break;
					case 4:
						nodeId0 = eIter[0];
						nodeId1 = eIter[1];
						nodeId2 = eIter[2];
						nodeId3 = eIter[3];
						if( mesh->getNode( nodeId0, node0 ) &&
							mesh->getNode( nodeId1, node1 ) &&
							mesh->getNode( nodeId2, node2 ) &&
							mesh->getNode( nodeId3, node3 ) &&
							displace->getPhysicalValue( nodeId0, d0 ) &&
							displace->getPhysicalValue( nodeId1, d1 ) &&
							displace->getPhysicalValue( nodeId2, d2 ) &&
							displace->getPhysicalValue( nodeId3, d3 ) )
						{
							colorMap->setGLColor( v[comp] );
							drawQuad
								(node0.x() + factor * d0[0], node0.y() + factor * d0[1], node0.z() + factor * d0[2],
								 node1.x() + factor * d1[0], node1.y() + factor * d1[1], node1.z() + factor * d1[2],
								 node2.x() + factor * d2[0], node2.y() + factor * d2[1], node2.z() + factor * d2[2],
								 node3.x() + factor * d3[0], node3.y() + factor * d3[1], node3.z() + factor * d3[2]);
						}
						break;
					default:
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
kmb::MeshGL::drawFaceGroupNodeContour(const char* faceName, const char* physicalValue, kmb::ColorMap* colorMap,int comp)
{
	const kmb::DataBindings* data = NULL;
	const kmb::DataBindings* nodeValue = NULL;
	if( mesh != NULL && colorMap != NULL &&
		(data = mesh->getDataBindingsPtr( faceName )) != NULL &&
		data->getBindingMode() == kmb::DataBindings::FaceGroup &&
		(nodeValue = mesh->getDataBindingsPtr( physicalValue )) != NULL &&
		nodeValue->getBindingMode() == kmb::DataBindings::NodeVariable )
	{
		::glPushAttrib( GL_ENABLE_BIT );
		::glEnable(GL_COLOR_MATERIAL);

		kmb::Node node0,node1,node2,node3,center;
		kmb::bodyIdType bodyId = data->getTargetBodyId();
		kmb::Face f;
		kmb::nodeIdType nodeId0 = kmb::nullNodeId, nodeId1 = kmb::nullNodeId, nodeId2 = kmb::nullNodeId, nodeId3 = kmb::nullNodeId;
		kmb::DataBindings::const_iterator fIter = data->begin();
		if( nodeValue->getValueType() == kmb::PhysicalValue::Scalar ){
			double v0 = 0.0, v1 = 0.0, v2 = 0.0, v3 = 0.0, mean = 0.0;
			if( colorMap->getStep() == 0 ){
				while( !fIter.isFinished() )
				{
					if( !fIter.getFace(f) ){
						++fIter;
						continue;
					}
					kmb::ElementContainer::const_iterator element = mesh->findElement( f.getElementId(), bodyId );
					kmb::idType localId = f.getLocalFaceId();
					if( !element.isFinished() ){
						switch( element.getBoundaryVertexCount(localId) )
						{
						case 3:
							nodeId0 = element.getBoundaryCellId(localId,0);
							nodeId1 = element.getBoundaryCellId(localId,1);
							nodeId2 = element.getBoundaryCellId(localId,2);
							if( mesh->getNode( nodeId0, node0 ) &&
								mesh->getNode( nodeId1, node1 ) &&
								mesh->getNode( nodeId2, node2 ) &&
								nodeValue->getPhysicalValue( nodeId0, &v0 ) &&
								nodeValue->getPhysicalValue( nodeId1, &v1 ) &&
								nodeValue->getPhysicalValue( nodeId2, &v2 ) )
							{
								drawContourTriangle(
									node0.x(),node0.y(),node0.z(),v0,
									node1.x(),node1.y(),node1.z(),v1,
									node2.x(),node2.y(),node2.z(),v2,colorMap);
							}
							break;
						case 4:
							nodeId0 = element.getBoundaryCellId(localId,0);
							nodeId1 = element.getBoundaryCellId(localId,1);
							nodeId2 = element.getBoundaryCellId(localId,2);
							nodeId3 = element.getBoundaryCellId(localId,3);
							if( mesh->getNode( nodeId0, node0 ) &&
								mesh->getNode( nodeId1, node1 ) &&
								mesh->getNode( nodeId2, node2 ) &&
								mesh->getNode( nodeId3, node3 ) &&
								nodeValue->getPhysicalValue( nodeId0, &v0 ) &&
								nodeValue->getPhysicalValue( nodeId1, &v1 ) &&
								nodeValue->getPhysicalValue( nodeId2, &v2 ) &&
								nodeValue->getPhysicalValue( nodeId3, &v3 ) )
							{
								drawContourQuad(
									node0.x(), node0.y(), node0.z(), v0,
									node1.x(), node1.y(), node1.z(), v1,
									node2.x(), node2.y(), node2.z(), v2,
									node3.x(), node3.y(), node3.z(), v3, colorMap);
							}
							break;
						}
					}
					++fIter;
				}
			}else{ //solid contour
				while( !fIter.isFinished() )
				{
					if( !fIter.getFace(f) ){
						++fIter;
						continue;
					}
					kmb::ElementContainer::const_iterator element = mesh->findElement( f.getElementId(), bodyId );
					kmb::idType localId = f.getLocalFaceId();
					if( !element.isFinished() ){
						switch( element.getBoundaryVertexCount(localId) )
						{
						case 3:
							nodeId0 = element.getBoundaryCellId(localId,0);
							nodeId1 = element.getBoundaryCellId(localId,1);
							nodeId2 = element.getBoundaryCellId(localId,2);
							if( mesh->getNode( nodeId0, node0 ) &&
								mesh->getNode( nodeId1, node1 ) &&
								mesh->getNode( nodeId2, node2 ) &&
								nodeValue->getPhysicalValue( nodeId0, &v0 ) &&
								nodeValue->getPhysicalValue( nodeId1, &v1 ) &&
								nodeValue->getPhysicalValue( nodeId2, &v2 ) )
							{
								drawSolidContourTriangle(&node0,&node1,&node2,v0,v1,v2,colorMap);
							}
							break;
						case 4:
							nodeId0 = element.getBoundaryCellId(localId,0);
							nodeId1 = element.getBoundaryCellId(localId,1);
							nodeId2 = element.getBoundaryCellId(localId,2);
							nodeId3 = element.getBoundaryCellId(localId,3);
							if( mesh->getNode( nodeId0, node0 ) &&
								mesh->getNode( nodeId1, node1 ) &&
								mesh->getNode( nodeId2, node2 ) &&
								mesh->getNode( nodeId3, node3 ) &&
								nodeValue->getPhysicalValue( nodeId0, &v0 ) &&
								nodeValue->getPhysicalValue( nodeId1, &v1 ) &&
								nodeValue->getPhysicalValue( nodeId2, &v2 ) &&
								nodeValue->getPhysicalValue( nodeId3, &v3 ) )
							{
								mean = 0.25*(v0 + v1 + v2 + v3);
								center.setCoordinate(
									0.25*(node0.x()+node1.x()+node2.x()+node3.x()),
									0.25*(node0.y()+node1.y()+node2.y()+node3.y()),
									0.25*(node0.z()+node1.z()+node2.z()+node3.z()));
								drawSolidContourTriangle(&node0,&node1,&center,v0,v1,mean,colorMap);
								drawSolidContourTriangle(&node1,&node2,&center,v1,v2,mean,colorMap);
								drawSolidContourTriangle(&node2,&node3,&center,v2,v3,mean,colorMap);
								drawSolidContourTriangle(&node3,&node0,&center,v3,v0,mean,colorMap);
							}
							break;
						}
					}
					++fIter;
				}
			}
		}else if( nodeValue->getValueType() == kmb::PhysicalValue::Vector3 ){
			double v0[3] = {0.0, 0.0, 0.0},
				v1[3] = {0.0, 0.0, 0.0},
				v2[3] = {0.0, 0.0, 0.0},
				v3[3] = {0.0, 0.0, 0.0},
				u0 = 0.0, u1 =0.0, u2= 0.0, u3 = 0.0, mean = 0.0;
			if( colorMap->getStep() == 0 ){
				if( comp== -1 ){
					while( !fIter.isFinished() )
					{
						if( !fIter.getFace(f) ){
							++fIter;
							continue;
						}
						kmb::ElementContainer::const_iterator element = mesh->findElement( f.getElementId(), bodyId );
						kmb::idType localId = f.getLocalFaceId();
						if( !element.isFinished() ){
							switch( element.getBoundaryVertexCount(localId) )
							{
							case 3:
								nodeId0 = element.getBoundaryCellId(localId,0);
								nodeId1 = element.getBoundaryCellId(localId,1);
								nodeId2 = element.getBoundaryCellId(localId,2);
								if( mesh->getNode( nodeId0, node0 ) &&
									mesh->getNode( nodeId1, node1 ) &&
									mesh->getNode( nodeId2, node2 ) &&
									nodeValue->getPhysicalValue( nodeId0, v0 ) &&
									nodeValue->getPhysicalValue( nodeId1, v1 ) &&
									nodeValue->getPhysicalValue( nodeId2, v2 ) )
								{
									drawContourTriangle(
										node0.x(), node0.y(), node0.z(), kmb::Vector3D::abs(v0),
										node1.x(), node1.y(), node1.z(), kmb::Vector3D::abs(v1),
										node2.x(), node2.y(), node2.z(), kmb::Vector3D::abs(v2), colorMap);
								}
								break;
							case 4:
								nodeId0 = element.getBoundaryCellId(localId,0);
								nodeId1 = element.getBoundaryCellId(localId,1);
								nodeId2 = element.getBoundaryCellId(localId,2);
								nodeId3 = element.getBoundaryCellId(localId,3);
								if( mesh->getNode( nodeId0, node0 ) &&
									mesh->getNode( nodeId1, node1 ) &&
									mesh->getNode( nodeId2, node2 ) &&
									mesh->getNode( nodeId3, node3 ) &&
									nodeValue->getPhysicalValue( nodeId0, v0 ) &&
									nodeValue->getPhysicalValue( nodeId1, v1 ) &&
									nodeValue->getPhysicalValue( nodeId2, v2 ) &&
									nodeValue->getPhysicalValue( nodeId3, v3 ) )
								{
									drawContourQuad(
										node0.x(), node0.y(), node0.z(), kmb::Vector3D::abs(v0),
										node1.x(), node1.y(), node1.z(), kmb::Vector3D::abs(v1),
										node2.x(), node2.y(), node2.z(), kmb::Vector3D::abs(v2),
										node3.x(), node3.y(), node3.z(), kmb::Vector3D::abs(v3), colorMap);
								}
								break;
							}
						}
						++fIter;
					}
				}else if( 0 <= comp && comp < 3 ){
					while( !fIter.isFinished() )
					{
						if( !fIter.getFace(f) ){
							++fIter;
							continue;
						}
						kmb::ElementContainer::const_iterator element = mesh->findElement( f.getElementId(), bodyId );
						kmb::idType localId = f.getLocalFaceId();
						if( !element.isFinished() ){
							switch( element.getBoundaryVertexCount(localId) )
							{
							case 3:
								nodeId0 = element.getBoundaryCellId(localId,0);
								nodeId1 = element.getBoundaryCellId(localId,1);
								nodeId2 = element.getBoundaryCellId(localId,2);
								if( mesh->getNode( nodeId0, node0 ) &&
									mesh->getNode( nodeId1, node1 ) &&
									mesh->getNode( nodeId2, node2 ) &&
									nodeValue->getPhysicalValue( nodeId0, v0 ) &&
									nodeValue->getPhysicalValue( nodeId1, v1 ) &&
									nodeValue->getPhysicalValue( nodeId2, v2 ) )
								{
									drawContourTriangle(
										node0.x(), node0.y(), node0.z(), v0[comp],
										node1.x(), node1.y(), node1.z(), v1[comp],
										node2.x(), node2.y(), node2.z(), v2[comp], colorMap);
								}
								break;
							case 4:
								nodeId0 = element.getBoundaryCellId(localId,0);
								nodeId1 = element.getBoundaryCellId(localId,1);
								nodeId2 = element.getBoundaryCellId(localId,2);
								nodeId3 = element.getBoundaryCellId(localId,3);
								if( mesh->getNode( nodeId0, node0 ) &&
									mesh->getNode( nodeId1, node1 ) &&
									mesh->getNode( nodeId2, node2 ) &&
									mesh->getNode( nodeId3, node3 ) &&
									nodeValue->getPhysicalValue( nodeId0, v0 ) &&
									nodeValue->getPhysicalValue( nodeId1, v1 ) &&
									nodeValue->getPhysicalValue( nodeId2, v2 ) &&
									nodeValue->getPhysicalValue( nodeId3, v3 ) )
								{
									drawContourQuad(
										node0.x(), node0.y(), node0.z(), v0[comp],
										node1.x(), node1.y(), node1.z(), v1[comp],
										node2.x(), node2.y(), node2.z(), v2[comp],
										node3.x(), node3.y(), node3.z(), v3[comp], colorMap);
								}
								break;
							}
						}
						++fIter;
					}
				}
			}else{
				if( comp== -1 ){
					while( !fIter.isFinished() )
					{
						if( !fIter.getFace(f) ){
							++fIter;
							continue;
						}
						kmb::ElementContainer::const_iterator element = mesh->findElement( f.getElementId(), bodyId );
						kmb::idType localId = f.getLocalFaceId();
						if( !element.isFinished() ){
							switch( element.getBoundaryVertexCount(localId) )
							{
							case 3:
								nodeId0 = element.getBoundaryCellId(localId,0);
								nodeId1 = element.getBoundaryCellId(localId,1);
								nodeId2 = element.getBoundaryCellId(localId,2);
								if( mesh->getNode( nodeId0, node0 ) &&
									mesh->getNode( nodeId1, node1 ) &&
									mesh->getNode( nodeId2, node2 ) &&
									nodeValue->getPhysicalValue( nodeId0, v0 ) &&
									nodeValue->getPhysicalValue( nodeId1, v1 ) &&
									nodeValue->getPhysicalValue( nodeId2, v2 ) )
								{
									drawSolidContourTriangle( &node0, &node1, &node2,
										kmb::Vector3D::abs(v0),
										kmb::Vector3D::abs(v1),
										kmb::Vector3D::abs(v2),
										colorMap);
								}
								break;
							case 4:
								nodeId0 = element.getBoundaryCellId(localId,0);
								nodeId1 = element.getBoundaryCellId(localId,1);
								nodeId2 = element.getBoundaryCellId(localId,2);
								nodeId3 = element.getBoundaryCellId(localId,3);
								if( mesh->getNode( nodeId0, node0 ) &&
									mesh->getNode( nodeId1, node1 ) &&
									mesh->getNode( nodeId2, node2 ) &&
									mesh->getNode( nodeId3, node3 ) &&
									nodeValue->getPhysicalValue( nodeId0, v0 ) &&
									nodeValue->getPhysicalValue( nodeId1, v1 ) &&
									nodeValue->getPhysicalValue( nodeId2, v2 ) &&
									nodeValue->getPhysicalValue( nodeId3, v3 ) )
								{
									u0 = kmb::Vector3D::abs( v0 );
									u1 = kmb::Vector3D::abs( v1 );
									u2 = kmb::Vector3D::abs( v2 );
									u3 = kmb::Vector3D::abs( v3 );
									mean = 0.25*(u0 + u1 + u2 + u3);
									center.setCoordinate(
										0.25*(node0.x()+node1.x()+node2.x()+node3.x()),
										0.25*(node0.y()+node1.y()+node2.y()+node3.y()),
										0.25*(node0.z()+node1.z()+node2.z()+node3.z()));
									drawSolidContourTriangle(&node0,&node1,&center,u0,u1,mean,colorMap);
									drawSolidContourTriangle(&node1,&node2,&center,u1,u2,mean,colorMap);
									drawSolidContourTriangle(&node2,&node3,&center,u2,u3,mean,colorMap);
									drawSolidContourTriangle(&node3,&node0,&center,u3,u0,mean,colorMap);
								}
								break;
							}
						}
						++fIter;
					}
				}else if(0 <= comp && comp < 3 ){
					while( !fIter.isFinished() )
					{
						if( !fIter.getFace(f) ){
							++fIter;
							continue;
						}
						kmb::ElementContainer::const_iterator element = mesh->findElement( f.getElementId(), bodyId );
						kmb::idType localId = f.getLocalFaceId();
						if( !element.isFinished() ){
							switch( element.getBoundaryVertexCount(localId) )
							{
							case 3:
								nodeId0 = element.getBoundaryCellId(localId,0);
								nodeId1 = element.getBoundaryCellId(localId,1);
								nodeId2 = element.getBoundaryCellId(localId,2);
								if( mesh->getNode( nodeId0, node0 ) &&
									mesh->getNode( nodeId1, node1 ) &&
									mesh->getNode( nodeId2, node2 ) &&
									nodeValue->getPhysicalValue( nodeId0, v0 ) &&
									nodeValue->getPhysicalValue( nodeId1, v1 ) &&
									nodeValue->getPhysicalValue( nodeId2, v2 ) )
								{
									drawSolidContourTriangle( &node0, &node1, &node2,
										v0[comp],
										v1[comp],
										v2[comp],
										colorMap);
								}
								break;
							case 4:
								nodeId0 = element.getBoundaryCellId(localId,0);
								nodeId1 = element.getBoundaryCellId(localId,1);
								nodeId2 = element.getBoundaryCellId(localId,2);
								nodeId3 = element.getBoundaryCellId(localId,3);
								if( mesh->getNode( nodeId0, node0 ) &&
									mesh->getNode( nodeId1, node1 ) &&
									mesh->getNode( nodeId2, node2 ) &&
									mesh->getNode( nodeId3, node3 ) &&
									nodeValue->getPhysicalValue( nodeId0, v0 ) &&
									nodeValue->getPhysicalValue( nodeId1, v1 ) &&
									nodeValue->getPhysicalValue( nodeId2, v2 ) &&
									nodeValue->getPhysicalValue( nodeId3, v3 ) )
								{
									u0 = v0[comp];
									u1 = v1[comp];
									u2 = v2[comp];
									u3 = v3[comp];
									mean = 0.25*(u0 + u1 + u2 + u3);
									center.setCoordinate(
										0.25*(node0.x()+node1.x()+node2.x()+node3.x()),
										0.25*(node0.y()+node1.y()+node2.y()+node3.y()),
										0.25*(node0.z()+node1.z()+node2.z()+node3.z()));
									drawSolidContourTriangle(&node0,&node1,&center,u0,u1,mean,colorMap);
									drawSolidContourTriangle(&node1,&node2,&center,u1,u2,mean,colorMap);
									drawSolidContourTriangle(&node2,&node3,&center,u2,u3,mean,colorMap);
									drawSolidContourTriangle(&node3,&node0,&center,u3,u0,mean,colorMap);
								}
								break;
							}
						}
						++fIter;
					}
				}
			}
		}else if( nodeValue->getValueType() == kmb::PhysicalValue::Tensor6 ){
			double v0[6] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0},
				v1[6] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0},
				v2[6] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0},
				v3[6] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0},
				u0 = 0.0, u1 =0.0, u2= 0.0, u3 = 0.0, mean = 0.0;
			if( colorMap->getStep() == 0 ){
				if( 0 <= comp && comp < 6 ){
					while( !fIter.isFinished() )
					{
						if( !fIter.getFace(f) ){
							++fIter;
							continue;
						}
						kmb::ElementContainer::const_iterator element = mesh->findElement( f.getElementId(), bodyId );
						kmb::idType localId = f.getLocalFaceId();
						if( !element.isFinished() ){
							switch( element.getBoundaryVertexCount(localId) )
							{
							case 3:
								nodeId0 = element.getBoundaryCellId(localId,0);
								nodeId1 = element.getBoundaryCellId(localId,1);
								nodeId2 = element.getBoundaryCellId(localId,2);
								if( mesh->getNode( nodeId0, node0 ) &&
									mesh->getNode( nodeId1, node1 ) &&
									mesh->getNode( nodeId2, node2 ) &&
									nodeValue->getPhysicalValue( nodeId0, v0 ) &&
									nodeValue->getPhysicalValue( nodeId1, v1 ) &&
									nodeValue->getPhysicalValue( nodeId2, v2 ) )
								{
									drawContourTriangle(
										node0.x(), node0.y(), node0.z(), v0[comp],
										node1.x(), node1.y(), node1.z(), v1[comp],
										node2.x(), node2.y(), node2.z(), v2[comp], colorMap);
								}
								break;
							case 4:
								nodeId0 = element.getBoundaryCellId(localId,0);
								nodeId1 = element.getBoundaryCellId(localId,1);
								nodeId2 = element.getBoundaryCellId(localId,2);
								nodeId3 = element.getBoundaryCellId(localId,3);
								if( mesh->getNode( nodeId0, node0 ) &&
									mesh->getNode( nodeId1, node1 ) &&
									mesh->getNode( nodeId2, node2 ) &&
									mesh->getNode( nodeId3, node3 ) &&
									nodeValue->getPhysicalValue( nodeId0, v0 ) &&
									nodeValue->getPhysicalValue( nodeId1, v1 ) &&
									nodeValue->getPhysicalValue( nodeId2, v2 ) &&
									nodeValue->getPhysicalValue( nodeId3, v3 ) )
								{
									drawContourQuad(
										node0.x(), node0.y(), node0.z(), v0[comp],
										node1.x(), node1.y(), node1.z(), v1[comp],
										node2.x(), node2.y(), node2.z(), v2[comp],
										node3.x(), node3.y(), node3.z(), v3[comp], colorMap);
								}
								break;
							}
						}
						++fIter;
					}
				}
			}else{
				if(0 <= comp && comp < 6 ){
					while( !fIter.isFinished() )
					{
						if( !fIter.getFace(f) ){
							++fIter;
							continue;
						}
						kmb::ElementContainer::const_iterator element = mesh->findElement( f.getElementId(), bodyId );
						kmb::idType localId = f.getLocalFaceId();
						if( !element.isFinished() ){
							switch( element.getBoundaryVertexCount(localId) )
							{
							case 3:
								nodeId0 = element.getBoundaryCellId(localId,0);
								nodeId1 = element.getBoundaryCellId(localId,1);
								nodeId2 = element.getBoundaryCellId(localId,2);
								if( mesh->getNode( nodeId0, node0 ) &&
									mesh->getNode( nodeId1, node1 ) &&
									mesh->getNode( nodeId2, node2 ) &&
									nodeValue->getPhysicalValue( nodeId0, v0 ) &&
									nodeValue->getPhysicalValue( nodeId1, v1 ) &&
									nodeValue->getPhysicalValue( nodeId2, v2 ) )
								{
									drawSolidContourTriangle( &node0, &node1, &node2,
										v0[comp],
										v1[comp],
										v2[comp],
										colorMap);
								}
								break;
							case 4:
								nodeId0 = element.getBoundaryCellId(localId,0);
								nodeId1 = element.getBoundaryCellId(localId,1);
								nodeId2 = element.getBoundaryCellId(localId,2);
								nodeId3 = element.getBoundaryCellId(localId,3);
								if( mesh->getNode( nodeId0, node0 ) &&
									mesh->getNode( nodeId1, node1 ) &&
									mesh->getNode( nodeId2, node2 ) &&
									mesh->getNode( nodeId3, node3 ) &&
									nodeValue->getPhysicalValue( nodeId0, v0 ) &&
									nodeValue->getPhysicalValue( nodeId1, v1 ) &&
									nodeValue->getPhysicalValue( nodeId2, v2 ) &&
									nodeValue->getPhysicalValue( nodeId3, v3 ) )
								{
									u0 = v0[comp];
									u1 = v1[comp];
									u2 = v2[comp];
									u3 = v3[comp];
									mean = 0.25*(u0 + u1 + u2 + u3);
									center.setCoordinate(
										0.25*(node0.x()+node1.x()+node2.x()+node3.x()),
										0.25*(node0.y()+node1.y()+node2.y()+node3.y()),
										0.25*(node0.z()+node1.z()+node2.z()+node3.z()));
									drawSolidContourTriangle(&node0,&node1,&center,u0,u1,mean,colorMap);
									drawSolidContourTriangle(&node1,&node2,&center,u1,u2,mean,colorMap);
									drawSolidContourTriangle(&node2,&node3,&center,u2,u3,mean,colorMap);
									drawSolidContourTriangle(&node3,&node0,&center,u3,u0,mean,colorMap);
								}
								break;
							}
						}
						++fIter;
					}
				}
			}
		}else if( nodeValue->getValueType() == kmb::PhysicalValue::Vector2withInt && 0 <= comp && comp < 2 ){
			double v0[3] = {0.0, 0.0},
				v1[3] = {0.0, 0.0},
				v2[3] = {0.0, 0.0},
				v3[3] = {0.0, 0.0},
				u0 = 0.0, u1 =0.0, u2= 0.0, u3 = 0.0, mean = 0.0;
			if( colorMap->getStep() == 0 ){
				while( !fIter.isFinished() )
				{
					if( !fIter.getFace(f) ){
						++fIter;
						continue;
					}
					kmb::ElementContainer::const_iterator element = mesh->findElement( f.getElementId(), bodyId );
					kmb::idType localId = f.getLocalFaceId();
					if( !element.isFinished() ){
						switch( element.getBoundaryVertexCount(localId) )
						{
						case 3:
							nodeId0 = element.getBoundaryCellId(localId,0);
							nodeId1 = element.getBoundaryCellId(localId,1);
							nodeId2 = element.getBoundaryCellId(localId,2);
							if( mesh->getNode( nodeId0, node0 ) &&
								mesh->getNode( nodeId1, node1 ) &&
								mesh->getNode( nodeId2, node2 ) &&
								nodeValue->getPhysicalValue( nodeId0, v0 ) &&
								nodeValue->getPhysicalValue( nodeId1, v1 ) &&
								nodeValue->getPhysicalValue( nodeId2, v2 ) )
							{
								drawContourTriangle(
									node0.x(), node0.y(), node0.z(), v0[comp],
									node1.x(), node1.y(), node1.z(), v1[comp],
									node2.x(), node2.y(), node2.z(), v2[comp], colorMap);
							}
							break;
						case 4:
							nodeId0 = element.getBoundaryCellId(localId,0);
							nodeId1 = element.getBoundaryCellId(localId,1);
							nodeId2 = element.getBoundaryCellId(localId,2);
							nodeId3 = element.getBoundaryCellId(localId,3);
							if( mesh->getNode( nodeId0, node0 ) &&
								mesh->getNode( nodeId1, node1 ) &&
								mesh->getNode( nodeId2, node2 ) &&
								mesh->getNode( nodeId3, node3 ) &&
								nodeValue->getPhysicalValue( nodeId0, v0 ) &&
								nodeValue->getPhysicalValue( nodeId1, v1 ) &&
								nodeValue->getPhysicalValue( nodeId2, v2 ) &&
								nodeValue->getPhysicalValue( nodeId3, v3 ) )
							{
								drawContourQuad(
									node0.x(), node0.y(), node0.z(), v0[comp],
									node1.x(), node1.y(), node1.z(), v1[comp],
									node2.x(), node2.y(), node2.z(), v2[comp],
									node3.x(), node3.y(), node3.z(), v3[comp], colorMap);
							}
							break;
						}
					}
					++fIter;
				}
			}else{
				while( !fIter.isFinished() )
				{
					if( !fIter.getFace(f) ){
						++fIter;
						continue;
					}
					kmb::ElementContainer::const_iterator element = mesh->findElement( f.getElementId(), bodyId );
					kmb::idType localId = f.getLocalFaceId();
					if( !element.isFinished() ){
						switch( element.getBoundaryVertexCount(localId) )
						{
						case 3:
							nodeId0 = element.getBoundaryCellId(localId,0);
							nodeId1 = element.getBoundaryCellId(localId,1);
							nodeId2 = element.getBoundaryCellId(localId,2);
							if( mesh->getNode( nodeId0, node0 ) &&
								mesh->getNode( nodeId1, node1 ) &&
								mesh->getNode( nodeId2, node2 ) &&
								nodeValue->getPhysicalValue( nodeId0, v0 ) &&
								nodeValue->getPhysicalValue( nodeId1, v1 ) &&
								nodeValue->getPhysicalValue( nodeId2, v2 ) )
							{
								drawSolidContourTriangle( &node0, &node1, &node2,
									v0[comp],
									v1[comp],
									v2[comp],
									colorMap);
							}
							break;
						case 4:
							nodeId0 = element.getBoundaryCellId(localId,0);
							nodeId1 = element.getBoundaryCellId(localId,1);
							nodeId2 = element.getBoundaryCellId(localId,2);
							nodeId3 = element.getBoundaryCellId(localId,3);
							if( mesh->getNode( nodeId0, node0 ) &&
								mesh->getNode( nodeId1, node1 ) &&
								mesh->getNode( nodeId2, node2 ) &&
								mesh->getNode( nodeId3, node3 ) &&
								nodeValue->getPhysicalValue( nodeId0, v0 ) &&
								nodeValue->getPhysicalValue( nodeId1, v1 ) &&
								nodeValue->getPhysicalValue( nodeId2, v2 ) &&
								nodeValue->getPhysicalValue( nodeId3, v3 ) )
							{
								u0 = v0[comp];
								u1 = v1[comp];
								u2 = v2[comp];
								u3 = v3[comp];
								mean = 0.25*(u0 + u1 + u2 + u3);
								center.setCoordinate(
									0.25*(node0.x()+node1.x()+node2.x()+node3.x()),
									0.25*(node0.y()+node1.y()+node2.y()+node3.y()),
									0.25*(node0.z()+node1.z()+node2.z()+node3.z()));
								drawSolidContourTriangle(&node0,&node1,&center,u0,u1,mean,colorMap);
								drawSolidContourTriangle(&node1,&node2,&center,u1,u2,mean,colorMap);
								drawSolidContourTriangle(&node2,&node3,&center,u2,u3,mean,colorMap);
								drawSolidContourTriangle(&node3,&node0,&center,u3,u0,mean,colorMap);
							}
							break;
						}
					}
					++fIter;
				}
			}
		}else if( nodeValue->getValueType() == kmb::PhysicalValue::Vector2withInt && comp == 2 ){
			long v0=0L, v1=0L, v2=0L, v3=0L;
			double mean=0.0;
			if( colorMap->getStep() == 0 ){
				while( !fIter.isFinished() )
				{
					if( !fIter.getFace(f) ){
						++fIter;
						continue;
					}
					kmb::ElementContainer::const_iterator element = mesh->findElement( f.getElementId(), bodyId );
					kmb::idType localId = f.getLocalFaceId();
					if( !element.isFinished() ){
						switch( element.getBoundaryVertexCount(localId) )
						{
						case 3:
							nodeId0 = element.getBoundaryCellId(localId,0);
							nodeId1 = element.getBoundaryCellId(localId,1);
							nodeId2 = element.getBoundaryCellId(localId,2);
							if( mesh->getNode( nodeId0, node0 ) &&
								mesh->getNode( nodeId1, node1 ) &&
								mesh->getNode( nodeId2, node2 ) &&
								nodeValue->getPhysicalValue( nodeId0, &v0 ) &&
								nodeValue->getPhysicalValue( nodeId1, &v1 ) &&
								nodeValue->getPhysicalValue( nodeId2, &v2 ) )
							{
								drawContourTriangle(
									node0.x(), node0.y(), node0.z(), static_cast<double>(v0),
									node1.x(), node1.y(), node1.z(), static_cast<double>(v1),
									node2.x(), node2.y(), node2.z(), static_cast<double>(v2), colorMap);
							}
							break;
						case 4:
							nodeId0 = element.getBoundaryCellId(localId,0);
							nodeId1 = element.getBoundaryCellId(localId,1);
							nodeId2 = element.getBoundaryCellId(localId,2);
							nodeId3 = element.getBoundaryCellId(localId,3);
							if( mesh->getNode( nodeId0, node0 ) &&
								mesh->getNode( nodeId1, node1 ) &&
								mesh->getNode( nodeId2, node2 ) &&
								mesh->getNode( nodeId3, node3 ) &&
								nodeValue->getPhysicalValue( nodeId0, &v0 ) &&
								nodeValue->getPhysicalValue( nodeId1, &v1 ) &&
								nodeValue->getPhysicalValue( nodeId2, &v2 ) &&
								nodeValue->getPhysicalValue( nodeId3, &v3 ) )
							{
								drawContourQuad(
									node0.x(), node0.y(), node0.z(), static_cast<double>(v0),
									node1.x(), node1.y(), node1.z(), static_cast<double>(v1),
									node2.x(), node2.y(), node2.z(), static_cast<double>(v2),
									node3.x(), node3.y(), node3.z(), static_cast<double>(v3), colorMap);
							}
							break;
						}
					}
					++fIter;
				}
			}else{
				while( !fIter.isFinished() )
				{
					if( !fIter.getFace(f) ){
						++fIter;
						continue;
					}
					kmb::ElementContainer::const_iterator element = mesh->findElement( f.getElementId(), bodyId );
					kmb::idType localId = f.getLocalFaceId();
					if( !element.isFinished() ){
						switch( element.getBoundaryVertexCount(localId) )
						{
						case 3:
							nodeId0 = element.getBoundaryCellId(localId,0);
							nodeId1 = element.getBoundaryCellId(localId,1);
							nodeId2 = element.getBoundaryCellId(localId,2);
							if( mesh->getNode( nodeId0, node0 ) &&
								mesh->getNode( nodeId1, node1 ) &&
								mesh->getNode( nodeId2, node2 ) &&
								nodeValue->getPhysicalValue( nodeId0, &v0 ) &&
								nodeValue->getPhysicalValue( nodeId1, &v1 ) &&
								nodeValue->getPhysicalValue( nodeId2, &v2 ) )
							{
								drawSolidContourTriangle( &node0, &node1, &node2,
									static_cast<double>(v0),
									static_cast<double>(v1),
									static_cast<double>(v2),
									colorMap);
							}
							break;
						case 4:
							nodeId0 = element.getBoundaryCellId(localId,0);
							nodeId1 = element.getBoundaryCellId(localId,1);
							nodeId2 = element.getBoundaryCellId(localId,2);
							nodeId3 = element.getBoundaryCellId(localId,3);
							if( mesh->getNode( nodeId0, node0 ) &&
								mesh->getNode( nodeId1, node1 ) &&
								mesh->getNode( nodeId2, node2 ) &&
								mesh->getNode( nodeId3, node3 ) &&
								nodeValue->getPhysicalValue( nodeId0, &v0 ) &&
								nodeValue->getPhysicalValue( nodeId1, &v1 ) &&
								nodeValue->getPhysicalValue( nodeId2, &v2 ) &&
								nodeValue->getPhysicalValue( nodeId3, &v3 ) )
							{
								mean = 0.25*static_cast<double>( (v0 + v1 + v2 + v3) );
								center.setCoordinate(
									0.25*(node0.x()+node1.x()+node2.x()+node3.x()),
									0.25*(node0.y()+node1.y()+node2.y()+node3.y()),
									0.25*(node0.z()+node1.z()+node2.z()+node3.z()));
								drawSolidContourTriangle(&node0,&node1,&center,
									static_cast<double>(v0),static_cast<double>(v1),mean,colorMap);
								drawSolidContourTriangle(&node1,&node2,&center,
									static_cast<double>(v1),static_cast<double>(v2),mean,colorMap);
								drawSolidContourTriangle(&node2,&node3,&center,
									static_cast<double>(v2),static_cast<double>(v3),mean,colorMap);
								drawSolidContourTriangle(&node3,&node0,&center,
									static_cast<double>(v3),static_cast<double>(v0),mean,colorMap);
							}
							break;
						}
					}
					++fIter;
				}
			}
		}
		::glPopAttrib();
		::glFlush();
	}
}

void
kmb::MeshGL::drawNodeContourOnFaceGroup(const char* faceName, const char* physicalValue, kmb::ColorMap* colorMap,int comp)
{
	const kmb::DataBindings* data = NULL;
	const kmb::DataBindings* nodeValue = NULL;
	if( mesh != NULL && colorMap != NULL &&
		(data = mesh->getDataBindingsPtr( faceName )) != NULL &&
		data->getBindingMode() == kmb::DataBindings::FaceGroup &&
		(nodeValue = mesh->getDataBindingsPtr( physicalValue )) != NULL &&
		nodeValue->getBindingMode() == kmb::DataBindings::NodeVariable )
	{
		::glBegin( GL_POINTS );
		std::set< kmb::nodeIdType > nodeSet;
		kmb::Node node;
		kmb::bodyIdType bodyId = data->getTargetBodyId();
		kmb::Face f;
		kmb::DataBindings::const_iterator fIter = data->begin();
		if( nodeValue->getValueType() == kmb::PhysicalValue::Scalar ){
			double v = 0.0;
			while( !fIter.isFinished() )
			{
				if( !fIter.getFace(f) ){
					++fIter;
					continue;
				}
				kmb::ElementContainer::const_iterator element = mesh->findElement( f.getElementId(), bodyId );
				kmb::idType localId = f.getLocalFaceId();
				if( !element.isFinished() ){
					int nodeCount = element.getBoundaryNodeCount(localId);
					for(int i=0;i<nodeCount;++i){
						kmb::nodeIdType nodeId = element.getBoundaryCellId(localId,i);
						if( nodeSet.insert(nodeId).second &&
							mesh->getNode( nodeId, node ) &&
							nodeValue->getPhysicalValue( nodeId, &v ) )
						{
							colorMap->setGLColor( v );
							::glVertex3d( node.x(), node.y(), node.z() );
						}
					}
				}
				++fIter;
			}
		}else if( nodeValue->getValueType() == kmb::PhysicalValue::Vector3 ){
			double v[3] = {0.0, 0.0, 0.0};
			if( comp == -1 ){
				while( !fIter.isFinished() )
				{
					if( !fIter.getFace(f) ){
						++fIter;
						continue;
					}
					kmb::ElementContainer::const_iterator element = mesh->findElement( f.getElementId(), bodyId );
					kmb::idType localId = f.getLocalFaceId();
					if( !element.isFinished() ){
						int nodeCount = element.getBoundaryNodeCount(localId);
						for(int i=0;i<nodeCount;++i){
							kmb::nodeIdType nodeId = element.getBoundaryCellId(localId,i);
							if( nodeSet.insert(nodeId).second &&
								mesh->getNode( nodeId, node ) &&
								nodeValue->getPhysicalValue( nodeId, v ) )
							{
								colorMap->setGLColor( kmb::Vector3D::abs(v) );
								::glVertex3d( node.x(), node.y(), node.z() );
							}
						}
						++fIter;
					}
				}
			}else if( 0 <= comp && comp < 3 ){
				while( !fIter.isFinished() )
				{
					if( !fIter.getFace(f) ){
						++fIter;
						continue;
					}
					kmb::ElementContainer::const_iterator element = mesh->findElement( f.getElementId(), bodyId );
					kmb::idType localId = f.getLocalFaceId();
					if( !element.isFinished() ){
						int nodeCount = element.getBoundaryNodeCount(localId);
						for(int i=0;i<nodeCount;++i){
							kmb::nodeIdType nodeId = element.getBoundaryCellId(localId,i);
							if( nodeSet.insert(nodeId).second &&
								mesh->getNode( nodeId, node ) &&
								nodeValue->getPhysicalValue( nodeId, v ) )
							{
								colorMap->setGLColor( v[comp] );
								::glVertex3d( node.x(), node.y(), node.z() );
							}
						}
						++fIter;
					}
				}
			}
		}else if( nodeValue->getValueType() == kmb::PhysicalValue::Tensor6 ){
			double v[6] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
			if( 0 <= comp && comp < 6 ){
				while( !fIter.isFinished() )
				{
					if( !fIter.getFace(f) ){
						++fIter;
						continue;
					}
					kmb::ElementContainer::const_iterator element = mesh->findElement( f.getElementId(), bodyId );
					kmb::idType localId = f.getLocalFaceId();
					if( !element.isFinished() ){
						int nodeCount = element.getBoundaryNodeCount(localId);
						for(int i=0;i<nodeCount;++i){
							kmb::nodeIdType nodeId = element.getBoundaryCellId(localId,i);
							if( nodeSet.insert(nodeId).second &&
								mesh->getNode( nodeId, node ) &&
								nodeValue->getPhysicalValue( nodeId, v ) )
							{
								colorMap->setGLColor( v[comp] );
								::glVertex3d( node.x(), node.y(), node.z() );
							}
						}
						++fIter;
					}
				}
			}
		}else if( nodeValue->getValueType() == kmb::PhysicalValue::Vector2withInt && 0 <= comp && comp < 2 ){
			double v[2] = {0.0, 0.0};
			while( !fIter.isFinished() )
			{
				if( !fIter.getFace(f) ){
					++fIter;
					continue;
				}
				kmb::ElementContainer::const_iterator element = mesh->findElement( f.getElementId(), bodyId );
				kmb::idType localId = f.getLocalFaceId();
				if( !element.isFinished() ){
					int nodeCount = element.getBoundaryNodeCount(localId);
					for(int i=0;i<nodeCount;++i){
						kmb::nodeIdType nodeId = element.getBoundaryCellId(localId,i);
						if( nodeSet.insert(nodeId).second &&
							mesh->getNode( nodeId, node ) &&
							nodeValue->getPhysicalValue( nodeId, v ) )
						{
							colorMap->setGLColor( v[comp] );
							::glVertex3d( node.x(), node.y(), node.z() );
						}
					}
					++fIter;
				}
			}
		}else if( nodeValue->getValueType() == kmb::PhysicalValue::Vector2withInt && comp == 2 ){
			long v = 0L;
			while( !fIter.isFinished() )
			{
				if( !fIter.getFace(f) ){
					++fIter;
					continue;
				}
				kmb::ElementContainer::const_iterator element = mesh->findElement( f.getElementId(), bodyId );
				kmb::idType localId = f.getLocalFaceId();
				if( !element.isFinished() ){
					int nodeCount = element.getBoundaryNodeCount(localId);
					for(int i=0;i<nodeCount;++i){
						kmb::nodeIdType nodeId = element.getBoundaryCellId(localId,i);
						if( nodeSet.insert(nodeId).second &&
							mesh->getNode( nodeId, node ) &&
							nodeValue->getPhysicalValue( nodeId, &v ) )
						{
							colorMap->setGLColor( static_cast<double>(v) );
							::glVertex3d( node.x(), node.y(), node.z() );
						}
					}
					++fIter;
				}
			}
		}else if( nodeValue->getValueType() == kmb::PhysicalValue::Integer ){
			long v = 0L;
			while( !fIter.isFinished() )
			{
				if( !fIter.getFace(f) ){
					++fIter;
					continue;
				}
				kmb::ElementContainer::const_iterator element = mesh->findElement( f.getElementId(), bodyId );
				kmb::idType localId = f.getLocalFaceId();
				if( !element.isFinished() ){
					int nodeCount = element.getBoundaryNodeCount(localId);
					for(int i=0;i<nodeCount;++i){
						kmb::nodeIdType nodeId = element.getBoundaryCellId(localId,i);
						if( nodeSet.insert(nodeId).second &&
							mesh->getNode( nodeId, node ) &&
							nodeValue->getPhysicalValue( nodeId, &v ) )
						{
							colorMap->setGLColorByStep( v );
							::glVertex3d( node.x(), node.y(), node.z() );
						}
					}
					++fIter;
				}
			}
		}
		::glEnd();
		::glFlush();
	}
}

void
kmb::MeshGL::drawNodeContourAllNodes(const char* physicalValue, kmb::ColorMap* colorMap,int comp)
{
	const kmb::DataBindings* nodeValue = NULL;
	if( mesh != NULL && colorMap != NULL &&
		(nodeValue = mesh->getDataBindingsPtr( physicalValue )) != NULL &&
		nodeValue->getBindingMode() == kmb::DataBindings::NodeVariable )
	{
		::glPushAttrib( GL_ENABLE_BIT );
		::glEnable( GL_COLOR_MATERIAL );
		::glBegin( GL_POINTS );
		if( nodeValue->getValueType() == kmb::PhysicalValue::Scalar ){
			double x = 0.0, y = 0.0, z = 0.0, v=0.0;
			kmb::Point3DContainer::const_iterator pIter = mesh->getNodes()->begin();
			while( !pIter.isFinished() ){
				if( nodeValue->getPhysicalValue( pIter.getId(), &v ) ){
					colorMap->setGLColor( v );
					pIter.getXYZ( x, y, z );
					::glVertex3d( x, y, z );
				}
				++pIter;
			}
		}else if( nodeValue->getValueType() == kmb::PhysicalValue::Vector3 ){
			double x = 0.0, y = 0.0, z = 0.0;
			double v[3] = {0.0, 0.0, 0.0};
			if( comp == -1 ){
				kmb::Point3DContainer::const_iterator pIter = mesh->getNodes()->begin();
				while( !pIter.isFinished() ){
					if( nodeValue->getPhysicalValue( pIter.getId(), v ) ){
						colorMap->setGLColor( kmb::Vector3D::abs(v) );
						pIter.getXYZ( x, y, z );
						::glVertex3d( x, y, z );
					}
					++pIter;
				}
			}else if( 0 <= comp && comp < 3 ){
				kmb::Point3DContainer::const_iterator pIter = mesh->getNodes()->begin();
				while( !pIter.isFinished() ){
					if( nodeValue->getPhysicalValue( pIter.getId(), v ) ){
						colorMap->setGLColor( v[comp] );
						pIter.getXYZ( x, y, z );
						::glVertex3d( x, y, z );
					}
					++pIter;
				}
			}
		}else if( nodeValue->getValueType() == kmb::PhysicalValue::Integer ){
			double x = 0.0, y = 0.0, z = 0.0;
			long v = 0L;
			kmb::Point3DContainer::const_iterator pIter = mesh->getNodes()->begin();
			while( !pIter.isFinished() ){
				if( nodeValue->getPhysicalValue( pIter.getId(), &v ) ){
					colorMap->setGLColorByStep( v );
					pIter.getXYZ( x, y, z );
					::glVertex3d( x, y, z );
				}
				++pIter;
			}
		}
		::glEnd();
		::glPopAttrib();
		::glFlush();
	}
}

void
kmb::MeshGL::deformFaceGroupNodeContour
(const char* faceName, const char* displacement, double factor,
 const char* physicalValue, kmb::ColorMap* colorMap,int comp)
{
	const kmb::DataBindings* data = NULL;
	const kmb::DataBindings* displace = NULL;
	const kmb::DataBindings* nodeValue = NULL;
	if( mesh != NULL && colorMap != NULL &&
		(data = mesh->getDataBindingsPtr( faceName )) != NULL &&
		data->getBindingMode() == kmb::DataBindings::FaceGroup &&
		(displace = mesh->getDataBindingsPtr( displacement )) != NULL &&
		displace->getBindingMode() == kmb::DataBindings::NodeVariable &&
		displace->getValueType() == kmb::PhysicalValue::Vector3 &&
		(nodeValue = mesh->getDataBindingsPtr( physicalValue )) != NULL &&
		nodeValue->getBindingMode() == kmb::DataBindings::NodeVariable )
	{
		::glPushAttrib( GL_ENABLE_BIT );
		::glEnable(GL_COLOR_MATERIAL);

		kmb::bodyIdType bodyId = data->getTargetBodyId();
		kmb::nodeIdType nodeId0 = kmb::nullNodeId, nodeId1 = kmb::nullNodeId, nodeId2 = kmb::nullNodeId, nodeId3 = kmb::nullNodeId;
		kmb::Node node0,node1,node2,node3,n0,n1,n2,n3,center;
		kmb::Face f;
		double d0[3] = {0.0, 0.0, 0.0},
			d1[3] = {0.0, 0.0, 0.0},
			d2[3] = {0.0, 0.0, 0.0},
			d3[3] = {0.0, 0.0, 0.0};
		kmb::DataBindings::const_iterator fIter = data->begin();
		if( nodeValue->getValueType() == kmb::PhysicalValue::Scalar ){
			double v0 = 0.0, v1 = 0.0, v2 = 0.0, v3 = 0.0, mean = 0.0;
			if( colorMap->getStep() == 0 ){
				while( !fIter.isFinished() ){
					if( !fIter.getFace(f) ){
						++fIter;
						continue;
					}
					kmb::ElementContainer::const_iterator element = mesh->findElement( f.getElementId(), bodyId );
					kmb::idType localId = f.getLocalFaceId();
					if( !element.isFinished() ){
						switch( element.getBoundaryVertexCount( localId ) ){
							case 3:
								nodeId0 = element.getBoundaryCellId(localId,0);
								nodeId1 = element.getBoundaryCellId(localId,1);
								nodeId2 = element.getBoundaryCellId(localId,2);
								if( mesh->getNode( nodeId0, node0 ) &&
									mesh->getNode( nodeId1, node1 ) &&
									mesh->getNode( nodeId2, node2 ) &&
									displace->getPhysicalValue( nodeId0, d0 ) &&
									displace->getPhysicalValue( nodeId1, d1 ) &&
									displace->getPhysicalValue( nodeId2, d2 ) &&
									nodeValue->getPhysicalValue( nodeId0, &v0 ) &&
									nodeValue->getPhysicalValue( nodeId1, &v1 ) &&
									nodeValue->getPhysicalValue( nodeId2, &v2 ) )
								{
									drawContourTriangle(
										node0.x() + factor * d0[0],
										node0.y() + factor * d0[1],
										node0.z() + factor * d0[2],
										v0,
										node1.x() + factor * d1[0],
										node1.y() + factor * d1[1],
										node1.z() + factor * d1[2],
										v1,
										node2.x() + factor * d2[0],
										node2.y() + factor * d2[1],
										node2.z() + factor * d2[2],
										v2,
										colorMap );
								}
								break;
							case 4:
								nodeId0 = element.getBoundaryCellId(localId,0);
								nodeId1 = element.getBoundaryCellId(localId,1);
								nodeId2 = element.getBoundaryCellId(localId,2);
								nodeId3 = element.getBoundaryCellId(localId,3);
								if( mesh->getNode( nodeId0, node0 ) &&
									mesh->getNode( nodeId1, node1 ) &&
									mesh->getNode( nodeId2, node2 ) &&
									mesh->getNode( nodeId3, node3 ) &&
									displace->getPhysicalValue( nodeId0, d0 ) &&
									displace->getPhysicalValue( nodeId1, d1 ) &&
									displace->getPhysicalValue( nodeId2, d2 ) &&
									displace->getPhysicalValue( nodeId3, d3 ) &&
									nodeValue->getPhysicalValue( nodeId0, &v0 ) &&
									nodeValue->getPhysicalValue( nodeId1, &v1 ) &&
									nodeValue->getPhysicalValue( nodeId2, &v2 ) &&
									nodeValue->getPhysicalValue( nodeId3, &v3 ) )
								{
									drawContourQuad(
										node0.x() + factor * d0[0],
										node0.y() + factor * d0[1],
										node0.z() + factor * d0[2],
										v0,
										node1.x() + factor * d1[0],
										node1.y() + factor * d1[1],
										node1.z() + factor * d1[2],
										v1,
										node2.x() + factor * d2[0],
										node2.y() + factor * d2[1],
										node2.z() + factor * d2[2],
										v2,
										node3.x() + factor * d3[0],
										node3.y() + factor * d3[1],
										node3.z() + factor * d3[2],
										v3,
										colorMap );
								}
								break;
							default:
								break;
						}
					}
					++fIter;
				}
			}else{ // solid contour
				while( !fIter.isFinished() ){
					if( !fIter.getFace(f) ){
						++fIter;
						continue;
					}
					kmb::ElementContainer::const_iterator element = mesh->findElement( f.getElementId(), bodyId );
					kmb::idType localId = f.getLocalFaceId();
					if( !element.isFinished() ){
						switch( element.getBoundaryVertexCount( localId ) ){
							case 3:
								nodeId0 = element.getBoundaryCellId(localId,0);
								nodeId1 = element.getBoundaryCellId(localId,1);
								nodeId2 = element.getBoundaryCellId(localId,2);
								if( mesh->getNode( nodeId0, node0 ) &&
									mesh->getNode( nodeId1, node1 ) &&
									mesh->getNode( nodeId2, node2 ) &&
									displace->getPhysicalValue( nodeId0, d0 ) &&
									displace->getPhysicalValue( nodeId1, d1 ) &&
									displace->getPhysicalValue( nodeId2, d2 ) &&
									nodeValue->getPhysicalValue( nodeId0, &v0 ) &&
									nodeValue->getPhysicalValue( nodeId1, &v1 ) &&
									nodeValue->getPhysicalValue( nodeId2, &v2 ) )
								{
									n0.setCoordinate(
										node0.x() + factor * d0[0],
										node0.y() + factor * d0[1],
										node0.z() + factor * d0[2]);
									n1.setCoordinate(
										node1.x() + factor * d1[0],
										node1.y() + factor * d1[1],
										node1.z() + factor * d1[2]);
									n2.setCoordinate(
										node2.x() + factor * d2[0],
										node2.y() + factor * d2[1],
										node2.z() + factor * d2[2]);
									drawSolidContourTriangle(&n0,&n1,&n2,v0,v1,v2,colorMap);
								}
								break;
							case 4:
								nodeId0 = element.getBoundaryCellId(localId,0);
								nodeId1 = element.getBoundaryCellId(localId,1);
								nodeId2 = element.getBoundaryCellId(localId,2);
								nodeId3 = element.getBoundaryCellId(localId,3);
								if( mesh->getNode( nodeId0, node0 ) &&
									mesh->getNode( nodeId1, node1 ) &&
									mesh->getNode( nodeId2, node2 ) &&
									mesh->getNode( nodeId3, node3 ) &&
									displace->getPhysicalValue( nodeId0, d0 ) &&
									displace->getPhysicalValue( nodeId1, d1 ) &&
									displace->getPhysicalValue( nodeId2, d2 ) &&
									displace->getPhysicalValue( nodeId3, d3 ) &&
									nodeValue->getPhysicalValue( nodeId0, &v0 ) &&
									nodeValue->getPhysicalValue( nodeId1, &v1 ) &&
									nodeValue->getPhysicalValue( nodeId2, &v2 ) &&
									nodeValue->getPhysicalValue( nodeId3, &v3 ) )
								{
									n0.setCoordinate(
										node0.x() + factor * d0[0],
										node0.y() + factor * d0[1],
										node0.z() + factor * d0[2]);
									n1.setCoordinate(
										node1.x() + factor * d1[0],
										node1.y() + factor * d1[1],
										node1.z() + factor * d1[2]);
									n2.setCoordinate(
										node2.x() + factor * d2[0],
										node2.y() + factor * d2[1],
										node2.z() + factor * d2[2]);
									n3.setCoordinate(
										node3.x() + factor * d3[0],
										node3.y() + factor * d3[1],
										node3.z() + factor * d3[2]);
									center.setCoordinate(
										0.25*(n0.x()+n1.x()+n2.x()+n3.x()),
										0.25*(n0.y()+n1.y()+n2.y()+n3.y()),
										0.25*(n0.z()+n1.z()+n2.z()+n3.z()));
									mean = 0.25*(v0 + v1 + v2 + v3);
									drawSolidContourTriangle(&n0,&n1,&center,v0,v1,mean,colorMap);
									drawSolidContourTriangle(&n1,&n2,&center,v1,v2,mean,colorMap);
									drawSolidContourTriangle(&n2,&n3,&center,v2,v3,mean,colorMap);
									drawSolidContourTriangle(&n3,&n0,&center,v3,v0,mean,colorMap);
								}
								break;
							default:
								break;
						}
					}
					++fIter;
				}
			}
		}else if( data->getValueType() == kmb::PhysicalValue::Vector3 && comp == -1){
			double v0[3] = {0.0, 0.0, 0.0},
				v1[3] = {0.0, 0.0, 0.0},
				v2[3] = {0.0, 0.0, 0.0},
				v3[3] = {0.0, 0.0, 0.0},
				u0 = 0.0, u1 =0.0, u2= 0.0, u3 = 0.0, mean = 0.0;
			if( colorMap->getStep() == 0 ){
				while( !fIter.isFinished() ){
					if( !fIter.getFace(f) ){
						++fIter;
						continue;
					}
					kmb::ElementContainer::const_iterator element = mesh->findElement( f.getElementId(), bodyId );
					kmb::idType localId = f.getLocalFaceId();
					if( !element.isFinished() ){
						switch( element.getBoundaryVertexCount( localId ) ){
							case 3:
								nodeId0 = element.getBoundaryCellId(localId,0);
								nodeId1 = element.getBoundaryCellId(localId,1);
								nodeId2 = element.getBoundaryCellId(localId,2);
								if( mesh->getNode( nodeId0, node0 ) &&
									mesh->getNode( nodeId1, node1 ) &&
									mesh->getNode( nodeId2, node2 ) &&
									displace->getPhysicalValue( nodeId0, d0 ) &&
									displace->getPhysicalValue( nodeId1, d1 ) &&
									displace->getPhysicalValue( nodeId2, d2 ) &&
									nodeValue->getPhysicalValue( nodeId0, v0 ) &&
									nodeValue->getPhysicalValue( nodeId1, v1 ) &&
									nodeValue->getPhysicalValue( nodeId2, v2 ) )
								{
									drawContourTriangle(
										node0.x() + factor * d0[0],
										node0.y() + factor * d0[1],
										node0.z() + factor * d0[2],
										kmb::Vector3D::abs(v0),
										node1.x() + factor * d1[0],
										node1.y() + factor * d1[1],
										node1.z() + factor * d1[2],
										kmb::Vector3D::abs(v1),
										node2.x() + factor * d2[0],
										node2.y() + factor * d2[1],
										node2.z() + factor * d2[2],
										kmb::Vector3D::abs(v2),
										colorMap );
								}
								break;
							case 4:
								nodeId0 = element.getBoundaryCellId(localId,0);
								nodeId1 = element.getBoundaryCellId(localId,1);
								nodeId2 = element.getBoundaryCellId(localId,2);
								nodeId3 = element.getBoundaryCellId(localId,3);
								if( mesh->getNode( nodeId0, node0 ) &&
									mesh->getNode( nodeId1, node1 ) &&
									mesh->getNode( nodeId2, node2 ) &&
									mesh->getNode( nodeId3, node3 ) &&
									displace->getPhysicalValue( nodeId0, d0 ) &&
									displace->getPhysicalValue( nodeId1, d1 ) &&
									displace->getPhysicalValue( nodeId2, d2 ) &&
									displace->getPhysicalValue( nodeId3, d3 ) &&
									nodeValue->getPhysicalValue( nodeId0, v0 ) &&
									nodeValue->getPhysicalValue( nodeId1, v1 ) &&
									nodeValue->getPhysicalValue( nodeId2, v2 ) &&
									nodeValue->getPhysicalValue( nodeId3, v3 ) )
								{
									drawContourQuad(
										node0.x() + factor * d0[0],
										node0.y() + factor * d0[1],
										node0.z() + factor * d0[2],
										kmb::Vector3D::abs(v0),
										node1.x() + factor * d1[0],
										node1.y() + factor * d1[1],
										node1.z() + factor * d1[2],
										kmb::Vector3D::abs(v1),
										node2.x() + factor * d2[0],
										node2.y() + factor * d2[1],
										node2.z() + factor * d2[2],
										kmb::Vector3D::abs(v2),
										node3.x() + factor * d3[0],
										node3.y() + factor * d3[1],
										node3.z() + factor * d3[2],
										kmb::Vector3D::abs(v3),
										colorMap );
								}
								break;
							default:
								break;
						}
					}
					++fIter;
				}
			}else{ // solid contour
				while( !fIter.isFinished() ){
					if( !fIter.getFace(f) ){
						++fIter;
						continue;
					}
					kmb::ElementContainer::const_iterator element = mesh->findElement( f.getElementId(), bodyId );
					kmb::idType localId = f.getLocalFaceId();
					if( !element.isFinished() ){
						switch( element.getBoundaryVertexCount( localId ) ){
							case 3:
								nodeId0 = element.getBoundaryCellId(localId,0);
								nodeId1 = element.getBoundaryCellId(localId,1);
								nodeId2 = element.getBoundaryCellId(localId,2);
								if( mesh->getNode( nodeId0, node0 ) &&
									mesh->getNode( nodeId1, node1 ) &&
									mesh->getNode( nodeId2, node2 ) &&
									displace->getPhysicalValue( nodeId0, d0 ) &&
									displace->getPhysicalValue( nodeId1, d1 ) &&
									displace->getPhysicalValue( nodeId2, d2 ) &&
									nodeValue->getPhysicalValue( nodeId0, v0 ) &&
									nodeValue->getPhysicalValue( nodeId1, v1 ) &&
									nodeValue->getPhysicalValue( nodeId2, v2 ) )
								{
									n0.setCoordinate(
										node0.x() + factor * d0[0],
										node0.y() + factor * d0[1],
										node0.z() + factor * d0[2]);
									n1.setCoordinate(
										node1.x() + factor * d1[0],
										node1.y() + factor * d1[1],
										node1.z() + factor * d1[2]);
									n2.setCoordinate(
										node2.x() + factor * d2[0],
										node2.y() + factor * d2[1],
										node2.z() + factor * d2[2]);
									drawSolidContourTriangle(&n0,&n1,&n2,
										kmb::Vector3D::abs(v0),
										kmb::Vector3D::abs(v1),
										kmb::Vector3D::abs(v2),
										colorMap);
								}
								break;
							case 4:
								nodeId0 = element.getBoundaryCellId(localId,0);
								nodeId1 = element.getBoundaryCellId(localId,1);
								nodeId2 = element.getBoundaryCellId(localId,2);
								nodeId3 = element.getBoundaryCellId(localId,3);
								if( mesh->getNode( nodeId0, node0 ) &&
									mesh->getNode( nodeId1, node1 ) &&
									mesh->getNode( nodeId2, node2 ) &&
									mesh->getNode( nodeId3, node3 ) &&
									displace->getPhysicalValue( nodeId0, d0 ) &&
									displace->getPhysicalValue( nodeId1, d1 ) &&
									displace->getPhysicalValue( nodeId2, d2 ) &&
									displace->getPhysicalValue( nodeId3, d3 ) &&
									nodeValue->getPhysicalValue( nodeId0, v0 ) &&
									nodeValue->getPhysicalValue( nodeId1, v1 ) &&
									nodeValue->getPhysicalValue( nodeId1, v2 ) &&
									nodeValue->getPhysicalValue( nodeId2, v3 ) )
								{
									n0.setCoordinate(
										node0.x() + factor * d0[0],
										node0.y() + factor * d0[1],
										node0.z() + factor * d0[2]);
									n1.setCoordinate(
										node1.x() + factor * d1[0],
										node1.y() + factor * d1[1],
										node1.z() + factor * d1[2]);
									n2.setCoordinate(
										node2.x() + factor * d2[0],
										node2.y() + factor * d2[1],
										node2.z() + factor * d2[2]);
									n3.setCoordinate(
										node3.x() + factor * d3[0],
										node3.y() + factor * d3[1],
										node3.z() + factor * d3[2]);
									center.setCoordinate(
										0.25*(n0.x()+n1.x()+n2.x()+n3.x()),
										0.25*(n0.y()+n1.y()+n2.y()+n3.y()),
										0.25*(n0.z()+n1.z()+n2.z()+n3.z()));
									u0 = kmb::Vector3D::abs( v0 );
									u1 = kmb::Vector3D::abs( v1 );
									u2 = kmb::Vector3D::abs( v2 );
									u3 = kmb::Vector3D::abs( v3 );
									mean = 0.25*(u0 + u1 + u2 + u3);
									drawSolidContourTriangle(&n0,&n1,&center,u0,u1,mean,colorMap);
									drawSolidContourTriangle(&n1,&n2,&center,u1,u2,mean,colorMap);
									drawSolidContourTriangle(&n2,&n3,&center,u2,u3,mean,colorMap);
									drawSolidContourTriangle(&n3,&n0,&center,u3,u0,mean,colorMap);
								}
								break;
							default:
								break;
						}
					}
					++fIter;
				}
			}
		}else if( data->getValueType() == kmb::PhysicalValue::Vector3 && 0 <= comp && comp < 3){
			double v0[3] = {0.0, 0.0, 0.0},
				v1[3] = {0.0, 0.0, 0.0},
				v2[3] = {0.0, 0.0, 0.0},
				v3[3] = {0.0, 0.0, 0.0},
				u0 = 0.0, u1 =0.0, u2= 0.0, u3 = 0.0, mean = 0.0;
			if( colorMap->getStep() == 0 ){
				while( !fIter.isFinished() ){
					if( !fIter.getFace(f) ){
						++fIter;
						continue;
					}
					kmb::ElementContainer::const_iterator element = mesh->findElement( f.getElementId(), bodyId );
					kmb::idType localId = f.getLocalFaceId();
					if( !element.isFinished() ){
						switch( element.getBoundaryVertexCount( localId ) ){
							case 3:
								nodeId0 = element.getBoundaryCellId(localId,0);
								nodeId1 = element.getBoundaryCellId(localId,1);
								nodeId2 = element.getBoundaryCellId(localId,2);
								if( mesh->getNode( nodeId0, node0 ) &&
									mesh->getNode( nodeId1, node1 ) &&
									mesh->getNode( nodeId2, node2 ) &&
									displace->getPhysicalValue( nodeId0, d0 ) &&
									displace->getPhysicalValue( nodeId1, d1 ) &&
									displace->getPhysicalValue( nodeId2, d2 ) &&
									nodeValue->getPhysicalValue( nodeId0, v0 ) &&
									nodeValue->getPhysicalValue( nodeId1, v1 ) &&
									nodeValue->getPhysicalValue( nodeId2, v2 ) )
								{
									drawContourTriangle(
										node0.x() + factor * d0[0],
										node0.y() + factor * d0[1],
										node0.z() + factor * d0[2],
										v0[comp],
										node1.x() + factor * d1[0],
										node1.y() + factor * d1[1],
										node1.z() + factor * d1[2],
										v1[comp],
										node2.x() + factor * d2[0],
										node2.y() + factor * d2[1],
										node2.z() + factor * d2[2],
										v2[comp],
										colorMap );
								}
								break;
							case 4:
								nodeId0 = element.getBoundaryCellId(localId,0);
								nodeId1 = element.getBoundaryCellId(localId,1);
								nodeId2 = element.getBoundaryCellId(localId,2);
								nodeId3 = element.getBoundaryCellId(localId,3);
								if( mesh->getNode( nodeId0, node0 ) &&
									mesh->getNode( nodeId1, node1 ) &&
									mesh->getNode( nodeId2, node2 ) &&
									mesh->getNode( nodeId3, node3 ) &&
									displace->getPhysicalValue( nodeId0, d0 ) &&
									displace->getPhysicalValue( nodeId1, d1 ) &&
									displace->getPhysicalValue( nodeId2, d2 ) &&
									displace->getPhysicalValue( nodeId3, d3 ) &&
									nodeValue->getPhysicalValue( nodeId0, v0 ) &&
									nodeValue->getPhysicalValue( nodeId1, v1 ) &&
									nodeValue->getPhysicalValue( nodeId2, v2 ) &&
									nodeValue->getPhysicalValue( nodeId3, v3 ) )
								{
									drawContourQuad(
										node0.x() + factor * d0[0],
										node0.y() + factor * d0[1],
										node0.z() + factor * d0[2],
										v0[comp],
										node1.x() + factor * d1[0],
										node1.y() + factor * d1[1],
										node1.z() + factor * d1[2],
										v1[comp],
										node2.x() + factor * d2[0],
										node2.y() + factor * d2[1],
										node2.z() + factor * d2[2],
										v2[comp],
										node3.x() + factor * d3[0],
										node3.y() + factor * d3[1],
										node3.z() + factor * d3[2],
										v3[comp],
										colorMap );
								}
								break;
							default:
								break;
						}
					}
					++fIter;
				}
			}else{ // solid contour
				while( !fIter.isFinished() ){
					if( !fIter.getFace(f) ){
						++fIter;
						continue;
					}
					kmb::ElementContainer::const_iterator element = mesh->findElement( f.getElementId(), bodyId );
					kmb::idType localId = f.getLocalFaceId();
					if( !element.isFinished() ){
						switch( element.getBoundaryVertexCount( localId ) ){
							case 3:
								nodeId0 = element.getBoundaryCellId(localId,0);
								nodeId1 = element.getBoundaryCellId(localId,1);
								nodeId2 = element.getBoundaryCellId(localId,2);
								if( mesh->getNode( nodeId0, node0 ) &&
									mesh->getNode( nodeId1, node1 ) &&
									mesh->getNode( nodeId2, node2 ) &&
									displace->getPhysicalValue( nodeId0, d0 ) &&
									displace->getPhysicalValue( nodeId1, d1 ) &&
									displace->getPhysicalValue( nodeId2, d2 ) &&
									nodeValue->getPhysicalValue( nodeId0, v0 ) &&
									nodeValue->getPhysicalValue( nodeId1, v1 ) &&
									nodeValue->getPhysicalValue( nodeId2, v2 ) )
								{
									n0.setCoordinate(
										node0.x() + factor * d0[0],
										node0.y() + factor * d0[1],
										node0.z() + factor * d0[2]);
									n1.setCoordinate(
										node1.x() + factor * d1[0],
										node1.y() + factor * d1[1],
										node1.z() + factor * d1[2]);
									n2.setCoordinate(
										node2.x() + factor * d2[0],
										node2.y() + factor * d2[1],
										node2.z() + factor * d2[2]);
									drawSolidContourTriangle(&n0,&n1,&n2,
										v0[comp],
										v1[comp],
										v2[comp],
										colorMap);
								}
								break;
							case 4:
								nodeId0 = element.getBoundaryCellId(localId,0);
								nodeId1 = element.getBoundaryCellId(localId,1);
								nodeId2 = element.getBoundaryCellId(localId,2);
								nodeId3 = element.getBoundaryCellId(localId,3);
								if( mesh->getNode( nodeId0, node0 ) &&
									mesh->getNode( nodeId1, node1 ) &&
									mesh->getNode( nodeId2, node2 ) &&
									mesh->getNode( nodeId3, node3 ) &&
									displace->getPhysicalValue( nodeId0, d0 ) &&
									displace->getPhysicalValue( nodeId1, d1 ) &&
									displace->getPhysicalValue( nodeId2, d2 ) &&
									displace->getPhysicalValue( nodeId3, d3 ) &&
									nodeValue->getPhysicalValue( nodeId0, v0 ) &&
									nodeValue->getPhysicalValue( nodeId1, v1 ) &&
									nodeValue->getPhysicalValue( nodeId1, v2 ) &&
									nodeValue->getPhysicalValue( nodeId2, v3 ) )
								{
									n0.setCoordinate(
										node0.x() + factor * d0[0],
										node0.y() + factor * d0[1],
										node0.z() + factor * d0[2]);
									n1.setCoordinate(
										node1.x() + factor * d1[0],
										node1.y() + factor * d1[1],
										node1.z() + factor * d1[2]);
									n2.setCoordinate(
										node2.x() + factor * d2[0],
										node2.y() + factor * d2[1],
										node2.z() + factor * d2[2]);
									n3.setCoordinate(
										node3.x() + factor * d3[0],
										node3.y() + factor * d3[1],
										node3.z() + factor * d3[2]);
									center.setCoordinate(
										0.25*(n0.x()+n1.x()+n2.x()+n3.x()),
										0.25*(n0.y()+n1.y()+n2.y()+n3.y()),
										0.25*(n0.z()+n1.z()+n2.z()+n3.z()));
									u0 = v0[comp];
									u1 = v1[comp];
									u2 = v2[comp];
									u3 = v3[comp];
									mean = 0.25*(u0 + u1 + u2 + u3);
									drawSolidContourTriangle(&n0,&n1,&center,u0,u1,mean,colorMap);
									drawSolidContourTriangle(&n1,&n2,&center,u1,u2,mean,colorMap);
									drawSolidContourTriangle(&n2,&n3,&center,u2,u3,mean,colorMap);
									drawSolidContourTriangle(&n3,&n0,&center,u3,u0,mean,colorMap);
								}
								break;
							default:
								break;
						}
					}
					++fIter;
				}
			}
		}else if( data->getValueType() == kmb::PhysicalValue::Tensor6 && 0 <= comp && comp < 6){
			double v0[] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0},
				v1[] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0},
				v2[] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0},
				v3[] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0},
				u0 = 0.0, u1 =0.0, u2= 0.0, u3 = 0.0, mean = 0.0;
			if( colorMap->getStep() == 0 ){
				while( !fIter.isFinished() ){
					if( !fIter.getFace(f) ){
						++fIter;
						continue;
					}
					kmb::ElementContainer::const_iterator element = mesh->findElement( f.getElementId(), bodyId );
					kmb::idType localId = f.getLocalFaceId();
					if( !element.isFinished() ){
						switch( element.getBoundaryVertexCount( localId ) ){
							case 3:
								nodeId0 = element.getBoundaryCellId(localId,0);
								nodeId1 = element.getBoundaryCellId(localId,1);
								nodeId2 = element.getBoundaryCellId(localId,2);
								if( mesh->getNode( nodeId0, node0 ) &&
									mesh->getNode( nodeId1, node1 ) &&
									mesh->getNode( nodeId2, node2 ) &&
									displace->getPhysicalValue( nodeId0, d0 ) &&
									displace->getPhysicalValue( nodeId1, d1 ) &&
									displace->getPhysicalValue( nodeId2, d2 ) &&
									nodeValue->getPhysicalValue( nodeId0, v0 ) &&
									nodeValue->getPhysicalValue( nodeId1, v1 ) &&
									nodeValue->getPhysicalValue( nodeId2, v2 ) )
								{
									drawContourTriangle(
										node0.x() + factor * d0[0],
										node0.y() + factor * d0[1],
										node0.z() + factor * d0[2],
										v0[comp],
										node1.x() + factor * d1[0],
										node1.y() + factor * d1[1],
										node1.z() + factor * d1[2],
										v1[comp],
										node2.x() + factor * d2[0],
										node2.y() + factor * d2[1],
										node2.z() + factor * d2[2],
										v2[comp],
										colorMap );
								}
								break;
							case 4:
								nodeId0 = element.getBoundaryCellId(localId,0);
								nodeId1 = element.getBoundaryCellId(localId,1);
								nodeId2 = element.getBoundaryCellId(localId,2);
								nodeId3 = element.getBoundaryCellId(localId,3);
								if( mesh->getNode( nodeId0, node0 ) &&
									mesh->getNode( nodeId1, node1 ) &&
									mesh->getNode( nodeId2, node2 ) &&
									mesh->getNode( nodeId3, node3 ) &&
									displace->getPhysicalValue( nodeId0, d0 ) &&
									displace->getPhysicalValue( nodeId1, d1 ) &&
									displace->getPhysicalValue( nodeId2, d2 ) &&
									displace->getPhysicalValue( nodeId3, d3 ) &&
									nodeValue->getPhysicalValue( nodeId0, v0 ) &&
									nodeValue->getPhysicalValue( nodeId1, v1 ) &&
									nodeValue->getPhysicalValue( nodeId2, v2 ) &&
									nodeValue->getPhysicalValue( nodeId3, v3 ) )
								{
									drawContourQuad(
										node0.x() + factor * d0[0],
										node0.y() + factor * d0[1],
										node0.z() + factor * d0[2],
										v0[comp],
										node1.x() + factor * d1[0],
										node1.y() + factor * d1[1],
										node1.z() + factor * d1[2],
										v1[comp],
										node2.x() + factor * d2[0],
										node2.y() + factor * d2[1],
										node2.z() + factor * d2[2],
										v2[comp],
										node3.x() + factor * d3[0],
										node3.y() + factor * d3[1],
										node3.z() + factor * d3[2],
										v3[comp],
										colorMap );
								}
								break;
							default:
								break;
						}
					}
					++fIter;
				}
			}else{ // solid contour
				while( !fIter.isFinished() ){
					if( !fIter.getFace(f) ){
						++fIter;
						continue;
					}
					kmb::ElementContainer::const_iterator element = mesh->findElement( f.getElementId(), bodyId );
					kmb::idType localId = f.getLocalFaceId();
					if( !element.isFinished() ){
						switch( element.getBoundaryVertexCount( localId ) ){
							case 3:
								nodeId0 = element.getBoundaryCellId(localId,0);
								nodeId1 = element.getBoundaryCellId(localId,1);
								nodeId2 = element.getBoundaryCellId(localId,2);
								if( mesh->getNode( nodeId0, node0 ) &&
									mesh->getNode( nodeId1, node1 ) &&
									mesh->getNode( nodeId2, node2 ) &&
									displace->getPhysicalValue( nodeId0, d0 ) &&
									displace->getPhysicalValue( nodeId1, d1 ) &&
									displace->getPhysicalValue( nodeId2, d2 ) &&
									nodeValue->getPhysicalValue( nodeId0, v0 ) &&
									nodeValue->getPhysicalValue( nodeId1, v1 ) &&
									nodeValue->getPhysicalValue( nodeId2, v2 ) )
								{
									n0.setCoordinate(
										node0.x() + factor * d0[0],
										node0.y() + factor * d0[1],
										node0.z() + factor * d0[2]);
									n1.setCoordinate(
										node1.x() + factor * d1[0],
										node1.y() + factor * d1[1],
										node1.z() + factor * d1[2]);
									n2.setCoordinate(
										node2.x() + factor * d2[0],
										node2.y() + factor * d2[1],
										node2.z() + factor * d2[2]);
									drawSolidContourTriangle(&n0,&n1,&n2,
										v0[comp],
										v1[comp],
										v2[comp],
										colorMap);
								}
								break;
							case 4:
								nodeId0 = element.getBoundaryCellId(localId,0);
								nodeId1 = element.getBoundaryCellId(localId,1);
								nodeId2 = element.getBoundaryCellId(localId,2);
								nodeId3 = element.getBoundaryCellId(localId,3);
								if( mesh->getNode( nodeId0, node0 ) &&
									mesh->getNode( nodeId1, node1 ) &&
									mesh->getNode( nodeId2, node2 ) &&
									mesh->getNode( nodeId3, node3 ) &&
									displace->getPhysicalValue( nodeId0, d0 ) &&
									displace->getPhysicalValue( nodeId1, d1 ) &&
									displace->getPhysicalValue( nodeId2, d2 ) &&
									displace->getPhysicalValue( nodeId3, d3 ) &&
									nodeValue->getPhysicalValue( nodeId0, v0 ) &&
									nodeValue->getPhysicalValue( nodeId1, v1 ) &&
									nodeValue->getPhysicalValue( nodeId1, v2 ) &&
									nodeValue->getPhysicalValue( nodeId2, v3 ) )
								{
									n0.setCoordinate(
										node0.x() + factor * d0[0],
										node0.y() + factor * d0[1],
										node0.z() + factor * d0[2]);
									n1.setCoordinate(
										node1.x() + factor * d1[0],
										node1.y() + factor * d1[1],
										node1.z() + factor * d1[2]);
									n2.setCoordinate(
										node2.x() + factor * d2[0],
										node2.y() + factor * d2[1],
										node2.z() + factor * d2[2]);
									n3.setCoordinate(
										node3.x() + factor * d3[0],
										node3.y() + factor * d3[1],
										node3.z() + factor * d3[2]);
									center.setCoordinate(
										0.25*(n0.x()+n1.x()+n2.x()+n3.x()),
										0.25*(n0.y()+n1.y()+n2.y()+n3.y()),
										0.25*(n0.z()+n1.z()+n2.z()+n3.z()));
									u0 = v0[comp];
									u1 = v1[comp];
									u2 = v2[comp];
									u3 = v3[comp];
									mean = 0.25*(u0 + u1 + u2 + u3);
									drawSolidContourTriangle(&n0,&n1,&center,u0,u1,mean,colorMap);
									drawSolidContourTriangle(&n1,&n2,&center,u1,u2,mean,colorMap);
									drawSolidContourTriangle(&n2,&n3,&center,u2,u3,mean,colorMap);
									drawSolidContourTriangle(&n3,&n0,&center,u3,u0,mean,colorMap);
								}
								break;
							default:
								break;
						}
					}
					++fIter;
				}
			}
		}
		::glPopAttrib();
		::glFlush();
	}
}

void
kmb::MeshGL::deformXYZFaceGroupNodeContour(
 const char* faceName,
 const char* displacementX, const char* displacementY, const char* displacementZ, double factor,
 const char* physicalValue, kmb::ColorMap* colorMap,int comp)
{
	const kmb::DataBindings* faceGroup = NULL;
	const kmb::DataBindings* displaceX = NULL;
	const kmb::DataBindings* displaceY = NULL;
	const kmb::DataBindings* displaceZ = NULL;
	const kmb::DataBindings* nodeValue = NULL;
	if( mesh != NULL && colorMap != NULL &&
		(faceGroup = mesh->getDataBindingsPtr( faceName )) != NULL &&
		faceGroup->getBindingMode() == kmb::DataBindings::FaceGroup &&
		(displaceX = mesh->getDataBindingsPtr( displacementX )) != NULL &&
		displaceX->getBindingMode() == kmb::DataBindings::NodeVariable &&
		displaceX->getValueType() == kmb::PhysicalValue::Scalar &&
		(displaceY = mesh->getDataBindingsPtr( displacementY )) != NULL &&
		displaceY->getBindingMode() == kmb::DataBindings::NodeVariable &&
		displaceY->getValueType() == kmb::PhysicalValue::Scalar &&
		(displaceZ = mesh->getDataBindingsPtr( displacementZ )) != NULL &&
		displaceZ->getBindingMode() == kmb::DataBindings::NodeVariable &&
		displaceZ->getValueType() == kmb::PhysicalValue::Scalar &&
		(nodeValue = mesh->getDataBindingsPtr( physicalValue )) != NULL &&
		nodeValue->getBindingMode() == kmb::DataBindings::NodeVariable )
	{
		::glPushAttrib( GL_ENABLE_BIT );
		::glEnable(GL_COLOR_MATERIAL);

		kmb::bodyIdType bodyId = faceGroup->getTargetBodyId();
		kmb::nodeIdType nodeId0 = kmb::nullNodeId, nodeId1 = kmb::nullNodeId, nodeId2 = kmb::nullNodeId, nodeId3 = kmb::nullNodeId;
		kmb::Node node0,node1,node2,node3,n0,n1,n2,n3,center;
		kmb::Face f;
		double d0[3] = {0.0, 0.0, 0.0},
			d1[3] = {0.0, 0.0, 0.0},
			d2[3] = {0.0, 0.0, 0.0},
			d3[3] = {0.0, 0.0, 0.0};
		kmb::DataBindings::const_iterator fIter = faceGroup->begin();
		if( nodeValue->getValueType() == kmb::PhysicalValue::Scalar ){
			double v0 = 0.0, v1 = 0.0, v2 = 0.0, v3 = 0.0, mean = 0.0;
			if( colorMap->getStep() == 0 ){
				while( !fIter.isFinished() ){
					if( !fIter.getFace(f) ){
						++fIter;
						continue;
					}
					kmb::ElementContainer::const_iterator element = mesh->findElement( f.getElementId(), bodyId );
					kmb::idType localId = f.getLocalFaceId();
					if( !element.isFinished() ){
						switch( element.getBoundaryVertexCount( localId ) ){
							case 3:
								nodeId0 = element.getBoundaryCellId(localId,0);
								nodeId1 = element.getBoundaryCellId(localId,1);
								nodeId2 = element.getBoundaryCellId(localId,2);
								if( mesh->getNode( nodeId0, node0 ) &&
									mesh->getNode( nodeId1, node1 ) &&
									mesh->getNode( nodeId2, node2 ) &&
									displaceX->getPhysicalValue( nodeId0, &d0[0] ) &&
									displaceY->getPhysicalValue( nodeId0, &d0[1] ) &&
									displaceZ->getPhysicalValue( nodeId0, &d0[2] ) &&
									displaceX->getPhysicalValue( nodeId1, &d1[0] ) &&
									displaceY->getPhysicalValue( nodeId1, &d1[1] ) &&
									displaceZ->getPhysicalValue( nodeId1, &d1[2] ) &&
									displaceX->getPhysicalValue( nodeId2, &d2[0] ) &&
									displaceY->getPhysicalValue( nodeId2, &d2[1] ) &&
									displaceZ->getPhysicalValue( nodeId2, &d2[2] ) &&
									nodeValue->getPhysicalValue( nodeId0, &v0 ) &&
									nodeValue->getPhysicalValue( nodeId1, &v1 ) &&
									nodeValue->getPhysicalValue( nodeId2, &v2 ) )
								{
									drawContourTriangle(
										node0.x() + factor * d0[0],
										node0.y() + factor * d0[1],
										node0.z() + factor * d0[2],
										v0,
										node1.x() + factor * d1[0],
										node1.y() + factor * d1[1],
										node1.z() + factor * d1[2],
										v1,
										node2.x() + factor * d2[0],
										node2.y() + factor * d2[1],
										node2.z() + factor * d2[2],
										v2,
										colorMap );
								}
								break;
							case 4:
								nodeId0 = element.getBoundaryCellId(localId,0);
								nodeId1 = element.getBoundaryCellId(localId,1);
								nodeId2 = element.getBoundaryCellId(localId,2);
								nodeId3 = element.getBoundaryCellId(localId,3);
								if( mesh->getNode( nodeId0, node0 ) &&
									mesh->getNode( nodeId1, node1 ) &&
									mesh->getNode( nodeId2, node2 ) &&
									mesh->getNode( nodeId3, node3 ) &&
									displaceX->getPhysicalValue( nodeId0, &d0[0] ) &&
									displaceY->getPhysicalValue( nodeId0, &d0[1] ) &&
									displaceZ->getPhysicalValue( nodeId0, &d0[2] ) &&
									displaceX->getPhysicalValue( nodeId1, &d1[0] ) &&
									displaceY->getPhysicalValue( nodeId1, &d1[1] ) &&
									displaceZ->getPhysicalValue( nodeId1, &d1[2] ) &&
									displaceX->getPhysicalValue( nodeId2, &d2[0] ) &&
									displaceY->getPhysicalValue( nodeId2, &d2[1] ) &&
									displaceZ->getPhysicalValue( nodeId2, &d2[2] ) &&
									displaceX->getPhysicalValue( nodeId3, &d3[0] ) &&
									displaceY->getPhysicalValue( nodeId3, &d3[1] ) &&
									displaceZ->getPhysicalValue( nodeId3, &d3[2] ) &&
									nodeValue->getPhysicalValue( nodeId0, &v0 ) &&
									nodeValue->getPhysicalValue( nodeId1, &v1 ) &&
									nodeValue->getPhysicalValue( nodeId2, &v2 ) &&
									nodeValue->getPhysicalValue( nodeId3, &v3 ) )
								{
									drawContourQuad(
										node0.x() + factor * d0[0],
										node0.y() + factor * d0[1],
										node0.z() + factor * d0[2],
										v0,
										node1.x() + factor * d1[0],
										node1.y() + factor * d1[1],
										node1.z() + factor * d1[2],
										v1,
										node2.x() + factor * d2[0],
										node2.y() + factor * d2[1],
										node2.z() + factor * d2[2],
										v2,
										node3.x() + factor * d3[0],
										node3.y() + factor * d3[1],
										node3.z() + factor * d3[2],
										v3,
										colorMap );
								}
								break;
							default:
								break;
						}
					}
					++fIter;
				}
			}else{ // solid contour
				while( !fIter.isFinished() ){
					if( !fIter.getFace(f) ){
						++fIter;
						continue;
					}
					kmb::ElementContainer::const_iterator element = mesh->findElement( f.getElementId(), bodyId );
					kmb::idType localId = f.getLocalFaceId();
					if( !element.isFinished() ){
						switch( element.getBoundaryVertexCount( localId ) ){
							case 3:
								nodeId0 = element.getBoundaryCellId(localId,0);
								nodeId1 = element.getBoundaryCellId(localId,1);
								nodeId2 = element.getBoundaryCellId(localId,2);
								if( mesh->getNode( nodeId0, node0 ) &&
									mesh->getNode( nodeId1, node1 ) &&
									mesh->getNode( nodeId2, node2 ) &&
									displaceX->getPhysicalValue( nodeId0, &d0[0] ) &&
									displaceY->getPhysicalValue( nodeId0, &d0[1] ) &&
									displaceZ->getPhysicalValue( nodeId0, &d0[2] ) &&
									displaceX->getPhysicalValue( nodeId1, &d1[0] ) &&
									displaceY->getPhysicalValue( nodeId1, &d1[1] ) &&
									displaceZ->getPhysicalValue( nodeId1, &d1[2] ) &&
									displaceX->getPhysicalValue( nodeId2, &d2[0] ) &&
									displaceY->getPhysicalValue( nodeId2, &d2[1] ) &&
									displaceZ->getPhysicalValue( nodeId2, &d2[2] ) &&
									nodeValue->getPhysicalValue( nodeId0, &v0 ) &&
									nodeValue->getPhysicalValue( nodeId1, &v1 ) &&
									nodeValue->getPhysicalValue( nodeId2, &v2 ) )
								{
									n0.setCoordinate(
										node0.x() + factor * d0[0],
										node0.y() + factor * d0[1],
										node0.z() + factor * d0[2]);
									n1.setCoordinate(
										node1.x() + factor * d1[0],
										node1.y() + factor * d1[1],
										node1.z() + factor * d1[2]);
									n2.setCoordinate(
										node2.x() + factor * d2[0],
										node2.y() + factor * d2[1],
										node2.z() + factor * d2[2]);
									drawSolidContourTriangle(&n0,&n1,&n2,v0,v1,v2,colorMap);
								}
								break;
							case 4:
								nodeId0 = element.getBoundaryCellId(localId,0);
								nodeId1 = element.getBoundaryCellId(localId,1);
								nodeId2 = element.getBoundaryCellId(localId,2);
								nodeId3 = element.getBoundaryCellId(localId,3);
								if( mesh->getNode( nodeId0, node0 ) &&
									mesh->getNode( nodeId1, node1 ) &&
									mesh->getNode( nodeId2, node2 ) &&
									mesh->getNode( nodeId3, node3 ) &&
									displaceX->getPhysicalValue( nodeId0, &d0[0] ) &&
									displaceY->getPhysicalValue( nodeId0, &d0[1] ) &&
									displaceZ->getPhysicalValue( nodeId0, &d0[2] ) &&
									displaceX->getPhysicalValue( nodeId1, &d1[0] ) &&
									displaceY->getPhysicalValue( nodeId1, &d1[1] ) &&
									displaceZ->getPhysicalValue( nodeId1, &d1[2] ) &&
									displaceX->getPhysicalValue( nodeId2, &d2[0] ) &&
									displaceY->getPhysicalValue( nodeId2, &d2[1] ) &&
									displaceZ->getPhysicalValue( nodeId2, &d2[2] ) &&
									displaceX->getPhysicalValue( nodeId3, &d3[0] ) &&
									displaceY->getPhysicalValue( nodeId3, &d3[1] ) &&
									displaceZ->getPhysicalValue( nodeId3, &d3[2] ) &&
									nodeValue->getPhysicalValue( nodeId0, &v0 ) &&
									nodeValue->getPhysicalValue( nodeId1, &v1 ) &&
									nodeValue->getPhysicalValue( nodeId1, &v2 ) &&
									nodeValue->getPhysicalValue( nodeId2, &v3 ) )
								{
									n0.setCoordinate(
										node0.x() + factor * d0[0],
										node0.y() + factor * d0[1],
										node0.z() + factor * d0[2]);
									n1.setCoordinate(
										node1.x() + factor * d1[0],
										node1.y() + factor * d1[1],
										node1.z() + factor * d1[2]);
									n2.setCoordinate(
										node2.x() + factor * d2[0],
										node2.y() + factor * d2[1],
										node2.z() + factor * d2[2]);
									n3.setCoordinate(
										node3.x() + factor * d3[0],
										node3.y() + factor * d3[1],
										node3.z() + factor * d3[2]);
									center.setCoordinate(
										0.25*(n0.x()+n1.x()+n2.x()+n3.x()),
										0.25*(n0.y()+n1.y()+n2.y()+n3.y()),
										0.25*(n0.z()+n1.z()+n2.z()+n3.z()));
									mean = 0.25*(v0 + v1 + v2 + v3);
									drawSolidContourTriangle(&n0,&n1,&center,v0,v1,mean,colorMap);
									drawSolidContourTriangle(&n1,&n2,&center,v1,v2,mean,colorMap);
									drawSolidContourTriangle(&n2,&n3,&center,v2,v3,mean,colorMap);
									drawSolidContourTriangle(&n3,&n0,&center,v3,v0,mean,colorMap);
								}
								break;
							default:
								break;
						}
					}
					++fIter;
				}
			}
		}else if( nodeValue->getValueType() == kmb::PhysicalValue::Vector3 && comp == -1){
			double v0[3] = {0.0, 0.0, 0.0},
				v1[3] = {0.0, 0.0, 0.0},
				v2[3] = {0.0, 0.0, 0.0},
				v3[3] = {0.0, 0.0, 0.0},
				u0 = 0.0, u1 =0.0, u2= 0.0, u3 = 0.0, mean = 0.0;
			if( colorMap->getStep() == 0 ){
				while( !fIter.isFinished() ){
					if( !fIter.getFace(f) ){
						++fIter;
						continue;
					}
					kmb::ElementContainer::const_iterator element = mesh->findElement( f.getElementId(), bodyId );
					kmb::idType localId = f.getLocalFaceId();
					if( !element.isFinished() ){
						switch( element.getBoundaryVertexCount( localId ) ){
							case 3:
								nodeId0 = element.getBoundaryCellId(localId,0);
								nodeId1 = element.getBoundaryCellId(localId,1);
								nodeId2 = element.getBoundaryCellId(localId,2);
								if( mesh->getNode( nodeId0, node0 ) &&
									mesh->getNode( nodeId1, node1 ) &&
									mesh->getNode( nodeId2, node2 ) &&
									displaceX->getPhysicalValue( nodeId0, &d0[0] ) &&
									displaceY->getPhysicalValue( nodeId0, &d0[1] ) &&
									displaceZ->getPhysicalValue( nodeId0, &d0[2] ) &&
									displaceX->getPhysicalValue( nodeId1, &d1[0] ) &&
									displaceY->getPhysicalValue( nodeId1, &d1[1] ) &&
									displaceZ->getPhysicalValue( nodeId1, &d1[2] ) &&
									displaceX->getPhysicalValue( nodeId2, &d2[0] ) &&
									displaceY->getPhysicalValue( nodeId2, &d2[1] ) &&
									displaceZ->getPhysicalValue( nodeId2, &d2[2] ) &&
									nodeValue->getPhysicalValue( nodeId0, v0 ) &&
									nodeValue->getPhysicalValue( nodeId1, v1 ) &&
									nodeValue->getPhysicalValue( nodeId2, v2 ) )
								{
									drawContourTriangle(
										node0.x() + factor * d0[0],
										node0.y() + factor * d0[1],
										node0.z() + factor * d0[2],
										kmb::Vector3D::abs(v0),
										node1.x() + factor * d1[0],
										node1.y() + factor * d1[1],
										node1.z() + factor * d1[2],
										kmb::Vector3D::abs(v1),
										node2.x() + factor * d2[0],
										node2.y() + factor * d2[1],
										node2.z() + factor * d2[2],
										kmb::Vector3D::abs(v2),
										colorMap );
								}
								break;
							case 4:
								nodeId0 = element.getBoundaryCellId(localId,0);
								nodeId1 = element.getBoundaryCellId(localId,1);
								nodeId2 = element.getBoundaryCellId(localId,2);
								nodeId3 = element.getBoundaryCellId(localId,3);
								if( mesh->getNode( nodeId0, node0 ) &&
									mesh->getNode( nodeId1, node1 ) &&
									mesh->getNode( nodeId2, node2 ) &&
									mesh->getNode( nodeId3, node3 ) &&
									displaceX->getPhysicalValue( nodeId0, &d0[0] ) &&
									displaceY->getPhysicalValue( nodeId0, &d0[1] ) &&
									displaceZ->getPhysicalValue( nodeId0, &d0[2] ) &&
									displaceX->getPhysicalValue( nodeId1, &d1[0] ) &&
									displaceY->getPhysicalValue( nodeId1, &d1[1] ) &&
									displaceZ->getPhysicalValue( nodeId1, &d1[2] ) &&
									displaceX->getPhysicalValue( nodeId2, &d2[0] ) &&
									displaceY->getPhysicalValue( nodeId2, &d2[1] ) &&
									displaceZ->getPhysicalValue( nodeId2, &d2[2] ) &&
									displaceX->getPhysicalValue( nodeId3, &d3[0] ) &&
									displaceY->getPhysicalValue( nodeId3, &d3[1] ) &&
									displaceZ->getPhysicalValue( nodeId3, &d3[2] ) &&
									nodeValue->getPhysicalValue( nodeId0, v0 ) &&
									nodeValue->getPhysicalValue( nodeId1, v1 ) &&
									nodeValue->getPhysicalValue( nodeId1, v2 ) &&
									nodeValue->getPhysicalValue( nodeId2, v3 ) )
								{
									drawContourQuad(
										node0.x() + factor * d0[0],
										node0.y() + factor * d0[1],
										node0.z() + factor * d0[2],
										kmb::Vector3D::abs(v0),
										node1.x() + factor * d1[0],
										node1.y() + factor * d1[1],
										node1.z() + factor * d1[2],
										kmb::Vector3D::abs(v1),
										node2.x() + factor * d2[0],
										node2.y() + factor * d2[1],
										node2.z() + factor * d2[2],
										kmb::Vector3D::abs(v2),
										node3.x() + factor * d3[0],
										node3.y() + factor * d3[1],
										node3.z() + factor * d3[2],
										kmb::Vector3D::abs(v3),
										colorMap );
								}
								break;
							default:
								break;
						}
					}
					++fIter;
				}
			}else{ // solid contour
				while( !fIter.isFinished() ){
					if( !fIter.getFace(f) ){
						++fIter;
						continue;
					}
					kmb::ElementContainer::const_iterator element = mesh->findElement( f.getElementId(), bodyId );
					kmb::idType localId = f.getLocalFaceId();
					if( !element.isFinished() ){
						switch( element.getBoundaryVertexCount( localId ) ){
							case 3:
								nodeId0 = element.getBoundaryCellId(localId,0);
								nodeId1 = element.getBoundaryCellId(localId,1);
								nodeId2 = element.getBoundaryCellId(localId,2);
								if( mesh->getNode( nodeId0, node0 ) &&
									mesh->getNode( nodeId1, node1 ) &&
									mesh->getNode( nodeId2, node2 ) &&
									displaceX->getPhysicalValue( nodeId0, &d0[0] ) &&
									displaceY->getPhysicalValue( nodeId0, &d0[1] ) &&
									displaceZ->getPhysicalValue( nodeId0, &d0[2] ) &&
									displaceX->getPhysicalValue( nodeId1, &d1[0] ) &&
									displaceY->getPhysicalValue( nodeId1, &d1[1] ) &&
									displaceZ->getPhysicalValue( nodeId1, &d1[2] ) &&
									displaceX->getPhysicalValue( nodeId2, &d2[0] ) &&
									displaceY->getPhysicalValue( nodeId2, &d2[1] ) &&
									displaceZ->getPhysicalValue( nodeId2, &d2[2] ) &&
									nodeValue->getPhysicalValue( nodeId0, v0 ) &&
									nodeValue->getPhysicalValue( nodeId1, v1 ) &&
									nodeValue->getPhysicalValue( nodeId2, v2 ) )
								{
									n0.setCoordinate(
										node0.x() + factor * d0[0],
										node0.y() + factor * d0[1],
										node0.z() + factor * d0[2]);
									n1.setCoordinate(
										node1.x() + factor * d1[0],
										node1.y() + factor * d1[1],
										node1.z() + factor * d1[2]);
									n2.setCoordinate(
										node2.x() + factor * d2[0],
										node2.y() + factor * d2[1],
										node2.z() + factor * d2[2]);
									drawSolidContourTriangle(&n0,&n1,&n2,
										kmb::Vector3D::abs(v0),
										kmb::Vector3D::abs(v1),
										kmb::Vector3D::abs(v2),
										colorMap);
								}
								break;
							case 4:
								nodeId0 = element.getBoundaryCellId(localId,0);
								nodeId1 = element.getBoundaryCellId(localId,1);
								nodeId2 = element.getBoundaryCellId(localId,2);
								nodeId3 = element.getBoundaryCellId(localId,3);
								if( mesh->getNode( nodeId0, node0 ) &&
									mesh->getNode( nodeId1, node1 ) &&
									mesh->getNode( nodeId2, node2 ) &&
									mesh->getNode( nodeId3, node3 ) &&
									displaceX->getPhysicalValue( nodeId0, &d0[0] ) &&
									displaceY->getPhysicalValue( nodeId0, &d0[1] ) &&
									displaceZ->getPhysicalValue( nodeId0, &d0[2] ) &&
									displaceX->getPhysicalValue( nodeId1, &d1[0] ) &&
									displaceY->getPhysicalValue( nodeId1, &d1[1] ) &&
									displaceZ->getPhysicalValue( nodeId1, &d1[2] ) &&
									displaceX->getPhysicalValue( nodeId2, &d2[0] ) &&
									displaceY->getPhysicalValue( nodeId2, &d2[1] ) &&
									displaceZ->getPhysicalValue( nodeId2, &d2[2] ) &&
									displaceX->getPhysicalValue( nodeId3, &d3[0] ) &&
									displaceY->getPhysicalValue( nodeId3, &d3[1] ) &&
									displaceZ->getPhysicalValue( nodeId3, &d3[2] ) &&
									nodeValue->getPhysicalValue( nodeId0, v0 ) &&
									nodeValue->getPhysicalValue( nodeId1, v1 ) &&
									nodeValue->getPhysicalValue( nodeId1, v2 ) &&
									nodeValue->getPhysicalValue( nodeId2, v3 ) )
								{
									n0.setCoordinate(
										node0.x() + factor * d0[0],
										node0.y() + factor * d0[1],
										node0.z() + factor * d0[2]);
									n1.setCoordinate(
										node1.x() + factor * d1[0],
										node1.y() + factor * d1[1],
										node1.z() + factor * d1[2]);
									n2.setCoordinate(
										node2.x() + factor * d2[0],
										node2.y() + factor * d2[1],
										node2.z() + factor * d2[2]);
									n3.setCoordinate(
										node3.x() + factor * d3[0],
										node3.y() + factor * d3[1],
										node3.z() + factor * d3[2]);
									center.setCoordinate(
										0.25*(n0.x()+n1.x()+n2.x()+n3.x()),
										0.25*(n0.y()+n1.y()+n2.y()+n3.y()),
										0.25*(n0.z()+n1.z()+n2.z()+n3.z()));
									u0 = kmb::Vector3D::abs( v0 );
									u1 = kmb::Vector3D::abs( v1 );
									u2 = kmb::Vector3D::abs( v2 );
									u3 = kmb::Vector3D::abs( v3 );
									mean = 0.25*(u0 + u1 + u2 + u3);
									drawSolidContourTriangle(&n0,&n1,&center,u0,u1,mean,colorMap);
									drawSolidContourTriangle(&n1,&n2,&center,u1,u2,mean,colorMap);
									drawSolidContourTriangle(&n2,&n3,&center,u2,u3,mean,colorMap);
									drawSolidContourTriangle(&n3,&n0,&center,u3,u0,mean,colorMap);
								}
								break;
							default:
								break;
						}
					}
					++fIter;
				}
			}
		}else if( nodeValue->getValueType() == kmb::PhysicalValue::Vector3 && 0 <= comp && comp < 3){
			double v0[3] = {0.0, 0.0, 0.0},
				v1[3] = {0.0, 0.0, 0.0},
				v2[3] = {0.0, 0.0, 0.0},
				v3[3] = {0.0, 0.0, 0.0},
				u0 = 0.0, u1 =0.0, u2= 0.0, u3 = 0.0, mean = 0.0;
			if( colorMap->getStep() == 0 ){
				while( !fIter.isFinished() ){
					if( !fIter.getFace(f) ){
						++fIter;
						continue;
					}
					kmb::ElementContainer::const_iterator element = mesh->findElement( f.getElementId(), bodyId );
					kmb::idType localId = f.getLocalFaceId();
					if( !element.isFinished() ){
						switch( element.getBoundaryVertexCount( localId ) ){
							case 3:
								nodeId0 = element.getBoundaryCellId(localId,0);
								nodeId1 = element.getBoundaryCellId(localId,1);
								nodeId2 = element.getBoundaryCellId(localId,2);
								if( mesh->getNode( nodeId0, node0 ) &&
									mesh->getNode( nodeId1, node1 ) &&
									mesh->getNode( nodeId2, node2 ) &&
									displaceX->getPhysicalValue( nodeId0, &d0[0] ) &&
									displaceY->getPhysicalValue( nodeId0, &d0[1] ) &&
									displaceZ->getPhysicalValue( nodeId0, &d0[2] ) &&
									displaceX->getPhysicalValue( nodeId1, &d1[0] ) &&
									displaceY->getPhysicalValue( nodeId1, &d1[1] ) &&
									displaceZ->getPhysicalValue( nodeId1, &d1[2] ) &&
									displaceX->getPhysicalValue( nodeId2, &d2[0] ) &&
									displaceY->getPhysicalValue( nodeId2, &d2[1] ) &&
									displaceZ->getPhysicalValue( nodeId2, &d2[2] ) &&
									nodeValue->getPhysicalValue( nodeId0, v0 ) &&
									nodeValue->getPhysicalValue( nodeId1, v1 ) &&
									nodeValue->getPhysicalValue( nodeId2, v2 ) )
								{
									drawContourTriangle(
										node0.x() + factor * d0[0],
										node0.y() + factor * d0[1],
										node0.z() + factor * d0[2],
										v0[comp],
										node1.x() + factor * d1[0],
										node1.y() + factor * d1[1],
										node1.z() + factor * d1[2],
										v1[comp],
										node2.x() + factor * d2[0],
										node2.y() + factor * d2[1],
										node2.z() + factor * d2[2],
										v2[comp],
										colorMap );
								}
								break;
							case 4:
								nodeId0 = element.getBoundaryCellId(localId,0);
								nodeId1 = element.getBoundaryCellId(localId,1);
								nodeId2 = element.getBoundaryCellId(localId,2);
								nodeId3 = element.getBoundaryCellId(localId,3);
								if( mesh->getNode( nodeId0, node0 ) &&
									mesh->getNode( nodeId1, node1 ) &&
									mesh->getNode( nodeId2, node2 ) &&
									mesh->getNode( nodeId3, node3 ) &&
									displaceX->getPhysicalValue( nodeId0, &d0[0] ) &&
									displaceY->getPhysicalValue( nodeId0, &d0[1] ) &&
									displaceZ->getPhysicalValue( nodeId0, &d0[2] ) &&
									displaceX->getPhysicalValue( nodeId1, &d1[0] ) &&
									displaceY->getPhysicalValue( nodeId1, &d1[1] ) &&
									displaceZ->getPhysicalValue( nodeId1, &d1[2] ) &&
									displaceX->getPhysicalValue( nodeId2, &d2[0] ) &&
									displaceY->getPhysicalValue( nodeId2, &d2[1] ) &&
									displaceZ->getPhysicalValue( nodeId2, &d2[2] ) &&
									displaceX->getPhysicalValue( nodeId3, &d3[0] ) &&
									displaceY->getPhysicalValue( nodeId3, &d3[1] ) &&
									displaceZ->getPhysicalValue( nodeId3, &d3[2] ) &&
									nodeValue->getPhysicalValue( nodeId0, v0 ) &&
									nodeValue->getPhysicalValue( nodeId1, v1 ) &&
									nodeValue->getPhysicalValue( nodeId1, v2 ) &&
									nodeValue->getPhysicalValue( nodeId2, v3 ) )
								{
									drawContourQuad(
										node0.x() + factor * d0[0],
										node0.y() + factor * d0[1],
										node0.z() + factor * d0[2],
										v0[comp],
										node1.x() + factor * d1[0],
										node1.y() + factor * d1[1],
										node1.z() + factor * d1[2],
										v1[comp],
										node2.x() + factor * d2[0],
										node2.y() + factor * d2[1],
										node2.z() + factor * d2[2],
										v2[comp],
										node3.x() + factor * d3[0],
										node3.y() + factor * d3[1],
										node3.z() + factor * d3[2],
										v3[comp],
										colorMap );
								}
								break;
							default:
								break;
						}
					}
					++fIter;
				}
			}else{ // solid contour
				while( !fIter.isFinished() ){
					if( !fIter.getFace(f) ){
						++fIter;
						continue;
					}
					kmb::ElementContainer::const_iterator element = mesh->findElement( f.getElementId(), bodyId );
					kmb::idType localId = f.getLocalFaceId();
					if( !element.isFinished() ){
						switch( element.getBoundaryVertexCount( localId ) ){
							case 3:
								nodeId0 = element.getBoundaryCellId(localId,0);
								nodeId1 = element.getBoundaryCellId(localId,1);
								nodeId2 = element.getBoundaryCellId(localId,2);
								if( mesh->getNode( nodeId0, node0 ) &&
									mesh->getNode( nodeId1, node1 ) &&
									mesh->getNode( nodeId2, node2 ) &&
									displaceX->getPhysicalValue( nodeId0, &d0[0] ) &&
									displaceY->getPhysicalValue( nodeId0, &d0[1] ) &&
									displaceZ->getPhysicalValue( nodeId0, &d0[2] ) &&
									displaceX->getPhysicalValue( nodeId1, &d1[0] ) &&
									displaceY->getPhysicalValue( nodeId1, &d1[1] ) &&
									displaceZ->getPhysicalValue( nodeId1, &d1[2] ) &&
									displaceX->getPhysicalValue( nodeId2, &d2[0] ) &&
									displaceY->getPhysicalValue( nodeId2, &d2[1] ) &&
									displaceZ->getPhysicalValue( nodeId2, &d2[2] ) &&
									nodeValue->getPhysicalValue( nodeId0, v0 ) &&
									nodeValue->getPhysicalValue( nodeId1, v1 ) &&
									nodeValue->getPhysicalValue( nodeId2, v2 ) )
								{
									n0.setCoordinate(
										node0.x() + factor * d0[0],
										node0.y() + factor * d0[1],
										node0.z() + factor * d0[2]);
									n1.setCoordinate(
										node1.x() + factor * d1[0],
										node1.y() + factor * d1[1],
										node1.z() + factor * d1[2]);
									n2.setCoordinate(
										node2.x() + factor * d2[0],
										node2.y() + factor * d2[1],
										node2.z() + factor * d2[2]);
									drawSolidContourTriangle(&n0,&n1,&n2,
										v0[comp],
										v1[comp],
										v2[comp],
										colorMap);
								}
								break;
							case 4:
								nodeId0 = element.getBoundaryCellId(localId,0);
								nodeId1 = element.getBoundaryCellId(localId,1);
								nodeId2 = element.getBoundaryCellId(localId,2);
								nodeId3 = element.getBoundaryCellId(localId,3);
								if( mesh->getNode( nodeId0, node0 ) &&
									mesh->getNode( nodeId1, node1 ) &&
									mesh->getNode( nodeId2, node2 ) &&
									mesh->getNode( nodeId3, node3 ) &&
									displaceX->getPhysicalValue( nodeId0, &d0[0] ) &&
									displaceY->getPhysicalValue( nodeId0, &d0[1] ) &&
									displaceZ->getPhysicalValue( nodeId0, &d0[2] ) &&
									displaceX->getPhysicalValue( nodeId1, &d1[0] ) &&
									displaceY->getPhysicalValue( nodeId1, &d1[1] ) &&
									displaceZ->getPhysicalValue( nodeId1, &d1[2] ) &&
									displaceX->getPhysicalValue( nodeId2, &d2[0] ) &&
									displaceY->getPhysicalValue( nodeId2, &d2[1] ) &&
									displaceZ->getPhysicalValue( nodeId2, &d2[2] ) &&
									displaceX->getPhysicalValue( nodeId3, &d3[0] ) &&
									displaceY->getPhysicalValue( nodeId3, &d3[1] ) &&
									displaceZ->getPhysicalValue( nodeId3, &d3[2] ) &&
									nodeValue->getPhysicalValue( nodeId0, v0 ) &&
									nodeValue->getPhysicalValue( nodeId1, v1 ) &&
									nodeValue->getPhysicalValue( nodeId1, v2 ) &&
									nodeValue->getPhysicalValue( nodeId2, v3 ) )
								{
									n0.setCoordinate(
										node0.x() + factor * d0[0],
										node0.y() + factor * d0[1],
										node0.z() + factor * d0[2]);
									n1.setCoordinate(
										node1.x() + factor * d1[0],
										node1.y() + factor * d1[1],
										node1.z() + factor * d1[2]);
									n2.setCoordinate(
										node2.x() + factor * d2[0],
										node2.y() + factor * d2[1],
										node2.z() + factor * d2[2]);
									n3.setCoordinate(
										node3.x() + factor * d3[0],
										node3.y() + factor * d3[1],
										node3.z() + factor * d3[2]);
									center.setCoordinate(
										0.25*(n0.x()+n1.x()+n2.x()+n3.x()),
										0.25*(n0.y()+n1.y()+n2.y()+n3.y()),
										0.25*(n0.z()+n1.z()+n2.z()+n3.z()));
									u0 = v0[comp];
									u1 = v1[comp];
									u2 = v2[comp];
									u3 = v3[comp];
									mean = 0.25*(u0 + u1 + u2 + u3);
									drawSolidContourTriangle(&n0,&n1,&center,u0,u1,mean,colorMap);
									drawSolidContourTriangle(&n1,&n2,&center,u1,u2,mean,colorMap);
									drawSolidContourTriangle(&n2,&n3,&center,u2,u3,mean,colorMap);
									drawSolidContourTriangle(&n3,&n0,&center,u3,u0,mean,colorMap);
								}
								break;
							default:
								break;
						}
					}
					++fIter;
				}
			}
		}else if( nodeValue->getValueType() == kmb::PhysicalValue::Tensor6 && 0 <= comp && comp < 6){
			double v0[] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0},
				v1[] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0},
				v2[] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0},
				v3[] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0},
				u0 = 0.0, u1 =0.0, u2= 0.0, u3 = 0.0, mean = 0.0;
			if( colorMap->getStep() == 0 ){
				while( !fIter.isFinished() ){
					if( !fIter.getFace(f) ){
						++fIter;
						continue;
					}
					kmb::ElementContainer::const_iterator element = mesh->findElement( f.getElementId(), bodyId );
					kmb::idType localId = f.getLocalFaceId();
					if( !element.isFinished() ){
						switch( element.getBoundaryVertexCount( localId ) ){
							case 3:
								nodeId0 = element.getBoundaryCellId(localId,0);
								nodeId1 = element.getBoundaryCellId(localId,1);
								nodeId2 = element.getBoundaryCellId(localId,2);
								if( mesh->getNode( nodeId0, node0 ) &&
									mesh->getNode( nodeId1, node1 ) &&
									mesh->getNode( nodeId2, node2 ) &&
									displaceX->getPhysicalValue( nodeId0, &d0[0] ) &&
									displaceY->getPhysicalValue( nodeId0, &d0[1] ) &&
									displaceZ->getPhysicalValue( nodeId0, &d0[2] ) &&
									displaceX->getPhysicalValue( nodeId1, &d1[0] ) &&
									displaceY->getPhysicalValue( nodeId1, &d1[1] ) &&
									displaceZ->getPhysicalValue( nodeId1, &d1[2] ) &&
									displaceX->getPhysicalValue( nodeId2, &d2[0] ) &&
									displaceY->getPhysicalValue( nodeId2, &d2[1] ) &&
									displaceZ->getPhysicalValue( nodeId2, &d2[2] ) &&
									nodeValue->getPhysicalValue( nodeId0, v0 ) &&
									nodeValue->getPhysicalValue( nodeId1, v1 ) &&
									nodeValue->getPhysicalValue( nodeId2, v2 ) )
								{
									drawContourTriangle(
										node0.x() + factor * d0[0],
										node0.y() + factor * d0[1],
										node0.z() + factor * d0[2],
										v0[comp],
										node1.x() + factor * d1[0],
										node1.y() + factor * d1[1],
										node1.z() + factor * d1[2],
										v1[comp],
										node2.x() + factor * d2[0],
										node2.y() + factor * d2[1],
										node2.z() + factor * d2[2],
										v2[comp],
										colorMap );
								}
								break;
							case 4:
								nodeId0 = element.getBoundaryCellId(localId,0);
								nodeId1 = element.getBoundaryCellId(localId,1);
								nodeId2 = element.getBoundaryCellId(localId,2);
								nodeId3 = element.getBoundaryCellId(localId,3);
								if( mesh->getNode( nodeId0, node0 ) &&
									mesh->getNode( nodeId1, node1 ) &&
									mesh->getNode( nodeId2, node2 ) &&
									mesh->getNode( nodeId3, node3 ) &&
									displaceX->getPhysicalValue( nodeId0, &d0[0] ) &&
									displaceY->getPhysicalValue( nodeId0, &d0[1] ) &&
									displaceZ->getPhysicalValue( nodeId0, &d0[2] ) &&
									displaceX->getPhysicalValue( nodeId1, &d1[0] ) &&
									displaceY->getPhysicalValue( nodeId1, &d1[1] ) &&
									displaceZ->getPhysicalValue( nodeId1, &d1[2] ) &&
									displaceX->getPhysicalValue( nodeId2, &d2[0] ) &&
									displaceY->getPhysicalValue( nodeId2, &d2[1] ) &&
									displaceZ->getPhysicalValue( nodeId2, &d2[2] ) &&
									displaceX->getPhysicalValue( nodeId3, &d3[0] ) &&
									displaceY->getPhysicalValue( nodeId3, &d3[1] ) &&
									displaceZ->getPhysicalValue( nodeId3, &d3[2] ) &&
									nodeValue->getPhysicalValue( nodeId0, v0 ) &&
									nodeValue->getPhysicalValue( nodeId1, v1 ) &&
									nodeValue->getPhysicalValue( nodeId1, v2 ) &&
									nodeValue->getPhysicalValue( nodeId2, v3 ) )
								{
									drawContourQuad(
										node0.x() + factor * d0[0],
										node0.y() + factor * d0[1],
										node0.z() + factor * d0[2],
										v0[comp],
										node1.x() + factor * d1[0],
										node1.y() + factor * d1[1],
										node1.z() + factor * d1[2],
										v1[comp],
										node2.x() + factor * d2[0],
										node2.y() + factor * d2[1],
										node2.z() + factor * d2[2],
										v2[comp],
										node3.x() + factor * d3[0],
										node3.y() + factor * d3[1],
										node3.z() + factor * d3[2],
										v3[comp],
										colorMap );
								}
								break;
							default:
								break;
						}
					}
					++fIter;
				}
			}else{ // solid contour
				while( !fIter.isFinished() ){
					if( !fIter.getFace(f) ){
						++fIter;
						continue;
					}
					kmb::ElementContainer::const_iterator element = mesh->findElement( f.getElementId(), bodyId );
					kmb::idType localId = f.getLocalFaceId();
					if( !element.isFinished() ){
						switch( element.getBoundaryVertexCount( localId ) ){
							case 3:
								nodeId0 = element.getBoundaryCellId(localId,0);
								nodeId1 = element.getBoundaryCellId(localId,1);
								nodeId2 = element.getBoundaryCellId(localId,2);
								if( mesh->getNode( nodeId0, node0 ) &&
									mesh->getNode( nodeId1, node1 ) &&
									mesh->getNode( nodeId2, node2 ) &&
									displaceX->getPhysicalValue( nodeId0, &d0[0] ) &&
									displaceY->getPhysicalValue( nodeId0, &d0[1] ) &&
									displaceZ->getPhysicalValue( nodeId0, &d0[2] ) &&
									displaceX->getPhysicalValue( nodeId1, &d1[0] ) &&
									displaceY->getPhysicalValue( nodeId1, &d1[1] ) &&
									displaceZ->getPhysicalValue( nodeId1, &d1[2] ) &&
									displaceX->getPhysicalValue( nodeId2, &d2[0] ) &&
									displaceY->getPhysicalValue( nodeId2, &d2[1] ) &&
									displaceZ->getPhysicalValue( nodeId2, &d2[2] ) &&
									nodeValue->getPhysicalValue( nodeId0, v0 ) &&
									nodeValue->getPhysicalValue( nodeId1, v1 ) &&
									nodeValue->getPhysicalValue( nodeId2, v2 ) )
								{
									n0.setCoordinate(
										node0.x() + factor * d0[0],
										node0.y() + factor * d0[1],
										node0.z() + factor * d0[2]);
									n1.setCoordinate(
										node1.x() + factor * d1[0],
										node1.y() + factor * d1[1],
										node1.z() + factor * d1[2]);
									n2.setCoordinate(
										node2.x() + factor * d2[0],
										node2.y() + factor * d2[1],
										node2.z() + factor * d2[2]);
									drawSolidContourTriangle(&n0,&n1,&n2,
										v0[comp],
										v1[comp],
										v2[comp],
										colorMap);
								}
								break;
							case 4:
								nodeId0 = element.getBoundaryCellId(localId,0);
								nodeId1 = element.getBoundaryCellId(localId,1);
								nodeId2 = element.getBoundaryCellId(localId,2);
								nodeId3 = element.getBoundaryCellId(localId,3);
								if( mesh->getNode( nodeId0, node0 ) &&
									mesh->getNode( nodeId1, node1 ) &&
									mesh->getNode( nodeId2, node2 ) &&
									mesh->getNode( nodeId3, node3 ) &&
									displaceX->getPhysicalValue( nodeId0, &d0[0] ) &&
									displaceY->getPhysicalValue( nodeId0, &d0[1] ) &&
									displaceZ->getPhysicalValue( nodeId0, &d0[2] ) &&
									displaceX->getPhysicalValue( nodeId1, &d1[0] ) &&
									displaceY->getPhysicalValue( nodeId1, &d1[1] ) &&
									displaceZ->getPhysicalValue( nodeId1, &d1[2] ) &&
									displaceX->getPhysicalValue( nodeId2, &d2[0] ) &&
									displaceY->getPhysicalValue( nodeId2, &d2[1] ) &&
									displaceZ->getPhysicalValue( nodeId2, &d2[2] ) &&
									displaceX->getPhysicalValue( nodeId3, &d3[0] ) &&
									displaceY->getPhysicalValue( nodeId3, &d3[1] ) &&
									displaceZ->getPhysicalValue( nodeId3, &d3[2] ) &&
									nodeValue->getPhysicalValue( nodeId0, v0 ) &&
									nodeValue->getPhysicalValue( nodeId1, v1 ) &&
									nodeValue->getPhysicalValue( nodeId1, v2 ) &&
									nodeValue->getPhysicalValue( nodeId2, v3 ) )
								{
									n0.setCoordinate(
										node0.x() + factor * d0[0],
										node0.y() + factor * d0[1],
										node0.z() + factor * d0[2]);
									n1.setCoordinate(
										node1.x() + factor * d1[0],
										node1.y() + factor * d1[1],
										node1.z() + factor * d1[2]);
									n2.setCoordinate(
										node2.x() + factor * d2[0],
										node2.y() + factor * d2[1],
										node2.z() + factor * d2[2]);
									n3.setCoordinate(
										node3.x() + factor * d3[0],
										node3.y() + factor * d3[1],
										node3.z() + factor * d3[2]);
									center.setCoordinate(
										0.25*(n0.x()+n1.x()+n2.x()+n3.x()),
										0.25*(n0.y()+n1.y()+n2.y()+n3.y()),
										0.25*(n0.z()+n1.z()+n2.z()+n3.z()));
									u0 = v0[comp];
									u1 = v1[comp];
									u2 = v2[comp];
									u3 = v3[comp];
									mean = 0.25*(u0 + u1 + u2 + u3);
									drawSolidContourTriangle(&n0,&n1,&center,u0,u1,mean,colorMap);
									drawSolidContourTriangle(&n1,&n2,&center,u1,u2,mean,colorMap);
									drawSolidContourTriangle(&n2,&n3,&center,u2,u3,mean,colorMap);
									drawSolidContourTriangle(&n3,&n0,&center,u3,u0,mean,colorMap);
								}
								break;
							default:
								break;
						}
					}
					++fIter;
				}
			}
		}
		::glPopAttrib();
		::glFlush();
	}
}

// n0 と n1 を t : 1-t に内分した点を与える
void glVertex3dAtMiddle(kmb::Node* n0,kmb::Node* n1,double t){
	::glVertex3d(
		(1.0-t)*n0->x() + t*n1->x(),
		(1.0-t)*n0->y() + t*n1->y(),
		(1.0-t)*n0->z() + t*n1->z());
}

// x0,y0 と x1,y1 を t : 1-t に内分した点を与える
void glVertex2dAtMiddle(double x0,double y0,double x1,double y1,double t){
	::glVertex2d(
		(1.0-t)*x0 + t*x1,
		(1.0-t)*y0 + t*y1);
}

// n0 と n1 と n2 について
// 0 < t < 1 なら n0 と n1 を t0 : 1-t0 に内分した点を与える
// t0 = 1     なら n1 を与える
// 1 < t0 < 2 なら n1 と n2 を (t0-1) : 1-(t0-1) に内分した点を与える
// t0 と t1 の間に 1 があれば n1 を自動的に書く
void glVertex3dAtMiddle(kmb::Node* n0,kmb::Node* n1,kmb::Node* n2,double t0,double t1){
	if( 0.0 < t0 && t0 < 1.0 ){
		::glVertex3d(
			(1.0-t0)*n0->x() + t0*n1->x(),
			(1.0-t0)*n0->y() + t0*n1->y(),
			(1.0-t0)*n0->z() + t0*n1->z());
	}else if( t0 == 1.0 ){
		::glVertex3d( n1->x(), n1->y(), n1->z() );
	}else if( 1.0 < t0 && t0 < 2.0 ){
		::glVertex3d(
			(2.0-t0)*n1->x() + (t0-1.0)*n2->x(),
			(2.0-t0)*n1->y() + (t0-1.0)*n2->y(),
			(2.0-t0)*n1->z() + (t0-1.0)*n2->z());
	}
	if( t1 <= 0.0 ){
		if( 1.0 < t0 ){
			::glVertex3d( n1->x(), n1->y(), n1->z() );
		}
	}else if( t1 < 1.0 ){
		if( 1.0 < t0 ){
			::glVertex3d( n1->x(), n1->y(), n1->z() );
		}
		::glVertex3d(
			(1.0-t1)*n0->x() + t1*n1->x(),
			(1.0-t1)*n0->y() + t1*n1->y(),
			(1.0-t1)*n0->z() + t1*n1->z());
	}else if( t1 == 1.0 ){
		if( t0 != 1.0 ){
			::glVertex3d( n1->x(), n1->y(), n1->z() );
		}
	}else if( t1 < 2.0 ){
		if( t0 < 1.0 ){
			::glVertex3d( n1->x(), n1->y(), n1->z() );
		}
		::glVertex3d(
			(2.0-t1)*n1->x() + (t1-1.0)*n2->x(),
			(2.0-t1)*n1->y() + (t1-1.0)*n2->y(),
			(2.0-t1)*n1->z() + (t1-1.0)*n2->z());
	}else{
		if( t0 < 1.0 ){
			::glVertex3d( n1->x(), n1->y(), n1->z() );
		}
	}
}

// n0 と n1 と n2 について
// 0 < t < 1 なら n0 と n1 を t0 : 1-t0 に内分した点を与える
// t0 = 1     なら n1 を与える
// 1 < t0 < 2 なら n1 と n2 を (t0-1) : 1-(t0-1) に内分した点を与える
// t0 と t1 の間に 1 があれば n1 を自動的に書く
void glVertex2dAtMiddle(double x0,double y0,double x1,double y1,double x2,double y2,double t0,double t1){
	if( 0.0 < t0 && t0 < 1.0 ){
		::glVertex2d(
			(1.0-t0)*x0 + t0*x1,
			(1.0-t0)*y0 + t0*y1);
	}else if( t0 == 1.0 ){
		::glVertex2d( x1, y1 );
	}else if( 1.0 < t0 && t0 < 2.0 ){
		::glVertex2d(
			(2.0-t0)*x1 + (t0-1.0)*x2,
			(2.0-t0)*y1 + (t0-1.0)*y2);
	}
	if( t1 <= 0.0 ){
		if( 1.0 < t0 ){
			::glVertex2d( x1, y1 );
		}
	}else if( t1 < 1.0 ){
		if( 1.0 < t0 ){
			::glVertex2d( x1, y1 );
		}
		::glVertex2d(
			(1.0-t1)*x0 + t1*x1,
			(1.0-t1)*y0 + t1*y1);
	}else if( t1 == 1.0 ){
		if( t0 != 1.0 ){
			::glVertex2d( x1, y1 );
		}
	}else if( t1 < 2.0 ){
		if( t0 < 1.0 ){
			::glVertex2d( x1, y1 );
		}
		::glVertex2d(
			(2.0-t1)*x1 + (t1-1.0)*x2,
			(2.0-t1)*y1 + (t1-1.0)*y2);
	}else{
		if( t0 < 1.0 ){
			::glVertex2d( x1, y1 );
		}
	}
}

void
kmb::MeshGL::drawContourTriangle
(double x0,double y0,double z0,double v0,
 double x1,double y1,double z1,double v1,
 double x2,double y2,double z2,double v2,
 kmb::ColorMap* colorMap)
{
	kmb::Vector3D normal = kmb::Point3D::calcNormalVector(x0,y0,z0,x1,y1,z1,x2,y2,z2);
	::glBegin( GL_TRIANGLES );
	::glNormal3d( normal.x(), normal.y(), normal.z() );
	colorMap->setGLColor( v0 );
	::glVertex3d( x0, y0, z0 );
	colorMap->setGLColor( v1 );
	::glVertex3d( x1, y1, z1 );
	colorMap->setGLColor( v2 );
	::glVertex3d( x2, y2, z2 );
	::glEnd();
}

void
kmb::MeshGL::drawContourTriangle2D
(double x0,double y0,double v0,
 double x1,double y1,double v1,
 double x2,double y2,double v2,
 kmb::ColorMap* colorMap)
{
	::glBegin( GL_TRIANGLES );
	colorMap->setGLColor( v0 );
	::glVertex2d( x0, y0 );
	colorMap->setGLColor( v1 );
	::glVertex2d( x1, y1 );
	colorMap->setGLColor( v2 );
	::glVertex2d( x2, y2 );
	::glEnd();
}

void
kmb::MeshGL::drawContourQuad
(double x0,double y0,double z0,double v0,
 double x1,double y1,double z1,double v1,
 double x2,double y2,double z2,double v2,
 double x3,double y3,double z3,double v3,
 kmb::ColorMap* colorMap)
{
	kmb::Vector3D normal = kmb::Point3D::calcNormalVector(x0,y0,z0,x1,y1,z1,x2,y2,z2,x3,y3,z3);
	::glBegin( GL_QUADS );
	::glNormal3d( normal.x(), normal.y(), normal.z() );
	colorMap->setGLColor( v0 );
	::glVertex3d( x0, y0, z0 );
	colorMap->setGLColor( v1 );
	::glVertex3d( x1, y1, z1 );
	colorMap->setGLColor( v2 );
	::glVertex3d( x2, y2, z2 );
	colorMap->setGLColor( v3 );
	::glVertex3d( x3, y3, z3 );
	::glEnd();
}

void
kmb::MeshGL::drawContourQuad2D
(double x0,double y0,double v0,
 double x1,double y1,double v1,
 double x2,double y2,double v2,
 double x3,double y3,double v3,
 kmb::ColorMap* colorMap)
{
	::glBegin( GL_QUADS );
	colorMap->setGLColor( v0 );
	::glVertex2d( x0, y0 );
	colorMap->setGLColor( v1 );
	::glVertex2d( x1, y1 );
	colorMap->setGLColor( v2 );
	::glVertex2d( x2, y2 );
	colorMap->setGLColor( v3 );
	::glVertex2d( x3, y3 );
	::glEnd();
}

void
kmb::MeshGL::drawSolidContourTriangle
(kmb::Node* node0,kmb::Node* node1,kmb::Node* node2,
 double v0,double v1,double v2,kmb::ColorMap* colorMap)
{
	// node0 node1 node2 colorMap の NULL チェックはしないよ
	// 並び替え後 u0 < u1 < u2
	kmb::Node *n0, *n1, *n2;
	double u0,u1,u2;
	// 右回りか左回りか
	bool clockwise = true;   // 元の向きは反時計 false => maxIndex 2

	kmb::Vector3D normal = kmb::Point3D::calcNormalVector( *node0, *node1, *node2 );
	if( v1 <= v0 ){
		if( v2 <= v1 ){
			// v2 < v1 < v0
			n0 = node2; n1 = node1; n2 = node0;
			u0 = v2;    u1 = v1;    u2 = v0;
			clockwise = true;
		}else if( v0 <= v2 ){
			// v1 < v0 < v2
			n0 = node1; n1 = node0; n2 = node2;
			u0 = v1;    u1 = v0;    u2 = v2;
			clockwise = true;
		}else{
			// v1 < v2 < v0
			n0 = node1; n1 = node2; n2 = node0;
			u0 = v1;    u1 = v2;    u2 = v0;
			clockwise = false;
		}
	}else{
		if( v1 <= v2 ){
			// v0 < v1 < v2
			n0 = node0; n1 = node1; n2 = node2;
			u0 = v0;    u1 = v1;    u2 = v2;
			clockwise = false;
		}else if( v2 <= v0 ){
			// v2 < v0 < v1
			n0 = node2; n1 = node0; n2 = node1;
			u0 = v2;    u1 = v0;    u2 = v1;
			clockwise = false;
		}else{
			// v0 < v2 < v1
			n0 = node0; n1 = node2; n2 = node1;
			u0 = v0;    u1 = v2;    u2 = v1;
			clockwise = true;
		}
	}

	double* part01 = NULL;
	double* part12 = NULL;
	double* part02 = NULL;
	int count01 = 0;
	int count12 = 0;
	int count02 = 0;
	int step01 = colorMap->getSolidDivision(u0,u1,count01,part01);
	int step12 = colorMap->getSolidDivision(u1,u2,count12,part12);
	// step02 は使わないので
	colorMap->getSolidDivision(u0,u2,count02,part02);
	if( count02 == 0 ){
		// すべて同じ色
		colorMap->setGLColorByStep( step01 );
		::glBegin( GL_TRIANGLES );
		::glNormal3d( normal.x(), normal.y(), normal.z() );
		if( clockwise ){
			::glVertex3d( n0->x(), n0->y(), n0->z() );
			::glVertex3d( n2->x(), n2->y(), n2->z() );
			::glVertex3d( n1->x(), n1->y(), n1->z() );
		}else{
			::glVertex3d( n0->x(), n0->y(), n0->z() );
			::glVertex3d( n1->x(), n1->y(), n1->z() );
			::glVertex3d( n2->x(), n2->y(), n2->z() );
		}
		::glEnd();
		::glFlush();
	}else{
		// node0 -> node1 -> node2 の partions を作成
		double* part012 = new double[count02];
		int ind = 0;
		for(int i=0;i<count01;++i){
			part012[ind] = part01[i];
			++ind;
		}
		// node1 がちょうど色の切れ目
		if( step01 + count01 != step12 ){
			part012[ind] = 1.0;
			++ind;
		}
		for(int i=0;i<count12;++i){
			part012[ind] = part12[i] + 1.0;
			++ind;
		}
		// 最初
		colorMap->setGLColorByStep( step01 );
		::glBegin( GL_TRIANGLE_FAN );
		::glNormal3d( normal.x(), normal.y(), normal.z() );
		if( clockwise ){
			::glVertex3d( n0->x(), n0->y(), n0->z() );
			::glVertex3dAtMiddle( n0, n2, part02[0] );
			::glVertex3dAtMiddle( n0, n1, n2, part012[0], -1.0 ); // 必要なら自動的に node1 を追加する
		}else{
			::glVertex3d( n0->x(), n0->y(), n0->z() );
			::glVertex3dAtMiddle( n0, n1, n2, -1.0, part012[0] ); // 必要なら自動的に node1 を追加する
			::glVertex3dAtMiddle( n0, n2, part02[0] );
		}
		::glEnd();
		::glFlush();
		// 中間
		int i=1;
		while(i<count02){
			colorMap->setGLColorByStep( step01+i );
			::glBegin( GL_TRIANGLE_FAN );
			::glNormal3d( normal.x(), normal.y(), normal.z() );
			if( clockwise ){
				::glVertex3dAtMiddle( n0, n1, n2, part012[i], part012[i-1] );
				::glVertex3dAtMiddle( n0, n2, part02[i-1] );
				::glVertex3dAtMiddle( n0, n2, part02[i] );
			}else{
				::glVertex3dAtMiddle( n0, n1, n2, part012[i-1], part012[i] );
				::glVertex3dAtMiddle( n0, n2, part02[i] );
				::glVertex3dAtMiddle( n0, n2, part02[i-1] );
			}
			::glEnd();
			::glFlush();
			++i;
		}
		// 最後
		colorMap->setGLColorByStep( step01+count02 );
		::glBegin( GL_TRIANGLE_FAN );
		::glNormal3d( normal.x(), normal.y(), normal.z() );
		if( clockwise ){
			::glVertex3dAtMiddle( n0, n2, part02[count02-1] );
			::glVertex3d( n2->x(), n2->y(), n2->z() );
			::glVertex3dAtMiddle( n0, n1, n2, 2.0, part012[count02-1] );
		}else{
			::glVertex3dAtMiddle( n0, n1, n2, part012[count02-1], 2.0 );
			::glVertex3d( n2->x(), n2->y(), n2->z() );
			::glVertex3dAtMiddle( n0, n2, part02[count02-1] );
		}
		::glEnd();
		::glFlush();
		delete[] part012;
	}
	if( part01 != NULL ){
		delete[] part01;
	}
	if( part12 != NULL ){
		delete[] part12;
	}
	if( part02 != NULL ){
		delete[] part02;
	}
}

void
kmb::MeshGL::drawSolidContourTriangle2D
(double x0,double y0,double v0,
 double x1,double y1,double v1,
 double x2,double y2,double v2,
 kmb::ColorMap* colorMap)
{
	// 並び替え後 u0 < u1 < u2
	double u0,u1,u2;
	double xx0,yy0,xx1,yy1,xx2,yy2;
	// 右回りか左回りか
	bool clockwise = true;   // 元の向きは反時計 false => maxIndex 2

	if( v1 <= v0 ){
		if( v2 <= v1 ){
			// v2 < v1 < v0
			u0 = v2;	xx0 = x2;	yy0 = y2;
			u1 = v1;	xx1 = x1;	yy1 = y1;
			u2 = v0;	xx2 = x0;	yy2 = y0;
			clockwise = true;
		}else if( v0 <= v2 ){
			// v1 < v0 < v2
			u0 = v1;	xx0 = x1;	yy0 = y1;
			u1 = v0;	xx1 = x0;	yy1 = y0;
			u2 = v2;	xx2 = x2;	yy2 = y2;
			clockwise = true;
		}else{
			// v1 < v2 < v0
			u0 = v1;	xx0 = x1;	yy0 = y1;
			u1 = v2;	xx1 = x2;	yy1 = y2;
			u2 = v0;	xx2 = x0;	yy2 = y0;
			clockwise = false;
		}
	}else{
		if( v1 <= v2 ){
			// v0 < v1 < v2
			u0 = v0;	xx0 = x0;	yy0 = y0;
			u1 = v1;	xx1 = x1;	yy1 = y1;
			u2 = v2;	xx2 = x2;	yy2 = y2;
			clockwise = false;
		}else if( v2 <= v0 ){
			// v2 < v0 < v1
			u0 = v2;	xx0 = x2;	yy0 = y2;
			u1 = v0;	xx1 = x0;	yy1 = y0;
			u2 = v1;	xx2 = x1;	yy2 = y1;
			clockwise = false;
		}else{
			// v0 < v2 < v1
			u0 = v0;	xx0 = x0;	yy0 = y0;
			u1 = v2;	xx1 = x2;	yy1 = y2;
			u2 = v1;	xx2 = x1;	yy2 = y1;
			clockwise = true;
		}
	}

	double* part01 = NULL;
	double* part12 = NULL;
	double* part02 = NULL;
	int count01 = 0;
	int count12 = 0;
	int count02 = 0;
	int step01 = colorMap->getSolidDivision(u0,u1,count01,part01);
	int step12 = colorMap->getSolidDivision(u1,u2,count12,part12);
	// step02 は使わないので
	colorMap->getSolidDivision(u0,u2,count02,part02);
	if( count02 == 0 ){
		// すべて同じ色
		colorMap->setGLColorByStep( step01 );
		::glBegin( GL_TRIANGLES );
		if( clockwise ){
			::glVertex2d( xx0, yy0 );
			::glVertex2d( xx2, yy2 );
			::glVertex2d( xx1, yy1 );
		}else{
			::glVertex2d( xx0, yy0 );
			::glVertex2d( xx1, yy1 );
			::glVertex2d( xx2, yy2 );
		}
		::glEnd();
	}else{
		// node0 -> node1 -> node2 の partions を作成
		double* part012 = new double[count02];
		int ind = 0;
		for(int i=0;i<count01;++i){
			part012[ind] = part01[i];
			++ind;
		}
		// node1 がちょうど色の切れ目
		if( step01 + count01 != step12 ){
			part012[ind] = 1.0;
			++ind;
		}
		for(int i=0;i<count12;++i){
			part012[ind] = part12[i] + 1.0;
			++ind;
		}
		// 最初
		colorMap->setGLColorByStep( step01 );
		::glBegin( GL_TRIANGLE_FAN );
		if( clockwise ){
			::glVertex2d( xx0, yy0 );
			::glVertex2dAtMiddle( xx0, yy0, xx2, yy2, part02[0] );
			::glVertex2dAtMiddle( xx0, yy0, xx1, yy1, xx2, yy2, part012[0], -1.0 ); // 必要なら自動的に node1 を追加する
		}else{
			::glVertex2d( xx0, yy0 );
			::glVertex2dAtMiddle( xx0, yy0, xx1, yy1, xx2, yy2, -1.0, part012[0] ); // 必要なら自動的に node1 を追加する
			::glVertex2dAtMiddle( xx0, yy0, xx2, yy2, part02[0] );
		}
		::glEnd();
		// 中間
		int i=1;
		while(i<count02){
			colorMap->setGLColorByStep( step01+i );
			::glBegin( GL_TRIANGLE_FAN );
			if( clockwise ){
				::glVertex2dAtMiddle( xx0, yy0, xx1, yy1, xx2, yy2, part012[i], part012[i-1] );
				::glVertex2dAtMiddle( xx0, yy0, xx2, yy2, part02[i-1] );
				::glVertex2dAtMiddle( xx0, yy0, xx2, yy2, part02[i] );
			}else{
				::glVertex2dAtMiddle( xx0, yy0, xx1, yy1, xx2, yy2, part012[i-1], part012[i] );
				::glVertex2dAtMiddle( xx0, yy0, xx2, yy2, part02[i] );
				::glVertex2dAtMiddle( xx0, yy0, xx2, yy2, part02[i-1] );
			}
			::glEnd();
			++i;
		}
		// 最後
		colorMap->setGLColorByStep( step01+count02 );
		::glBegin( GL_TRIANGLE_FAN );
		if( clockwise ){
			::glVertex2dAtMiddle( xx0, yy0, xx2, yy2, part02[count02-1] );
			::glVertex2d( xx2, yy2 );
			::glVertex2dAtMiddle( xx0, yy0, xx1, yy1, xx2, yy2, 2.0, part012[count02-1] );
		}else{
			::glVertex2dAtMiddle( xx0, yy0, xx1, yy1, xx2, yy2, part012[count02-1], 2.0 );
			::glVertex2d( xx2, yy2 );
			::glVertex2dAtMiddle( xx0, yy0, xx2, yy2, part02[count02-1] );
		}
		::glEnd();
		delete[] part012;
	}
	if( part01 != NULL ){
		delete[] part01;
	}
	if( part12 != NULL ){
		delete[] part12;
	}
	if( part02 != NULL ){
		delete[] part02;
	}
}
