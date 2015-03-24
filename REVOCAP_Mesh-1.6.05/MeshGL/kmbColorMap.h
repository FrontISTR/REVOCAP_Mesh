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
#pragma once

#include "MeshDB/kmbTypes.h"

namespace kmb{

class MeshData;
class ScalarValue;
class Vector3Value;
class DataBindings;
class Color3fValueBindings;

class ColorMap
{
protected:
	double minVal;
	double maxVal;

	float *color;
	int step;
	float shininess[1];

	float rgb[3];
	float rgba[4];

	kmb::Color3fValueBindings* cache;

	bool cacheUpdate;
protected:

	void calcRGB(double x,float rgb[3]) const;

	int getStep(double x) const;
public:
	ColorMap(int s=5);
	virtual ~ColorMap(void);
	void setStep(int s);
	int getStep(void) const;
	double getContourValue(int i) const;
	void getMinMax(double& min,double& max) const;
	void setMinMax(double min,double max);
	void getRGB(double x,float rgb[3]) const;
	void getRGBByStep(int s,float rgb[3]) const;
	void getRGB(kmb::ScalarValue* value,float rgb[3]) const;
	void getRGB(kmb::Vector3Value* value,float rgb[3]) const;
	void getRGBA(double x,float rgba[4],float ratio=1.0) const;
	void getRGBAByStep(int s,float rgba[4],float ratio=1.0) const;




	bool createColorCache(const kmb::DataBindings* nodeValue, int comp=-1);
	bool updateColorCache(const kmb::DataBindings* nodeValue, int comp=-1);
	void clearColorCache(void);
	void deleteColorCache(void);
	const float* getColorCache(void) const;

	void setRGBAByStep(int s,const float rgba[4]);

	void setGLColor(double v);
	void setGLColorByStep(int step);








	int getSolidDivision(double v0,double v1,int& count,double* &partitions) const;
};

}
