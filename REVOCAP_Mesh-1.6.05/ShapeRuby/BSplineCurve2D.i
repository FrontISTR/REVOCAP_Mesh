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

==�T�v

�Q������ԓ��� B-Spline �Ȑ�������

==���\�b�h

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
    �R���X�g���N�^
=end
------------------------------------------------------------------------------*/
	BSplineCurve2D(void);
	virtual ~BSplineCurve2D(void);
/**--------------------------------------------------------------------------
=begin
--- clear
    �f�[�^�̃N���A
=end
------------------------------------------------------------------------------*/
	void clear(void);

/**--------------------------------------------------------------------------
=begin
--- getOrder
--- getDegree
    order �� knot �� ����_�̌����玩���I�Ɍ��܂�
    order = knot �̌� - ����_�̌�
    degree = order - 1
=end
------------------------------------------------------------------------------*/
	int getOrder(void) const;
	int getDegree(void) const;

/**--------------------------------------------------------------------------
=begin
--- appendKnot(k)
    knot �̒ǉ�
=end
------------------------------------------------------------------------------*/
	void appendKnot(double k);
/**--------------------------------------------------------------------------
=begin
--- getKnotCount
    knot �̌�
=end
------------------------------------------------------------------------------*/
	int getKnotCount(void) const;
/**--------------------------------------------------------------------------
=begin
--- eachKnot
    knot �̃C�e���[�^
--- getKnotArray
    knot �� Ruby �̔z��Ŏ擾
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
    ����_�̒ǉ�
=end
------------------------------------------------------------------------------*/
	void appendCtrlPt(double x,double y);

/**--------------------------------------------------------------------------
=begin
--- getCtrlPtCount
    ����_�̌���Ԃ�
=end
------------------------------------------------------------------------------*/
	int getCtrlPtCount(void) const;

/**--------------------------------------------------------------------------
=begin
--- eachCtrlPt
    ����_�̃C�e���[�^
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
    �Ȑ���̓_�̍��W��Ԃ�
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
    �����@�ŋ��߂�B�������Ȃ������Ƃ��� nil ��Ԃ�
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
