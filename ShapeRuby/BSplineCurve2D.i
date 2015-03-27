/*----------------------------------------------------------------------
#                                                                      #
# Software Name : REVOCAP_PrePost version 1.6                          #
# Ruby Class Name : BSplineCurve2D                                     #
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
=RevocapShape::BSplineCurve2D

==概要

２次元空間内の B-Spline 曲線を扱う

==メソッド

=end
------------------------------------------------------------------------------*/
%{
#include "Shape/kmbBSplineCurve2D.h"
%}

namespace kmb{

class BSplineCurve2D
{
public:
/**--------------------------------------------------------------------------
=begin
--- BSplineCurve2D.new
    コンストラクタ
=end
------------------------------------------------------------------------------*/
	BSplineCurve2D(void);
	virtual ~BSplineCurve2D(void);
/**--------------------------------------------------------------------------
=begin
--- clear
    データのクリア
=end
------------------------------------------------------------------------------*/
	void clear(void);

/**--------------------------------------------------------------------------
=begin
--- getOrder
--- getDegree
    order は knot と 制御点の個数から自動的に決まる
    order = knot の個数 - 制御点の個数
    degree = order - 1
=end
------------------------------------------------------------------------------*/
	int getOrder(void) const;
	int getDegree(void) const;

/**--------------------------------------------------------------------------
=begin
--- appendKnot(k)
    knot の追加
=end
------------------------------------------------------------------------------*/
	void appendKnot(double k);
/**--------------------------------------------------------------------------
=begin
--- getKnotCount
    knot の個数
=end
------------------------------------------------------------------------------*/
	int getKnotCount(void) const;
/**--------------------------------------------------------------------------
=begin
--- eachKnot
    knot のイテレータ
--- getKnotArray
    knot を Ruby の配列で取得
=end
------------------------------------------------------------------------------*/
%extend{
	void eachKnot(void){
		unsigned int num = self->getKnotCount();
		for(unsigned int i=0;i<num;++i){
			rb_yield( rb_float_new( self->getKnot(i) ) );
		}
	}
	VALUE getKnotArray(void){
		VALUE ary = rb_ary_new();
		unsigned int num = self->getKnotCount();
		for(unsigned int i=0;i<num;++i){
			rb_ary_store(ary,i,rb_float_new( self->getKnot(i) ) );
		}
		return ary;
	}
}

/**--------------------------------------------------------------------------
=begin
--- appendCtrlPt(x,y)
    制御点の追加
=end
------------------------------------------------------------------------------*/
	void appendCtrlPt(double x,double y);

/**--------------------------------------------------------------------------
=begin
--- getCtrlPtCount
    制御点の個数を返す
=end
------------------------------------------------------------------------------*/
	int getCtrlPtCount(void) const;

/**--------------------------------------------------------------------------
=begin
--- eachCtrlPt
    制御点のイテレータ
=end
------------------------------------------------------------------------------*/
%extend{
	void eachCtrlPt(void){
		int num = self->getCtrlPtCount();
		kmb::Point2D pt;
		for(int i=0;i<num;++i){
			self->getCtrlPt(i,pt);
			VALUE ary = rb_ary_new();
			rb_ary_store(ary,0,rb_float_new( pt.x() ));
			rb_ary_store(ary,1,rb_float_new( pt.y() ));
			rb_yield( ary );
		}
	}
}

/**--------------------------------------------------------------------------
=begin
--- getPoint(t)
    曲線上の点の座標を返す
=end
------------------------------------------------------------------------------*/
%extend{
	VALUE getPoint( double t ){
		kmb::Point2D pt;
		if( ! self->isDomain(t) ){
			return Qnil;
		}
		self->getPoint( t, pt );
		VALUE ary = rb_ary_new();
		rb_ary_store(ary,0,rb_float_new(pt.x()));
		rb_ary_store(ary,1,rb_float_new(pt.y()));
		return ary;
	}
}
/**--------------------------------------------------------------------------
=begin
--- getNearest(x,y,t=0.5)
    反復法で求める。収束しなかったときは nil を返す
=end
------------------------------------------------------------------------------*/
%extend{
	VALUE getNearest( double x, double y,double t=0.5){
		double t0 = t;
		kmb::Point2D pt(x,y);
		if( self->getNearest( pt, t0 ) ){
			return rb_float_new(t0);
		}else{
			return Qnil;
		}
	}
}
};

}
