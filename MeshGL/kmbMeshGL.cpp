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
#include "MeshDB/kmbElementContainerTriangleArray.h"
#include "MeshDB/kmbElementContainerOpenGLDraw.h"
#include "Geometry/kmbBoundingBox.h"
#include "Geometry/kmbPoint3DContainerArray.h"
#include "MeshGL/kmbColorMap.h"

#include <cmath>
#include <cstring>
#include <set>
#include <cstdio>

#ifdef WIN32
 #if _MSC_VER >= 1400
  #define sprintf sprintf_s
 #endif
#endif

kmb::MeshGL::MeshGL(size_t listSize)
: mesh(NULL)
, updateList(true)
, markSize(0.5)
, pointSize(8.0f)
, lineWidth(1.0f)
, nodeOffset(0)
, elementOffset(0)
, prefMode(kmb::MeshGL::PRECISION)
{
	lists.reserve(listSize);
	lists.insert( lists.begin(), listSize, 0 );
}

kmb::MeshGL::~MeshGL(void)
{
	clearCache();
}

void
kmb::MeshGL::clearCache(void)
{
	std::vector< GLuint >::iterator lIter = lists.begin();
	while( lIter != lists.end() ){
		GLuint list = *lIter;
		if( list != 0 ){
			glDeleteLists( list, 1 );
		}
		++lIter;
	}
	lists.clear();
}

size_t
kmb::MeshGL::getListSize(void) const
{
	return this->lists.size();
}

void
kmb::MeshGL::setList(size_t index,GLuint list)
{
	if( 0 <= index && index < lists.size() ){
		lists[index] = list;
	}
}

GLuint
kmb::MeshGL::getList(size_t index) const
{
	if( 0 <= index && index < lists.size() ){
		return lists[index];
	}else{
		return 0;
	}
}

void
kmb::MeshGL::update(bool flag)
{
	updateList = flag;
}

bool
kmb::MeshGL::isUpdate(void) const
{
	return updateList;
}

void
kmb::MeshGL::setPreferenceMode(kmb::MeshGL::drawingPreference mode)
{
	prefMode = mode;
}


kmb::MeshGL::drawingPreference
kmb::MeshGL::getPreferenceMode(void) const
{
	return prefMode;
}

void
kmb::MeshGL::setMesh(const kmb::MeshData* mesh)
{
	this->mesh = mesh;
}

void
kmb::MeshGL::setMarkSize(double size)
{
	this->markSize = size;
}

double
kmb::MeshGL::getMarkSize(void) const
{
	return this->markSize;
}

void
kmb::MeshGL::setPointSize(GLfloat size)
{
	this->pointSize = size;
}

GLfloat
kmb::MeshGL::getPointSize(void) const
{
	return this->pointSize;
}

void
kmb::MeshGL::setLineWidth(GLfloat size)
{
	this->lineWidth = size;
}

GLfloat
kmb::MeshGL::getLineWidth(void) const
{
	return this->lineWidth;
}

void
kmb::MeshGL::setNodeOffset(int offset)
{
	this->nodeOffset = offset;
}

int
kmb::MeshGL::getNodeOffset(void) const
{
	return this->nodeOffset;
}

void
kmb::MeshGL::setElementOffset(int offset)
{
	this->elementOffset = offset;
}

int
kmb::MeshGL::getElementOffset(void) const
{
	return this->elementOffset;
}

void
kmb::MeshGL::drawEdge(kmb::bodyIdType bodyId)
{
	const kmb::Body* body = NULL;
	if( mesh != NULL &&
		(body = mesh->getBodyPtr(bodyId)) != NULL &&
		body->isUniqueDim(1) )
	{
		double x0=0.0,y0=0.0,z0=0.0;
		double x1=0.0,y1=0.0,z1=0.0;
		kmb::ElementContainer::const_iterator eIter = body->begin();
		::glBegin( GL_LINES );
		while( !eIter.isFinished() )
		{
			if( mesh->getNodeXYZ( eIter.getCellId(0), x0, y0, z0 ) &&
				mesh->getNodeXYZ( eIter.getCellId(1), x1, y1, z1 ) )
			{
				::glVertex3d( x0, y0, z0 );
				::glVertex3d( x1, y1, z1 );
			}
			++eIter;
		}
		::glEnd();
	}
	::glFlush();
}

void
kmb::MeshGL::drawEdgeWithScale(kmb::bodyIdType bodyId,double x,double y,double z,double scale)
{
	const kmb::Body* body = NULL;
	if( mesh != NULL &&
		(body = mesh->getBodyPtr(bodyId)) != NULL &&
		body->isUniqueDim(1) )
	{
		double x0=0.0,y0=0.0,z0=0.0;
		double x1=0.0,y1=0.0,z1=0.0;
		kmb::ElementContainer::const_iterator eIter = body->begin();
		::glBegin( GL_LINES );
		while( !eIter.isFinished() )
		{
			if( mesh->getNodeXYZ( eIter.getCellId(0), x0, y0, z0 ) &&
				mesh->getNodeXYZ( eIter.getCellId(1), x1, y1, z1 ) )
			{
				::glVertex3d(
					scale * ( x0 - x ) + x,
					scale * ( y0 - y ) + y,
					scale * ( z0 - z ) + z);
				::glVertex3d(
					scale * ( x1 - x ) + x,
					scale * ( y1 - y ) + y,
					scale * ( z1 - z ) + z);
			}
			++eIter;
		}
		::glEnd();
		::glFlush();
	}
}

