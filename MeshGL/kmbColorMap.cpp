/*----------------------------------------------------------------------
#                                                                      #
# Software Name : REVOCAP_PrePost version 1.6                          #
# Class Name : ColorMap                                                #
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
#include "MeshGL/kmbColorMap.h"
#include "MeshDB/kmbMeshData.h"
#include "MeshDB/kmbDataBindings.h"
#include "MeshGL/kmbColor3fValueBindings.h"
#include "Geometry/kmbBoundingBox.h"

#include <iostream>

#ifdef WIN32
#include <windows.h>
#pragma comment(lib,"opengl32.lib")
#pragma comment(lib,"glu32.lib")
#endif

#ifdef __APPLE__
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#else
#include <GL/gl.h>
#include <GL/glu.h>
#endif

kmb::ColorMap::ColorMap(int s)
: minVal(0.0)
, maxVal(1.0)
, color(NULL)
, step(0)
, cache(NULL)
, cacheUpdate(true)
{
	setStep( s );
	shininess[0] = 32.0f;
}

kmb::ColorMap::~ColorMap(void)
{
	if( color ){
		delete[] color;
	}
	deleteColorCache();
}

void
kmb::ColorMap::setGLColor(double v)
{
	getRGB(v,rgb);
	::glColor3fv( rgb );
}

void
kmb::ColorMap::setGLColorByStep(int s)
{
	getRGBByStep(s,rgb);
	::glColor3fv( rgb );
}

void
kmb::ColorMap::calcRGB(double x,float rgb[3]) const
{
	/* HSV の色相を用いる */
	if(x <= 0.0f){
		rgb[0] = 0.0f;  // r
		rgb[1] = 0.0f;  // g
		rgb[2] = 1.0f;  // b
	}else if(x < 0.25f){
		rgb[0] = 0.0f;  // r
		rgb[1] = 4.0f * static_cast<float>( x ); //g
		rgb[2] = 1.0f;  // b
	}else if(x < 0.5f){
		rgb[0] = 0.0f;  // r
		rgb[1] = 1.0f;  // g
		rgb[2] = 4.0f * static_cast<float>( 0.5 - x );  // b
	}else if(x < 0.75f){
		rgb[0] = 4.0f * static_cast<float>( x - 0.5 );  // r
		rgb[1] = 1.0f;  // g
		rgb[2] = 0.0f;  // b
	}else if (x < 1.0f){
		rgb[0] = 1.0f;  // r
		rgb[1] = 4.0f * static_cast<float>( 1.0  - x );  // g
		rgb[2] = 0.0f;  // b
	}else{
		rgb[0] = 1.0f;  // r
		rgb[1] = 0.0f;  // g
		rgb[2] = 0.0f;  // b
	}
	/* HSV の色相を修正。関数を2次式にする */
/*
	if(x <= 0.0f){
		rgb[0] = 0.0f;  // r
		rgb[1] = 0.0f;  // g
		rgb[2] = 1.0f;  // b
	}else if(x < 0.25f){
		rgb[0] = 0.0f;  // r
		rgb[1] = 16.0f * static_cast<float>( x*(0.5-x) ); //g
		rgb[2] = 1.0f;  // b
	}else if(x < 0.5f){
		rgb[0] = 0.0f;  // r
		rgb[1] = 1.0f;  // g
		rgb[2] = 16.0f * static_cast<float>( x*(0.5-x) );  // b
	}else if(x < 0.75f){
		rgb[0] = 16.0f * static_cast<float>( (x-0.5)*(1.0-x) );  // r
		rgb[1] = 1.0f;  // g
		rgb[2] = 0.0f;  // b
	}else if (x < 1.0f){
		rgb[0] = 1.0f;  // r
		rgb[1] = 16.0f * static_cast<float>( (x-0.5)*(1.0-x) );  // g
		rgb[2] = 0.0f;  // b
	}else{
		rgb[0] = 1.0f;  // r
		rgb[1] = 0.0f;  // g
		rgb[2] = 0.0f;  // b
	}
*/
}

void
kmb::ColorMap::setStep(int s)
{
	if( s < 0 ){
		s = 0;
	}
	if( s != step ){
		this->step = s;
		if( color ){
			delete[] color;
			color = NULL;
		}
	}
	if( color == NULL && step > 0){
		float rgb[3];
		color = new float[3*step];
		for(int i=0;i<step;++i){
			float x = i / (step - 1.0f);
			calcRGB(x,rgb);
			color[3*i  ] = rgb[0];  // r
			color[3*i+1] = rgb[1];  // g
			color[3*i+2] = rgb[2];  // b
		}
	}
}

