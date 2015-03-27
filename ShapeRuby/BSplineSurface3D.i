/*----------------------------------------------------------------------
#                                                                      #
# Software Name : REVOCAP_PrePost version 1.6                          #
# Ruby Class Name : BSplineSurface3D                                   #
#                                                                      #
#                                Written by                            #
#                                           K. Tokunaga 2012/03/23     #
#                                                                      #
#      Contact Address: IIS, The University of Tokyo CISS              #
#                                                                      #
#      "Large Scale Assembly, Structural Correspondence,               #
#                                     Multi Dynamics Simulator"        #
----------------------------------------------------------------------*/
/*------------------------------------------------------------------------------
=begin
=RevocapShape::BSplineSurface3D

==メソッド一覧

==メソッド

=end
------------------------------------------------------------------------------*/

%{
#include "Shape/kmbBSplineSurface3D.h"
%}

namespace kmb{

class BSplineSurface3D
{
public:
/**--------------------------------------------------------------------------
=begin
--- BSplineSurface3D.new
=end
------------------------------------------------------------------------------*/
	BSplineSurface3D(void);
	virtual ~BSplineSurface3D(void);
/**--------------------------------------------------------------------------
=begin
--- clear
    データをクリアする
=end
------------------------------------------------------------------------------*/
	void clear(void);

/**--------------------------------------------------------------------------
=begin
--- setSurfaceId(id)
    表面データの識別子を与える
--- getSurfaceId
    表面データの識別子を取得する
=end
------------------------------------------------------------------------------*/
	long getSurfaceId(void) const;
	void setSurfaceId(long id);

/**--------------------------------------------------------------------------
=begin
--- setOrder(uOrder,vOrder)
    order を設定する
=end
------------------------------------------------------------------------------*/
	bool setOrder(unsigned int uOrder,unsigned int vOrder);
/**--------------------------------------------------------------------------
=begin
--- getOrder
    order を [uOrder,vOrder] の形式で取得する
=end
------------------------------------------------------------------------------*/
%extend{
	VALUE getOrder(void) const{
		unsigned int uOrder = 0;
		unsigned int vOrder = 0;
		self->getOrder( uOrder, vOrder );
		VALUE ary = rb_ary_new();
		rb_ary_store(ary,0,INT2NUM(uOrder));
		rb_ary_store(ary,1,INT2NUM(vOrder));
		return ary;
	}
}

/**--------------------------------------------------------------------------
=begin
--- getKnotCount
    knot の個数を [u方向の個数,v方向の個数] の形式で取得する
=end
------------------------------------------------------------------------------*/
%extend{
	VALUE getKnotCount(void) const{
		unsigned int uCount = 0;
		unsigned int vCount = 0;
		self->getKnotCount( uCount, vCount );
		VALUE ary = rb_ary_new();
		rb_ary_store(ary,0,INT2NUM(uCount));
		rb_ary_store(ary,1,INT2NUM(vCount));
		return ary;
	}
}

/**--------------------------------------------------------------------------
=begin
--- appendUKnot(k)
    u 方向の knot を追加する
--- appendVKnot(k)
    v 方向の knot を追加する
=end
------------------------------------------------------------------------------*/
	void appendUKnot(double k);
	void appendVKnot(double k);

/**--------------------------------------------------------------------------
=begin
--- eachUKnot
    u 方向の knot のイテレータ
--- eachVKnot
    v 方向の knot のイテレータ
--- getUKnotArray
--- getVKnotArray
=end
------------------------------------------------------------------------------*/
%extend{
	void eachUKnot(void){
		unsigned int uNum, vNum;
		self->getKnotCount(uNum,vNum);
		for(unsigned int i=0;i<uNum;++i){
			rb_yield( rb_float_new( self->getUKnot(i) ) );
		}
	}
	void eachVKnot(void){
		unsigned int uNum, vNum;
		self->getKnotCount(uNum,vNum);
		for(unsigned int i=0;i<vNum;++i){
			rb_yield( rb_float_new( self->getVKnot(i) ) );
		}
	}
	VALUE getUKnotArray(void){
		VALUE ary = rb_ary_new();
		unsigned int uNum, vNum;
		self->getKnotCount(uNum,vNum);
		for(unsigned int i=0;i<uNum;++i){
			rb_ary_store(ary,i,rb_float_new( self->getUKnot(i) ) );
		}
		return ary;
	}
	VALUE getVKnotArray(void){
		VALUE ary = rb_ary_new();
		unsigned int uNum, vNum;
		self->getKnotCount(uNum,vNum);
		for(unsigned int i=0;i<vNum;++i){
			rb_ary_store(ary,i,rb_float_new( self->getVKnot(i) ) );
		}
		return ary;
	}
}

/**--------------------------------------------------------------------------
=begin
--- getCtrlPtCount
    制御点の個数を返す。
    (u 方向の knot の個数 - u 方向の order) × (v 方向の knot の個数 - v 方向の order) と一致する。
--- appendCtrlPt(x,y,z)
    制御点を追加する
--- eachCtrlPt
    制御点のイテレータ
=end
------------------------------------------------------------------------------*/
	int getCtrlPtCount(void) const;
	void appendCtrlPt(double x,double y,double z);

%extend{
	void eachCtrlPt(void){
		int num = self->getCtrlPtCount();
		kmb::Point3D pt;
		for(int i=0;i<num;++i){
			self->getCtrlPt(i,pt);
			VALUE ary = rb_ary_new();
			rb_ary_store(ary,0,rb_float_new( pt.x() ));
			rb_ary_store(ary,1,rb_float_new( pt.y() ));
			rb_ary_store(ary,2,rb_float_new( pt.z() ));
			rb_yield( ary );
		}
	}
}

/**--------------------------------------------------------------------------
=begin
--- valid
    パラメータの妥当性を判断する。
    order と knot と制御点の個数の関係があっているのかのチェックをする。
=end
------------------------------------------------------------------------------*/
	bool valid(void) const;

/**--------------------------------------------------------------------------
=begin
--- getBoundingBox
    BoundingBox を返す。
=end
------------------------------------------------------------------------------*/
%extend{
	const kmb::BoundingBox getBoundingBox(void) const{
		kmb::BoundingBox bbox;
		self->getBoundingBox(bbox);
		return bbox;
	}
}

/**--------------------------------------------------------------------------
=begin
--- getPoint(u,v)
    曲面上の点の座標を返す
=end
------------------------------------------------------------------------------*/
%extend{
	VALUE getPoint( double u, double v ) const{
		kmb::Point3D pt;
		if( self->getPoint( u, v, pt ) ){
			VALUE ary = rb_ary_new();
			rb_ary_store(ary,0,rb_float_new(pt.x()));
			rb_ary_store(ary,1,rb_float_new(pt.y()));
			rb_ary_store(ary,2,rb_float_new(pt.z()));
			return ary;
		}else{
			return Qnil;
		}
	}
}
/**--------------------------------------------------------------------------
=begin
--- getNearest(x,y,z,u=0.5,v=0.5)
	反復法で求める。収束しなかったときは nil を返す
	u,v は反復法(Newton-Raphson) の初期値。
	初期値が定義域になかったら適当に初期値を探す
=end
------------------------------------------------------------------------------*/
%extend{
	VALUE getNearest( double x, double y,double z,double u=0.5,double v=0.5) const{
		double u0 = u;
		double v0 = v;
		kmb::Point3D pt(x,y,z);
		if( self->getNearest( pt, u0, v0 ) ){
			VALUE ary = rb_ary_new();
			rb_ary_store(ary,0,rb_float_new( u0 ));
			rb_ary_store(ary,1,rb_float_new( v0 ));
			return ary;
		}else{
			return Qnil;
		}
	}
}
};

}
