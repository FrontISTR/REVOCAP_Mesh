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
	// step ���� rgb ��ۑ�����
	float *color;
	int step;
	float shininess[1];
	// temporary for calc
	float rgb[3];
	float rgba[4];
	// color cache
	kmb::Color3fValueBindings* cache;
	// update ���K�v�Ȏ��� true
	bool cacheUpdate;
protected:
	// x �� [0,1] �ɐ��K�������ꍇ�� rgb �����߂�
	void calcRGB(double x,float rgb[3]) const;
	// x �� Solid Color �̉��X�e�b�v�ڂɊ܂܂�邩
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
	// NodeVariable �̂��̂ɂ��āA��C�� Color ���v�Z����
	// updateColorCache �� clearColorCache �����܂ōĐ������Ȃ�
	// clearColorCache �͍Đ����t���O�𗧂Ă邾��
	// deleteColorCache �̓|�C���^�̍폜
	bool createColorCache(const kmb::DataBindings* nodeValue, int comp=-1);
	bool updateColorCache(const kmb::DataBindings* nodeValue, int comp=-1);
	void clearColorCache(void);
	void deleteColorCache(void);
	const float* getColorCache(void) const;
	// calcRGB �ł͂Ȃ��A�}�j���A���ŐF��^����
	void setRGBAByStep(int s,const float rgba[4]);
	// OpenGL �̖��߂̒��ŐF�̐ݒ���s���@glColor ���Ăяo���B
	void setGLColor(double v);
	void setGLColorByStep(int step);
	// v0 < v1 �̂Ƃ��� Solid Contour �̕�����̗�����߂�
	// �l�� v0 ���� v1 �܂ł̊Ԃ� Solid Color �� count ��ω�����
	// Solid Color ���ω�����Ƃ���̔�(v0=0,v1=1�Ƃ���)�� partitions[0],...,partitions[count-1]
	// �߂�l�� s �̂Ƃ�
	// 0 �` partitions[0] �܂� s
	// partitions[0] �` partitions[1] �܂� s+1
	// ...
	// partitions[count-1] �` 1 �܂� s+count
	int getSolidDivision(double v0,double v1,int& count,double* &partitions) const;
};

}
