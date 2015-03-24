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

==�T�v

�Q������ԓ��� Nurbs �Ȑ��������N���X�B

==���\�b�h

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
    �f�[�^�̃N���A
=end
------------------------------------------------------------------------------*/
	void clear(void);

/**--------------------------------------------------------------------------
=begin
--- getOrder
    order ��Ԃ��B
--- getDegree
    �Ȑ��̎��� degree ��Ԃ��B
    degree = order - 1 �̊֌W������B
=end
------------------------------------------------------------------------------*/
	int getOrder(void) const;
	int getDegree(void) const;

/**--------------------------------------------------------------------------
=begin
--- getKnotCount
    knot �̌���Ԃ��B
--- appendKnot(k)
    knot ��ǉ�����B
--- eachKnot
    knot �̃C�e���[�^�B
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
    ����_�̌���Ԃ�
    knot �̌� - order �ƈ�v����B
--- appendCtrlPt(x,y,w=1.0)
    ����_��ǉ�����
--- eachCtrlPt
    ����_�̃C�e���[�^
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
    �Ȑ���̓_��Ԃ�
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
