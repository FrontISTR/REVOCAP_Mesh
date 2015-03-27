/*----------------------------------------------------------------------
#                                                                      #
# Software Name : REVOCAP_PrePost version 1.6                          #
# Class Name : MatrixGL                                                #
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

#include "MeshGL/kmbMatrixGL.h"

#ifdef __APPLE__
#include <OpenGL/gl.h>
#else
#include <GL/gl.h>
#endif

#include "Matrix/kmbMatrix.h"
#include "MeshGL/kmbColorMap.h"
#include "Common/kmbCalculator.h"
#include "Geometry/kmbBoundingBox.h"

kmb::MatrixGL::MatrixGL(void)
: unitSize(1)
, pixelSize(1)
, bEMode(kmb::MatrixGL::AVERAGE)
{
}


kmb::MatrixGL::~MatrixGL(void)
{
}

void
kmb::MatrixGL::setUnitSize(int u)
{
	unitSize = u;
}

int
kmb::MatrixGL::getUnitSize(void) const
{
	return unitSize;
}

void
kmb::MatrixGL::setPixelSize(int px)
{
	pixelSize = px;
}

int
kmb::MatrixGL::getPixelSize(void) const
{
	return pixelSize;
}

void
kmb::MatrixGL::setBlockEMode(blockEvaluationMode m)
{
	bEMode = m;
}

kmb::MatrixGL::blockEvaluationMode
kmb::MatrixGL::getBlockEMode(void) const
{
	return bEMode;
}

void
kmb::MatrixGL::drawMatrixContour(kmb::Matrix* matrix, kmb::ColorMap* colormap)
{
	if( matrix == NULL || colormap == NULL ){
		return;
	}
	int rowSize = matrix->getRowSize();
	int rowBSize = rowSize / unitSize;
	if( rowSize % unitSize > 0 ){
		++rowBSize;
	}
	int colSize = matrix->getColSize();
	int colBSize = colSize / unitSize;
	if( colSize % unitSize > 0 ){
		++colBSize;
	}
	kmb::Calculator calc;
	for(int i0=0;i0<rowBSize;++i0){
		for(int j0=0;j0<colBSize;++j0){
			calc.initialize();
			for(int i1=0;i1<unitSize;++i1){
				int i = i0*unitSize + i1;
				if( i < rowSize ){
					for(int j1=0;j1<unitSize;++j1){
						int j = j0*unitSize + j1;
						if( j < colSize ){
							double v = matrix->get(i,j);
							calc.update( v );
						}
					}
				}
			}
			switch(bEMode){
			case kmb::MatrixGL::AVERAGE:
				colormap->setGLColor( calc.getAverage() );
				break;
			case kmb::MatrixGL::MAX_VALUE:
				colormap->setGLColor( calc.getMax() );
				break;
			case kmb::MatrixGL::MIN_VALUE:
				colormap->setGLColor( calc.getMin() );
				break;
			case kmb::MatrixGL::MAX_ABS:
				colormap->setGLColor( calc.getMaxAbs() );
				break;
			default:
				break;
			}
			::glRectd( j0*pixelSize, i0*pixelSize, (j0+1.0)*pixelSize, (i0+1.0)*pixelSize );
		}
	}
	::glFlush();
}

void
kmb::MatrixGL::drawMatrixNonZeroContour(kmb::Matrix* matrix, kmb::ColorMap* colormap)
{
	if( matrix == NULL || colormap == NULL ){
		return;
	}
	int rowSize = matrix->getRowSize();
	int rowBSize = rowSize / unitSize;
	if( rowSize % unitSize > 0 ){
		++rowBSize;
	}
	int colSize = matrix->getColSize();
	int colBSize = colSize / unitSize;
	if( colSize % unitSize > 0 ){
		++colBSize;
	}
	kmb::Calculator calc;
	for(int i0=0;i0<rowBSize;++i0){
		for(int j0=0;j0<colBSize;++j0){
			calc.initialize();
			for(int i1=0;i1<unitSize;++i1){
				int i = i0*unitSize + i1;
				if( i < rowSize ){
					for(int j1=0;j1<unitSize;++j1){
						int j = j0*unitSize + j1;
						if( j < colSize ){
							double v = matrix->get(i,j);
							calc.update( v );
						}
					}
				}
			}
			if( calc.getSum() != 0.0 ){
				switch(bEMode){
				case kmb::MatrixGL::AVERAGE:
					colormap->setGLColor( calc.getAverage() );
					break;
				case kmb::MatrixGL::MAX_VALUE:
					colormap->setGLColor( calc.getMax() );
					break;
				case kmb::MatrixGL::MIN_VALUE:
					colormap->setGLColor( calc.getMin() );
					break;
				case kmb::MatrixGL::MAX_ABS:
					colormap->setGLColor( calc.getMaxAbs() );
					break;
				default:
					break;
				}
				::glRectd( j0*pixelSize, i0*pixelSize, (j0+1.0)*pixelSize, (i0+1.0)*pixelSize );
			}
		}
	}
	::glFlush();
}

void
kmb::MatrixGL::drawSubMatrixContour(kmb::Matrix* matrix, kmb::ColorMap* colormap,int i00,int i01,int j00,int j01)
{
	if( matrix == NULL || colormap == NULL ){
		return;
	}
	int rowSize = matrix->getRowSize();




	int colSize = matrix->getColSize();




	kmb::Calculator calc;
	for(int i0=i00;i0<i01;++i0){
		for(int j0=j00;j0<j01;++j0){
			calc.initialize();
			for(int i1=0;i1<unitSize;++i1){
				int i = i0*unitSize + i1;
				if( i < rowSize ){
					for(int j1=0;j1<unitSize;++j1){
						int j = j0*unitSize + j1;
						if( j < colSize ){
							double v = matrix->get(i,j);
							calc.update( v );
						}
					}
				}
			}
			switch(bEMode){
			case kmb::MatrixGL::AVERAGE:
				colormap->setGLColor( calc.getAverage() );
				break;
			case kmb::MatrixGL::MAX_VALUE:
				colormap->setGLColor( calc.getMax() );
				break;
			case kmb::MatrixGL::MIN_VALUE:
				colormap->setGLColor( calc.getMin() );
				break;
			case kmb::MatrixGL::MAX_ABS:
				colormap->setGLColor( calc.getMaxAbs() );
				break;
			default:
				break;
			}
			::glRectd( (j0-j00)*pixelSize, (i0-i00)*pixelSize, (j0-j00+1.0)*pixelSize, (i0-i00+1.0)*pixelSize );
		}
	}
	::glFlush();
}

void
kmb::MatrixGL::drawSubMatrixNonZeroContour(kmb::Matrix* matrix, kmb::ColorMap* colormap,int i00,int i01,int j00,int j01)
{
	if( matrix == NULL || colormap == NULL ){
		return;
	}
	int rowSize = matrix->getRowSize();




	int colSize = matrix->getColSize();




	kmb::Calculator calc;
	for(int i0=i00;i0<i01;++i0){
		for(int j0=j00;j0<j01;++j0){
			calc.initialize();
			for(int i1=0;i1<unitSize;++i1){
				int i = i0*unitSize + i1;
				if( i < rowSize ){
					for(int j1=0;j1<unitSize;++j1){
						int j = j0*unitSize + j1;
						if( j < colSize ){
							double v = matrix->get(i,j);
							calc.update( v );
						}
					}
				}
			}
			if( calc.getSum() != 0.0 ){
				switch(bEMode){
				case kmb::MatrixGL::AVERAGE:
					colormap->setGLColor( calc.getAverage() );
					break;
				case kmb::MatrixGL::MAX_VALUE:
					colormap->setGLColor( calc.getMax() );
					break;
				case kmb::MatrixGL::MIN_VALUE:
					colormap->setGLColor( calc.getMin() );
					break;
				case kmb::MatrixGL::MAX_ABS:
					colormap->setGLColor( calc.getMaxAbs() );
					break;
				default:
					break;
				}
				::glRectd( (j0-j00)*pixelSize, (i0-i00)*pixelSize, (j0-j00+1.0)*pixelSize, (i0-i00+1.0)*pixelSize );
			}
		}
	}
	::glFlush();
}