int
kmb::ColorMap::getStep(void) const
{
	return this->step;
}

// 今は rgba[3] を無視している
void
kmb::ColorMap::setRGBAByStep(int s,const float rgba[4])
{
	if( 0 <= s && s < step && color ){
		color[3*s  ] = rgba[0];
		color[3*s+1] = rgba[1];
		color[3*s+2] = rgba[2];
	}
}

double
kmb::ColorMap::getContourValue(int i) const
{
	if( i <= 0 ){
		return this->minVal;
	}else if( i >= this->step ){
		return this->maxVal;
	}else{
		return this->minVal + i * (this->maxVal-this->minVal) / this->step;
	}
}

void
kmb::ColorMap::setMinMax(double min,double max)
{
	this->minVal = min;
	this->maxVal = max;
}

void
kmb::ColorMap::getMinMax(double& min,double& max) const
{
	min = this->minVal;
	max = this->maxVal;
}

int
kmb::ColorMap::getStep(double x) const
{
	int ix;
	if(x <= minVal){
		ix = 0;
	}else if(x >= maxVal){
		ix = step - 1;
	}else{
		ix = static_cast<int>( (x - minVal) / (maxVal - minVal) * step );
	}
	return ix;
}

void
kmb::ColorMap::getRGB(double x,float rgb[3]) const
{
	if( this->step == 0 ){
		double v = (x - minVal) / (maxVal - minVal);
		calcRGB(v,rgb);
	}else{
		int ix = getStep(x);
		rgb[0] = color[3*ix  ];
		rgb[1] = color[3*ix+1];
		rgb[2] = color[3*ix+2];
	}
}

void
kmb::ColorMap::getRGBByStep(int s,float rgb[3]) const
{
	if( 0 <= s && s < step && color ){
		rgb[0] = color[3*s  ];
		rgb[1] = color[3*s+1];
		rgb[2] = color[3*s+2];
	}
}

void
kmb::ColorMap::getRGB(kmb::ScalarValue* value,float rgb[3]) const
{
	if( value ){
		getRGB( value->getValue(), rgb );
	}
}

void
kmb::ColorMap::getRGB(kmb::Vector3Value* value,float rgb[3]) const
{
	if( value ){
		getRGB( value->getLength(), rgb );
	}
}

void
kmb::ColorMap::getRGBA(double x,float rgba[4],float ratio) const
{
	if( this->step == 0 ){
		double v = (x - minVal) / (maxVal - minVal);
		float rgb[3];
		calcRGB(v,rgb);
		rgba[0] = rgb[0] * ratio;
		rgba[1] = rgb[1] * ratio;
		rgba[2] = rgb[2] * ratio;
		rgba[3] = 1.0f;
	}else{
		int ix = getStep(x);
		rgba[0] = color[3*ix  ] * ratio;
		rgba[1] = color[3*ix+1] * ratio;
		rgba[2] = color[3*ix+2] * ratio;
		rgba[3] = 1.0f;
	}
}

void
kmb::ColorMap::getRGBAByStep(int s,float rgba[4],float ratio) const
{
	if( 0 <= s && s < step && color ){
		rgba[0] = color[3*s  ] * ratio;
		rgba[1] = color[3*s+1] * ratio;
		rgba[2] = color[3*s+2] * ratio;
		rgba[3] = 1.0f;
	}
}

bool
kmb::ColorMap::createColorCache(const kmb::DataBindings* nodeValue, int comp)
{
	// nodeValue の節点番号はとびとびでないことを仮定
	if( cache == NULL ){
		cache = new kmb::Color3fValueBindings(nodeValue->getIdCount());
	}else if( cache->getIdCount() != nodeValue->getIdCount() ){
		delete cache;
		cache = new kmb::Color3fValueBindings(nodeValue->getIdCount());
	}
	cacheUpdate = true;
	updateColorCache(nodeValue,comp);
	return true;
}

