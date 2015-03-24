/*----------------------------------------------------------------------
#                                                                      #
# Software Name : REVOCAP_PrePost version 1.6                          #
# Ruby Class Name : BoundingBox                                        #
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
/**--------------------------------------------------------------------
=begin
= RevocapMesh::BoundingBox

==概要

３次元のバウンディングボックスを与えるクラス。

==メソッド一覧

((<BoundingBox.new>))
((<minX>))
((<minY>))
((<minZ>))
((<maxX>))
((<maxY>))
((<maxZ>))
((<centerX>))
((<centerY>))
((<centerZ>))
((<rangeX>))
((<rangeY>))
((<rangeZ>))
((<diameter>))
((<getVolume>))
((<update>))
((<intersect>))
((<intersectVolume>))
((<distanceSq>))
((<expand>))
((<to_a>))

=end
----------------------------------------------------------------------*/
%{
#include "Geometry/kmbRegion.h"
#include "Geometry/kmbBoxRegion.h"
#include "Geometry/kmbBoundingBox.h"
%}

namespace kmb{

class BoundingBox : public BoxRegion
{
public:
/**--------------------------------------------------------------------------
=begin
--- BoundingBox.new
=end
---------------------------------------------------------------------------*/
	BoundingBox(void);
	virtual ~BoundingBox(void);
/**--------------------------------------------------------------------------
=begin
--- minX
--- minY
--- minZ
    x 座標、y 座標、z 座標の最小値を与える
=end
---------------------------------------------------------------------------*/
	double minX(void) const;
	double minY(void) const;
	double minZ(void) const;
/**--------------------------------------------------------------------------
=begin
--- maxX
--- maxY
--- maxZ
    x 座標、y 座標、z 座標の最大値を与える
=end
---------------------------------------------------------------------------*/
	double maxX(void) const;
	double maxY(void) const;
	double maxZ(void) const;
/**--------------------------------------------------------------------------
=begin
--- centerX
--- centerY
--- centerZ
    x 座標、y 座標、z 座標の中心値を与える
=end
---------------------------------------------------------------------------*/
	double centerX(void) const;
	double centerY(void) const;
	double centerZ(void) const;
/**--------------------------------------------------------------------------
=begin
--- rangeX
--- rangeY
--- rangeZ
    x 座標、y 座標、z 座標の幅を与える
=end
---------------------------------------------------------------------------*/
	double rangeX(void) const;
	double rangeY(void) const;
	double rangeZ(void) const;
/**--------------------------------------------------------------------------
=begin
--- diameter
    対角線の長さを与える
=end
---------------------------------------------------------------------------*/
	double diameter(void) const;
/**--------------------------------------------------------------------------
=begin
--- getVolume
    体積を与える
=end
---------------------------------------------------------------------------*/
	double getVolume(void) const;
/**--------------------------------------------------------------------------
=begin
--- update(x,y,z)
    更新する
=end
---------------------------------------------------------------------------*/
	void update(const double x, const double y,const double z);
	void update(const BoundingBox& box);
/**--------------------------------------------------------------------------
=begin
--- intersect(box)
    交差判定
=end
---------------------------------------------------------------------------*/
	bool intersect(const kmb::BoundingBox& box) const;
/**--------------------------------------------------------------------------
=begin
--- intersectVolume(box)
    交差体積
=end
---------------------------------------------------------------------------*/
	double intersectVolume(const kmb::BoundingBox& box) const;
/**--------------------------------------------------------------------------
=begin
--- distanceSq(box)
    距離の2乗
    最大点同士の距離の2乗と最小点同士の距離の2乗の和
    交差していても0とは限らないので注意する
=end
---------------------------------------------------------------------------*/
	double distanceSq(const kmb::BoundingBox& box) const;
/**--------------------------------------------------------------------------
=begin
--- expand(r)
    拡大する
=end
---------------------------------------------------------------------------*/
	void expand(double ratio);
/**--------------------------------------------------------------------------
=begin
--- to_a
    配列に変換する
    形式は [[minX, minY, minZ], [maxX, maxY, maxZ]]
=end
---------------------------------------------------------------------------*/
%extend{
	VALUE to_a(void) const{
		VALUE minmax = rb_ary_new();
		VALUE min = rb_ary_new();
		VALUE max = rb_ary_new();
		rb_ary_store(min,0,rb_float_new(self->minX()));
		rb_ary_store(min,1,rb_float_new(self->minY()));
		rb_ary_store(min,2,rb_float_new(self->minZ()));
		rb_ary_store(max,0,rb_float_new(self->maxX()));
		rb_ary_store(max,1,rb_float_new(self->maxY()));
		rb_ary_store(max,2,rb_float_new(self->maxZ()));
		rb_ary_store(minmax,0,min);
		rb_ary_store(minmax,1,max);
		return minmax;
	}
}
};

/**--------------------------------------------------------------------
=begin
= RevocapMesh::BoundingBox2D

==概要

２次元版のバウンディングボックスを与えるクラス。

==メソッド一覧

((<BoundingBox2D.new>))
((<minX>))
((<minY>))
((<maxX>))
((<maxY>))
((<centerX>))
((<centerY>))
((<rangeX>))
((<rangeY>))
((<diameter>))
((<getArea>))
((<intersect>))
((<intersectArea>))
((<update>))
((<distanceSq>))
((<expand>))
((<to_a>))

=end
----------------------------------------------------------------------*/
class BoundingBox2D
{
public:
	BoundingBox2D(void);
	virtual ~BoundingBox2D(void);
	double minX(void) const;
	double minY(void) const;
	double maxX(void) const;
	double maxY(void) const;
	double centerX(void) const;
	double centerY(void) const;
	double rangeX(void) const;
	double rangeY(void) const;
	double diameter(void) const;
	double getArea(void) const;
	bool intersect(const kmb::BoundingBox2D& box) const;
	double intersectArea(const kmb::BoundingBox2D& box) const;
	void update(const double x, const double y);
	double distanceSq(const kmb::BoundingBox2D& box) const;
	void expand(double r);
/**--------------------------------------------------------------------------
=begin
--- to_a
    配列に変換する
=end
---------------------------------------------------------------------------*/
%extend{
	VALUE to_a(void) const{
		VALUE minmax = rb_ary_new();
		VALUE min = rb_ary_new();
		VALUE max = rb_ary_new();
		rb_ary_store(min,0,rb_float_new(self->minX()));
		rb_ary_store(min,1,rb_float_new(self->minY()));
		rb_ary_store(max,0,rb_float_new(self->maxX()));
		rb_ary_store(max,1,rb_float_new(self->maxY()));
		rb_ary_store(minmax,0,min);
		rb_ary_store(minmax,1,max);
		return minmax;
	}
}
};

}
