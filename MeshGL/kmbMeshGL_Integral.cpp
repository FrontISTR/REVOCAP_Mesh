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
#include "MeshGL/kmbColorMap.h"
#include "MeshDB/kmbTetrahedron.h"
#include "Geometry/kmbPlane.h"

#include <cmath>

//
// 積分曲線（オイラー法）
//

int
kmb::MeshGL::drawTrajectory(const char* vectorVal,double x,double y,double z,double delta,int step, kmb::ColorMap* colorMap, double tolerance)
{
	if( mesh == NULL ){
		return -1;
	}
	int count = 0;
	double v[3] = {0.0, 0.0, 0.0};
	::glPushAttrib( GL_ENABLE_BIT );
	::glEnable( GL_COLOR_MATERIAL );
	::glBegin( GL_LINE_STRIP );
	kmb::BoundingBox bbox = mesh->getBoundingBox();
	bbox.expand(1.1);
	if( prefMode == kmb::MeshGL::SPEED ){
		for(count=0;count<step;++count){
			if( bbox.intersect(x,y,z) == kmb::Region::OUTSIDE ){
				break;
			}
			// node cache を作っておく必要がある
			int res = mesh->getValueOnNearestNode(vectorVal,x,y,z,v);
			if( res != -1 ){
				if( colorMap ){
					colorMap->setGLColor( kmb::Vector3D::abs(v) );
				}
				::glVertex3d(x,y,z);
				x += delta*v[0];
				y += delta*v[1];
				z += delta*v[2];
			}else{
				break;
			}
		}
	}else{
		for(count=0;count<step;++count){
			int res = mesh->getInterpolatedValue(vectorVal,x,y,z,v);
			if( res != -1 ){
				if( colorMap ){
					colorMap->setGLColor( kmb::Vector3D::abs(v) );
				}
				::glVertex3d(x,y,z);
				x += delta*v[0];
				y += delta*v[1];
				z += delta*v[2];
			}else{
				break;
			}
		}
	}
	::glEnd();
	::glPopAttrib();
	::glFlush();
	return count;
}
