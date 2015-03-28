/*----------------------------------------------------------------------
#                                                                      #
# Software Name : REVOCAP_PrePost version 1.6                          #
# Class Name : Point3DStructured                                       #
#                                                                      #
#                                Written by                            #
#                                           K. Tokunaga 2012/03/23     #
#                                                                      #
#      Copyright (C) 2012 Advancesoft corp.                            #
#                                                                      #
----------------------------------------------------------------------*/

#include "Geometry/kmbPoint3DStructured.h"

#include "Common/kmbCommon.h"
#include "Geometry/kmbBoxRegion.h"
#include "Geometry/kmbPoint3DContainerArray.h"

std::string kmb::Point3DStructured::getFieldTypeString( kmb::Point3DStructured::fieldType ftype )
{
	switch(ftype){
	case kmb::Point3DStructured::kUniform:
		return "Uniform";
	case kmb::Point3DStructured::kRectLinear:
		return "RectLinear";
	case kmb::Point3DStructured::kIrregular:
		return "Irregular";
	case kmb::Point3DStructured::kUnknown:
		return "Unknown";
	default:
		break;
	}
	return "";
}

kmb::Point3DStructured::Point3DStructured(void)
  : kmb::StructuredBase<>(0,0,0), ftype(kmb::Point3DStructured::kUnknown)
{
}

kmb::Point3DStructured::Point3DStructured(int _nx,int _ny,int _nz,kmb::Point3DStructured::fieldType _ftype)
  : kmb::StructuredBase<>(_nx,_ny,_nz), ftype(_ftype)
{
}

kmb::Point3DStructured::~Point3DStructured(void)
{
}

void kmb::Point3DStructured::initialize(int _nx,int _ny,int _nz)
{
	nx = _nx;
	ny = _ny;
	nz = _nz;
}

kmb::Point3DStructured::fieldType kmb::Point3DStructured::getFieldType(void) const
{
	return ftype;
}
/*
int kmb::Point3DStructured::getNodeCount(void) const
{
	return nx * ny * nz;
}

int kmb::Point3DStructured::getSize(int n) const
{
	switch(n){
	case 0:
		return nx;
	case 1:
		return ny;
	case 2:
		return nz;
	}
	return 0;
}
bool kmb::Point3DStructured::valid(int i,int j,int k) const
{
	return (0 <= i) && (i < nx) && (0 <= j) && (j < ny) && (0 <= k) && (k < nz);
}
int kmb::Point3DStructured::getId(int i,int j,int k) const
{
	return (i*(ny) + j)*(nz) + k;
}

bool kmb::Point3DStructured::getIndices(int index,int &i,int &j,int &k) const
{
	if( index < 0 || index >= nx*ny*nz ){
		return false;
	}
	i = index / (ny*nz);
	int q = index - i * ny * nz;
	j = q / nz;
	k = q - j * nz;
	return true;
}
*/
/************* Uniform *******************/

kmb::Point3DUniformStructured::Point3DUniformStructured(int _nx,int _ny,int _nz)
	: kmb::Point3DStructured(_nx,_ny,_nz,kmb::Point3DStructured::kUniform), box(new BoxRegion(0.0,0.0,0.0,1.0,1.0,1.0))
{
}

kmb::Point3DUniformStructured::Point3DUniformStructured(double minx,double maxx,double miny,double maxy,double minz,double maxz,int _nx,int _ny,int _nz)
	: kmb::Point3DStructured(_nx,_ny,_nz,kmb::Point3DStructured::kUniform), box(new BoxRegion(minx,miny,minz,maxx,maxy,maxz))
{
}

kmb::Point3DUniformStructured::Point3DUniformStructured(kmb::BoxRegion &_box,int _nx,int _ny,int _nz)
	: kmb::Point3DStructured(_nx,_ny,_nz,kmb::Point3DStructured::kUniform), box(new BoxRegion(_box))
{
}

kmb::Point3DUniformStructured::~Point3DUniformStructured(void)
{
	if(box){
		delete box;
		box = NULL;
	}
}

void kmb::Point3DUniformStructured::setRegion(double minx,double miny,double minz,double maxx,double maxy,double maxz)
{
	if( box == NULL ){
		box = new BoxRegion(minx,miny,minz,maxx,maxy,maxz);
	}else{
		box->setMinMax(minx,miny,minz,maxx,maxy,maxz);
	}
}

void kmb::Point3DUniformStructured::setRegion(kmb::BoxRegion &_box)
{
	if( box == NULL ){
		box = new BoxRegion(_box);
	}else{
		*box = _box;
	}
}

