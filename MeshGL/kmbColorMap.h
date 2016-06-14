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
	// step 毎の rgb を保存する
	float *color;
	int step;
	float shininess[1];
	// temporary for calc
	float rgb[3];
	float rgba[4];
	float alpha;
	// color cache
	kmb::Color3fValueBindings* cache;
	// update が必要な時に true
	bool cacheUpdate;
protected:
	// x が [0,1] に正規化した場合の rgb を求める
	void calcRGB(double x,float rgb[3]) const;
	// x が Solid Color の何ステップ目に含まれるか
	int getStep(double x) const;
public:
	ColorMap(int s=5);
	virtual ~ColorMap(void);
	void setStep(int s);
	int getStep(void) const;
	double getContourValue(int i) const;
	void getMinMax(double& min,double& max) const;
	void setMinMax(double min,double max);
	float getAlpha(void) const;
	void setAlpha(float a);
	void getRGB(double x,float rgb[3]) const;
	void getRGBByStep(int s,float rgb[3]) const;
	void getRGB(kmb::ScalarValue* value,float rgb[3]) const;
	void getRGB(kmb::Vector3Value* value,float rgb[3]) const;
	void getRGBA(double x,float rgba[4],float ratio=1.0) const;
	void getRGBAByStep(int s,float rgba[4],float ratio=1.0) const;
	// NodeVariable のものについて、一気に Color を計算する
	// updateColorCache は clearColorCache されるまで再生成しない
	// clearColorCache は再生成フラグを立てるだけ
	// deleteColorCache はポインタの削除
	bool createColorCache(const kmb::DataBindings* nodeValue, int comp=-1);
	bool updateColorCache(const kmb::DataBindings* nodeValue, int comp=-1);
	void clearColorCache(void);
	void deleteColorCache(void);
	const float* getColorCache(void) const;
	// calcRGB ではなく、マニュアルで色を与える
	void setRGBAByStep(int s,const float rgba[4]);
	// OpenGL の命令の中で色の設定を行う　glColor を呼び出す。
	void setGLColor(double v);
	void setGLColorByStep(int step);
	// v0 < v1 のときに Solid Contour の分割比の列を求める
	// 値が v0 から v1 までの間に Solid Color が count 回変化する
	// Solid Color が変化するところの比(v0=0,v1=1とする)は partitions[0],...,partitions[count-1]
	// 戻り値が s のとき
	// 0 ～ partitions[0] まで s
	// partitions[0] ～ partitions[1] まで s+1
	// ...
	// partitions[count-1] ～ 1 まで s+count
	int getSolidDivision(double v0,double v1,int& count,double* &partitions) const;
};

}