void
kmb::MeshGL::drawSurface(kmb::bodyIdType bodyId)
{
	const kmb::Body* body = NULL;
	const kmb::Point3DContainer* points = NULL;
	if( mesh != NULL &&
		(body = mesh->getBodyPtr(bodyId)) != NULL &&
		(points = mesh->getNodes()) != NULL )
	{
		if( 0==strcmp(points->getContainerType(),kmb::Point3DContainerArray::CONTAINER_TYPE) &&
			::glIsEnabled( GL_VERTEX_ARRAY ) == GL_TRUE &&
			body->isUniqueDim(2) && body->getCount() > 0 )
		{

			if( 0==strcmp(kmb::ElementContainerTriangleArray::CONTAINER_TYPE,body->getContainerType()) ){
				::glEnableClientState(GL_VERTEX_ARRAY);
				::glVertexPointer(3 , GL_DOUBLE , 0 ,
					reinterpret_cast<const kmb::Point3DContainerArray*>(points)->getDoubleArray() );
				::glDrawElements(GL_TRIANGLES, 3*static_cast<GLsizei>(body->getCount()),
					GL_UNSIGNED_INT, reinterpret_cast<const kmb::ElementContainerTriangleArray*>(body)->getNodeTable() );
				::glDisableClientState(GL_VERTEX_ARRAY);
				::glFlush();
			}else if( 0==strcmp(kmb::ElementContainerOpenGLDraw::CONTAINER_TYPE,body->getContainerType()) ){
				::glEnableClientState(GL_VERTEX_ARRAY);

				::glVertexPointer(3 , GL_DOUBLE , 0 ,
					reinterpret_cast<const kmb::Point3DContainerArray*>(points)->getDoubleArray() );
				const kmb::ElementContainerOpenGLDraw* elements
					= reinterpret_cast<const kmb::ElementContainerOpenGLDraw*>(body);
				if( elements->getTriCount() > 0 ){
					const size_t triCount = elements->getTriCount();
					const unsigned int* triNodeTable = elements->getTriNodeTable();
					const float* triNormals = elements->getTriNormals();
					::glBegin( GL_TRIANGLES );
					for(size_t i=0;i<triCount;++i){
						::glNormal3f( triNormals[3*i], triNormals[3*i+1], triNormals[3*i+2] );
						::glArrayElement( triNodeTable[3*i] );
						::glArrayElement( triNodeTable[3*i+1] );
						::glArrayElement( triNodeTable[3*i+2] );
					}
					::glEnd();



				}
				if( elements->getQuadCount() > 0 ){
					const size_t quadCount = elements->getQuadCount();
					const unsigned int* quadNodeTable = elements->getQuadNodeTable();
					const float* quadNormals = elements->getQuadNormals();
					::glBegin( GL_QUADS );
					for(size_t i=0;i<quadCount;++i){
						::glNormal3f( quadNormals[3*i], quadNormals[3*i+1], quadNormals[3*i+2] );
						::glArrayElement( quadNodeTable[4*i] );
						::glArrayElement( quadNodeTable[4*i+1] );
						::glArrayElement( quadNodeTable[4*i+2] );
						::glArrayElement( quadNodeTable[4*i+3] );
					}
					::glEnd();



				}

				::glDisableClientState(GL_VERTEX_ARRAY);
				::glFlush();
			}else{
				::glEnableClientState(GL_VERTEX_ARRAY);
				::glVertexPointer(3 , GL_DOUBLE , 0 ,
					reinterpret_cast<const kmb::Point3DContainerArray*>(points)->getDoubleArray() );
				kmb::ElementContainer::const_iterator eIter = body->begin();
				while( !eIter.isFinished() )
				{
					switch( eIter.getType() ){
						case kmb::TRIANGLE:
						case kmb::TRIANGLE2:
							{
							::glBegin( GL_TRIANGLES );
							kmb::Vector3D normal = points->calcNormalVector
								( eIter.getCellId(0), eIter.getCellId(1), eIter.getCellId(2) );
							::glNormal3d( normal.x(), normal.y(), normal.z() );
							::glArrayElement( eIter.getCellId(0) );
							::glArrayElement( eIter.getCellId(1) );
							::glArrayElement( eIter.getCellId(2) );
							::glEnd();
							}
							break;
						case kmb::QUAD:
						case kmb::QUAD2:
							{
							::glBegin( GL_QUADS );
							kmb::Vector3D normal = points->calcNormalVector
								( eIter.getCellId(0), eIter.getCellId(1), eIter.getCellId(2), eIter.getCellId(3) );
							::glNormal3d( normal.x(), normal.y(), normal.z() );
							::glArrayElement( eIter.getCellId(0) );
							::glArrayElement( eIter.getCellId(1) );
							::glArrayElement( eIter.getCellId(2) );
							::glArrayElement( eIter.getCellId(3) );
							::glEnd();
							}
							break;
						default:
							break;
					}
					++eIter;
				}
				::glFlush();
				::glDisableClientState(GL_VERTEX_ARRAY);
			}
		}else if( body->isUniqueDim(2) ){
			double x0=0.0,y0=0.0,z0=0.0;
			double x1=0.0,y1=0.0,z1=0.0;
			double x2=0.0,y2=0.0,z2=0.0;
			double x3=0.0,y3=0.0,z3=0.0;
			kmb::ElementContainer::const_iterator eIter = body->begin();
			while( eIter != body->end() )
			{
				switch( eIter.getType() ){
					case kmb::TRIANGLE:
					case kmb::TRIANGLE2:
						if( mesh->getNodeXYZ( eIter.getCellId(0), x0, y0, z0 ) &&
							mesh->getNodeXYZ( eIter.getCellId(1), x1, y1, z1 ) &&
							mesh->getNodeXYZ( eIter.getCellId(2), x2, y2, z2 ) )
						{
							drawTriangle(
								x0, y0, z0,
								x1, y1, z1,
								x2, y2, z2);
						}
						break;
					case kmb::QUAD:
					case kmb::QUAD2:
						if( mesh->getNodeXYZ( eIter.getCellId(0), x0, y0, z0 ) &&
							mesh->getNodeXYZ( eIter.getCellId(1), x1, y1, z1 ) &&
							mesh->getNodeXYZ( eIter.getCellId(2), x2, y2, z2 ) &&
							mesh->getNodeXYZ( eIter.getCellId(3), x3, y3, z3 ) )
						{
							drawQuad(
								x0, y0, z0,
								x1, y1, z1,
								x2, y2, z2,
								x3, y3, z3);
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
}

void
kmb::MeshGL::drawSurface2D(kmb::bodyIdType bodyId)
{
	const kmb::Body* body = NULL;
	const kmb::Point2DContainer* points = NULL;
	if( mesh == NULL ||
		(body = mesh->getBodyPtr(bodyId)) == NULL ||
		(points = mesh->getNode2Ds()) == NULL ||
		body->isUniqueDim(2) == false )
	{
		return;
	}
	double x0=0.0,y0=0.0;
	double x1=0.0,y1=0.0;
	double x2=0.0,y2=0.0;
	double x3=0.0,y3=0.0;
	kmb::ElementContainer::const_iterator eIter = body->begin();
	while( eIter != body->end() )
	{
		switch( eIter.getType() ){
		case kmb::TRIANGLE:
		case kmb::TRIANGLE2:
			if( points->getXY( eIter.getCellId(0), x0, y0 ) &&
				points->getXY( eIter.getCellId(1), x1, y1 ) &&
				points->getXY( eIter.getCellId(2), x2, y2 ) )
			{
				drawTriangle2D(
					x0, y0,
					x1, y1,
					x2, y2);
			}
			break;
		case kmb::QUAD:
		case kmb::QUAD2:
			if( points->getXY( eIter.getCellId(0), x0, y0 ) &&
				points->getXY( eIter.getCellId(1), x1, y1 ) &&
				points->getXY( eIter.getCellId(2), x2, y2 ) &&
				points->getXY( eIter.getCellId(3), x3, y3 ) )
			{
				drawQuad2D(
					x0, y0,
					x1, y1,
					x2, y2,
					x3, y3);
			}
			break;
		default:
			break;
		}
		++eIter;
	}
	::glFlush();
}

void
kmb::MeshGL::drawAllNodes(double r)
{
	if( mesh != NULL ){
		if( r <= 0.0 ){
			::glBegin( GL_POINTS );
			kmb::Point3DContainer::const_iterator pIter = mesh->getNodes()->begin();
			double x = 0.0, y = 0.0, z = 0.0;
			while( !pIter.isFinished() ){
				pIter.getXYZ( x, y, z );
				::glVertex3d( x, y, z );
				++pIter;
			}
			::glEnd();
		}else{
			GLUquadricObj* q = ::gluNewQuadric();
			::gluQuadricDrawStyle( q, GLU_FILL);
			kmb::Point3DContainer::const_iterator pIter = mesh->getNodes()->begin();
			double x = 0.0, y = 0.0, z = 0.0;
			while( !pIter.isFinished() ){
				pIter.getXYZ( x, y, z );
				::glPushMatrix();
				::glTranslated(x,y,z);
				::gluSphere( q, r, 10, 10 );
				::glPopMatrix();
				++pIter;
			}
			::gluDeleteQuadric(q);
		}
		::glFlush();
	}
}


void
kmb::MeshGL::drawFaceGroup(const char* faceName)
{
	const kmb::DataBindings* data = NULL;
	const kmb::Point3DContainer* points = NULL;
	if( mesh != NULL &&
		(points = mesh->getNodes()) != NULL &&
		(data = mesh->getDataBindingsPtr( faceName )) != NULL &&
		data->getBindingMode() == kmb::DataBindings::FaceGroup )
	{
		if( 0==strcmp(points->getContainerType(),kmb::Point3DContainerArray::CONTAINER_TYPE) && ::glIsEnabled( GL_VERTEX_ARRAY ) == GL_TRUE ){
			::glEnableClientState(GL_VERTEX_ARRAY);
			::glVertexPointer(3 , GL_DOUBLE , 0 ,
				reinterpret_cast<const kmb::Point3DContainerArray*>(points)->getDoubleArray() );
			kmb::DataBindings::const_iterator fIter = data->begin();
			kmb::DataBindings::const_iterator fEnd = data->end();
			kmb::bodyIdType bodyId = data->getTargetBodyId();
			kmb::Face f;
			while( fIter != fEnd ){
				if( fIter.getFace( f ) ){
					kmb::ElementContainer::const_iterator element = mesh->findElement( f.getElementId(), bodyId );
					kmb::idType localId = f.getLocalFaceId();
					if( !element.isFinished() ){
						switch( element.getBoundaryVertexCount( localId ) ){
							case 3:
							{
								kmb::nodeIdType n0 = element.getBoundaryCellId(localId,0);
								kmb::nodeIdType n1 = element.getBoundaryCellId(localId,1);
								kmb::nodeIdType n2 = element.getBoundaryCellId(localId,2);
								::glBegin( GL_TRIANGLES );
								kmb::Vector3D normal = points->calcNormalVector(n0,n1,n2);
								::glNormal3d( normal.x(), normal.y(), normal.z() );
								::glArrayElement( n0 );
								::glArrayElement( n1 );
								::glArrayElement( n2 );
								::glEnd();
								break;
							}
							case 4:
							{
								kmb::nodeIdType n0 = element.getBoundaryCellId(localId,0);
								kmb::nodeIdType n1 = element.getBoundaryCellId(localId,1);
								kmb::nodeIdType n2 = element.getBoundaryCellId(localId,2);
								kmb::nodeIdType n3 = element.getBoundaryCellId(localId,3);
								::glBegin( GL_QUADS );
								kmb::Vector3D normal = points->calcNormalVector(n0,n1,n2,n3);
								::glNormal3d( normal.x(), normal.y(), normal.z() );
								::glArrayElement( n0 );
								::glArrayElement( n1 );
								::glArrayElement( n2 );
								::glArrayElement( n3 );
								::glEnd();
								break;
							}
							default:
								break;
						}
					}
				}
				++fIter;
			}
			::glFlush();
		}else{
			kmb::DataBindings::const_iterator fIter = data->begin();
			kmb::DataBindings::const_iterator fEnd = data->end();
			kmb::bodyIdType bodyId = data->getTargetBodyId();
			kmb::Face f;
			double x0=0.0,y0=0.0,z0=0.0;
			double x1=0.0,y1=0.0,z1=0.0;
			double x2=0.0,y2=0.0,z2=0.0;
			double x3=0.0,y3=0.0,z3=0.0;
			while( fIter != fEnd ){
				if( fIter.getFace( f ) ){
					kmb::ElementContainer::const_iterator element = mesh->findElement( f.getElementId(), bodyId );
					kmb::idType localId = f.getLocalFaceId();
					if( !element.isFinished() ){
						switch( element.getBoundaryVertexCount( localId ) ){
							case 3:
							{
								if( mesh->getNodeXYZ( element.getBoundaryCellId(localId,0), x0, y0, z0 ) &&
									mesh->getNodeXYZ( element.getBoundaryCellId(localId,1), x1, y1, z1 ) &&
									mesh->getNodeXYZ( element.getBoundaryCellId(localId,2), x2, y2, z2 ) )
								{
									drawTriangle(
										x0, y0, z0,
										x1, y1, z1,
										x2, y2, z2);
								}
								break;
							}
							case 4:
							{
								if( mesh->getNodeXYZ( element.getBoundaryCellId(localId,0), x0, y0, z0 ) &&
									mesh->getNodeXYZ( element.getBoundaryCellId(localId,1), x1, y1, z1 ) &&
									mesh->getNodeXYZ( element.getBoundaryCellId(localId,2), x2, y2, z2 ) &&
									mesh->getNodeXYZ( element.getBoundaryCellId(localId,3), x3, y3, z3 ) )
								{
									drawQuad(
										x0, y0, z0,
										x1, y1, z1,
										x2, y2, z2,
										x3, y3, z3);
								}
								break;
							}
							default:
								break;
						}
					}
				}
				++fIter;
			}
			::glFlush();
		}
	}
}

void
kmb::MeshGL::deformSurface(kmb::bodyIdType bodyId, const char* displacement, double factor)
{
	const kmb::Body* body = NULL;
	const kmb::DataBindings* data = NULL;
	const kmb::Point3DContainer* points = NULL;
	if( mesh != NULL &&
		(body = mesh->getBodyPtr(bodyId)) != NULL &&
		(points = mesh->getNodes()) != NULL &&
		(data = mesh->getDataBindingsPtr( displacement )) != NULL &&
		data->getBindingMode() == kmb::DataBindings::NodeVariable &&
		data->getValueType() == kmb::PhysicalValue::Vector3 )
	{
		if( body->isUniqueDim(2) ){
			double x0=0.0,y0=0.0,z0=0.0;
			double x1=0.0,y1=0.0,z1=0.0;
			double x2=0.0,y2=0.0,z2=0.0;
			double x3=0.0,y3=0.0,z3=0.0;
			double v0[3] = {0.0, 0.0, 0.0},
				v1[3] = {0.0, 0.0, 0.0},
				v2[3] = {0.0, 0.0, 0.0},
				v3[3] = {0.0, 0.0, 0.0};
			kmb::ElementContainer::const_iterator eIter = body->begin();
			while( !eIter.isFinished() )
			{
				switch( eIter.getType() ){
					case kmb::TRIANGLE:
					case kmb::TRIANGLE2:
						if( mesh->getNodeXYZ( eIter.getCellId(0), x0, y0, z0 ) &&
							mesh->getNodeXYZ( eIter.getCellId(1), x1, y1, z1 ) &&
							mesh->getNodeXYZ( eIter.getCellId(2), x2, y2, z2 ) &&
							data->getPhysicalValue( eIter.getCellId(0), v0 ) &&
							data->getPhysicalValue( eIter.getCellId(1), v1 ) &&
							data->getPhysicalValue( eIter.getCellId(2), v2 ) )
						{
							drawTriangle(
								x0 + factor * v0[0],
								y0 + factor * v0[1],
								z0 + factor * v0[2],
								x1 + factor * v1[0],
								y1 + factor * v1[1],
								z1 + factor * v1[2],
								x2 + factor * v2[0],
								y2 + factor * v2[1],
								z2 + factor * v2[2]);
						}
						break;
					case kmb::QUAD:
					case kmb::QUAD2:
						if( mesh->getNodeXYZ( eIter.getCellId(0), x0, y0, z0 ) &&
							mesh->getNodeXYZ( eIter.getCellId(1), x1, y1, z1 ) &&
							mesh->getNodeXYZ( eIter.getCellId(2), x2, y2, z2 ) &&
							mesh->getNodeXYZ( eIter.getCellId(3), x3, y3, z3 ) &&
							data->getPhysicalValue( eIter.getCellId(0), v0 ) &&
							data->getPhysicalValue( eIter.getCellId(1), v1 ) &&
							data->getPhysicalValue( eIter.getCellId(2), v2 ) &&
							data->getPhysicalValue( eIter.getCellId(3), v3 ) )
						{
							drawQuad(
								x0 + factor * v0[0],
								y0 + factor * v0[1],
								z0 + factor * v0[2],
								x1 + factor * v1[0],
								y1 + factor * v1[1],
								z1 + factor * v1[2],
								x2 + factor * v2[0],
								y2 + factor * v2[1],
								z2 + factor * v2[2],
								x3 + factor * v3[0],
								y3 + factor * v3[1],
								z3 + factor * v3[2]);
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
}

void
kmb::MeshGL::deformFaceGroup(const char* faceName, const char* displacement, double factor)
{
	const kmb::DataBindings* data0 = NULL;
	const kmb::DataBindings* data1 = NULL;
	if( mesh != NULL &&
		(data0 = mesh->getDataBindingsPtr( faceName )) != NULL &&
		data0->getBindingMode() == kmb::DataBindings::FaceGroup &&
		(data1 = mesh->getDataBindingsPtr( displacement )) != NULL &&
		data1->getBindingMode() == kmb::DataBindings::NodeVariable &&
		data1->getValueType() == kmb::PhysicalValue::Vector3 )
	{
		kmb::DataBindings::const_iterator fIter = data0->begin();
		kmb::DataBindings::const_iterator fEnd = data0->end();
		kmb::Face f;
		kmb::bodyIdType bodyId = data0->getTargetBodyId();
		double x0=0.0,y0=0.0,z0=0.0;
		double x1=0.0,y1=0.0,z1=0.0;
		double x2=0.0,y2=0.0,z2=0.0;
		double x3=0.0,y3=0.0,z3=0.0;
		double v0[3] = {0.0, 0.0, 0.0},
			v1[3] = {0.0, 0.0, 0.0},
			v2[3] = {0.0, 0.0, 0.0},
			v3[3] = {0.0, 0.0, 0.0};
		while( fIter != fEnd ){
			if( fIter.getFace(f) ){
				kmb::ElementContainer::const_iterator element = mesh->findElement( f.getElementId(), bodyId );
				kmb::idType localId = f.getLocalFaceId();
				if( !element.isFinished() ){
					switch( element.getBoundaryVertexCount( localId ) ){
						case 3:
						{
							kmb::nodeIdType nodeId0 = element.getBoundaryCellId(localId,0);
							kmb::nodeIdType nodeId1 = element.getBoundaryCellId(localId,1);
							kmb::nodeIdType nodeId2 = element.getBoundaryCellId(localId,2);
							if( mesh->getNodeXYZ( nodeId0, x0, y0, z0 ) &&
								mesh->getNodeXYZ( nodeId1, x1, y1, z1 ) &&
								mesh->getNodeXYZ( nodeId2, x2, y2, z2 ) &&
								data1->getPhysicalValue( nodeId0, v0 ) &&
								data1->getPhysicalValue( nodeId1, v1 ) &&
								data1->getPhysicalValue( nodeId2, v2 ) )
							{
								drawTriangle(
									x0 + factor * v0[0],
									y0 + factor * v0[1],
									z0 + factor * v0[2],
									x1 + factor * v1[0],
									y1 + factor * v1[1],
									z1 + factor * v1[2],
									x2 + factor * v2[0],
									y2 + factor * v2[1],
									z2 + factor * v2[2]);
							}
							break;
						}
						case 4:
						{
							kmb::nodeIdType nodeId0 = element.getBoundaryCellId(localId,0);
							kmb::nodeIdType nodeId1 = element.getBoundaryCellId(localId,1);
							kmb::nodeIdType nodeId2 = element.getBoundaryCellId(localId,2);
							kmb::nodeIdType nodeId3 = element.getBoundaryCellId(localId,3);
							if( mesh->getNodeXYZ( nodeId0, x0, y0, z0 ) &&
								mesh->getNodeXYZ( nodeId1, x1, y1, z1 ) &&
								mesh->getNodeXYZ( nodeId2, x2, y2, z2 ) &&
								mesh->getNodeXYZ( nodeId3, x3, y3, z3 ) &&
								data1->getPhysicalValue( nodeId0, v0 ) &&
								data1->getPhysicalValue( nodeId1, v1 ) &&
								data1->getPhysicalValue( nodeId2, v2 ) &&
								data1->getPhysicalValue( nodeId3, v3 ) )
							{
								drawQuad(
									x0 + factor * v0[0],
									y0 + factor * v0[1],
									z0 + factor * v0[2],
									x1 + factor * v1[0],
									y1 + factor * v1[1],
									z1 + factor * v1[2],
									x2 + factor * v2[0],
									y2 + factor * v2[1],
									z2 + factor * v2[2],
									x3 + factor * v3[0],
									y3 + factor * v3[1],
									z3 + factor * v3[2]);
							}
							break;
						}
						default:
							break;
					}
				}
			}
			++fIter;
		}
		::glFlush();
	}
}

void
kmb::MeshGL::deformXYZFaceGroup
(const char* faceName, const char* displacementX, const char* displacementY, const char* displacementZ, double factor)
{
	const kmb::DataBindings* fdata = NULL;
	const kmb::DataBindings* dataX = NULL;
	const kmb::DataBindings* dataY = NULL;
	const kmb::DataBindings* dataZ = NULL;
	double u0x,u0y,u0z,u1x,u1y,u1z,u2x,u2y,u2z,u3x,u3y,u3z;
	double vX,vY,vZ;
	if( mesh != NULL &&
		(fdata = mesh->getDataBindingsPtr( faceName )) != NULL &&
		fdata->getBindingMode() == kmb::DataBindings::FaceGroup &&
		(dataX = mesh->getDataBindingsPtr( displacementX )) != NULL &&
		dataX->getBindingMode() == kmb::DataBindings::NodeVariable &&
		dataX->getValueType() == kmb::PhysicalValue::Scalar &&
		(dataY = mesh->getDataBindingsPtr( displacementY )) != NULL &&
		dataY->getBindingMode() == kmb::DataBindings::NodeVariable &&
		dataY->getValueType() == kmb::PhysicalValue::Scalar &&
		(dataZ = mesh->getDataBindingsPtr( displacementZ )) != NULL &&
		dataZ->getBindingMode() == kmb::DataBindings::NodeVariable &&
		dataZ->getValueType() == kmb::PhysicalValue::Scalar )
	{
		double x0=0.0,y0=0.0,z0=0.0;
		double x1=0.0,y1=0.0,z1=0.0;
		double x2=0.0,y2=0.0,z2=0.0;
		double x3=0.0,y3=0.0,z3=0.0;
		kmb::DataBindings::const_iterator fIter = fdata->begin();
		kmb::DataBindings::const_iterator fEnd = fdata->end();
		kmb::bodyIdType bodyId = fdata->getTargetBodyId();
		kmb::Face f;
		while( fIter != fEnd ){
			if( fIter.getFace(f) ){
				kmb::ElementContainer::const_iterator element = mesh->findElement( f.getElementId(), bodyId );
				kmb::idType localId = f.getLocalFaceId();
				if( !element.isFinished() ){
					switch( element.getBoundaryVertexCount( localId ) ){
						case 3:
						{
							kmb::nodeIdType nodeId0 = element.getBoundaryCellId(localId,0);
							kmb::nodeIdType nodeId1 = element.getBoundaryCellId(localId,1);
							kmb::nodeIdType nodeId2 = element.getBoundaryCellId(localId,2);
							if( mesh->getNodeXYZ( nodeId0, x0, y0, z0 ) &&
								mesh->getNodeXYZ( nodeId1, x1, y1, z1 ) &&
								mesh->getNodeXYZ( nodeId2, x2, y2, z2 ) )
							{
								vX = 0.0; vY = 0.0; vZ = 0.0;
								dataX->getPhysicalValue( nodeId0, &vX );
								dataY->getPhysicalValue( nodeId0, &vY );
								dataZ->getPhysicalValue( nodeId0, &vZ );
								u0x = x0 + factor * vX;
								u0y = y0 + factor * vY;
								u0z = z0 + factor * vZ;
								vX = 0.0; vY = 0.0; vZ = 0.0;
								dataX->getPhysicalValue( nodeId1, &vX );
								dataY->getPhysicalValue( nodeId1, &vY );
								dataZ->getPhysicalValue( nodeId1, &vZ );
								u1x = x1 + factor * vX;
								u1y = y1 + factor * vY;
								u1z = z1 + factor * vZ;
								vX = 0.0; vY = 0.0; vZ = 0.0;
								dataX->getPhysicalValue( nodeId2, &vX );
								dataY->getPhysicalValue( nodeId2, &vY );
								dataZ->getPhysicalValue( nodeId2, &vZ );
								u2x = x2 + factor * vX;
								u2y = y2 + factor * vY;
								u2z = z2 + factor * vZ;
								drawTriangle(u0x,u0y,u0z,u1x,u1y,u1z,u2x,u2y,u2z);
							}
							break;
						}
						case 4:
						{
							kmb::nodeIdType nodeId0 = element.getBoundaryCellId(localId,0);
							kmb::nodeIdType nodeId1 = element.getBoundaryCellId(localId,1);
							kmb::nodeIdType nodeId2 = element.getBoundaryCellId(localId,2);
							kmb::nodeIdType nodeId3 = element.getBoundaryCellId(localId,3);
							if( mesh->getNodeXYZ( nodeId0, x0, y0, z0 ) &&
								mesh->getNodeXYZ( nodeId1, x1, y1, z1 ) &&
								mesh->getNodeXYZ( nodeId2, x2, y2, z2 ) &&
								mesh->getNodeXYZ( nodeId3, x3, y3, z3 ) )
							{
								vX = 0.0; vY = 0.0; vZ = 0.0;
								dataX->getPhysicalValue( nodeId0, &vX );
								dataY->getPhysicalValue( nodeId0, &vY );
								dataZ->getPhysicalValue( nodeId0, &vZ );
								u0x = x0 + factor * vX;
								u0y = y0 + factor * vY;
								u0z = z0 + factor * vZ;
								vX = 0.0; vY = 0.0; vZ = 0.0;
								dataX->getPhysicalValue( nodeId1, &vX );
								dataY->getPhysicalValue( nodeId1, &vY );
								dataZ->getPhysicalValue( nodeId1, &vZ );
								u1x = x1 + factor * vX;
								u1y = y1 + factor * vY;
								u1z = z1 + factor * vZ;
								vX = 0.0; vY = 0.0; vZ = 0.0;
								dataX->getPhysicalValue( nodeId2, &vX );
								dataY->getPhysicalValue( nodeId2, &vY );
								dataZ->getPhysicalValue( nodeId2, &vZ );
								u2x = x2 + factor * vX;
								u2y = y2 + factor * vY;
								u2z = z2 + factor * vZ;
								vX = 0.0; vY = 0.0; vZ = 0.0;
								dataX->getPhysicalValue( nodeId3, &vX );
								dataY->getPhysicalValue( nodeId3, &vY );
								dataZ->getPhysicalValue( nodeId3, &vZ );
								u3x = x3 + factor * vX;
								u3y = y3 + factor * vY;
								u3z = z3 + factor * vZ;
								drawQuad(u0x,u0y,u0z,u1x,u1y,u1z,u2x,u2y,u2z,u3x,u3y,u3z);
							}
							break;
						}
						default:
							break;
					}
				}
			}
			++fIter;
		}
		::glFlush();
	}
}

void
kmb::MeshGL::drawTriangle
(double x0,double y0,double z0,
 double x1,double y1,double z1,
 double x2,double y2,double z2)
{
	kmb::Vector3D normal =
		kmb::Point3D::calcNormalVector(x0,y0,z0,x1,y1,z1,x2,y2,z2);
	::glBegin( GL_TRIANGLES );
	::glNormal3d( normal.x(), normal.y(), normal.z() );
	::glVertex3d( x0, y0, z0 );
	::glVertex3d( x1, y1, z1 );
	::glVertex3d( x2, y2, z2 );
	::glEnd();
}

void
kmb::MeshGL::drawTriangle2D
(double x0,double y0,
 double x1,double y1,
 double x2,double y2)
{
	::glBegin( GL_TRIANGLES );
	::glVertex2d( x0, y0 );
	::glVertex2d( x1, y1 );
	::glVertex2d( x2, y2 );
	::glEnd();
}

void
kmb::MeshGL::drawQuad
(double x0,double y0,double z0,
 double x1,double y1,double z1,
 double x2,double y2,double z2,
 double x3,double y3,double z3)
{
	kmb::Vector3D normal = kmb::Point3D::calcNormalVector
		(x0,y0,z0,x1,y1,z1,x2,y2,z2,x3,y3,z3);
	::glBegin( GL_QUADS );
	::glNormal3d( normal.x(), normal.y(), normal.z() );
	::glVertex3d( x0, y0, z0 );
	::glVertex3d( x1, y1, z1 );
	::glVertex3d( x2, y2, z2 );
	::glVertex3d( x3, y3, z3 );
	::glEnd();
}

void
kmb::MeshGL::drawQuad2D
(double x0,double y0,
 double x1,double y1,
 double x2,double y2,
 double x3,double y3)
{
	::glBegin( GL_QUADS );
	::glVertex2d( x0, y0 );
	::glVertex2d( x1, y1 );
	::glVertex2d( x2, y2 );
	::glVertex2d( x3, y3 );
	::glEnd();
}

void
kmb::MeshGL::drawVector(const char* vectorName, double factor, kmb::ColorMap* colorMap)
{
	const kmb::DataBindings* data = NULL;
	if( mesh != NULL && colorMap != NULL &&
		(data = mesh->getDataBindingsPtr( vectorName )) != NULL &&
		data->getBindingMode() == kmb::DataBindings::NodeVariable &&
		data->getValueType() == kmb::PhysicalValue::Vector3 )
	{
		::glPushAttrib( GL_ENABLE_BIT );
		::glEnable( GL_COLOR_MATERIAL );
		double val[3] = {0.0,0.0,0.0};
		double x=0.0,y=0.0,z=0.0;
		kmb::DataBindings::const_iterator vIter = data->begin();
		while( !vIter.isFinished() ){
			if( mesh->getNodeXYZ( vIter.getId(), x, y, z ) && vIter.getValue( val ) ){
				drawVector( x, y, z, val[0], val[1], val[2], factor, colorMap );
			}
			++vIter;
		}
		::glPopAttrib();
		::glFlush();
	}
}

void
kmb::MeshGL::drawSurfaceVector(kmb::bodyIdType bodyId,const char* vectorName, double factor, kmb::ColorMap* colorMap)
{
	const kmb::Body* body = NULL;
	const kmb::DataBindings* data = NULL;
	if( mesh != NULL && colorMap != NULL &&
		(body = mesh->getBodyPtr(bodyId)) != NULL &&
		body->isUniqueDim(2) &&
		(data = mesh->getDataBindingsPtr( vectorName )) != NULL &&
		data->getBindingMode() == kmb::DataBindings::NodeVariable &&
		data->getValueType() == kmb::PhysicalValue::Vector3 )
	{
		::glPushAttrib( GL_ENABLE_BIT );
		::glEnable( GL_COLOR_MATERIAL );
		std::set<kmb::nodeIdType> nodeIds;
		body->getNodesOfBody( nodeIds );
		double val[3] = {0.0,0.0,0.0};
		double x=0.0,y=0.0,z=0.0;
		std::set<kmb::nodeIdType>::iterator nIter = nodeIds.begin();
		while( nIter != nodeIds.end() ){
			kmb::nodeIdType nodeId = *nIter;
			if( mesh->getNodeXYZ( nodeId, x, y, z ) && data->getPhysicalValue( nodeId, val ) ){
				drawVector( x, y, z, val[0], val[1], val[2], factor, colorMap );
			}
			++nIter;
		}
		::glPopAttrib();
		::glFlush();
	}
}

void
kmb::MeshGL::drawFaceGroupVector(const char* faceName,const char* vectorName, double factor, kmb::ColorMap* colorMap)
{
	const kmb::DataBindings* faceGroup = NULL;
	const kmb::DataBindings* data = NULL;
	if( mesh != NULL && colorMap != NULL &&
		(faceGroup = mesh->getDataBindingsPtr( faceName )) != NULL &&
		faceGroup->getBindingMode() == kmb::DataBindings::FaceGroup &&
		(data = mesh->getDataBindingsPtr( vectorName )) != NULL &&
		data->getBindingMode() == kmb::DataBindings::NodeVariable &&
		data->getValueType() == kmb::PhysicalValue::Vector3 )
	{
		::glPushAttrib( GL_ENABLE_BIT );
		::glEnable( GL_COLOR_MATERIAL );
		std::set<kmb::nodeIdType> nodeIds;
		mesh->getNodeSetFromDataBindings(nodeIds,faceName);
		double val[3] = {0.0,0.0,0.0};
		double x=0.0,y=0.0,z=0.0;
		std::set<kmb::nodeIdType>::iterator nIter = nodeIds.begin();
		while( nIter != nodeIds.end() ){
			kmb::nodeIdType nodeId = *nIter;
			if( mesh->getNodeXYZ( nodeId, x, y, z ) && data->getPhysicalValue( nodeId, val ) ){
				drawVector( x, y, z, val[0], val[1], val[2], factor, colorMap );
			}
			++nIter;
		}
		::glPopAttrib();
		::glFlush();
	}
}

void kmb::MeshGL::drawField(const char* vectorName, double arrowSize, kmb::ColorMap* colorMap)
{
	const kmb::DataBindings* data = NULL;
	if( mesh != NULL && colorMap != NULL &&
		(data = mesh->getDataBindingsPtr( vectorName )) != NULL &&
		data->getBindingMode() == kmb::DataBindings::NodeVariable &&
		data->getValueType() == kmb::PhysicalValue::Vector3 )
	{
		::glPushAttrib( GL_ENABLE_BIT );
		::glEnable( GL_COLOR_MATERIAL );
		double val[3] = {0.0,0.0,0.0};
		double x=0.0,y=0.0,z=0.0;
		kmb::DataBindings::const_iterator vIter = data->begin();
		while( !vIter.isFinished() ){
			if( mesh->getNodeXYZ( vIter.getId(), x, y, z ) && vIter.getValue( val ) ){
				drawArrow( x, y, z, val[0], val[1], val[2], arrowSize, colorMap );
			}
			++vIter;
		}
		::glPopAttrib();
		::glFlush();
	}
}

void kmb::MeshGL::drawSurfaceField(kmb::bodyIdType bodyId,const char* vectorName, double arrowSize, kmb::ColorMap* colorMap)
{
	const kmb::Body* body = NULL;
	const kmb::DataBindings* data = NULL;
	if( mesh != NULL && colorMap != NULL &&
		(body = mesh->getBodyPtr(bodyId)) != NULL &&
		body->isUniqueDim(2) &&
		(data = mesh->getDataBindingsPtr( vectorName )) != NULL &&
		data->getBindingMode() == kmb::DataBindings::NodeVariable &&
		data->getValueType() == kmb::PhysicalValue::Vector3 )
	{
		::glPushAttrib( GL_ENABLE_BIT );
		::glEnable( GL_COLOR_MATERIAL );
		std::set<kmb::nodeIdType> nodeIds;
		body->getNodesOfBody( nodeIds );
		double val[3] = {0.0,0.0,0.0};
		double x=0.0,y=0.0,z=0.0;
		std::set<kmb::nodeIdType>::iterator nIter = nodeIds.begin();
		while( nIter != nodeIds.end() ){
			kmb::nodeIdType nodeId = *nIter;
			if( mesh->getNodeXYZ( nodeId, x, y, z ) && data->getPhysicalValue( nodeId, val ) ){
				drawArrow( x, y, z, val[0], val[1], val[2], arrowSize, colorMap );
			}
			++nIter;
		}
		::glPopAttrib();
		::glFlush();
	}
}

void
kmb::MeshGL::getDeformFaceGroupBoundingBox(kmb::BoundingBox& bbox,const char* faceName, const char* displacement, double factor) const
{
	const kmb::DataBindings* data0 = NULL;
	const kmb::DataBindings* data1 = NULL;
	if( mesh != NULL &&
		(data0 = mesh->getDataBindingsPtr( faceName )) != NULL &&
		data0->getBindingMode() == kmb::DataBindings::FaceGroup &&
		(data1 = mesh->getDataBindingsPtr( displacement )) != NULL &&
		data1->getBindingMode() == kmb::DataBindings::NodeVariable &&
		data1->getValueType() == kmb::PhysicalValue::Vector3 )
	{
		double x0=0.0,y0=0.0,z0=0.0;
		double x1=0.0,y1=0.0,z1=0.0;
		double x2=0.0,y2=0.0,z2=0.0;
		double x3=0.0,y3=0.0,z3=0.0;
		double v0[3], v1[3], v2[3], v3[3];
		kmb::Face f;
		kmb::bodyIdType bodyId = data0->getTargetBodyId();
		kmb::DataBindings::const_iterator fIter = data0->begin();
		while( !fIter.isFinished() ){
			if( fIter.getFace(f) ){
				kmb::ElementContainer::const_iterator element = mesh->findElement( f.getElementId(), bodyId );
				kmb::idType localId = f.getLocalFaceId();
				if( !element.isFinished() ){
					switch( element.getBoundaryVertexCount( localId ) ){
						case 3:
						{
							kmb::nodeIdType nodeId0 = element.getBoundaryCellId(localId,0);
							kmb::nodeIdType nodeId1 = element.getBoundaryCellId(localId,1);
							kmb::nodeIdType nodeId2 = element.getBoundaryCellId(localId,2);
							if( mesh->getNodeXYZ( nodeId0, x0, y0, z0 ) &&
								mesh->getNodeXYZ( nodeId1, x1, y1, z1 ) &&
								mesh->getNodeXYZ( nodeId2, x2, y2, z2 ) &&
								data1->getPhysicalValue( nodeId0, v0 ) &&
								data1->getPhysicalValue( nodeId1, v1 ) &&
								data1->getPhysicalValue( nodeId2, v2 ) )
							{
								bbox.update(
									x0 + factor * v0[0],
									y0 + factor * v0[1],
									z0 + factor * v0[2]);
								bbox.update(
									x1 + factor * v1[0],
									y1 + factor * v1[1],
									z1 + factor * v1[2]);
								bbox.update(
									x2 + factor * v2[0],
									y2 + factor * v2[1],
									z2 + factor * v2[2]);
							}
							break;
						}
						case 4:
						{
							kmb::nodeIdType nodeId0 = element.getBoundaryCellId(localId,0);
							kmb::nodeIdType nodeId1 = element.getBoundaryCellId(localId,1);
							kmb::nodeIdType nodeId2 = element.getBoundaryCellId(localId,2);
							kmb::nodeIdType nodeId3 = element.getBoundaryCellId(localId,3);
							if( mesh->getNodeXYZ( nodeId0, x0, y0, z0 ) &&
								mesh->getNodeXYZ( nodeId1, x1, y1, z1 ) &&
								mesh->getNodeXYZ( nodeId2, x2, y2, z2 ) &&
								mesh->getNodeXYZ( nodeId3, x3, y3, z3 ) &&
								data1->getPhysicalValue( nodeId0, v0 ) &&
								data1->getPhysicalValue( nodeId1, v1 ) &&
								data1->getPhysicalValue( nodeId2, v2 ) &&
								data1->getPhysicalValue( nodeId3, v3 ) )
							{
								bbox.update(
									x0 + factor * v0[0],
									y0 + factor * v0[1],
									z0 + factor * v0[2]);
								bbox.update(
									x1 + factor * v1[0],
									y1 + factor * v1[1],
									z1 + factor * v1[2]);
								bbox.update(
									x2 + factor * v2[0],
									y2 + factor * v2[1],
									z2 + factor * v2[2]);
								bbox.update(
									x3 + factor * v3[0],
									y3 + factor * v3[1],
									z3 + factor * v3[2]);
							}
							break;
						}
						default:
							break;
					}
				}
			}
			++fIter;
		}
	}
}

void
kmb::MeshGL::getDeformXYZFaceGroupBoundingBox(kmb::BoundingBox& bbox,const char* faceName, const char* displacementX, const char* displacementY, const char* displacementZ, double factor) const
{
	const kmb::DataBindings* fdata = NULL;
	const kmb::DataBindings* dataX = NULL;
	const kmb::DataBindings* dataY = NULL;
	const kmb::DataBindings* dataZ = NULL;
	double uX,uY,uZ;
	if( mesh != NULL &&
		(fdata = mesh->getDataBindingsPtr( faceName )) != NULL &&
		fdata->getBindingMode() == kmb::DataBindings::FaceGroup &&
		(dataX = mesh->getDataBindingsPtr( displacementX )) != NULL &&
		dataX->getBindingMode() == kmb::DataBindings::NodeVariable &&
		dataX->getValueType() == kmb::PhysicalValue::Scalar &&
		(dataY = mesh->getDataBindingsPtr( displacementY )) != NULL &&
		dataY->getBindingMode() == kmb::DataBindings::NodeVariable &&
		dataY->getValueType() == kmb::PhysicalValue::Scalar &&
		(dataZ = mesh->getDataBindingsPtr( displacementZ )) != NULL &&
		dataZ->getBindingMode() == kmb::DataBindings::NodeVariable &&
		dataZ->getValueType() == kmb::PhysicalValue::Scalar )
	{
		double x0=0.0,y0=0.0,z0=0.0;
		double x1=0.0,y1=0.0,z1=0.0;
		double x2=0.0,y2=0.0,z2=0.0;
		double x3=0.0,y3=0.0,z3=0.0;
		kmb::Face f;
		kmb::bodyIdType bodyId = fdata->getTargetBodyId();
		kmb::DataBindings::const_iterator fIter = fdata->begin();
		kmb::DataBindings::const_iterator fEnd = fdata->end();
		while( fIter != fEnd ){
			if( fIter.getFace(f) ){
				kmb::ElementContainer::const_iterator element = mesh->findElement( f.getElementId(), bodyId );
				kmb::idType localId = f.getLocalFaceId();
				if( !element.isFinished() ){
					switch( element.getBoundaryVertexCount( localId ) ){
						case 3:
						{
							kmb::nodeIdType nodeId0 = element.getBoundaryCellId(localId,0);
							kmb::nodeIdType nodeId1 = element.getBoundaryCellId(localId,1);
							kmb::nodeIdType nodeId2 = element.getBoundaryCellId(localId,2);
							if( mesh->getNodeXYZ( nodeId0, x0, y0, z0 ) &&
								mesh->getNodeXYZ( nodeId1, x1, y1, z1 ) &&
								mesh->getNodeXYZ( nodeId2, x2, y2, z2 ) )
							{
								uX = 0.0; uY = 0.0; uZ = 0.0;
								dataX->getPhysicalValue( nodeId0, &uX );
								dataY->getPhysicalValue( nodeId0, &uY );
								dataZ->getPhysicalValue( nodeId0, &uZ );
								bbox.update(
									x0 + factor * uX,
									y0 + factor * uY,
									z0 + factor * uZ);
								uX = 0.0; uY = 0.0; uZ = 0.0;
								dataX->getPhysicalValue( nodeId1, &uX );
								dataY->getPhysicalValue( nodeId1, &uY );
								dataZ->getPhysicalValue( nodeId1, &uZ );
								bbox.update(
									x1 + factor * uX,
									y1 + factor * uY,
									z1 + factor * uZ);
								uX = 0.0; uY = 0.0; uZ = 0.0;
								dataX->getPhysicalValue( nodeId2, &uX );
								dataY->getPhysicalValue( nodeId2, &uY );
								dataZ->getPhysicalValue( nodeId2, &uZ );
								bbox.update(
									x2 + factor * uX,
									y2 + factor * uY,
									z2 + factor * uZ);
							}
							break;
						}
						case 4:
						{
							kmb::nodeIdType nodeId0 = element.getBoundaryCellId(localId,0);
							kmb::nodeIdType nodeId1 = element.getBoundaryCellId(localId,1);
							kmb::nodeIdType nodeId2 = element.getBoundaryCellId(localId,2);
							kmb::nodeIdType nodeId3 = element.getBoundaryCellId(localId,3);
							if( mesh->getNodeXYZ( nodeId0, x0, y0, z0 ) &&
								mesh->getNodeXYZ( nodeId1, x1, y1, z1 ) &&
								mesh->getNodeXYZ( nodeId2, x2, y2, z2 ) &&
								mesh->getNodeXYZ( nodeId3, x3, y3, z3 ) )
							{
								uX = 0.0; uY = 0.0; uZ = 0.0;
								dataX->getPhysicalValue( nodeId0, &uX );
								dataY->getPhysicalValue( nodeId0, &uY );
								dataZ->getPhysicalValue( nodeId0, &uZ );
								bbox.update(
									x0 + factor * uX,
									y0 + factor * uY,
									z0 + factor * uZ);
								uX = 0.0; uY = 0.0; uZ = 0.0;
								dataX->getPhysicalValue( nodeId1, &uX );
								dataY->getPhysicalValue( nodeId1, &uY );
								dataZ->getPhysicalValue( nodeId1, &uZ );
								bbox.update(
									x1 + factor * uX,
									y1 + factor * uY,
									z1 + factor * uZ);
								uX = 0.0; uY = 0.0; uZ = 0.0;
								dataX->getPhysicalValue( nodeId2, &uX );
								dataY->getPhysicalValue( nodeId2, &uY );
								dataZ->getPhysicalValue( nodeId2, &uZ );
								bbox.update(
									x2 + factor * uX,
									y2 + factor * uY,
									z2 + factor * uZ);
								uX = 0.0; uY = 0.0; uZ = 0.0;
								dataX->getPhysicalValue( nodeId3, &uX );
								dataY->getPhysicalValue( nodeId3, &uY );
								dataZ->getPhysicalValue( nodeId3, &uZ );
								bbox.update(
									x3 + factor * uX,
									y3 + factor * uY,
									z3 + factor * uZ);
							}
							break;
						}
						default:
							break;
					}
				}
			}
			++fIter;
		}
	}
}

void
kmb::MeshGL::drawMarking(const char* condName,const char* mode)
{
	const kmb::DataBindings* data = NULL;
	double x=0.0,y=0.0,z=0.0;
	if( mesh != NULL &&
		(data = mesh->getDataBindingsPtr( condName )) != NULL )
	{

		if( data->getBindingMode() == kmb::DataBindings::NodeGroup && strcmp(mode,"VECTOR") == 0 ){
			if( data->getValueType() == kmb::PhysicalValue::Vector3 ){
				double v[3] = {0.0, 0.0, 0.0};
				kmb::Vector3Value* vData =
					reinterpret_cast<kmb::Vector3Value*>(data->getPhysicalValue());
				if( vData ){
					v[0] = vData->getValue(0);
					v[1] = vData->getValue(1);
					v[2] = vData->getValue(2);
				}
				double len = kmb::Vector3D::abs( v );
				if( len > 0.0 ){
					double diameter = mesh->getBoundingBox().diameter() * 0.05 / len;
					v[0] *= diameter;
					v[1] *= diameter;
					v[2] *= diameter;
				}
				kmb::DataBindings::const_iterator nIter = data->begin();
				while( !nIter.isFinished() ){
					if( mesh->getNodeXYZ( nIter.getId(), x, y, z ) ){
						drawVectorMark( x, y, z, v[0], v[1], v[2] );
					}
					++nIter;
				}
			}
			else if( data->getValueType() == kmb::PhysicalValue::Tensor6 ){
				double v[3] = {0.0, 0.0, 0.0};
				kmb::Tensor6Value* vData =
					reinterpret_cast<kmb::Tensor6Value*>(data->getPhysicalValue());
				if( vData ){
					v[0] = vData->getValue(0);
					v[1] = vData->getValue(1);
					v[2] = vData->getValue(2);
				}
				double len = kmb::Vector3D::abs( v );
				if( len > 0.0 ){
					double diameter = mesh->getBoundingBox().diameter() * 0.05 / len;
					v[0] *= diameter;
					v[1] *= diameter;
					v[2] *= diameter;
				}
				kmb::DataBindings::const_iterator nIter = data->begin();
				while( !nIter.isFinished() ){
					if( mesh->getNodeXYZ( nIter.getId(), x, y, z ) ){
						drawVectorMark( x, y, z, v[0], v[1], v[2] );
					}
					++nIter;
				}
			}
		}
		else if( data->getBindingMode() == kmb::DataBindings::NodeVariable && strcmp(mode,"VECTOR") == 0 ){
			if( data->getValueType() == kmb::PhysicalValue::Vector3 ){
				kmb::DataBindings::const_iterator nIter = data->begin();
				while( !nIter.isFinished() )
				{
					const kmb::Vector3Value* vData = reinterpret_cast<const kmb::Vector3Value*>( nIter.getValue() );
					if( vData && mesh->getNodeXYZ( nIter.getId(), x, y, z ) ){
						drawVectorMark( x, y, z, vData->getValue(0), vData->getValue(1), vData->getValue(2) );
					}
					++nIter;
				}
			}
			else if( data->getValueType() == kmb::PhysicalValue::Tensor6 ){
				kmb::DataBindings::const_iterator nIter = data->begin();
				while( !nIter.isFinished() )
				{
					const kmb::Tensor6Value* vData = reinterpret_cast<const kmb::Tensor6Value*>( nIter.getValue() );
					if( vData && mesh->getNodeXYZ( nIter.getId(), x, y, z ) ){
						drawVectorMark( x, y, z, vData->getValue(0), vData->getValue(1), vData->getValue(2) );
					}
					++nIter;
				}
			}
		}
		else if( data->getBindingMode() == kmb::DataBindings::NodeVariable && strcmp(mode,"NCOUPLE") == 0 ){
			if( data->getValueType() == kmb::PhysicalValue::Integer ){
				kmb::DataBindings::const_iterator nIter = data->begin();
				while( !nIter.isFinished() )
				{
					long nodeId1 = 0;
					if( nIter.getValue( &nodeId1 ) && mesh->getNodeXYZ( nodeId1, x, y, z ) ){
						drawDaggerMark( x, y, z );
					}
					++nIter;
				}
			}
		}
		else if( data->getBindingMode() == kmb::DataBindings::NodeGroup ||
			data->getBindingMode() == kmb::DataBindings::NodeVariable ){
			if( strcmp(mode,"TENSOR") == 0 ){
				kmb::DataBindings::const_iterator nIter = data->begin();
				while( !nIter.isFinished() ){
					if( mesh->getNodeXYZ( nIter.getId(), x, y, z ) ){
						drawTensorMark( x, y, z );
					}
					++nIter;
				}
			}
			else if( strcmp(mode,"DOT") == 0 ){
				kmb::DataBindings::const_iterator nIter = data->begin();
				while( !nIter.isFinished() ){
					if( mesh->getNodeXYZ( nIter.getId(), x, y, z ) ){
						drawDotMark( x, y, z );
					}
					++nIter;
				}
			}
			else if( strcmp(mode,"DAGGER") == 0 ){
				kmb::DataBindings::const_iterator nIter = data->begin();
				while( !nIter.isFinished() ){
					if( mesh->getNodeXYZ( nIter.getId(), x, y, z ) ){
						drawDaggerMark( x, y, z );
					}
					++nIter;
				}
			}
			else if( strcmp(mode,"CIRCLE") == 0 ){
				kmb::DataBindings::const_iterator nIter = data->begin();
				while( !nIter.isFinished() ){
					if( mesh->getNodeXYZ( nIter.getId(), x, y, z ) ){
						drawCircleMark( x, y, z );
					}
					++nIter;
				}
			}
			else if( strcmp(mode,"CROSS") == 0 ){
				kmb::DataBindings::const_iterator nIter = data->begin();
				while( !nIter.isFinished() ){
					if( mesh->getNodeXYZ( nIter.getId(), x, y, z ) ){
						drawCrossMark( x, y, z );
					}
					++nIter;
				}
			}
			else if( strcmp(mode,"DELTA") == 0 ){
				kmb::DataBindings::const_iterator nIter = data->begin();
				while( !nIter.isFinished() ){
					if( mesh->getNodeXYZ( nIter.getId(), x, y, z ) ){
						drawDeltaMark( x, y, z );
					}
					++nIter;
				}
			}
			else if( strcmp(mode,"SQUARE") == 0 ){
				kmb::DataBindings::const_iterator nIter = data->begin();
				while( !nIter.isFinished() ){
					if( mesh->getNodeXYZ( nIter.getId(), x, y, z ) ){
						drawSquareMark( x, y, z );
					}
					++nIter;
				}
			}
			else if( strcmp(mode,"LEFT") == 0 ){
				kmb::DataBindings::const_iterator nIter = data->begin();
				while( !nIter.isFinished() ){
					if( mesh->getNodeXYZ( nIter.getId(), x, y, z ) ){
						drawLeftMark( x, y, z );
					}
					++nIter;
				}
			}
			else if( strcmp(mode,"RIGHT") == 0 ){
				kmb::DataBindings::const_iterator nIter = data->begin();
				while( !nIter.isFinished() ){
					if( mesh->getNodeXYZ( nIter.getId(), x, y, z ) ){
						drawRightMark( x, y, z );
					}
					++nIter;
				}
			}
			else if( strcmp(mode,"NUMERIC") == 0 ){
				switch( data->getValueType() )
				{
				case kmb::PhysicalValue::Integer:
				case kmb::PhysicalValue::NodeId:
				case kmb::PhysicalValue::ElementId:
					{
						kmb::DataBindings::const_iterator nIter = data->begin();
						while( !nIter.isFinished() ){
							long l = 0;
							nIter.getValue(&l);
							if( mesh->getNodeXYZ( nIter.getId(), x, y, z ) ){
								drawNumericMark( x, y, z, l );
							}
							++nIter;
						}
					}
					break;
				default:
					break;
				}
			}
			else if( strcmp(mode,"ID") == 0 ){
				kmb::DataBindings::const_iterator nIter = data->begin();
				while( !nIter.isFinished() ){
					long l = nIter.getId() + this->nodeOffset;
					if( mesh->getNodeXYZ( nIter.getId(), x, y, z ) ){
						drawNumericMark( x, y, z, l );
					}
					++nIter;
				}
			}
		}
		else if( data->getBindingMode() == kmb::DataBindings::FaceGroup ||
				 data->getBindingMode() == kmb::DataBindings::FaceVariable ){
			kmb::bodyIdType bodyId = data->getTargetBodyId();
			std::set<kmb::nodeIdType> nodeSet;
			kmb::Face f;
			kmb::DataBindings::const_iterator fIter = data->begin();
			if( strcmp(mode,"CDOT") == 0 ){
				while( !fIter.isFinished() ){
					if( fIter.getFace(f) ){
						kmb::ElementContainer::const_iterator element = mesh->findElement( f.getElementId(), bodyId );
						if( !element.isFinished() ){
							const int localId = f.getLocalFaceId();
							const int faceNodeCount = element.getBoundaryNodeCount( localId );
							kmb::Point3D center;
							for(int i=0;i<faceNodeCount;++i)
							{
								if( mesh->getNodeXYZ( element.getBoundaryCellId( localId, i ), x, y, z ) ){
									center.addCoordinate( x, y, z );
								}
							}
							drawDotMark( center.x() / faceNodeCount, center.y() / faceNodeCount, center.z() / faceNodeCount );
						}
					}
					++fIter;
				}
			}else if( strcmp(mode,"CCROSS") == 0 ){
				while( !fIter.isFinished() ){
					if( fIter.getFace(f) ){
						kmb::ElementContainer::const_iterator element = mesh->findElement( f.getElementId(), bodyId );
						if( !element.isFinished() ){
							const int localId = f.getLocalFaceId();
							const int faceNodeCount = element.getBoundaryNodeCount( localId );
							kmb::Point3D center;
							for(int i=0;i<faceNodeCount;++i)
							{
								if( mesh->getNodeXYZ( element.getBoundaryCellId( localId, i ), x, y, z ) ){
									center.addCoordinate( x, y, z );
								}
							}
							drawCrossMark( center.x() / faceNodeCount, center.y() / faceNodeCount, center.z() / faceNodeCount );
						}
					}
					++fIter;
				}
			}else if( strcmp(mode,"CTENSOR") == 0 ){
				while( !fIter.isFinished() ){
					if( fIter.getFace(f) ){
						kmb::ElementContainer::const_iterator element = mesh->findElement( f.getElementId(), bodyId );
						if( !element.isFinished() ){
							const int localId = f.getLocalFaceId();
							const int faceNodeCount = element.getBoundaryNodeCount( localId );
							kmb::Point3D center;
							for(int i=0;i<faceNodeCount;++i)
							{
								if( mesh->getNodeXYZ( element.getBoundaryCellId( localId, i ), x, y, z ) ){
									center.addCoordinate( x, y, z );
								}
							}
							drawTensorMark( center.x() / faceNodeCount, center.y() / faceNodeCount, center.z() / faceNodeCount );
						}
					}
					++fIter;
				}
			}else if( strcmp(mode,"CSQUARE") == 0 ){
				while( !fIter.isFinished() ){
					if( fIter.getFace(f) ){
						kmb::ElementContainer::const_iterator element = mesh->findElement( f.getElementId(), bodyId );
						if( !element.isFinished() ){
							const int localId = f.getLocalFaceId();
							const int faceNodeCount = element.getBoundaryNodeCount( localId );
							kmb::Point3D center;
							for(int i=0;i<faceNodeCount;++i)
							{
								if( mesh->getNodeXYZ( element.getBoundaryCellId( localId, i ), x, y, z ) ){
									center.addCoordinate( x, y, z );
								}
							}
							drawSquareMark( center.x() / faceNodeCount, center.y() / faceNodeCount, center.z() / faceNodeCount );
						}
					}
					++fIter;
				}
			}else if( strcmp(mode,"EDGE") == 0 ){
				while( !fIter.isFinished() ){
					if( fIter.getFace(f) ){
						kmb::ElementContainer::const_iterator element = mesh->findElement( f.getElementId(), bodyId );
						if( !element.isFinished() ){
							const int localId = f.getLocalFaceId();
							drawFaceEdge( element, localId );
						}
					}
					++fIter;
				}
			}else{
				while( !fIter.isFinished() ){
					if( fIter.getFace(f) ){
						kmb::ElementContainer::const_iterator element = mesh->findElement( f.getElementId(), bodyId );
						if( !element.isFinished() ){
							const int localId = f.getLocalFaceId();
							const int faceNodeCount = element.getBoundaryNodeCount( localId );
							for(int i=0;i<faceNodeCount;++i)
							{
								nodeSet.insert( element.getBoundaryCellId( localId, i ) );
							}
						}
					}
					++fIter;
				}
				if( strcmp(mode,"TENSOR") == 0 ){
					std::set< kmb::nodeIdType >::iterator nIter = nodeSet.begin();
					while( nIter != nodeSet.end() )
					{
						if( mesh->getNodeXYZ( *nIter, x, y, z ) ){
							drawTensorMark( x, y, z );
						}
						++nIter;
					}
				}
				else if( strcmp(mode,"DOT") == 0 ){
					std::set< kmb::nodeIdType >::iterator nIter = nodeSet.begin();
					while( nIter != nodeSet.end() )
					{
						if( mesh->getNodeXYZ( *nIter, x, y, z ) ){
							drawDotMark( x, y, z );
						}
						++nIter;
					}
				}
				else if( strcmp(mode,"CROSS") == 0 ){
					std::set< kmb::nodeIdType >::iterator nIter = nodeSet.begin();
					while( nIter != nodeSet.end() )
					{
						if( mesh->getNodeXYZ( *nIter, x, y, z ) ){
							drawCrossMark( x, y, z );
						}
						++nIter;
					}
				}
				else if( strcmp(mode,"VECTOR") == 0 ){

					if( data->getValueType() == kmb::PhysicalValue::Vector3 ){
						const kmb::Vector3Value* vData =
							reinterpret_cast<const kmb::Vector3Value*>(data->getPhysicalValue());
						if( vData ){
							double v0 = vData->getValue(0);
							double v1 = vData->getValue(1);
							double v2 = vData->getValue(2);
							std::set< kmb::nodeIdType >::iterator nIter = nodeSet.begin();
							while( nIter != nodeSet.end() )
							{
								if( mesh->getNodeXYZ( *nIter, x, y, z ) ){
									drawVectorMark( x, y, z, v0, v1, v2 );
								}
								++nIter;
							}
						}
					}
				}
			}
		}
		else if( data->getBindingMode() == kmb::DataBindings::ElementGroup ){
			kmb::bodyIdType bodyId = data->getTargetBodyId();
			kmb::DataBindings::const_iterator eIter = data->begin();
			if( strcmp(mode,"CDOT") == 0 ){
				while( !eIter.isFinished() ){
					kmb::ElementContainer::const_iterator element = mesh->findElement( eIter.getId(), bodyId );
					if( !element.isFinished() ){
						const int nodeCount = element.getNodeCount();
						kmb::Point3D center;
						for(int i=0;i<nodeCount;++i)
						{
							if( mesh->getNodeXYZ( element.getCellId( i ), x, y, z ) ){
								center.addCoordinate( x, y, z );
							}
						}
						center.scale( 1.0/nodeCount );
						drawDotMark( center.x(), center.y(), center.z() );
					}
					++eIter;
				}
			}
			else if( strcmp(mode,"EDGE") == 0 ){
				while( !eIter.isFinished() ){
					kmb::ElementContainer::const_iterator element = mesh->findElement( eIter.getId(), bodyId );
					if( !element.isFinished() ){
						drawElementEdge( element );
					}
					++eIter;
				}
			}
			else if( strcmp(mode,"ID") == 0 ){
				while( !eIter.isFinished() ){
					kmb::ElementContainer::const_iterator element = mesh->findElement( eIter.getId(), bodyId );
					if( !element.isFinished() ){
						const int nodeCount = element.getNodeCount();
						kmb::Point3D center;
						for(int i=0;i<nodeCount;++i)
						{
							if( mesh->getNodeXYZ( element.getCellId( i ), x, y, z ) ){
								center.addCoordinate( x, y, z );
							}
						}
						center.scale( 1.0/nodeCount );
						long l = eIter.getId() + this->elementOffset;
						drawNumericMark( center.x(), center.y(), center.z(), l );
					}
					++eIter;
				}
			}
		}
	}
}

void
kmb::MeshGL::drawMarkingEdgeWithScale(kmb::bodyIdType bodyId,const char* mode,double x,double y,double z,double scale)
{
	const kmb::Body* edge = NULL;
	if( mesh != NULL &&
		(edge = mesh->getBodyPtr(bodyId)) != NULL &&
		edge->isUniqueDim(1) )
	{
		double x0=0.0,y0=0.0,z0=0.0;
		if( strcmp(mode,"TENSOR") == 0 ){
			kmb::ElementContainer::const_iterator eIter = edge->begin();
			while( !eIter.isFinished() ){
				if( mesh->getNodeXYZ( eIter.getCellId(0), x0, y0, z0 ) ){
					drawTensorMark(
						scale * ( x0 - x ) + x,
						scale * ( y0 - y ) + y,
						scale * ( z0 - z ) + z);
				}
				++eIter;
			}
		}
		else if( strcmp(mode,"DAGGER") == 0 ){
			kmb::ElementContainer::const_iterator eIter = edge->begin();
			while( eIter != edge->end() ){
				if( mesh->getNodeXYZ( eIter.getCellId(0), x0, y0, z0 ) ){
					drawDaggerMark(
						scale * ( x0 - x ) + x,
						scale * ( y0 - y ) + y,
						scale * ( z0 - z ) + z);
				}
				++eIter;
			}
		}
		else if( strcmp(mode,"CIRCLE") == 0 ){
			kmb::ElementContainer::const_iterator eIter = edge->begin();
			while( eIter != edge->end() ){
				if( mesh->getNodeXYZ( eIter.getCellId(0), x0, y0, z0 ) ){
					drawCircleMark(
						scale * ( x0 - x ) + x,
						scale * ( y0 - y ) + y,
						scale * ( z0 - z ) + z);
				}
				++eIter;
			}
		}
		else if( strcmp(mode,"CROSS") == 0 || strcmp(mode,"CCROSS") == 0){
			kmb::ElementContainer::const_iterator eIter = edge->begin();
			while( eIter != edge->end() ){
				if( mesh->getNodeXYZ( eIter.getCellId(0), x0, y0, z0 ) ){
					drawCrossMark(
						scale * ( x0 - x ) + x,
						scale * ( y0 - y ) + y,
						scale * ( z0 - z ) + z);
				}
				++eIter;
			}
		}
		else if( strcmp(mode,"DOT") == 0 || strcmp(mode,"CDOT") == 0){
			kmb::ElementContainer::const_iterator eIter = edge->begin();
			while( eIter != edge->end() ){
				if( mesh->getNodeXYZ( eIter.getCellId(0), x0, y0, z0 ) ){
					drawDotMark(
						scale * ( x0 - x ) + x,
						scale * ( y0 - y ) + y,
						scale * ( z0 - z ) + z);
				}
				++eIter;
			}
		}
	}
}

void
kmb::MeshGL::drawTensorMark(kmb::Point3D& point)
{
	drawTensorMark( point.x(), point.y(), point.z() );
}

void
kmb::MeshGL::drawTensorMark(double x,double y,double z)
{
	double c = 0.5;
	double s = 0.707;
	::glBegin( GL_TRIANGLE_STRIP );
	::glVertex3d( x + markSize,   y,              z );
	::glVertex3d( x - c*markSize, y + s*markSize, z );
	::glVertex3d( x - c*markSize, y - s*markSize, z );
	::glVertex3d( x,              y,              z + markSize );
	::glVertex3d( x + markSize,   y,              z );
	::glVertex3d( x - c*markSize, y + s*markSize, z );
	::glEnd();
	::glFlush();
}

double
kmb::MeshGL::calcPerpendicular(double v0,double v1,double v2,kmb::Vector3D &normal)
{
	double mag = v0*v0 + v1*v1 + v2*v2;


	if( v2 == 0.0 || mag == 0.0 ){
		normal.setCoordinate(0.0,0.0,1.0);
	}else if( v2*v2 / mag <= 0.5 ){
		normal.setCoordinate(-v0*v2,-v1*v2,1.0-v2*v2);
		normal.normalize();
	}else{
		normal.setCoordinate(1.0-v0*v0,-v0*v1,-v0*v2);
		normal.normalize();
	}
	return mag;
}

double kmb::MeshGL::calcFrame(double v0,double v1,double v2,kmb::Vector3D &normal0,kmb::Vector3D &normal1)
{
	double mag = v0*v0 + v1*v1 + v2*v2;


	if( v2 == 0.0 || mag == 0.0 ){
		normal0.setCoordinate(0.0,0.0,1.0);
		normal1.setCoordinate(0.0,-1.0,0.0);
	}else if( v2*v2 / mag <= 0.5 ){
		normal0.setCoordinate(-v0*v2,-v1*v2,mag-v2*v2);
		normal0.normalize();
	}else{
		normal0.setCoordinate(mag-v0*v0,-v0*v1,-v0*v2);
		normal0.normalize();
	}
	normal1.setCoordinate(
		v1*normal0.z() - v2*normal0.y(),
		v2*normal0.x() - v0*normal0.z(),
		v0*normal0.y() - v1*normal0.x()
	);
	normal1.normalize();
	return mag;
}

void
kmb::MeshGL::drawVectorMark(kmb::Point3D& point,kmb::Vector3D& vec)
{
	drawVectorMark( point.x(), point.y(), point.z(), vec.x(), vec.y(), vec.z() );
}

void
kmb::MeshGL::drawVectorMark(double x,double y,double z,double v0,double v1,double v2)
{
	Vector3D normal;
	double mag = this->calcPerpendicular(v0,v1,v2,normal);
	if( mag <= 0.0 ){
		drawDeltaMark( x, y, z );
	}else{

		double r = 1.0 / sqrt(mag);

		::glBegin( GL_LINE_STRIP );
		::glVertex3d(
			x - v0*markSize*r + normal.x()*markSize*0.6,
			y - v1*markSize*r + normal.y()*markSize*0.6,
			z - v2*markSize*r + normal.z()*markSize*0.6);
		::glVertex3d( x, y, z );
		::glVertex3d(
			x - v0*markSize*r - normal.x()*markSize*0.6,
			y - v1*markSize*r - normal.y()*markSize*0.6,
			z - v2*markSize*r - normal.z()*markSize*0.6);
		::glEnd();

		::glBegin( GL_LINES );
		::glVertex3d( x, y, z );
		::glVertex3d( x - v0 , y - v1, z - v2 );
		::glEnd();
		::glFlush();
	}
}

void
kmb::MeshGL::drawCrossMark(kmb::Point3D& point)
{
	drawCrossMark( point.x(), point.y(), point.z() );
}

void
kmb::MeshGL::drawCrossMark(double x,double y,double z)
{
	double d = markSize * 0.5;
	::glBegin( GL_QUADS );
	::glVertex3d( x - d, y + d, z - d );
	::glVertex3d( x - d, y - d, z - d );
	::glVertex3d( x + d, y - d, z + d );
	::glVertex3d( x + d, y + d, z + d );
	::glVertex3d( x - d, y + d, z + d );
	::glVertex3d( x - d, y - d, z + d );
	::glVertex3d( x + d, y - d, z - d );
	::glVertex3d( x + d, y + d, z - d );
	::glEnd();
	::glFlush();
}

void
kmb::MeshGL::drawDotMark(kmb::Point3D& point)
{
	drawDotMark( point.x(), point.y(), point.z() );
}

void
kmb::MeshGL::drawDotMark(double x,double y,double z)
{
	::glPointSize( this->pointSize );
	::glBegin( GL_POINTS );
	::glVertex3d( x, y, z );
	::glEnd();
	::glFlush();
}

const GLubyte dagger[] =
{
 0x18, 0x00, 0x00, 0x00,
 0x18, 0x00, 0x00, 0x00,
 0x18, 0x00, 0x00, 0x00,
 0x18, 0x00, 0x00, 0x00,
 0x18, 0x00, 0x00, 0x00,
 0x7e, 0x00, 0x00, 0x00,
 0xff, 0x00, 0x00, 0x00,
 0x99, 0x00, 0x00, 0x00,
 0x18, 0x00, 0x00, 0x00,
 0x18, 0x00, 0x00, 0x00,
};

void
kmb::MeshGL::drawDaggerMark(double x,double y,double z)
{
	::glRasterPos3d( x, y, z );
	::glBitmap( 8, 10, 4.0f, 4.0f, 0.0f, 0.0f, dagger );
}

const GLuint circle[] = { 0x3c, 0x66, 0xc3, 0xc3, 0xc3, 0x66, 0x3c };

void
kmb::MeshGL::drawCircleMark(double x,double y,double z)
{
	::glRasterPos3d( x, y, z );
	::glBitmap( 8, 7, 4.0f, 3.5f, 0.0f, 0.0f, reinterpret_cast<const GLubyte*>(circle) );
}


const GLubyte delta[] =
{
 0xff, 0x80, 0x00, 0x00,
 0x7f, 0x00, 0x00, 0x00,
 0x7f, 0x00, 0x00, 0x00,
 0x3e, 0x00, 0x00, 0x00,
 0x3e, 0x00, 0x00, 0x00,
 0x1c, 0x00, 0x00, 0x00,
 0x08, 0x00, 0x00, 0x00,
};

void
kmb::MeshGL::drawDeltaMark(double x,double y,double z)
{
	::glRasterPos3d( x, y, z );
	::glBitmap( 9, 7, 4.5f, 3.5f, 0.0f, 0.0f, delta );
}


const GLubyte square[] =
{
 0x7e, 0x00, 0x00, 0x00,
 0x7e, 0x00, 0x00, 0x00,
 0x7e, 0x00, 0x00, 0x00,
 0x7e, 0x00, 0x00, 0x00,
 0x7e, 0x00, 0x00, 0x00,
 0x7e, 0x00, 0x00, 0x00,
};

void
kmb::MeshGL::drawSquareMark(double x,double y,double z)
{
	::glRasterPos3d( x, y, z );
	::glBitmap( 8, 6, 4.0f, 3.0f, 0.0f, 0.0f, square );
}


const GLubyte left[] =
{
 0x03, 0x00, 0x00, 0x00,
 0x0c, 0x00, 0x00, 0x00,
 0x30, 0x00, 0x00, 0x00,
 0xc0, 0x00, 0x00, 0x00,
 0x30, 0x00, 0x00, 0x00,
 0x0c, 0x00, 0x00, 0x00,
 0x03, 0x00, 0x00, 0x00,
};

void
kmb::MeshGL::drawLeftMark(double x,double y,double z)
{
	::glRasterPos3d( x, y, z );
	::glBitmap( 8, 7, 4.0f, 3.5f, 0.0f, 0.0f, left );
}


const GLubyte right[] =
{
 0xc0, 0x00, 0x00, 0x00,
 0x30, 0x00, 0x00, 0x00,
 0x0c, 0x00, 0x00, 0x00,
 0x03, 0x00, 0x00, 0x00,
 0x0c, 0x00, 0x00, 0x00,
 0x30, 0x00, 0x00, 0x00,
 0xc0, 0x00, 0x00, 0x00,
};

void
kmb::MeshGL::drawRightMark(double x,double y,double z)
{
	::glRasterPos3d( x, y, z );
	::glBitmap( 8, 7, 4.0f, 3.5f, 0.0f, 0.0f, right );
}

/* 8 x 16 */
const GLuint font[][16] = {

	{ 0x00, 0x00, 0x00, 0x08, 0x49, 0x49, 0x2a, 0x08, 0x2a, 0x49, 0x49, 0x08, 0x00, 0x00, 0x00, 0x00,},

	{ 0x00, 0x00, 0x00, 0x08, 0x08, 0x08, 0x08, 0x7f, 0x08, 0x08, 0x08, 0x08, 0x00, 0x00, 0x00, 0x00,},

	{ 0x10, 0x08, 0x14, 0x38, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,},

	{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3e, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,},

	{ 0x00, 0x00, 0x10, 0x38, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,},

	{ 0x00, 0x00, 0x40, 0x40, 0x20, 0x20, 0x10, 0x10, 0x08, 0x08, 0x04, 0x04, 0x02, 0x02, 0x00, 0x00,},

	{ 0x00, 0x00, 0x3e, 0x41, 0x41, 0x43, 0x45, 0x49, 0x51, 0x61, 0x41, 0x41, 0x41, 0x3e, 0x00, 0x00,},

	{ 0x00, 0x00, 0x1c, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x28, 0x18, 0x08, 0x08, 0x00, 0x00,},

	{ 0x00, 0x00, 0x7f, 0x40, 0x40, 0x20, 0x10, 0x08, 0x04, 0x02, 0x41, 0x41, 0x22, 0x1c, 0x00, 0x00,},

	{ 0x00, 0x00, 0x1c, 0x22, 0x41, 0x41, 0x01, 0x02, 0x1c, 0x02, 0x01, 0x41, 0x22, 0x1c, 0x00, 0x00,},

	{ 0x00, 0x00, 0x04, 0x04, 0x04, 0x7f, 0x44, 0x44, 0x44, 0x44, 0x24, 0x14, 0x0c, 0x04, 0x00, 0x00,},

	{ 0x00, 0x00, 0x1c, 0x22, 0x41, 0x01, 0x01, 0x41, 0x62, 0x5c, 0x40, 0x40, 0x40, 0x7e, 0x00, 0x00,},

	{ 0x00, 0x00, 0x1c, 0x22, 0x41, 0x41, 0x41, 0x62, 0x5c, 0x40, 0x40, 0x20, 0x10, 0x0c, 0x00, 0x00,},

	{ 0x00, 0x00, 0x08, 0x08, 0x08, 0x08, 0x08, 0x04, 0x04, 0x04, 0x02, 0x02, 0x41, 0x7f, 0x00, 0x00,},

	{ 0x00, 0x00, 0x1c, 0x22, 0x41, 0x41, 0x41, 0x22, 0x1c, 0x22, 0x22, 0x22, 0x22, 0x1c, 0x00, 0x00,},

	{ 0x00, 0x00, 0x1e, 0x21, 0x01, 0x01, 0x01, 0x1d, 0x23, 0x41, 0x41, 0x41, 0x22, 0x1c, 0x00, 0x00,},
};

void
kmb::MeshGL::drawNumericMark(double x,double y,double z,long l)
{
	char buf[32];
	sprintf(buf,"%ld",l);
	::glRasterPos3d( x, y, z );
	int i = 0;
	while( buf[i] != '\0' && i < 32 ){
		int n = static_cast<int>( buf[i] ) - 0x2a;
		glBitmap(8, 16, 0.0f, 0.0f, 9.0f, 0.0f, reinterpret_cast<const GLubyte*>(font[n]) );
		++i;
	}
}

void
kmb::MeshGL::drawVector(double x,double y,double z,double v0,double v1,double v2,double factor,kmb::ColorMap* colorMap)
{
	Vector3D normal;
	double mag = this->calcPerpendicular(v0,v1,v2,normal);
	double d = sqrt( mag );
	float rgb[3] = {0.0,0.0,0.0};
	colorMap->getRGB( d, rgb );
	::glColor3fv( rgb );

	::glBegin( GL_LINE_STRIP );
	::glVertex3d(
			x+factor*(v0*0.75+normal.x()*d*0.1),
			y+factor*(v1*0.75+normal.y()*d*0.1),
			z+factor*(v2*0.75+normal.z()*d*0.1));
	::glVertex3d( x+factor*v0, y+factor*v1, z+factor*v2 );
	::glVertex3d(
			x+factor*(v0*0.75-normal.x()*d*0.1),
			y+factor*(v1*0.75-normal.y()*d*0.1),
			z+factor*(v2*0.75-normal.z()*d*0.1));
	::glEnd();

	::glBegin( GL_LINES );
	::glVertex3d( x, y, z );
	::glVertex3d( x+factor*v0, y+factor*v1, z+factor*v2 );
	::glEnd();
}

void
kmb::MeshGL::drawArrow(double x,double y,double z,double v0,double v1,double v2,double arrowSize,kmb::ColorMap* colorMap)
{
	Vector3D normal0,normal1;
	double mag = this->calcFrame(v0,v1,v2,normal0,normal1);
	if( mag < arrowSize * 1.0e-6 ){
		return;
	}
	double d = sqrt( mag );
	float rgb[3] = {0.0,0.0,0.0};
	colorMap->getRGB( d, rgb );
	::glColor3fv( rgb );
	v0 /= d;
	v1 /= d;
	v2 /= d;
	::glBegin( GL_TRIANGLES );
	::glVertex3d(
			x+arrowSize*(v0*0.5),
			y+arrowSize*(v1*0.5),
			z+arrowSize*(v2*0.5));
	::glVertex3d(
			x+arrowSize*(-v0*0.5+0.125*normal0.x()),
			y+arrowSize*(-v1*0.5+0.125*normal0.y()),
			z+arrowSize*(-v2*0.5+0.125*normal0.z()));
	::glVertex3d(
			x+arrowSize*(-v0*0.5-0.125*normal0.x()),
			y+arrowSize*(-v1*0.5-0.125*normal0.y()),
			z+arrowSize*(-v2*0.5-0.125*normal0.z()));
	::glVertex3d(
			x+arrowSize*(v0*0.5),
			y+arrowSize*(v1*0.5),
			z+arrowSize*(v2*0.5));
	::glVertex3d(
			x+arrowSize*(-v0*0.5+0.125*normal1.x()),
			y+arrowSize*(-v1*0.5+0.125*normal1.y()),
			z+arrowSize*(-v2*0.5+0.125*normal1.z()));
	::glVertex3d(
			x+arrowSize*(-v0*0.5-0.125*normal1.x()),
			y+arrowSize*(-v1*0.5-0.125*normal1.y()),
			z+arrowSize*(-v2*0.5-0.125*normal1.z()));
	::glEnd();
}

void
kmb::MeshGL::drawElementEdge(kmb::ElementBase& element)
{
	const int len = element.getEdgeCount();
	::glBegin( GL_LINES );
	double x0=0.0, y0=0.0, z0=0.0, x1=0.0, y1=0.0, z1=0.0;
	for(int i=0;i<len;++i){
		mesh->getNodeXYZ( element.getEdgeCellId(i,0), x0, y0, z0 );
		mesh->getNodeXYZ( element.getEdgeCellId(i,1), x1, y1, z1 );
		::glVertex3d( x0, y0, z0 );
		::glVertex3d( x1, y1, z1 );
	}
	::glEnd();
	::glFlush();
}

void
kmb::MeshGL::drawFaceEdge(kmb::ElementBase& element,int index)
{
	const int len = element.getBoundaryVertexCount(index);
	switch(len){
	case 2:
		::glBegin( GL_LINES );
		break;
	case 3:
	case 4:
		::glBegin( GL_LINE_LOOP );
		break;
	default:
		return;
	}
	double x0=0.0, y0=0.0, z0=0.0;
	for(int i=0;i<len;++i){
		mesh->getNodeXYZ( element.getBoundaryCellId(index,i), x0, y0, z0 );
		::glVertex3d( x0, y0, z0 );
	}
	::glEnd();
	::glFlush();
}
