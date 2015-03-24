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



#pragma once

#ifdef WIN32
#include <windows.h>
#pragma comment(lib,"opengl32.lib")
#pragma comment(lib,"glu32.lib")
#endif

namespace kmb{

class Matrix;
class ColorMap;

class MatrixGL
{
public:
	enum blockEvaluationMode{
		AVERAGE,
		MAX_VALUE,
		MIN_VALUE,
		MAX_ABS
	};
private:
	int unitSize;
	int pixelSize;
	blockEvaluationMode bEMode;
public:
	MatrixGL(void);
	~MatrixGL(void);
	void setUnitSize(int u);
	int getUnitSize(void) const;
	void setPixelSize(int px);
	int getPixelSize(void) const;
	void setBlockEMode(blockEvaluationMode m);
	blockEvaluationMode getBlockEMode(void) const;
	void drawMatrixContour(kmb::Matrix* matrix, kmb::ColorMap* colormap);
	void drawMatrixNonZeroContour(kmb::Matrix* matrix, kmb::ColorMap* colormap);

	void drawSubMatrixContour(kmb::Matrix* matrix, kmb::ColorMap* colormap,int i00,int i01,int j00,int j01);
	void drawSubMatrixNonZeroContour(kmb::Matrix* matrix, kmb::ColorMap* colormap,int i00,int i01,int j00,int j01);
};

}