bool
kmb::ColorMap::updateColorCache(const kmb::DataBindings* nodeValue, int comp)
{
	if( cacheUpdate == false ){
		return false;
	}
	if( nodeValue == NULL || nodeValue->getBindingMode() != kmb::DataBindings::NodeVariable ){
		return false;
	}
	if( cache == NULL || cache->getIdCount() != nodeValue->getIdCount() ){
		return false;
	}
	float rgb[3];
	switch( nodeValue->getValueType() )
	{
		case kmb::PhysicalValue::Scalar:
		{
			double d = 0.0;
			kmb::DataBindings::const_iterator dIter = nodeValue->begin();
			while( !dIter.isFinished() ){
				if( dIter.getValue( &d ) ){
					getRGB( d, rgb );
					cache->setColorValue( dIter.getId(), rgb );
				}
				++dIter;
			}
			cacheUpdate = false;
			return true;
		}
		case kmb::PhysicalValue::Vector2:
		{
			double d[] = {0.0, 0.0};
			if( comp==-1 ){
				kmb::DataBindings::const_iterator dIter = nodeValue->begin();
				while( !dIter.isFinished() ){
					if( dIter.getValue( d ) ){
						getRGB( kmb::Vector2D::abs(d), rgb );
						cache->setColorValue( dIter.getId(), rgb );
					}
					++dIter;
				}
				cacheUpdate = false;
				return true;
			}else if(0 <= comp && comp < 2){
				kmb::DataBindings::const_iterator dIter = nodeValue->begin();
				while( !dIter.isFinished() ){
					if( dIter.getValue( d ) ){
						getRGB( d[comp], rgb );
						cache->setColorValue( dIter.getId(), rgb );
					}
					++dIter;
				}
				cacheUpdate = false;
				return true;
			}
			break;
		}
		case kmb::PhysicalValue::Vector3:
		{
			double d[] = {0.0, 0.0, 0.0};
			if( comp==-1 ){
				kmb::DataBindings::const_iterator dIter = nodeValue->begin();
				while( !dIter.isFinished() ){
					if( dIter.getValue( d ) ){
						getRGB( kmb::Vector3D::abs(d), rgb );
						cache->setColorValue( dIter.getId(), rgb );
					}
					++dIter;
				}
				cacheUpdate = false;
				return true;
			}else if(0 <= comp && comp < 3){
				kmb::DataBindings::const_iterator dIter = nodeValue->begin();
				while( !dIter.isFinished() ){
					if( dIter.getValue( d ) ){
						getRGB( d[comp], rgb );
						cache->setColorValue( dIter.getId(), rgb );
					}
					++dIter;
				}
				cacheUpdate = false;
				return true;
			}
			break;
		}
		case kmb::PhysicalValue::Tensor6:
		{
			double d[] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
			if(0 <= comp && comp < 6){
				kmb::DataBindings::const_iterator dIter = nodeValue->begin();
				while( !dIter.isFinished() ){
					if( dIter.getValue( d ) ){
						getRGB( d[comp], rgb );
						cache->setColorValue( dIter.getId(), rgb );
					}
					++dIter;
				}
				cacheUpdate = false;
				return true;
			}
			break;
		}
		case kmb::PhysicalValue::Vector2withInt:
		{
			double d[] = {0.0, 0.0};
			long l;
			if(0 <= comp && comp < 2){
				kmb::DataBindings::const_iterator dIter = nodeValue->begin();
				while( !dIter.isFinished() ){
					if( dIter.getValue( d ) ){
						getRGB( d[comp], rgb );
						cache->setColorValue( dIter.getId(), rgb );
					}
					++dIter;
				}
				cacheUpdate = false;
				return true;
			}else if(comp == 2){
				kmb::DataBindings::const_iterator dIter = nodeValue->begin();
				while( !dIter.isFinished() ){
					if( dIter.getValue( &l ) ){
						getRGB( static_cast<double>(l), rgb );
						cache->setColorValue( dIter.getId(), rgb );
					}
					++dIter;
				}
				cacheUpdate = false;
				return true;
			}
			break;
		}
		default:
		{
			break;
		}
	}
	return false;
}

void
kmb::ColorMap::clearColorCache(void)
{
	cacheUpdate = true;
}

void
kmb::ColorMap::deleteColorCache(void)
{
	if( cache ){
		delete cache;
		cache = NULL;
	}
}

const float*
kmb::ColorMap::getColorCache(void) const
{
	if( cache ){
		return cache->getFloatArray();
	}
	return NULL;
}

int
kmb::ColorMap::getSolidDivision(double v0,double v1,int& count,double* &partitions) const
{
	if( v0 > v1 ){
		count = 0;
		return -1;
	}
	double c0 = (v0 - minVal) / (maxVal - minVal) * step;
	double c1 = (v1 - minVal) / (maxVal - minVal) * step;
	int i0 = getStep(v0);
	int i1 = getStep(v1);
	count = i1 -i0;
	if( count > 0 ){
		partitions = new double[count];
		for(int i=0;i<count;++i){
			partitions[i] = (i0+i+1 - c0) / (c1 - c0);
		}
	}
	return i0;
}
