/*----------------------------------------------------------------------
#                                                                      #
# Software Name : REVOCAP_PrePost version 1.6                          #
# Ruby Class Name : BezierCurve2D                                      #
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
=RevocapShape::BezierCurve2D

2次元空間内のベジエ曲線を扱うためのクラス

==メソッド一覧

((<BezierCurve2D.new>))
((<clear>))
((<getOrder>))
((<getDegree>))
((<appendCtrlPt>))
((<getPoint>))
((<getNearest>))

==メソッド

=end
------------------------------------------------------------------------------*/

%{
#include "Shape/kmbBezierCurve2D.h"
%}

namespace kmb{

class BezierCurve2D
{
public:
/**--------------------------------------------------------------------------
=begin
--- BezierCurve2D.new
=end
------------------------------------------------------------------------------*/
	BezierCurve2D(void);
	virtual ~BezierCurve2D(void);
/**--------------------------------------------------------------------------
=begin
--- clear
    データをクリアする
=end
------------------------------------------------------------------------------*/
	void clear(void);

/**--------------------------------------------------------------------------
=begin
--- getOrder
    order(=制御点の個数)を返す。
--- getDegree
    degree(=曲線の次数)を返す。degree = order - 1 の関係がある。
=end
------------------------------------------------------------------------------*/
	int getOrder(void) const;
	int getDegree(void) const;

/**--------------------------------------------------------------------------
=begin
--- appendCtrlPt(x,y)
    制御点を追加する
=end
------------------------------------------------------------------------------*/
	void appendCtrlPt(double x,double y);
/**--------------------------------------------------------------------------
=begin
--- getPoint(t)
    曲線上の点の座標を求める
=end
------------------------------------------------------------------------------*/
%extend{
	VALUE getPoint( double t ){
		kmb::Point2D pt;
		if( t < 0 || 1 < t ){
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