const kmb::BoxRegion* kmb::Point3DUniformStructured::getRegion() const
{
	return box;
}

kmb::BoxRegion* kmb::Point3DUniformStructured::getRegion()
{
	return box;
}

bool kmb::Point3DUniformStructured::getPoint(int i,int j,int k,kmb::Point3D &pt) const
{
	if( box == NULL || !valid(i,j,k) ){
		return false;
	}
	pt.setCoordinate(
		box->minX() + i * box->rangeX() / (nx-1),
		box->minY() + j * box->rangeY() / (ny-1),
		box->minZ() + k * box->rangeZ() / (nz-1) );
	return true;
}

void kmb::Point3DUniformStructured::translate(double x,double y,double z)
{
	box->translate(x,y,z);
}

/************* RectLinear *******************/

kmb::Point3DRectLinearStructured::Point3DRectLinearStructured(int _nx,int _ny,int _nz)
	: kmb::Point3DStructured(_nx,_ny,_nz,kmb::Point3DStructured::kRectLinear), xAxis(new double[_nx]), yAxis(new double[_ny]), zAxis(new double[_nz])
{
}

kmb::Point3DRectLinearStructured::~Point3DRectLinearStructured(void)
{
	if( xAxis ){
		delete[] xAxis;
		xAxis = NULL;
	}
	if( yAxis ){
		delete[] yAxis;
		yAxis = NULL;
	}
	if( zAxis ){
		delete[] zAxis;
		zAxis = NULL;
	}
}

void kmb::Point3DRectLinearStructured::setXAxis(int i,double x)
{
	xAxis[i] = x;
}

void kmb::Point3DRectLinearStructured::setYAxis(int j,double y)
{
	yAxis[j] = y;
}

void kmb::Point3DRectLinearStructured::setZAxis(int k,double z)
{
	zAxis[k] = z;
}

bool kmb::Point3DRectLinearStructured::getPoint(int i,int j,int k,kmb::Point3D &pt) const
{
	if( xAxis == NULL || yAxis == NULL || zAxis == NULL || !valid(i,j,k) )
	{
		return false;
	}
	pt.setCoordinate( xAxis[i], yAxis[j], zAxis[k] );
	return true;
}

void kmb::Point3DRectLinearStructured::translate(double x,double y,double z)
{
	for(int i=0;i<nx;++i){
		xAxis[i] += x;
	}
	for(int j=0;j<ny;++j){
		yAxis[j] += y;
	}
	for(int k=0;k<nz;++k){
		zAxis[k] += z;
	}
}


/************* Irregular *******************/

kmb::Point3DIrregularStructured::Point3DIrregularStructured(int _nx,int _ny,int _nz)
	: kmb::Point3DStructured(_nx,_ny,_nz,kmb::Point3DStructured::kIrregular), points(NULL)
{
	points = new kmb::Point3DContainerArray();
	points->initialize(_nx*_ny*_nz);
}

kmb::Point3DIrregularStructured::~Point3DIrregularStructured(void)
{
	delete points;
}

bool kmb::Point3DIrregularStructured::getPoint(int i,int j,int k,kmb::Point3D &pt) const
{
	return points->getPoint(getId(i,j,k),pt);
}

bool kmb::Point3DIrregularStructured::setPoint(int i,int j,int k,kmb::Point3D &pt)
{
	return (points->addPoint(pt,getId(i,j,k)) != kmb::nullNodeId);
}

bool kmb::Point3DIrregularStructured::setPoint(int i,int j,int k,double x,double y,double z)
{
	return (points->addPoint(x,y,z,getId(i,j,k)) != kmb::nullNodeId);
}

bool kmb::Point3DIrregularStructured::getPoint(kmb::nodeIdType nodeId,kmb::Point3D &pt) const
{
	return points->getPoint(nodeId,pt);
}

bool kmb::Point3DIrregularStructured::setPoint(kmb::nodeIdType nodeId,double x,double y,double z)
{
	return (points->addPoint(x,y,z,nodeId) != kmb::nullNodeId);
}


void kmb::Point3DIrregularStructured::translate(double x,double y,double z)
{
	if( points == NULL ){
		return;
	}
	kmb::Point3DContainer::iterator pIter = points->begin();
	while( !pIter.isFinished() ){
		double x1 = pIter.x() + x;
		double y1 = pIter.y() + y;
		double z1 = pIter.z() + z;
		pIter.setXYZ(x1,y1,z1);
		++pIter;
	}
}
