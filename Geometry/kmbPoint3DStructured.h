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
/*
 * 構造格子用の節点コンテナ
 *
 * (i*(ny) + j)*(nz) + k;
 * ループで回す時は
 * for(i){
 *  for(j){
 *   for(k){
 *   }
 *  }
 * }
 *
 */
#pragma once

#include "Common/kmbIndex.h"
#include "Geometry/kmbIdTypes.h"
#include <string>

namespace kmb{

class BoxRegion;
class Point3D;
class Point3DContainerArray;
class Index3;


template<kmb::orderType o=kmb::kNormalOrder>
class StructuredBase{
public:
	static kmb::orderType getOrderType(void){
		return o;
	}
	typedef kmb::Index3 indexType;
protected:
	int nx,ny,nz;
public:
	StructuredBase(void)
		: nx(1), ny(1), nz(1) {}
	StructuredBase(int _nx,int _ny,int _nz)
		: nx(_nx), ny(_ny), nz(_nz) {}
	void initialize(int _nx,int _ny,int _nz){
		nx = _nx;
		ny = _ny;
		nz = _nz;
	}
	indexType end(void) const{
		return indexType(nx,ny,nz);
	}
	int getId(int i,int j,int k) const{
		return (i*(ny) + j)*(nz) + k;
	}
	int getId(const indexType &ind) const{
		return ind(nx,ny,nz);
	}
	bool getIndices(int index,int &i,int &j,int &k) const{
		if( index < 0 || index >= nx*ny*nz ){
			return false;
		}
		i = index / (ny*nz);
		int q = index - i * ny * nz;
		j = q / nz;
		k = q - j * nz;
		return true;
	}
	bool getIndices(int index,indexType &ind) const{
		return ind.set(index,nx,ny,nz);
	}
	int getCount(void) const{
		return nx*ny*nz;
	}
	int getCount(int n) const{
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
	bool valid(int i,int j,int k) const{
		return (0 <= i) && (i < nx) && (0 <= j) && (j < ny) && (0 <= k) && (k < nz);
	}
};


template<>
class StructuredBase<kmb::kReverseOrder>{
public:
	static kmb::orderType getOrderType(void){
		return kmb::kReverseOrder;
	}
	typedef kmb::Index3R indexType;
protected:
	int nx,ny,nz;
public:
	StructuredBase(int _nx,int _ny,int _nz)
		: nx(_nx), ny(_ny), nz(_nz) {}
	int getId(int i,int j,int k) const{
		return (k*(ny) + j)*(nx) + i;
	}
	int getId(const indexType &ind) const{
		return ind(nx,ny,nz);
	}
	bool getIndices(int index,int &i,int &j,int &k) const{
		if( index < 0 || index >= nx*ny*nz ){
			return false;
		}
		k = index / (nx*ny);
		int q = index - k * nx * ny;
		j = q / nx;
		i = q - j * nx;
		return true;
	}
	bool getIndices(int index,indexType &ind) const{
		return ind.set(index,nx,ny,nz);
	}
	int getCount(void) const{
		return nx*ny*nz;
	}
	int getCount(int n) const{
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
	bool valid(int i,int j,int k) const{
		return (0 <= i) && (i < nx) && (0 <= j) && (j < ny) && (0 <= k) && (k < nz);
	}
};

class Point3DStructured : public StructuredBase<>
{
public:
	enum fieldType{
		kUnknown = -1,
		kUniform,
		kRectLinear,
		kIrregular
	};
	static std::string getFieldTypeString( fieldType ftype );
protected:
	fieldType ftype;
public:
	Point3DStructured(void);
	Point3DStructured(int _nx,int _ny,int _nz,fieldType _ftype);
	virtual ~Point3DStructured(void);
	void initialize(int _nx,int _ny,int _nz);
	virtual bool getPoint(int i,int j,int k,kmb::Point3D &pt) const = 0;
	virtual void translate(double x,double y,double z) = 0;
	fieldType getFieldType(void) const;
};

class Point3DUniformStructured : public Point3DStructured
{
protected:
	kmb::BoxRegion* box;
public:
	Point3DUniformStructured(int _nx,int _ny,int _nz);
	Point3DUniformStructured(double minx,double maxx,double miny,double maxy,double minz,double maxz,int _nx,int _ny,int _nz);
	Point3DUniformStructured(kmb::BoxRegion &_box,int _nx,int _ny,int _nz);
	virtual ~Point3DUniformStructured(void);
	void setRegion(double minx,double miny,double minz,double maxx,double maxy,double maxz);
	void setRegion(kmb::BoxRegion &_box);
	const kmb::BoxRegion* getRegion() const;
	kmb::BoxRegion* getRegion();
	bool getPoint(int i,int j,int k,kmb::Point3D &pt) const;
	void translate(double x,double y,double z);
};

class Point3DRectLinearStructured : public Point3DStructured
{
protected:
	double* xAxis;
	double* yAxis;
	double* zAxis;
public:
	Point3DRectLinearStructured(int _nx,int _ny,int _nz);
	virtual ~Point3DRectLinearStructured(void);
	void setXAxis(int i,double x);
	void setYAxis(int j,double y);
	void setZAxis(int k,double z);
	bool getPoint(int i,int j,int k,kmb::Point3D &pt) const;
	void translate(double x,double y,double z);
};

class Point3DIrregularStructured : public Point3DStructured
{
protected:
	kmb::Point3DContainerArray* points;
public:
	Point3DIrregularStructured(int _nx,int _ny,int _nz);
	virtual ~Point3DIrregularStructured(void);
	bool getPoint(int i,int j,int k,kmb::Point3D &pt) const;
	void translate(double x,double y,double z);
	bool setPoint(int i,int j,int k,kmb::Point3D &pt);
	bool setPoint(int i,int j,int k,double x,double y,double z);
	bool getPoint(kmb::nodeIdType nodeid,kmb::Point3D &pt) const;
	bool setPoint(kmb::nodeIdType nodeid,kmb::Point3D &pt);
	bool setPoint(kmb::nodeIdType nodeid,double x,double y,double z);
};

}
