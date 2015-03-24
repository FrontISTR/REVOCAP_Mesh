/*----------------------------------------------------------------------
#                                                                      #
# Software Name : REVOCAP_PrePost version 1.6                          #
# Ruby Class Name : NurbsCurve2D                                       #
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
=RevocapShape::NurbsCurve2D

==概要

２次元空間内の Nurbs 曲線を扱うクラス。

==メソッド

=end
------------------------------------------------------------------------------*/
%{
#include "Shape/kmbNurbsCurve2D.h"
%}

namespace kmb{

class NurbsCurve2D
{
public:
/**--------------------------------------------------------------------------
=begin
--- NurbsCurve2D.new
=end
------------------------------------------------------------------------------*/
	NurbsCurve2D(void);
	virtual ~NurbsCurve2D(void);
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
    order を返す。
--- getDegree
    曲線の次元 degree を返す。
    degree = order - 1 の関係がある。
=end
------------------------------------------------------------------------------*/
	int getOrder(void) const;
	int getDegree(void) const;

/**--------------------------------------------------------------------------
=begin
--- getKnotCount
    knot の個数を返す。
--- appendKnot(k)
    knot を追加する。
--- eachKnot
    knot のイテレータ。
--- getKnotArray
=end
------------------------------------------------------------------------------*/
	int getKnotCount(void) const;
	void appendKnot(double k);
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
--- getCtrlPtCount
    制御点の個数を返す
    knot の個数 - order と一致する。
--- appendCtrlPt(x,y,w=1.0)
    制御点を追加する
--- eachCtrlPt
    制御点のイテレータ
=end
------------------------------------------------------------------------------*/
	int getCtrlPtCount(void) const;
	void appendCtrlPt(double x,double y,double w=1.0);

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
--- getPoint(t)
    曲線上の点を返す
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
